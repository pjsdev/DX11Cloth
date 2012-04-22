////////////////////////////////////////////////////////////////////////////////
// Filename: Light.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Light.h"


Light::Light()
{
}


Light::Light(const Light& other)
{
}


Light::~Light()
{
}

void Light::SetSpecularColor(float red, float green, float blue, float alpha)
{
	m_specularColor = Vec4(red, green, blue, alpha);
	return;
}

void Light::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = Vec4(red, green, blue, alpha);
	return;
}


void Light::SetAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambientColor = Vec4(red, green, blue, alpha);
	return;
}

void Light::SetDirection(float x, float y, float z)
{
	m_direction = Vec3(x, y, z);
	return;
}

void Light::setSpecularPower(float _pow)
{
	m_specularPower = _pow;
	return;
}

float Light::getSpecularPower()
{
	return m_specularPower;
}

Vec4 Light::GetSpecularColor()
{
	return m_specularColor;
}

Vec4 Light::GetAmbientColor()
{
	return m_ambientColor;
}

Vec4 Light::GetDiffuseColor()
{
	return m_diffuseColor;
}


Vec3 Light::GetDirection()
{
	return m_direction;
}
