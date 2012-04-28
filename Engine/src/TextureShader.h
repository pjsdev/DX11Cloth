////////////////////////////////////////////////////////////////////////////////
// Filename: textureshaderclass.h
// based on code from http://www.rastertek.com/tutdx11.html April 2012
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTURESHADERCLASS_H_
#define _TEXTURESHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include "Api.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: TextureShaderClass
////////////////////////////////////////////////////////////////////////////////
class TextureShader
{
private:
	struct MatrixBufferType
	{
		pjs::Matrix model;
		pjs::Matrix world;
		pjs::Matrix view;
		pjs::Matrix projection;
	};

	struct CameraBufferType
	{
		pjs::Vec3 cameraPosition;
		float padding;
	};

	//make sure we're 16 bit aligned for createBuffer()
	struct LightBufferType
	{
		pjs::Vec4 ambientColor;
		pjs::Vec4 diffuseColor;
		pjs::Vec3 lightDir;
		float specularPower;
		pjs::Vec4 specularColor;
	};

public:
	TextureShader();
	TextureShader(const TextureShader&);
	~TextureShader();

	bool initialize(ID3D11Device*, HWND);
	void shutdown();
	bool render(ID3D11DeviceContext*, int, pjs::Matrix,pjs::Matrix, pjs::Matrix, pjs::Matrix, 
		ID3D11ShaderResourceView*, pjs::Vec4, pjs::Vec4, pjs::Vec3, 
		float, pjs::Vec4, pjs::Vec3);

private:
	bool initializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void shutdownShader();
	void outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool setShaderParameters(ID3D11DeviceContext*, 
		pjs::Matrix,pjs::Matrix, pjs::Matrix, pjs::Matrix, 
		ID3D11ShaderResourceView*, pjs::Vec4, pjs::Vec4, pjs::Vec3, 
		float, pjs::Vec4, pjs::Vec3);
	void renderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_cameraBuffer;
	ID3D11Buffer* m_lightBuffer;
	ID3D11SamplerState* m_sampleState;
};

#endif