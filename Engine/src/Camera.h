////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
// based on code from http://www.rastertek.com/tutdx11.html April 2012
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>

#include "Api.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class Camera
{
public:
	Camera();
	Camera(const Camera&);
	~Camera();

	void setPosition(float, float, float);
	void setRotation(float, float, float);

	D3DXVECTOR3 getPosition();
	D3DXVECTOR3 getRotation();

	void render();
	void getViewMatrix(pjs::Matrix&);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	pjs::Matrix m_viewMatrix;
};

#endif