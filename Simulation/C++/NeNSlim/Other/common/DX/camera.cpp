

#include "camera.h"



CAMERA::CAMERA()
{
	pitch		= 0.0f;
	yaw			= 0.0f;
	PITCH		= 15.0f*3.141592f/180.0f;
	YAW			= 0.0f;

	HighSpd		= 5.0f;
	LowSpd		= 2.0f;
	angleRate	= 0.002f;
	
	initY		=  0.0f;
	initZ		=  0.0f;
	radius		= 100.0f;
	
	from.x  = 0.0f;		from.y  = initY;	from.z  = initZ;	//from.w  = 1.0f;
	at.x    = 0.0f;		at.y    = 0.0f;		at.z    = 1.0f;		//at.w    = 1.0f;
	right.x = 1.0f;		right.y = 0.0f;		right.z = 0.0f;		//right.w = 1.0f;
	up.x    = 0.0f;		up.y    = 1.0f;		up.z    = 0.0f;		//up.w    = 1.0f;

	Yaw( YAW );
	Pitch( PITCH );
}




// only in plot3d & portrait3d
CAMERA::CAMERA( bool plot )
{
	pitch		= 0.0f;
	yaw			= 0.0f;
	PITCH		= 15.0f*3.141592f/180.0f;
	YAW			= 0.0f;
	
	HighSpd		= 0.05f;
	LowSpd		= 0.025f;
	angleRate	= 0.002f;
	
	initY		=  0.0f;
	initZ		=  0.0f;
	radius		= 40.0f;
	
	from.x  = 0.0f;		from.y  = initY;	from.z  = initZ;	//from.w  = 1.0f;
	at.x    = 0.0f;		at.y    = 0.0f;		at.z    = 1.0f;		//at.w    = 1.0f;
	right.x = 1.0f;		right.y = 0.0f;		right.z = 0.0f;		//right.w = 1.0f;
	up.x    = 0.0f;		up.y    = 1.0f;		up.z    = 0.0f;		//up.w    = 1.0f;
	
	Yaw( YAW );
	Pitch( PITCH );
}


CAMERA::CAMERA( float x, float y, float z, float pitch, float yaw )
{
	from.x  = x;		from.y  = y;		from.z  = z;		//from.w  = 1.0f;
	at.x    = 0.0f;		at.y    = 0.0f;		at.z    = 1.0f;		//at.w    = 1.0f;
	right.x = 1.0f;		right.y = 0.0f;		right.z = 0.0f;		//right.w = 1.0f;
	up.x    = 0.0f;		up.y    = 1.0f;		up.z    = 0.0f;		//up.w    = 1.0f;

	Yaw( yaw );
	Pitch( pitch );
}



void CAMERA::SetCam( VEC3 f , VEC3 a , VEC3 u )
{

}


void CAMERA::Yaw( float angle )
{
//	MATRIX3D mat;
	MAT33 mat;
	mat.RotateMatrixAbs( up , angle );
		
	at = mat * at;
	at.Normalize();	
	right = up^at;
	right.Normalize();
	
}

void CAMERA::Pitch( float angle )
{
//	MATRIX3D mat;
	MAT33 mat;

	mat.RotateMatrixAbs( right , angle );
		
	at = mat * at;
	at.Normalize();	
}

void CAMERA::Roll( float angle )
{
		
}



