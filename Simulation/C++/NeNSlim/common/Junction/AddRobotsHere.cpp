

#include "../NeuroUse/neurouse.h"



void RIGIDSTATE::SetRobotFuncName( void )
{
	int rKind = sim->robotKind;
	
	switch( rKind )
	{		
		

		
		case WALKER_QUAD:
			SetBodyFuncName			= &RIGIDSTATE::SetBody_Quad;
			SetSensorFuncName		= &RIGIDSTATE::SetSensor_Quad;
			AfterConstructFuncName	= &RIGIDSTATE::AfterConstruct_Quad;
		break;

		case WALKER_HEX:
			SetBodyFuncName			= &RIGIDSTATE::SetBody_Hex;
			SetSensorFuncName		= &RIGIDSTATE::SetSensor_Hex;
			AfterConstructFuncName	= &RIGIDSTATE::AfterConstruct_Hex;
		break;

		
		
		case WALKER_QUAD2:
			SetBodyFuncName			= &RIGIDSTATE::SetBody_Quad2;
			SetSensorFuncName		= &RIGIDSTATE::SetSensor_Quad2;
			AfterConstructFuncName	= &RIGIDSTATE::AfterConstruct_Quad2;
		break;

		case WALKER_QUAD3:
			SetBodyFuncName			= &RIGIDSTATE::SetBody_Quad3;
			SetSensorFuncName		= &RIGIDSTATE::SetSensor_Quad3;
			AfterConstructFuncName	= &RIGIDSTATE::AfterConstruct_Quad3;
		break;

		case WALKER_TRI:
			SetBodyFuncName			= &RIGIDSTATE::SetBody_Tri;
			SetSensorFuncName		= &RIGIDSTATE::SetSensor_Tri;
			AfterConstructFuncName	= &RIGIDSTATE::AfterConstruct_Tri;
		break;

		case STAR_ROBOT:
			SetBodyFuncName			= &RIGIDSTATE::SetBody_Star;
			SetSensorFuncName		= &RIGIDSTATE::SetSensor_Star;
			AfterConstructFuncName	= &RIGIDSTATE::AfterConstruct_Star;
		break;

		

	}



	

}



static char	robotName[50];
char*	GetRobotName( void ){	return robotName;	}



void OSCNET::SetController( void )
{
	
	switch( sim->robotKind )
	{
		case WALKER_QUAD:		sim->nrp.Select_NRPARAM( 8 );	break;
		case WALKER_QUAD2:		sim->nrp.Select_NRPARAM( 8 );	break;
		case WALKER_QUAD3:		sim->nrp.Select_NRPARAM( 8 );	break;

		case STAR_ROBOT:		sim->nrp.Select_NRPARAM( 8 );	break;
		
		case WALKER_HEX:		sim->nrp.Select_NRPARAM( 12 );	break;
		case WALKER_TRI:		sim->nrp.Select_NRPARAM( 6 );	break;		
	}


	sprintf( robotName, ENUM_ROBOT_NAME[sim->robotKind] );

}



#include "../resource.h"
#include "../Status.h"
#include "../NeuroUse/neurouse.h"

void RecomposeNN( char* );


#define NUM_C_BOX	6

static HWND hCombo[NUM_C_BOX] = {0,};
static int initCursor[NUM_C_BOX] = {0,};



BOOL CALLBACK SelectRobotProc( HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	int RK, subRadio;	// for wnd inits

	int cCurser = 0;
	
		
	switch( iMsg )
	{

		case WM_INITDIALOG:
						
			hCombo[0] = GetDlgItem( hWnd , IDC_COMBO_RIGID		);	// Rigid Robots
			hCombo[1] = GetDlgItem( hWnd , IDC_COMBO_SPRING_FIX	);	// SpringNets - Fixed
			hCombo[2] = GetDlgItem( hWnd , IDC_COMBO_SPRING_MOVE);	// SpringNets - Moving
			hCombo[3] = GetDlgItem( hWnd , IDC_COMBO_FIXED		);	// Fixed Limbs

			hCombo[4] = GetDlgItem( hWnd , IDC_COMBO_VOXEL		);	// Voxels


			hCombo[NUM_C_BOX-1] = GetDlgItem( hWnd , IDC_COMBO_EVAL		);	// Eval Method

			
			for( int i=WALKER_START ; i<WALKER_END ; i++ )
				SendMessage( hCombo[0] , CB_ADDSTRING , 0 , (LPARAM)ENUM_ROBOT_NAME[i] );
			
			

			// Set item from inits & histories
			RK = g_sim[0]->robotKind;

			subRadio	  = IDC_RADIO_RIGID;
			initCursor[0] = RK;
			


			CheckRadioButton( hWnd, IDC_RADIO_NO_ROBOT, IDC_RADIO_VOXEL, subRadio );

			for( int i=0 ; i<NUM_C_BOX ; i++ )		SendMessage ( hCombo[i] , CB_SETCURSEL , initCursor[i] , 0 );

		
		break;

		case WM_KEYDOWN:
			switch (wParam)
            {
                case VK_ESCAPE:
					PostMessage(hWnd, WM_CLOSE, 0, 0);
                break;
            }			
		break;
		
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
				case IDOK:					
								
					cCurser = WALKER_START + SendMessage( hCombo[0] , CB_GETCURSEL , 0 , 0 );				
					
					g_sim[0]->robotKind = (ENUM_ROBOT)cCurser;

					RecomposeNN( NULL );

					EndDialog( hWnd , 0 );

				break;


				case IDCANCEL:
					EndDialog( hWnd , 0 );
				break;


		
				
			}
		break;
			
	}


	return FALSE;

}

