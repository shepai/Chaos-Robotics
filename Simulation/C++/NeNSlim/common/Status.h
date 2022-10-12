


#ifndef STATUS_H
#define STATUS_H


#include <windows.h>

#include "..\common\math\vec3.h"



// Global states including simulation & UI altogether (only a few variables are used currently...)


class STATUS
{
public:
		
	char		RunDir[_MAX_PATH];
	HWND		g_hwnd;
	HINSTANCE	g_hinstance;
		
	RECT		rect;
	
	HWND		panelwnd;
	HWND		DX3DWnd;
	

	HWND		NN_Status_Wnd1, NN_Status_Wnd2, NN_Status_Wnd3, NN_Status_Wnd4;
	int			rowCount[4];

	
	bool		oneStep, goStep;

	bool		isPlot2d, isPlot3d, isPlot2dPortrait, isPlot3dPortrait, isPlotOut, isDrawNN, isPlotPNN;
	
	bool		is_Loop_Per_Step_Thread;

	bool		isDX3D, isRenderDX, isDX2D, isMotorText;

	bool		isHW;
	bool		isPlotMultiOUT, isPlot2dmap;

	bool		isInputDisp , isOutputDisp , is_Oscil_CTRNN_Disp;
	
	
	// write data files
	int			sample_itvl[20];
//	int			sample_Robot, sample_Eff, sample_Neuron;
//	int			sample_SenSO, sample_RawSen, sample_OscCon, sample_Poincare;

	int			SimCount;
	double		dynaStep;

		
	// temp
	bool		targetObject;
	bool		isWind;
	
	// thread
	bool		isMT;


	// Repeat Setting
	int			repeatMode;
	int			total_repeat;
	int			duration;

	bool shut_Down_After_Finish;


	bool		limited_Step;
	int			limit_duration;


	char		oscInitFileName[500];

	DWORD		elapsedTime;	// measure time per frame

	STATUS();


	~STATUS()
	{
		
	}

};


extern STATUS* g_Status;
//STATUS* GetStatus( void );





#endif