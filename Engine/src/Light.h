////////////////////////////////////////////////////////////////////////////////
// Filename: light.h
// based on code from http://www.rastertek.com/tutdx11.html April 2012
////////////////////////////////////////////////////////////////////////////////
#ifndef _Light_H_
#define _Light_H_


//////////////
// INCLUDES //
//////////////
#include "Api.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Light
////////////////////////////////////////////////////////////////////////////////
class Light
{
public:
	Light();
	Light(const Light&);
	~Light();

	void setAmbientColor(float, float, float, float);
	void setDiffuseColor(float, float, float, float);
	void setSpecularColor(float, float, float, float);
	void setDirection(float, float, float);
	void setSpecularPower(float);

	pjs::Vec4 getSpecularColor();
	float getSpecularPower();
	pjs::Vec4 getAmbientColor();
	pjs::Vec4 getDiffuseColor();
	pjs::Vec3 getDirection();

private:
	pjs::Vec4 m_ambientColor;
	pjs::Vec4 m_specularColor;
	pjs::Vec4 m_diffuseColor;
	pjs::Vec3 m_direction;
	float m_specularPower;
};

#endif