
//////////////////////////////////////////////////////////
//
//  Junction of dynamics and DX
//
//////////////////////////////////////////////////////////





#include <windows.h>
#include "commctrl.h"

#include <stdio.h>

#include "../resource.h"


#include "../Status.h"

#include "input.h"
#include "dxwnd.h"
#include "../../common/NeuroUse/neurouse.h"








DXHANDLE*	dxhandle = NULL;
DXHANDLE*	GetDXHANDLE( void ){	return dxhandle;	}



HRESULT		InitD3D( DXHANDLE* );
HRESULT		InitGeometry( DXHANDLE* );
VOID		CleanupGeometry( DXHANDLE* );
VOID		CleanupGraphics(void);
VOID		Render( DXHANDLE* );
void		UpdateKeyBoard( CAMERA* , bool );


void BodyTextDisplay( void );




void SetDXSize( DXHANDLE* dxh )
{
	RECT rect;
	GetClientRect( dxh->hWnd , &rect );
	dxh->dxWidth  = rect.right - rect.left;
	dxh->dxHeight = rect.bottom - rect.top;
}



DWORD WINAPI DXInitialise( DXHANDLE* dxh )
{
	SetDXSize( dxh );
	InitD3D( dxh );	// don't change variables --> sharing with HW
	InitGeometry( dxh );

	
	return TRUE;
}
 

void DX3DLoop( void )
{
	dxhandle->SetObjMatrix (  );
	
	Render( dxhandle );
		
	if( dxhandle->isActive )	dxhandle->cam->UpdateKeyBoard( dxhandle->lockcam );
}


void DelDX( void )
{
	CleanupGeometry( dxhandle );
	CleanupGraphics();
}










// Snapshot, Wingtip & Other Auxilary Drawings !! --> This function is called every simulation tik.



VEC3* wTip;	// set &address at initialisation
void SetHistoricRenderPrevMatrix( void )
{
	if( dxhandle->isSnapShot )
	{
		if( g_Status->SimCount % dxhandle->SS_Interval < 1 )	
		{
			for( int i=0 ; i<dxhandle->NUM_OBJ-2 ; i++ )
			{			
				dxhandle->robj[i].prev_matRot[dxhandle->SS_COUNT] = dxhandle->robj[i].matRot;
				dxhandle->robj[i].prev_pos[dxhandle->SS_COUNT].x  = dxhandle->robj[i].tx;
				dxhandle->robj[i].prev_pos[dxhandle->SS_COUNT].y  = dxhandle->robj[i].ty;
				dxhandle->robj[i].prev_pos[dxhandle->SS_COUNT].z  = dxhandle->robj[i].tz;
			}
						
			dxhandle->SS_COUNT++;

			if( dxhandle->SS_COUNT == 1000 )	dxhandle->SS_COUNT = 0;

		}
	}


	
	

	// WingTip Trajectory
	if( dxhandle->isWingTip )
	{
		if( g_Status->SimCount % dxhandle->TIP_ITVL < 1 )	
		{
			for( int i=0 ; i<dxhandle->NUM_TIP ; i++ )
			{
				dxhandle->prev_tip[i][dxhandle->TIP_COUNT] = wTip[i] * 100.0;
			}

			dxhandle->TIP_COUNT++;

			if( dxhandle->TIP_COUNT == MAX_DX_HISTORIC )	dxhandle->TIP_COUNT = 0;
		}

	}


	
	
	
	

}




// small rendering window size = 257x257

////////////////////////////////////////////////////////////////////////////////////////

#include "../Junction/Enumerations.h"

bool	goraud;

void DeleteWOBJ(void);





void SetDXGridScale( int size, int wd );	// Grid Scale for Robots & Springs


void DXHANDLE::Init_DXHANDLE( void )
{
	rs = g_sim[0]->rs;

	isGoraud = goraud;

	is_bg_color = true;
	bg_color = D3DCOLOR_XRGB(150,150,250);
	grid_color = D3DCOLOR_XRGB(255,255,255);

	muscle_color = D3DCOLOR_XRGB(255,255,0);
	spring_color = D3DCOLOR_XRGB(0,255,0);
			
	// Initialize Camera
	cam = new CAMERA;
	cam->angleRate = 0.005f;
//	cam->angleRate = 0.01f;
						
	// ROBJ
	AllocROBJ( dxhandle );
	

	int rkind = g_sim[0]->robotKind;

	
		NUM_TIP = rs->NUM_TIP;

		if( rkind >= WALKER_START )
		{
			wTip = &rs->footTip[0];
		}
	
	

	tip_color[0] = 0xffff0000;
	tip_color[1] = 0xff0000ff;
	tip_color[2] = 0xff000000;
	tip_color[3] = 0xff00ff00;


	// Init Cam View -- Robots & Springs, ETC.
	
		cam->radius = 400.0f;
		SetDXGridScale( 20000, 200 );
	


	

}





