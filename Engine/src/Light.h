////////////////////////////////////////////////////////////////////////////////
// Filename: light.h
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

	Vec4 getSpecularColor();
	float getSpecularPower();
	Vec4 getAmbientColor();
	Vec4 getDiffuseColor();
	Vec3 getDirection();

private:
	Vec4 m_ambientColor;
	Vec4 m_specularColor;
	Vec4 m_diffuseColor;
	Vec3 m_direction;
	float m_specularPower;
};

#endif