////////////////////////////////////////////////////////////////////////////////
// Filename: Renderer.h
// based on code from http://www.rastertek.com/tutdx11.html April 2012
////////////////////////////////////////////////////////////////////////////////
#ifndef _Renderer_H_
#define _Renderer_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "D3D.h"
#include "Camera.h"
#include "Cloth.h"
#include "Solver.h"
#include "TextureShader.h"
#include "TessellationShader.h"
#include "Light.h"

#include "Text.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: Renderer
////////////////////////////////////////////////////////////////////////////////
class Renderer
{
public:
	Renderer();
	Renderer(const Renderer&);
	~Renderer();

	bool initialize(int, int, HWND);
	void shutdown();
	bool frame(int, int, int, pjs::Solver* _solver, float _timeStep);

	inline pjs::Cloth* cloth(){return m_cloth;}

	inline void moveMode(float _mode)
	{
		m_moveMode = _mode;
	}

	bool render();
private:
	bool renderUI();
	D3DClass* m_D3D;
	Camera* m_camera;
	pjs::Cloth* m_cloth;
	TextureShader* m_textureShader;
	TessellationShader* m_tessellationShader;
	Light* m_light;
	Text* m_text;
	float m_zoom, m_rotY, m_rotX;
	float m_moveMode;
};

#endif