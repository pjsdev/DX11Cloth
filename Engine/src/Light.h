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

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetSpecularColor(float, float, float, float);
	void SetDirection(float, float, float);
	void setSpecularPower(float);

	Vec4 GetSpecularColor();
	float getSpecularPower();
	Vec4 GetAmbientColor();
	Vec4 GetDiffuseColor();
	Vec3 GetDirection();

private:
	Vec4 m_ambientColor;
	Vec4 m_specularColor;
	Vec4 m_diffuseColor;
	Vec3 m_direction;
	float m_specularPower;
};

#endif