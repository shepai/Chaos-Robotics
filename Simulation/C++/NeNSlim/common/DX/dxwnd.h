


#ifndef DXWND_H
#define DXWND_H

#include <windows.h>
#include "Robj.h"
#include "..\..\common\dynamics\rigidode.h"
#include "camera.h"



class DXHANDLE
{
public:
	
	RIGIDSTATE* rs;

	HWND		hWnd, motorWnd, bodyWnd;

	int			dxWidth, dxHeight;
	bool		lockcam, isIC, isBackGround, isGoraud;
	bool		isActive;
	bool		isDXText;

	bool		isText_AngVelTorq, isText_SenseAdapt, isText_AlphaWeight;
	int			alpha_Idx;
	
	bool		isSnapShot;
	bool		isWingTip;
	
	CAMERA*		cam;

	bool		is_bg_color;
	DWORD		bg_color, grid_color;
	DWORD		spring_color, muscle_color;
	
	DWORD		startTime;
	DWORD		endTime;
	DWORD		resTime;

	unsigned int flipCount;
	unsigned int frameRate;

	// snap shot
	int SS_COUNT;
	int SS_Interval;

	// wing or limb tip trajectory
	int NUM_TIP;
	int TIP_COUNT;
	int TIP_ITVL;

	DWORD tip_color[10];
	VEC3 prev_tip[10][MAX_DX_HISTORIC];
	


	// render object
	int NUM_OBJ, NUM_CCYL;
	float render_scale;
	ROBJ* robj;
	
	
	
	// Spring Net
	int NUM_SPRING, NUM_MUSCLE;
	int NUM_FIN, NUM_TOTAL_LINE;
	VEC3* sprVec;
	
	
	void (*Render)( int , int );


	DXHANDLE()	
	{
		rs				= NULL;

		hWnd			= NULL;
		motorWnd		= NULL;
		bodyWnd			= NULL;
		
		dxWidth			= 0;
		dxHeight		= 0;

		lockcam			= true;
		isIC			= false;
		isBackGround	= false;
		isGoraud		= false;

		isActive		= false;
		isDXText		= true;

		isText_AngVelTorq = isText_SenseAdapt = isText_AlphaWeight	= false;
		alpha_Idx		= 0;

		isSnapShot		= false;
		SS_COUNT		= 0;
		SS_Interval		= 1;

		isWingTip	= false;
		NUM_TIP		= 0;
		TIP_COUNT	= 0;
		TIP_ITVL	= 1;


		cam				= NULL;

		// FrameRate		
		startTime		= 0;
		endTime			= 0;
		resTime			= 0;
		flipCount		= 0;
		frameRate		= 0;


		// Render obj
		NUM_OBJ = NUM_CCYL = 0;
		render_scale = 100.0f / (float)G_LENGTH_SCALE;
		robj = NULL;
	
		
		
		// Spring Net
		NUM_SPRING = NUM_MUSCLE = NUM_FIN = NUM_TOTAL_LINE = 0;
		sprVec = NULL;
	}

	~DXHANDLE() 
	{
		delete cam;
		delete [] robj;
				
	}

	void Init_DXHANDLE( void );

	void SetRigidMatrix( int , int );
	void SetObjMatrix ( void );

	
};

DXHANDLE*	GetDXHANDLE( void );




#endif