
#include <windows.h>
#include <stdio.h>

#include <commctrl.h>
#include <objbase.h>

#include "../resource.h"

#include "../Status.h"

#include "../NeuroUse/neurouse.h"
#include "../dynamics/rigidode.h"


BOOL CALLBACK InitParamProc	 ( HWND, UINT, WPARAM, LPARAM );
BOOL CALLBACK InfoProc		 ( HWND, UINT, WPARAM, LPARAM );
BOOL CALLBACK SelectRobotProc( HWND, UINT, WPARAM, LPARAM );




void InitTextDisplay( void );
void TextDisplay( void );

void GL_Main_Plot2D( HWND );
void GL_Main_Plot3D( HWND );

void GL_Main_MultiOUT( HWND );
void GL_Main_Map2D( HWND );

void DX3DMain( HWND , bool );

void Zero_System	( bool );
void RecomposeNN	( char* );


BOOL CALLBACK PanelDialogProc( HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	static	OPENFILENAME	ofn;
	static	char			file[500], fileName[500];
	static	BOOL			fileNewFlag = TRUE;		


	switch( iMsg )
	{
		case WM_INITDIALOG:

			g_Status->panelwnd = hWnd;
			
			CoInitialize(NULL);		// Wow! that was tough..		

			ofn.lStructSize			=	sizeof(OPENFILENAME);
			ofn.hwndOwner			=	hWnd;
			ofn.hInstance			=	NULL;
			ofn.lpstrFilter			=	"Neuronomicon NET File (*.net) \0 *.net";
			ofn.lpstrCustomFilter	=	NULL;
			ofn.nMaxCustFilter		=	0;
			ofn.nFilterIndex		=	0;
			ofn.nMaxFile			=	_MAX_PATH;
			ofn.nMaxFileTitle		=	_MAX_FNAME + _MAX_EXT;
			ofn.lpstrInitialDir		=	g_Status->RunDir;
			ofn.nFileOffset			=	0;
			ofn.lCustData			=	0L;
			ofn.lpfnHook			=	NULL;
			ofn.lpTemplateName		=	NULL;
			ofn.lpstrDefExt			=	"net";

			SetWindowText( hWnd , "NeuronomicoN Control Center - Oscillator" );	// Panel system bar Text


		return TRUE;


		

				
		case WM_COMMAND:
			
			switch( LOWORD(wParam) )
			{				
				case IDC_BUTTON_EXIT:

					g_Status->goStep = false;

					if( g_Status->isDX3D )	PostMessage( g_Status->DX3DWnd , WM_CLOSE, 0, 0 );

					if( !g_Status->isPlot2d && !g_Status->isPlot3d )
					{
						DestroyWindow( hWnd );
						PostMessage( g_Status->g_hwnd , WM_CLOSE, 0, 0);
					}

				break;
				
				
				case IDC_BUTTON_INFO:
					DialogBox( g_Status->g_hinstance , (LPSTR)IDD_DIALOG_INFO , hWnd , InfoProc );
				break;

				case IDC_BUTTON_RECOMPOSE:					
					RecomposeNN( NULL );
				break;


				case IDC_BUTTON_RESET:
					Zero_System( false );
				break;
				
				
				case IDC_BUTTON_ONESTEP:	g_Status->oneStep = true;					break;

				case IDC_BUTTON_GOSTEP:		g_Status->goStep = !g_Status->goStep;		break;



				// DX Robot Simulation //////////////////////////////////////////////////////////////////////

				case IDC_BUTTON_ROBOT3D:
					
						if( g_Status->isDX3D )
						{
							MessageBox( hWnd , "Close Robot Window !" , "Sorry, Cheers" , 0 );
							break;
						}
											
					
						if(IsDlgButtonChecked(hWnd, IDC_CHECK_GORAUD))	DX3DMain( hWnd , true );
						else											DX3DMain( hWnd , false );
						

				break;


				case IDC_BUTTON_SELECT_ROBOT:

						if( g_Status->isDX3D ){	MessageBox( hWnd , "Close Robot Window !" , "Sorry, Cheers" , 0 );	break;	}
						
						DialogBox( g_Status->g_hinstance , (LPSTR)IDD_DIALOG_SELECT_ROBOT , hWnd , SelectRobotProc );
				break;




				// OpenGL Plot 2D & 3D Control //////////////////////////////////////////////////////////////////////
				
				case IDC_BUTTON_3D:
					if( !g_Status->isPlot3d )
					{
						
							if( g_sim[0]->GetNumNode() > 2 )	GL_Main_Plot3D( hWnd );
							else					MessageBox( hWnd , "Use 2D Plot Window !" , "Sorry, Cheers" , 0 );
						
					}
				break;

				case IDC_BUTTON_2D:
					if( !g_Status->isPlot2d )	GL_Main_Plot2D( hWnd );					
				break;

				// Multi-Out Graph
				case IDC_BUTTON_MULTI_OUT:
					if( !g_Status->isPlotMultiOUT )	GL_Main_MultiOUT( hWnd );
				break;


				// Display realtime variables ON/OFF ////////////////////////////////////////////////////////////

				case IDC_BUTTON_INPUT_DISPLAY:
					g_Status->isInputDisp = !g_Status->isInputDisp;					
					if( g_Status->isInputDisp )	SetWindowText( GetDlgItem( hWnd, IDC_BUTTON_INPUT_DISPLAY ) , "ON" );
					else						SetWindowText( GetDlgItem( hWnd, IDC_BUTTON_INPUT_DISPLAY ) , "OFF" );
				break;

				case IDC_BUTTON_OUTPUT_DISPLAY:
					g_Status->isOutputDisp = !g_Status->isOutputDisp;
					if( g_Status->isOutputDisp )	SetWindowText( GetDlgItem( hWnd, IDC_BUTTON_OUTPUT_DISPLAY ) , "ON" );
					else							SetWindowText( GetDlgItem( hWnd, IDC_BUTTON_OUTPUT_DISPLAY ) , "OFF" );
				break;



				

				// Doya Weight 2D Map
				case IDC_BUTTON_WEIGHT_2D_MAP:
					if( !g_Status->isPlot2dmap )	GL_Main_Map2D( hWnd );
				break;

		
				
			}


			// Radio Buttons & Check Boxes
			if (HIWORD(wParam) == BN_CLICKED )
			{				
				switch (LOWORD(wParam)) 
				{
					case IDC_CHECK_RANDOM:		break;
					case IDC_CHECK_LOOPDELAY:	break;					
				}
			}
			
		break;
			
	}

	return FALSE;



}



BOOL CALLBACK InfoProc( HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam )
{		
	switch( iMsg )
	{
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
				case IDOK:					
					EndDialog( hWnd , 0 );
				break;
			}
		break;
	}
	return FALSE;
}


