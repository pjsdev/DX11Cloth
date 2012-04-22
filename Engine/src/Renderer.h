////////////////////////////////////////////////////////////////////////////////
// Filename: Renderer.h
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
	bool frame(pjs::Solver* _solver, float _timeStep);

	inline pjs::Cloth* cloth(){return m_cloth;}

private:
	bool render(float);

private:
	D3DClass* m_D3D;
	Camera* m_camera;
	pjs::Cloth* m_cloth;
	TextureShader* m_textureShader;
	TessellationShader* m_tessellationShader;
	Light* m_light;
};

#endif