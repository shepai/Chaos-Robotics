
#ifndef MAT33_H
#define MAT33_H

#include "vec3.h"





struct MAT33
{
public:
	union
	{
		struct
		{
			double _11, _12, _13;
			double _21, _22, _23;
			double _31, _32, _33;			
		};
		double m[3][3];
	};

	MAT33	operator*( MAT33 );
	VEC3	operator*( VEC3 );
	MAT33	operator^( VEC3 );

	void Identity(void);
	void Zero(void);
	void Transpose(void);
	void NormalizeColumn(void);
	
	
//	void TranslateMatrix( double tx, double ty, double tz );
//	void RotateMatrixX( double a );
//	void RotateMatrixY( double a );
//	void RotateMatrixZ( double a );
	void RotateMatrixAbs( VEC3 , double );
//	void RotateMatrixAbs( VECTOR3D , VECTOR3D , double );
//	void ScaleMatrix( double sx, double sy, double sz );

//	void MirrorMatrix( int X_or_Y );
//	void ShearMatrix( double shx, double shy, double shz );

};




#endif