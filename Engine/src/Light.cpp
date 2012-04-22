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

void Light::setSpecularColor(float red, float green, float blue, float alpha)
{
	m_specularColor = Vec4(red, green, blue, alpha);
	return;
}

void Light::setDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = Vec4(red, green, blue, alpha);
	return;
}


void Light::setAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambientColor = Vec4(red, green, blue, alpha);
	return;
}

void Light::setDirection(float x, float y, float z)
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

Vec4 Light::getSpecularColor()
{
	return m_specularColor;
}

Vec4 Light::getAmbientColor()
{
	return m_ambientColor;
}

Vec4 Light::getDiffuseColor()
{
	return m_diffuseColor;
}


Vec3 Light::getDirection()
{
	return m_direction;
}
