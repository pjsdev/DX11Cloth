////////////////////////////////////////////////////////////////////////////////
// Filename: Tessellationshaderclass.h
// based on code from http://www.rastertek.com/tutdx11.html April 2012
////////////////////////////////////////////////////////////////////////////////
#ifndef _TessellationSHADERCLASS_H_
#define _TessellationSHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include "Api.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: TessellationShaderClass
////////////////////////////////////////////////////////////////////////////////
class TessellationShader
{
private:
	struct MatrixBufferType
	{
		pjs::Matrix world;
		pjs::Matrix view;
		pjs::Matrix projection;
	};

	struct TessellationBufferType
	{
		float tessellationAmount;
		pjs::Vec3 padding;
	};

	//make sure we're 16 bit aligned for createBuffer()
	struct LightBufferType
	{
		pjs::Vec4 ambientColor;
		pjs::Vec4 diffuseColor;
		pjs::Vec3 lightDir;
		float padding;
	};

public:
	TessellationShader();
	TessellationShader(const TessellationShader&);
	~TessellationShader();

	bool initialize(ID3D11Device*, HWND);
	void shutdown();
	bool render(ID3D11DeviceContext*, int, pjs::Matrix, pjs::Matrix, pjs::Matrix, ID3D11ShaderResourceView*, pjs::Vec4, pjs::Vec4, pjs::Vec3, float);

private:
	bool initializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*, WCHAR*, WCHAR*);
	void shutdownShader();
	void outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool setShaderParameters(ID3D11DeviceContext*, pjs::Matrix, pjs::Matrix, pjs::Matrix, ID3D11ShaderResourceView*, pjs::Vec4, pjs::Vec4, pjs::Vec3, float);
	void renderShader(ID3D11DeviceContext*, int);

private:
	ID3D11HullShader* m_hullShader;
	ID3D11DomainShader* m_domainShader;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_tessellationBuffer;
	ID3D11Buffer* m_lightBuffer;
	ID3D11SamplerState* m_sampleState;
};

#endif