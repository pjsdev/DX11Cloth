////////////////////////////////////////////////////////////////////////////////
// Filename: fontclass.h
// based on code from http://www.rastertek.com/tutdx11.html April 2012
////////////////////////////////////////////////////////////////////////////////
#ifndef _FONTCLASS_H_
#define _FONTCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
using namespace std;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Texture.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: FontClass
////////////////////////////////////////////////////////////////////////////////
class Font
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
	};

public:
	Font();
	Font(const Font&);
	~Font();

	bool initialize(ID3D11Device*, char*, WCHAR*);
	void shutdown();

	ID3D11ShaderResourceView* getTexture();

	void buildVertexArray(void*, char*, float, float);

private:
	bool loadFontData(char*);
	void releaseFontData();
	bool loadTexture(ID3D11Device*, WCHAR*);
	void releaseTexture();

private:
	FontType* m_font;
	Texture* m_texture;
};

#endif