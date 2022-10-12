




#include "mat33.h"
#include <math.h>


void MAT33::Identity(void)
{
	_11 = _22 = _33 = 1.0;
	_12 = _13 = _21 = _23 = _31 = _32 = 0.0;
}


void MAT33::Zero(void)
{
	_11 = _22 = _33 = 0.0;
	_12 = _13 = _21 = _23 = _31 = _32 = 0.0;
}

void MAT33::Transpose(void)
{
	double _12t, _13t, _23t;

	_12t = _12;
	_13t = _13;
	_23t = _23;

	_12 = _21;
	_13 = _31;
	_23 = _32;

	_21 = _12t;
	_31 = _13t;
	_32 = _23t;
}

void MAT33::NormalizeColumn(void)
{
	VEC3 temp;

	temp.x = _11;
	temp.y = _21;
	temp.z = _31;
	temp.Normalize();
	_11 = temp.x;
	_21 = temp.y;
	_31 = temp.z;

	temp.x = _12;
	temp.y = _22;
	temp.z = _32;
	temp.Normalize();
	_12 = temp.x;
	_22 = temp.y;
	_32 = temp.z;

	temp.x = _13;
	temp.y = _23;
	temp.z = _33;
	temp.Normalize();
	_13 = temp.x;
	_23 = temp.y;
	_33 = temp.z;
}


void MAT33::RotateMatrixAbs( VEC3 vec , double a )
{
	_11 = vec.x*vec.x*(1.0-(double)cos(a))+(double)cos(a);
	_12 = vec.x*vec.y*(1.0-(double)cos(a))-vec.z*(double)sin(a);
	_13 = vec.x*vec.z*(1.0-(double)cos(a))+vec.y*(double)sin(a);
	

	_21 = vec.y*vec.x*(1.0-(double)cos(a))+vec.z*(double)sin(a);
	_22 = vec.y*vec.y*(1.0-(double)cos(a))+(double)cos(a);
	_23 = vec.y*vec.z*(1.0-(double)cos(a))-vec.x*(double)sin(a);
	

	_31 = vec.z*vec.x*(1.0-(double)cos(a))-vec.y*(double)sin(a);
	_32 = vec.z*vec.y*(1.0-(double)cos(a))+vec.x*(double)sin(a);
	_33 = vec.z*vec.z*(1.0-(double)cos(a))+(double)cos(a);

}

MAT33 MAT33::operator*( MAT33 mat )
{
	MAT33 res;

	res._11 = _11*mat._11 + _12*mat._21 + _13*mat._31;
	res._12 = _11*mat._12 + _12*mat._22 + _13*mat._32;
	res._13 = _11*mat._13 + _12*mat._23 + _13*mat._33;
	
	res._21 = _21*mat._11 + _22*mat._21 + _23*mat._31;
	res._22 = _21*mat._12 + _22*mat._22 + _23*mat._32;
	res._23 = _21*mat._13 + _22*mat._23 + _23*mat._33;
	
	res._31 = _31*mat._11 + _32*mat._21 + _33*mat._31;
	res._32 = _31*mat._12 + _32*mat._22 + _33*mat._32;
	res._33 = _31*mat._13 + _32*mat._23 + _33*mat._33;
	

	return res;

}
	

VEC3 MAT33::operator*( VEC3 v )
{
	VEC3 res;

	res.x = _11*(double)v.x + _12*(double)v.y + _13*(double)v.z;
	res.y = _21*(double)v.x + _22*(double)v.y + _23*(double)v.z;
	res.z = _31*(double)v.x + _32*(double)v.y + _33*(double)v.z;
	
	return res;

}


MAT33 MAT33::operator^( VEC3 w )
{
	MAT33	res;
	VEC3	temp;

	temp.x = _11;
	temp.y = _21;
	temp.z = _31;

	temp = w^temp;

	res._11 = temp.x;
	res._21 = temp.y;
	res._31 = temp.z;

	////
	temp.x = _12;
	temp.y = _22;
	temp.z = _32;

	temp = w^temp;

	res._12 = temp.x;
	res._22 = temp.y;
	res._32 = temp.z;

	////
	temp.x = _13;
	temp.y = _23;
	temp.z = _33;

	temp = w^temp;

	res._13 = temp.x;
	res._23 = temp.y;
	res._33 = temp.z;
	
	
	return res;

}



