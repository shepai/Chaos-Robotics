



#include <windows.h>
#include "../resource.h"


#include "oglwnd.h"




// All can be opened at the same time.

GLHANDLE*	glhPlot2D = NULL;
GLHANDLE*	glhPlot3D = NULL;

GLHANDLE*	glhPlotMultiOUT = NULL;
GLHANDLE*	Get_glh_MultiOUT( void ){	return glhPlotMultiOUT;		}

GLHANDLE*	glhMap2D = NULL;

GLHANDLE*	glhPNN = NULL;





////////////////////////// GL Common ////////////////////////////////////////////////////////////////////////////

void SaveGLHistory( void )
{
	if( g_Status->isPlot2d )		glhPlot2D->SaveGL_2D( );
	if( g_Status->isPlot3d )		glhPlot3D->SaveGL_3D( );

	if( g_Status->isPlotMultiOUT )	glhPlotMultiOUT->SaveGL_MultiOUT( );
}



#include "..\..\common\NeuroUse\neurouse.h"

void ResetGLPlot( void )
{
	if( g_Status->isPlot2d )			glhPlot2D->Zero( );
	if( g_Status->isPlot3d )			glhPlot3D->Zero( );	
	if( g_Status->isPlotMultiOUT )		glhPlotMultiOUT->Zero( );

}




bool refresh_gl_one_frame = false;
void Refresh_GL_One_Frame( void ){	refresh_gl_one_frame = true;	};

void GL_Loop( void )
{
	
	// Homeo Adapt
	if( g_Status->isPlotMultiOUT )
	{
		wglMakeCurrent( glhPlotMultiOUT->hDC , glhPlotMultiOUT->hRC );
		glhPlotMultiOUT->ResizeGL_MultiOUT(  );
		glhPlotMultiOUT->DrawGL_MultiOUT(  );
		SwapBuffers( glhPlotMultiOUT->hDC );	
	}


	if( g_Status->isPlot2d )
	{
		wglMakeCurrent( glhPlot2D->hDC , glhPlot2D->hRC );
		glhPlot2D->ResizeGL_2D(  );
		glhPlot2D->DrawGL_2D(  );
		SwapBuffers( glhPlot2D->hDC );	
	}

	if( g_Status->isPlot3d )
	{
		wglMakeCurrent( glhPlot3D->hDC , glhPlot3D->hRC );
		glhPlot3D->ResizeGL_3D(  );
		glhPlot3D->DrawGL_3D( );
		SwapBuffers( glhPlot3D->hDC );
		if( glhPlot3D->isActive )	glhPlot3D->cam->UpdateKeyBoard( glhPlot3D->lockcam );
	}
		
	// Weight Matrix
	if( g_Status->isPlot2dmap )
	{
		wglMakeCurrent( glhMap2D->hDC , glhMap2D->hRC );
		glhMap2D->ResizeGL_Map2D(  );
		glhMap2D->DrawGL_Map2D(  );
		SwapBuffers( glhMap2D->hDC );
	}

	

}



/////////////////////////////////// Plot 2D ////////////////////////////////////////////////////////////

LRESULT CALLBACK Plot2D_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT res = glhPlot2D->Plot2D_Proc( hDlg, message, wParam, lParam );

	if( res ){	delete glhPlot2D;	glhPlot2D = NULL;	}

	return res;
}



void GL_Main_Plot2D( HWND hWnd )
{
	glhPlot2D = new GLHANDLE;

	HWND dlg = CreateDialog( g_Status->g_hinstance, MAKEINTRESOURCE(IDD_DIALOG_2D_PLOT), 
								g_Status->g_hwnd, (DLGPROC)Plot2D_Proc );
	ShowWindow( dlg, SW_SHOW );
}




//////////////////////////////////// Plot 3D ////////////////////////////////////////////////////////////

LRESULT CALLBACK Plot3D_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT res = glhPlot3D->Plot3D_Proc( hDlg, message, wParam, lParam );

	if( res ){	delete glhPlot3D;	glhPlot3D = NULL;	}

	return res;
}


void GL_Main_Plot3D( HWND hWnd )
{
	glhPlot3D = new GLHANDLE;

	HWND dlg = CreateDialog( g_Status->g_hinstance, MAKEINTRESOURCE(IDD_DIALOG_3D_PLOT), 
								g_Status->g_hwnd, (DLGPROC)Plot3D_Proc );
	
	ShowWindow( dlg, SW_SHOW );

}



//////////////////////////// Plot Weight Windows /////////////////////////////////////////////////////////

LRESULT CALLBACK PlotMultiOUT_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT res = glhPlotMultiOUT->PlotMultiOUT_Proc( hDlg, message, wParam, lParam );

	if( res ){	delete glhPlotMultiOUT;	  glhPlotMultiOUT = NULL;	}

	return res;
}



void GL_Main_MultiOUT( HWND hWnd )
{
	glhPlotMultiOUT = new GLHANDLE;

	HWND dlg = CreateDialog( g_Status->g_hinstance, MAKEINTRESOURCE(IDD_DIALOG_OUTPUT_BIG), 
								g_Status->g_hwnd, (DLGPROC)PlotMultiOUT_Proc );
	ShowWindow( dlg, SW_SHOW );
}




//////////////////////////// Plot 2D Map /////////////////////////////////////////////////////////////////

LRESULT CALLBACK Map2D_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT res = glhMap2D->Map2D_Proc( hDlg, message, wParam, lParam );

	if( res ){	delete glhMap2D;	glhMap2D = NULL;	}

	return res;
}



void GL_Main_Map2D( HWND hWnd )
{
	glhMap2D = new GLHANDLE;

	HWND dlg = CreateDialog( g_Status->g_hinstance, MAKEINTRESOURCE(IDD_DIALOG_2D_MAP), 
								g_Status->g_hwnd, (DLGPROC)Map2D_Proc );
	ShowWindow( dlg, SW_SHOW );
}




