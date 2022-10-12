

#ifndef CAMERA_H
#define CAMERA_H

//#include "..\..\common\math\matrix3D.h"
#include "..\..\common\math\mat33.h"
#include <math.h>

class CAMERA
{
public:
//	VECTOR3D from, at, up, right, At;
	VEC3 from, at, up, right, At;

	float pitch;					// Camera Rotations
	float yaw;
	
	float radius;
	float HighSpd;
	float LowSpd;
	float angleRate;
	
	float initY;
	float initZ;
	float PITCH;
	float YAW;

	CAMERA();
	CAMERA( bool plot );
	CAMERA( float x, float y, float z, float pitch, float yaw );
	

//	void SetCam( VECTOR3D f , VECTOR3D a , VECTOR3D u );
	void SetCam( VEC3 f , VEC3 a , VEC3 u );
		
	void Go( float speed, bool lockY )
	{		
		from = from + (at*speed);

		if( lockY )
			from.y -= at.y*speed;

		At = from + at;
	}

	
	void Yaw( float angle );
	void Pitch( float angle );
	void Roll( float angle );

	void UpdateKeyBoard( bool );
};



#endif