void InitSimulation( int );
void EndSimulation( int );





// Realtime Damage Robot
void DisableBody( void )
{
	RIGIDSTATE* rs		= g_sim[0]->rs;
	OSCNET*		oscil	= g_sim[0]->oscil;

	
		int di = rs->NUM_ODE_BODY-1;
		
		dJointDestroy( rs->joint[di-1][0] );
		dBodyDisable( rs->body[di] );
		dBodyDestroy( rs->body[di] );
		dGeomDestroy( rs->bodygeom[di] );

		rs->NUM_ODE_BODY--;


		// Mark damaged alpha weights
		int damIdx1 = (di-1)*2;
		int damIdx2 = damIdx1 + 1;

		for( int a=0 ; a<oscil->num_alpha_neuron ; a++ )
		{
			oscil->alpha[a]->Damage( damIdx1 );
			oscil->alpha[a]->Damage( damIdx2 );
		}

		oscil->IaSen[damIdx1]->isAlive = oscil->IaSen[damIdx2]->isAlive = false;
		oscil->alpha[damIdx1]->isAlive = oscil->alpha[damIdx2]->isAlive = false;
		oscil->osc[damIdx1]->isAlive = oscil->osc[damIdx2]->isAlive = false;

}





// for checking rk4 var num
void RefreshFixedText( void );


void Quit_DX3D_Wnd( HWND hDlg , WPARAM wParam )
{	
	g_Status->goStep = false;

	g_Status->isDX3D = false;

	

	

					
	EndSimulation(0);
	g_sim[0]->EndRS();

					
	DI_Term();			
	DelDX( );
					
				
	Sleep(100);				
	EndDialog(hDlg, LOWORD(wParam));
	delete dxhandle;

	RefreshFixedText( );

	g_Status->DX3DWnd = NULL;

	LOG( -2 , "\n-------[Quit DX Wnd]----------------------------\n\n" );

}

