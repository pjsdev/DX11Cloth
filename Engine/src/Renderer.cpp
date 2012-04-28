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
	m_text = 0;
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
	Matrix baseViewMatrix;

	m_zoom = 0.0f;
	m_rotY = 0.0f;
	m_rotX = 0.0f;

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
	m_camera->setPosition(0.0f, -15.0f, -90.0f);
	m_camera->render();
	m_camera->getViewMatrix(baseViewMatrix);


	// Create the text object.
	m_text = new Text;
	if(!m_text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_text->initialize(m_D3D->getDevice(), m_D3D->getDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	m_cloth = new Cloth(20,15,3.0f);
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
	m_light->setDiffuseColor(1.7f,1.7f,1.7f,1.0f);
	m_light->setDirection(0.5f,0.3f,0.3f);
	m_light->setSpecularColor(0.3f,0.3f,0.3f,1.0f);
	m_light->setSpecularPower(4.0f);

	return true;
}


void Renderer::shutdown()
{
	// Release the text object.
	if(m_text)
	{
		m_text->shutdown();
		delete m_text;
		m_text = 0;
	}

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


bool Renderer::frame(int _mouseX, int _mouseY, int _mouseZ, pjs::Solver* _solver, float _timeStep)
{
	bool result;

	result = m_text->setMousePosition(_mouseX, _mouseY, m_D3D->getDeviceContext());
	if(!result)
	{
		return false;
	}

	m_zoom = -(float)_mouseZ / 12.0f;
	m_rotY = -(float)_mouseX / 12.0f;
	m_rotX = -(float)_mouseY / 12.0f;

	m_cloth->frame(_solver, _timeStep, m_D3D->getDeviceContext());

	return true;
}

bool Renderer::renderUI()
{
	bool result;
	Matrix worldMatrix;
	matMakeIdentity(worldMatrix);
	Matrix orthoMatrix;
	m_D3D->getOrthoMatrix(orthoMatrix);
	m_D3D->turnZBufferOff();
	// Turn on the alpha blending before rendering the text.
	m_D3D->turnOnAlphaBlending();
	// Render the text strings.
	result = m_text->render(m_D3D->getDeviceContext(), worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}
	// Turn off alpha blending after rendering the text.
	m_D3D->turnOffAlphaBlending();
	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->turnZBufferOn();

	return true;
}

bool Renderer::render()
{
	Matrix modelMatrix, viewMatrix, projectionMatrix, worldMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	m_D3D->beginScene(0.4f, 0.4f, 0.4f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_camera->render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_camera->getViewMatrix(viewMatrix);
	m_D3D->getProjectionMatrix(projectionMatrix);
	m_D3D->getWorldMatrix(worldMatrix);

	if(!renderUI())
	{
		return false;
	}

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_cloth->render(m_D3D->getDeviceContext());
	m_cloth->getModelMatrix(modelMatrix);

	Matrix tmp;
	D3DXMatrixMultiply(&modelMatrix, &modelMatrix, D3DXMatrixTranslation(&tmp, 0.0f,0.0f,m_zoom));

	// Render the model using the color shader.
	result = m_textureShader->render(
		m_D3D->getDeviceContext(), m_cloth->getIndexCount(), 
		modelMatrix, worldMatrix, viewMatrix, projectionMatrix, 
		m_cloth->getTexture(), 
		m_light->getAmbientColor(), m_light->getDiffuseColor(), m_light->getDirection(),
		m_light->getSpecularPower(),m_light->getSpecularColor(),
		m_camera->getPosition());

	//result = m_tessellationShader->render(
	//	m_D3D->getDeviceContext(), m_cloth->getIndexCount(), 
	//	worldMatrix, viewMatrix, projectionMatrix, 
	//	m_cloth->getTexture(), m_light->getAmbientColor(), 
	//	m_light->getDiffuseColor(), m_light->getDirection(),
	//	14.0f);

	if(!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->endScene();

	return true;
}