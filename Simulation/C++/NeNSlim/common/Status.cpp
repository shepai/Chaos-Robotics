


#include <stdio.h>

#include "status.h"


STATUS::STATUS()
{		
		isMT			= false;
		
		isWind			= false;
		targetObject	= false;
		
		g_hwnd			= NULL;
		g_hinstance		= NULL;
		
		panelwnd		= DX3DWnd = NULL;

		NN_Status_Wnd1	= NULL;
		NN_Status_Wnd2	= NULL;
		NN_Status_Wnd3	= NULL;
		NN_Status_Wnd4	= NULL;

		for( int i=0 ; i<4 ; i++ )	rowCount[i] = 0;
	
		oneStep			= false;
		goStep			= false;



		isPlot2d		= false;
		isPlot3d		= false;
		isPlot2dPortrait = false;
		isPlot3dPortrait = false;
		isPlotOut		= false;
		isDrawNN		= false;
		isDX3D			= false;
		isDX2D			= false;
		
		isRenderDX		= true;
		isMotorText		= true;

		isHW			= false;

		isPlotMultiOUT	= false;
		isPlot2dmap		= false;
		isPlotPNN		= false;
		
		
		isInputDisp		= false;
		isOutputDisp	= false;
		is_Oscil_CTRNN_Disp = false;

		is_Loop_Per_Step_Thread	= false;

		SimCount		= 0;
		dynaStep		= 0.0;




		repeatMode		= 0;
		total_repeat	= 50;
		duration		= 896000;  //896000;

		shut_Down_After_Finish = false;


		for( int i=0 ; i<20 ; i++ )		sample_itvl[i] = 0;


		limited_Step = false;
		limit_duration = 100;

		sprintf( oscInitFileName ,"No File Loaded" );


		elapsedTime = 0;
			
}