LRESULT CALLBACK DX3DProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	RIGIDSTATE* rs		= g_sim[0]->rs;
	OSCNET*		oscil	= g_sim[0]->oscil;
	EVAL*		eval	= g_sim[0]->eval;

	ENUM_ROBOT rk;
	
	char temp[50];
			
	switch (message)
	{
		case WM_INITDIALOG:

			LOG( -2 , "\n-------[Init DX Wnd]----------------------------\n\n" );

			// restart rigidstate
			g_sim[0]->EndRS();
			g_sim[0]->InitRS();
												
			// Initialize Simulation
			InitSimulation(0);
			
			rs		= g_sim[0]->rs;
			oscil	= g_sim[0]->oscil;
			eval	= g_sim[0]->eval;
			
			g_Status->SimCount = 0;

			
			// DXHANDLE Initialise /////////////
			dxhandle = new DXHANDLE;
			dxhandle->Init_DXHANDLE( );
			////////////////////////////////////
			
			
			dxhandle->hWnd		= GetDlgItem( hDlg , IDC_EDIT_PLOT_3D		);		// render window
			dxhandle->motorWnd	= GetDlgItem( hDlg , IDC_LIST_MOTOR_STATUS	);		// realtime status
			dxhandle->bodyWnd	= GetDlgItem( hDlg , IDC_LIST_BODY_STATUS	);		// body parameters
			
			// Init DX
			DXInitialise( dxhandle );
			Sleep(10);
						
			BodyTextDisplay( );		// One time Init render & select DX/HW texts

			
			

			// Init Environment
			CheckRadioButton(hDlg , IDC_RADIO_TERRESTRIAL, IDC_RADIO_NO_ENV, IDC_RADIO_TERRESTRIAL );
			
			SetWindowText( GetDlgItem( hDlg , IDC_STATIC_SAMPLE_STATUS  ), "NONE" );
			SetWindowText( GetDlgItem( hDlg , IDC_EDIT_SS_INTERVAL		), "10" );

			// Check Buttons			
			rk = g_sim[0]->robotKind;
			
				
				if( oscil->isDoyaLearn		)	CheckDlgButton( hDlg, IDC_CHECK_LEARN_DOYA,				BST_CHECKED );
				if( oscil->isDoyaAlphaLearn )	CheckDlgButton( hDlg, IDC_CHECK_LEARN_DOYA_ALPHA,		BST_CHECKED );
				if( oscil->isSensorCut		)	CheckDlgButton( hDlg, IDC_CHECK_LEARN_SENSORCUT,		BST_CHECKED );
				if( oscil->is_Alpha_Reset	)	CheckDlgButton( hDlg, IDC_CHECK_LEARN_DOYA_ALPHA_RESET, BST_CHECKED );

				oscil->Toggle_fb_Str_Alpha( );
			


		
			
			// dz Mode
			if( eval )
			{
				switch( eval->fix_dzRate_Mode )
				{
					case  0:	CheckRadioButton(hDlg , IDC_RADIO_DZ_STABLE, IDC_RADIO_DZ_CHAOS, IDC_RADIO_DZ_STABLE );		break;
					case -1:	CheckRadioButton(hDlg , IDC_RADIO_DZ_STABLE, IDC_RADIO_DZ_CHAOS, IDC_RADIO_DZ_EVAL );		break;
					case  1:	CheckRadioButton(hDlg , IDC_RADIO_DZ_STABLE, IDC_RADIO_DZ_CHAOS, IDC_RADIO_DZ_CHAOS );		break;
				}
			}


			
			RefreshFixedText( );


			g_Status->isDX3D = true;

			

			
		return TRUE;



		case WM_ACTIVATE:
			if( wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE )
			{				
				DI_Term();
				InitDInput(hDlg);
				Sleep(100);
				dxhandle->isActive = true;
			}
			
			if( wParam == WA_INACTIVE )		dxhandle->isActive = false;

		break;
		
		
	
		case WM_COMMAND:

			if (HIWORD(wParam) == BN_CLICKED )
			{				
				switch (LOWORD(wParam)) 
				{ 
					

					// dz Mode
					case IDC_RADIO_DZ_STABLE:	eval->fix_dzRate_Mode =  0;	break;
					case IDC_RADIO_DZ_EVAL:		eval->fix_dzRate_Mode = -1;	break;
					case IDC_RADIO_DZ_CHAOS:	eval->fix_dzRate_Mode =  1;	break;


					case IDC_CHECK_LEARN_DOYA:						
						if( IsDlgButtonChecked(hDlg, IDC_CHECK_LEARN_DOYA) )	oscil->isDoyaLearn = true;
						else													oscil->isDoyaLearn = false;						
					break;

					case IDC_CHECK_LEARN_DOYA_ALPHA:
						if( IsDlgButtonChecked(hDlg, IDC_CHECK_LEARN_DOYA_ALPHA) )	oscil->isDoyaAlphaLearn = true;
						else														oscil->isDoyaAlphaLearn = false;

						oscil->Toggle_fb_Str_Alpha( );
					break;

					case IDC_CHECK_LEARN_SENSORCUT:						
						if( IsDlgButtonChecked(hDlg, IDC_CHECK_LEARN_SENSORCUT) )	oscil->isSensorCut = true;
						else														oscil->isSensorCut = false;						
					break;


					case IDC_CHECK_LEARN_DOYA_ALPHA_RESET:					
						oscil->Toggle_fb_Str_Alpha( );
					break;

				}				
			}


			

			switch( LOWORD(wParam) )
			{
				case IDC_BUTTON_ONOFF:
					g_Status->isRenderDX = !g_Status->isRenderDX;				
					InvalidateRect( GetDlgItem( hDlg , IDC_EDIT_PLOT_3D ) , NULL , TRUE );
				break;
						
				case IDC_BUTTON_LOCKCAM:	dxhandle->lockcam = !dxhandle->lockcam;					break;

				case IDC_BUTTON_BACKGROUND:	dxhandle->isBackGround = !dxhandle->isBackGround;		break;

				case IDC_BUTTON_TEXT_ANGVELTORQ:	dxhandle->isText_AngVelTorq = !dxhandle->isText_AngVelTorq;		break;
				case IDC_BUTTON_TEXT_SENSEADAPT:	dxhandle->isText_SenseAdapt = !dxhandle->isText_SenseAdapt;		break;
				case IDC_BUTTON_TEXT_ALPHAWEIGHT:	dxhandle->isText_AlphaWeight = !dxhandle->isText_AlphaWeight;	break;

				case IDC_BUTTON_TEXT_ALPHA_IDX_UP:
					dxhandle->alpha_Idx++;
					if( dxhandle->alpha_Idx == oscil->num_alpha_neuron )	dxhandle->alpha_Idx = 0;
				break;

				case IDC_BUTTON_TEXT_ALPHA_IDX_DOWN:
					dxhandle->alpha_Idx--;
					if( dxhandle->alpha_Idx < 0 )	dxhandle->alpha_Idx = oscil->num_alpha_neuron-1;
				break;


				case IDC_BUTTON_DXTEXT:		dxhandle->isDXText = !dxhandle->isDXText;				break;

				case IDC_BUTTON_BGCOLOR:
					dxhandle->is_bg_color = !dxhandle->is_bg_color;
					if( dxhandle->is_bg_color )
					{	
						dxhandle->bg_color = D3DCOLOR_XRGB(150,150,250);
						dxhandle->grid_color = D3DCOLOR_XRGB(255,255,255);

						dxhandle->spring_color = D3DCOLOR_XRGB(255,255,0);
						dxhandle->muscle_color = D3DCOLOR_XRGB(0,255,0);
					}
					else
					{
						dxhandle->bg_color = D3DCOLOR_XRGB(255,255,255);
						dxhandle->grid_color = D3DCOLOR_XRGB(0,0,0);

						dxhandle->spring_color = D3DCOLOR_XRGB(0,0,0);
						dxhandle->muscle_color = D3DCOLOR_XRGB(255,0,0);
					}
				break;

				
				case IDC_BUTTON_EVAL_ONOFF:
					g_Status->isMotorText = !g_Status->isMotorText;
					InvalidateRect( dxhandle->motorWnd , NULL , TRUE );
				break;


							
				
				case IDC_BUTTON_SNAPSHOT:
					
					g_Status->isRenderDX = false;

					dxhandle->SS_COUNT = 0;

					GetWindowText( GetDlgItem( hDlg , IDC_EDIT_SS_INTERVAL ) , temp , 10 );
					dxhandle->SS_Interval = atoi( temp );

					dxhandle->isSnapShot = !dxhandle->isSnapShot;

					if( dxhandle->isSnapShot )	sprintf( temp , "SS-%d", dxhandle->SS_Interval );
					else						sprintf( temp , "SS-OFF" );
					SetWindowText( GetDlgItem( hDlg , IDC_BUTTON_SNAPSHOT ) , temp );

					g_Status->isRenderDX = true;				
					
				break;


				// Simulate for fixed interval then stop
				case IDC_BUTTON_INTERMIT:
								
					GetWindowText( GetDlgItem( hDlg , IDC_EDIT_SS_INTERVAL ) , temp , 10 );
					g_Status->limit_duration = atoi( temp );

					g_Status->limited_Step = !g_Status->limited_Step;

					if( g_Status->limited_Step )		sprintf( temp , "SE-%d", g_Status->limit_duration );
					else								sprintf( temp , "SE-OFF" );
					SetWindowText( GetDlgItem( hDlg , IDC_BUTTON_INTERMIT ) , temp );
					
				break;


				
				case IDC_BUTTON_DAMAGE:					
					DisableBody( );
				break;

				
				case IDC_BUTTON_EXIT:
										
					PostMessage( hDlg , WM_CLOSE, 0, 0);
					

				break;

			

			}
			
		break;


		case WM_CLOSE:
			Quit_DX3D_Wnd( hDlg , wParam );
		break;

		case WM_SIZE:
		
		break;
		
	}

    return FALSE;

}




HWND dxDlg = NULL;

void DX3DMain( HWND hWnd , bool go )
{
	goraud	= go;	

	g_Status->DX3DWnd = CreateDialog( g_Status->g_hinstance, MAKEINTRESOURCE(IDD_DIALOG_DX3D), 
								g_Status->g_hwnd, (DLGPROC)DX3DProc );
	ShowWindow( g_Status->DX3DWnd, SW_SHOW );	

	
}







