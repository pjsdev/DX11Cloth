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
	m_cloth = 0;
}


Renderer::Renderer(const Renderer& other)
{
}


Renderer::~Renderer()
{
}


bool Renderer::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
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
	m_camera->SetPosition(-1.0f, -60.0f, -100.0f);

	// Create the model object.
	m_cloth = new Cloth(25,20,3.0f);
	if(!m_cloth)
	{
		return false;
	}

	// Initialize the model object.
	result = m_cloth->Initialize(m_D3D->GetDevice(), L"../Engine/textures/ukFlag.jpg");
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
	result = m_textureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void Renderer::Shutdown()
{
	// Release the color shader object.
	if(m_textureShader)
	{
		m_textureShader->Shutdown();
		delete m_textureShader;
		m_textureShader = 0;
	}

	if(m_cloth)
	{
		m_cloth->Shutdown();
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
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool Renderer::Frame(pjs::Solver* _solver, float _timeStep)
{
	bool result;

	m_cloth->Frame(_solver, _timeStep, m_D3D->GetDeviceContext());
	// Render the graphics scene.
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}


bool Renderer::Render()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_cloth->Render(m_D3D->GetDeviceContext());

	// Render the model using the color shader.
	result = m_textureShader->Render(m_D3D->GetDeviceContext(), m_cloth->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_cloth->GetTexture());
	if(!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}