//-----------------------------------------------------------------------------
// File: WinMain.cpp / Neuronomicon ver. 0.1
//
// Coded By Yoonsik Shim 2015.07.21.FRI.
//-----------------------------------------------------------------------------


// This is code reduction in 2022-10-11




#include <windows.h>
#include "resource.h"





//-------------------------------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-------------------------------------------------------------------------------------------------

HINSTANCE l_hInst = NULL;
HINSTANCE Get_hInstance( void ){	return l_hInst;		}

LRESULT WINAPI Viewer_MsgProc	( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

void LoopMain( void );

//INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
int WINAPI WinMain( _In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR szCmdLine, _In_ int iCmdShow )
{

	// Just for extra DialogBoxes called in other cpp files..
	l_hInst = hInst;


	WNDCLASSEX				wc;
	wc.cbSize			=	sizeof(wc);
	wc.style			=	CS_HREDRAW | CS_VREDRAW;	
	wc.cbWndExtra		=	0;
	wc.cbClsExtra		=	0;
	wc.hInstance		=	hInst;

	wc.hIcon		=	LoadIcon(hInst, (const char *)IDI_ICON_BIG);
	wc.hIconSm		=	LoadIcon(hInst, (const char *)IDI_ICON_SMALL);		

	wc.hCursor			=	NULL;
	wc.hbrBackground	=	(HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName		=	NULL;
	wc.lpszClassName	=	"The Frame";

	wc.lpfnWndProc = Viewer_MsgProc;

    RegisterClassEx( &wc );
	

	char* appName = "NeuronomicoN - Viewer";

    // Create the application's window
	HWND hWnd = CreateWindow(	"The Frame", 
								appName,
								WS_OVERLAPPEDWINDOW, 
								100, 100, 
								700, 700,
								GetDesktopWindow(), NULL, wc.hInstance, NULL );

	UpdateWindow( hWnd );
				
	
	MSG msg;	ZeroMemory( &msg, sizeof(msg) );
	
	while( msg.message!=WM_QUIT )
	{
	   if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
	   {
		  TranslateMessage( &msg );
		  DispatchMessage( &msg );
	   }
	   else
	   {
		   LoopMain( );
	   }
	}

	
	UnregisterClass( "The Frame", wc.hInstance );

    return 0;
}









#include <stdio.h>
#include <thread>

std::thread thread_Loop_Per_Step;

void Thread_Func_Loop_Per_Step( int cc );

void LOG( int idtKind , const char* _Format, ... );


#include "../common/neurouse/neurouse.h"
void Init_SIMULATION( ENUM_ROBOT rKind  );

void Delete_SIMULATION( void );

void InitRandomSeed( void );

void InitKeyHook( void );	void EndKeyHook( void );

void InitTextDisplay( void );

BOOL CALLBACK SelectProc( HWND, UINT, WPARAM, LPARAM );			// Select NetKind
BOOL CALLBACK PanelDialogProc( HWND, UINT, WPARAM, LPARAM );	// Main Control Panel



#include "Status.h"
STATUS* g_Status = NULL;	// for monitoring all states both in simulation & UIs


// WndProc for NeuronomicoN Viewer
LRESULT WINAPI Viewer_MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
									
    switch( msg )
    {
		case WM_CREATE:

		#ifdef _DEBUG
			// Logging - 2021-10-25
			AllocConsole();
			freopen("CONIN$", "rb", stdin);		freopen("CONOUT$", "wb", stdout);	freopen("CONOUT$", "wb", stderr);			
		#endif

			LOG( -2 , "*********************************\n" );
			LOG( -2 , "** Profiling NeuronomicoN Viewer **\n" );
			LOG( -2 , "*********************************\n" );

						
			g_Status = new STATUS();

			g_Status->g_hinstance = GetModuleHandle(NULL);
			g_Status->g_hwnd = hWnd;

			GetCurrentDirectory( _MAX_PATH , g_Status->RunDir );

			g_Status->is_Loop_Per_Step_Thread = false;



			
			InitRandomSeed( );

			Init_SIMULATION(   WALKER_QUAD  );
			
						
			g_Status->panelwnd = CreateDialog( g_Status->g_hinstance, MAKEINTRESOURCE( IDD_PANEL_DIALOG ),
												hWnd, (DLGPROC)PanelDialogProc );
			ShowWindow( g_Status->panelwnd, SW_SHOW );	
				
			g_Status->NN_Status_Wnd1 = GetDlgItem( g_Status->panelwnd, IDC_EDIT_STATUS1 );
			g_Status->NN_Status_Wnd2 = GetDlgItem( g_Status->panelwnd, IDC_EDIT_STATUS2 );		
			g_Status->NN_Status_Wnd3 = GetDlgItem( g_Status->panelwnd, IDC_EDIT_STATUS3 );
			g_Status->NN_Status_Wnd4 = GetDlgItem( g_Status->panelwnd, IDC_EDIT_STATUS_HOMEO );



			InitTextDisplay( );
			
			InitKeyHook( );
			

			if( g_Status->is_Loop_Per_Step_Thread )
			{
				thread_Loop_Per_Step = std::thread( Thread_Func_Loop_Per_Step , 0 );
				LOG( -2 , "\n*** Rendering Thread Created !! ***\n\n" );
			}
					
		return 0;

        
		case WM_CLOSE:

			if( g_Status->is_Loop_Per_Step_Thread )
			{
				g_Status->is_Loop_Per_Step_Thread = false;
				thread_Loop_Per_Step.join();
			}
				
			EndKeyHook();
			
			Delete_SIMULATION();

			delete g_Status;

			DestroyWindow( hWnd );

		#ifdef _DEBUG
			// Close Log
			fclose( stdin );	fclose( stdout );	fclose( stderr );
			FreeConsole();
		#endif

		return 0;
		
		case WM_DESTROY:
			PostQuitMessage(0);
        return 0; 

		default:
			return DefWindowProc( hWnd, msg, wParam, lParam );

    } 
	

}






//////////////  Logging -- 2021.10.30 //////////////////////////////
// Console Text Indentation is controlled by +1 , -1, or 0 (no-indent)

#ifdef _DEBUG
//doing something
#else
extern "C" {
	FILE* _iob = NULL;
}
#endif




static int g_Indent = 0;

void LOG( int idtKind , const char* _Format, ... )
{
#ifndef _DEBUG
	return;
#endif
    
	if( idtKind == -1 )		g_Indent--;

	if( idtKind > -2 )
	for( int i=0 ; i<g_Indent ; i++ )
	{
		printf( "   " );	// tab (indent)
	}


	va_list _ArgList;
    va_start(_ArgList, _Format);
    
	vprintf(_Format, _ArgList);

	if( idtKind > -2 )  printf("\n");

    va_end(_ArgList);


	if( idtKind == 1 )		g_Indent++;

}