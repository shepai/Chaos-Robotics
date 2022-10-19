

//-------------------------------------------------------------------------------------------------
// Name: LoopMain()
// Desc: Main Loop including All
//-------------------------------------------------------------------------------------------------



#include "Status.h"

#include "../common/neurouse/neurouse.h"



bool Loop_All( int T );
void SaveGLHistory( void );
void SetHistoricRenderPrevMatrix( void );

// LOOP One Step !! 
void Loop_Per_Step( void )
{
		
	if( Loop_All(0) )	g_Status->goStep = false;
	

	// save data for the trail of OpenGL graph
	SaveGLHistory( );

	// For Drawing snapshots for DX rendering (SS-ON/OFF toggle button in DX window)
	if( g_Status->isDX3D )	SetHistoricRenderPrevMatrix( );

	
	g_Status->SimCount++;

}




int loopDelay = 1;//6000(real clock?);//8000;	// if >1, slowing down simulation
int slowCount = 0;



// Loop simulation in a separate thread (speed-up when displaying lots of realtime text)
void Thread_Func_Loop_Per_Step( int cc )
{	
	while( g_Status->is_Loop_Per_Step_Thread )
	{	
		if( g_Status->goStep )
		{
			if( loopDelay < 2 )
			{
				Loop_Per_Step( );	
			}
			else
			{
				if( slowCount % loopDelay < 1 )	Loop_Per_Step( );
				slowCount++;
				if( slowCount > loopDelay )	slowCount = 1;
			}	
		}
	}
}








DWORD fLastTime = 0;
static DWORD StaticFrame = 16;		// Rendering: 1000/16 = 62.5 FPS

DWORD fLastTimeText = 0;
static DWORD StaticFrameText = 48;	// text: 1000/50 = 20 FPS

static int stepCount = 0;

void TextDisplay( void );
void MotorTextDisplay( void );

void DX3DLoop( void );
void GL_Loop( void );


// This is the outer-most Loop !
void LoopMain( void )
{

	// If press F4
	if( g_Status->oneStep )
	{
		g_Status->goStep = false;
		Sleep(10);
		g_Status->oneStep = false;

		Loop_Per_Step( );	
	}
	

	// Simulation & Data Calculation -- If press F2
	if( !g_Status->is_Loop_Per_Step_Thread )
	{
		if( g_Status->goStep )
		{
			if( loopDelay < 2 )
			{
				Loop_Per_Step( );	
			}
			else
			{
				if( slowCount % loopDelay < 1 )	Loop_Per_Step( );
				slowCount++;
				if( slowCount > loopDelay )	slowCount = 1;
			}	
		}
	}



	// Rendering
	DWORD fTime = timeGetTime();

	if( fTime-fLastTime > StaticFrame )
	{
		fLastTime = fTime;
		
		GL_Loop( );

		if( g_Status->isDX3D && g_Status->isRenderDX )	DX3DLoop( );
	}


	// Realtime Window Text
	DWORD fTimeText = timeGetTime();

	if( fTimeText-fLastTimeText  > StaticFrameText )
	{
		fLastTimeText = fTimeText;

		// Main Panel text update
		TextDisplay( );

		if( g_Status->isMotorText )
		{			
			if( g_Status->isDX3D )	MotorTextDisplay();			
		}
			
	}



	// Simulate for fixed interval then stop WHEN limited_Step=true -- in dxWnd.cpp
	if( g_Status->limited_Step && g_Status->goStep )
	{
		stepCount++;

		if( stepCount == g_Status->limit_duration )
		{
			g_Status->goStep = false;
			stepCount = 0;
		}
	}
	
	
}



