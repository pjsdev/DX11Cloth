////////////////////////////////////////////////////////////////////////////////
// Filename: Renderer.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Renderer.h"


using namespace pjs;

Renderer::Renderer()
{
	m_D3D = 0;
	m_camera = 0;
	m_textureShader = 0;
	m_tessellationShader = 0;
	m_cloth = 0;
	m_light = 0;
}


Renderer::Renderer(const Renderer& other)
{
}


Renderer::~Renderer()
{
}


bool Renderer::initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_camera = new Camera;
	if(!m_camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_camera->setPosition(-1.0f, -25.0f, -50.0f);

	// Create the model object.
	m_cloth = new Cloth(15,10,3.0f);
	if(!m_cloth)
	{
		return false;
	}

	// Initialize the model object.
	result = m_cloth->initialize(m_D3D->getDevice(), L"../Engine/textures/ukFlag.jpg");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the cloth object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	m_textureShader = new TextureShader;
	if(!m_textureShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_textureShader->initialize(m_D3D->getDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	m_tessellationShader = new TessellationShader;
	if(!m_tessellationShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_tessellationShader->initialize(m_D3D->getDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the tessellation shader object.", L"Error", MB_OK);
		return false;
	}

	//Create the light
	m_light = new Light;
	if(!m_light)
	{
		return false;
	}

	m_light->setAmbientColor(0.5f,0.5f,0.5f,1.0f);
	m_light->setDiffuseColor(0.7f,0.7f,0.7f,1.0f);
	m_light->setDirection(0.5f,0.0f,0.5f);
	m_light->setSpecularColor(0.3f,0.3f,0.3f,1.0f);
	m_light->setSpecularPower(4.0f);

	return true;
}


void Renderer::shutdown()
{
	//release the light
	if(m_light)
	{
		delete m_light;
		m_light = 0;
	}

	// Release the color shader object.
	if(m_textureShader)
	{
		m_textureShader->shutdown();
		delete m_textureShader;
		m_textureShader = 0;
	}

	// Release the color shader object.
	if(m_tessellationShader)
	{
		m_tessellationShader->shutdown();
		delete m_tessellationShader;
		m_tessellationShader = 0;
	}

	if(m_cloth)
	{
		m_cloth->shutdown();
		delete m_cloth;
		m_cloth = 0;
	}

	// Release the camera object.
	if(m_camera)
	{
		delete m_camera;
		m_camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool Renderer::frame(pjs::Solver* _solver, float _timeStep)
{
	bool result;

	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.001f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	m_cloth->frame(_solver, _timeStep, m_D3D->getDeviceContext());
	// Render the graphics scene.
	result = render(rotation);
	if(!result)
	{
		return false;
	}

	return true;
}


bool Renderer::render(float _rotation)
{
	Matrix worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	m_D3D->beginScene(0.4f, 0.4f, 0.4f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_camera->render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_camera->getViewMatrix(viewMatrix);
	m_D3D->getWorldMatrix(worldMatrix);
	m_D3D->getProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	D3DXMatrixRotationY(&worldMatrix, _rotation);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_cloth->render(m_D3D->getDeviceContext());

	// Render the model using the color shader.
	//result = m_textureShader->render(
	//	m_D3D->getDeviceContext(), m_cloth->getIndexCount(), 
	//	worldMatrix, viewMatrix, projectionMatrix, 
	//	m_cloth->getTexture(), m_light->getAmbientColor(), m_light->getDiffuseColor(), m_light->getDirection(),
	//	m_light->getSpecularPower(),m_light->getSpecularColor(), m_camera->getPosition());

	result = m_tessellationShader->render(
		m_D3D->getDeviceContext(), m_cloth->getIndexCount(), 
		worldMatrix, viewMatrix, projectionMatrix, 
		m_cloth->getTexture(), m_light->getAmbientColor(), 
		m_light->getDiffuseColor(), m_light->getDirection(),
		10.0f);

	if(!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->endScene();

	return true;
}