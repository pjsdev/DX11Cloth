#ifndef API
#define API

#include <D3DX10math.h>

typedef D3DXVECTOR2 Vec2;
typedef D3DXVECTOR3 Vec3;
typedef D3DXVECTOR4 Vec4;

typedef D3DXMATRIX Matrix;

inline void makeIdentity(Matrix &_mat)
{
	D3DXMatrixIdentity(&_mat);
}

#endif
