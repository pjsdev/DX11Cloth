#ifndef API
#define API

#include <D3DX10math.h>

namespace pjs{

typedef D3DXVECTOR2 Vec2;
typedef D3DXVECTOR3 Vec3;
typedef D3DXVECTOR4 Vec4;

typedef D3DXMATRIX Matrix;

inline void matMakeIdentity(Matrix &_mat)
{
	D3DXMatrixIdentity(&_mat);
}

inline void matRotateY(Matrix &_mat, float _rot)
{
	Matrix tmp;
	D3DXMatrixRotationY(&tmp, _rot);
	D3DXMatrixMultiply(&_mat, &_mat, &tmp);
}

inline void matRotateZ(Matrix &_mat, float _rot)
{
	Matrix tmp;
	D3DXMatrixRotationZ(&tmp, _rot);
	D3DXMatrixMultiply(&_mat, &_mat, &tmp);
}

inline void matRotateX(Matrix &_mat, float _rot)
{
	Matrix tmp;
	D3DXMatrixRotationX(&tmp, _rot);
	D3DXMatrixMultiply(&_mat, &_mat, &tmp);
}

}
#endif
