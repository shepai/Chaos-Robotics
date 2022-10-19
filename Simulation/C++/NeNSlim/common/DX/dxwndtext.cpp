



#include <stdio.h>

#include "dxwnd.h"
#include "../../common/dynamics/rigidode.h"
#include "../../common/NeuroUse/neurouse.h"



#define T_BUFFER	50







bool* l_isText_AngVelTorq = NULL;
bool* l_isText_SenseAdapt = NULL;
bool* l_isText_AlphaWeight = NULL;

int* l_alpha_Idx = NULL;

HWND g_bodyWnd, g_motorWnd;


int		rawcount[2] = {0,0};		// number of edit windows - 5




void MotorTextDisplay( void )
{

	char	buffer[T_BUFFER];
	

	SendMessage( g_motorWnd, LB_RESETCONTENT, 0, 0 );

	rawcount[0] = 0;
			

	
	OSCNET* oscil = g_sim[0]->oscil;
	RIGIDSTATE* rs = g_sim[0]->rs;
	EVAL*		eval	= g_sim[0]->eval;
	
	int rKind = g_sim[0]->robotKind;


	if( (*l_isText_AngVelTorq) )	// AVT button
	{

		for( int n=0 ; n<rs->NUM_ODE_BODY-1 ; n++ )
		{			
			for( int d=0 ; d<rs->motor[n].dof ; d++ )
			{
				sprintf( buffer,"J%d[%d]  A:%2.4f  V:%2.4f", 
								n, d, (float)rs->motor[n].angle[d] , (float)rs->motor[n].vel[d] );			
			
				SendMessage( g_motorWnd, LB_ADDSTRING, 0, (LPARAM)buffer );
				rawcount[0]++;	
			}
		}

		ZeroMemory( buffer, T_BUFFER );	SendMessage( g_motorWnd, LB_ADDSTRING, 0, (LPARAM)buffer );	rawcount[0]++;
		
		for( int n=0 ; n<rs->NUM_ODE_BODY-1 ; n++ )
		{			
			for( int d=0 ; d<rs->motor[n].dof ; d++ )
			{
				sprintf( buffer,"T%d[%d]:%2.9f", n, d, 	(float)rs->motor[n].torque[d] );

				SendMessage( g_motorWnd, LB_ADDSTRING, 0, (LPARAM)buffer );
				rawcount[0]++;	
			}
		}

		ZeroMemory( buffer, T_BUFFER );	SendMessage( g_motorWnd, LB_ADDSTRING, 0, (LPARAM)buffer );	rawcount[0]++;
		
	}
		


		
				if( (*l_isText_SenseAdapt) )
				{
					
					for( int c=0 ; c<oscil->num_osc ; c++ )
					{
						sprintf( buffer,"G%d:%2.6f O%d:%2.6f", c , oscil->IaSen[c]->sensorScale 
															 , c , oscil->IaSen[c]->sensorOffset );
						SendMessage( g_motorWnd, LB_ADDSTRING, 0, (LPARAM)buffer );
						rawcount[0]++;				
					}

					ZeroMemory( buffer, T_BUFFER );	SendMessage( g_motorWnd, LB_ADDSTRING, 0, (LPARAM)buffer );	rawcount[0]++;

				}


				

				if( (*l_isText_AlphaWeight) )
				{
					int a = (*l_alpha_Idx);
					for( int j=0 ; j<oscil->alpha[a]->num_weight ; j++ )
					{
						sprintf( buffer,"IaC[%02d-%02d]: %2.7f", a, j, oscil->alpha[a]->weight[j] );
						SendMessage( g_motorWnd, LB_ADDSTRING, 0, (LPARAM)buffer );
						rawcount[0]++;
					}

					ZeroMemory( buffer, T_BUFFER );	SendMessage( g_motorWnd, LB_ADDSTRING, 0, (LPARAM)buffer );	rawcount[0]++;
				}



				// Eval Display /////////////////////////////////////////////////////////////////////
				// Max Efficiency
				sprintf( buffer,"Inc:%2.2f Dec:%2.7f", eval->maxET_Inc , eval->maxET_Dec );
				SendMessage( g_motorWnd, LB_ADDSTRING, 0, (LPARAM)buffer );
				rawcount[0]++;

				sprintf( buffer,"maxE:%2.7f %2.7f", eval->maxE , eval->minE );
				SendMessage( g_motorWnd, LB_ADDSTRING, 0, (LPARAM)buffer );
				rawcount[0]++;

				// Efficiency
				sprintf( buffer,"EFF  : %2.7f", eval->EFFICIENCY );
				SendMessage( g_motorWnd, LB_ADDSTRING, 0, (LPARAM)buffer );
				rawcount[0]++;

				if( 1 )
				{
					double zr;
					if( eval->dzRate_y < eval->zEpsilon )	zr = 0.0;
					else									zr = eval->dzRate;
					sprintf( buffer,"dz: %2.15f", zr * eval->mdz );
				}
				SendMessage( g_motorWnd, LB_ADDSTRING, 0, (LPARAM)buffer );
				rawcount[0]++;

				// dzRate
				sprintf( buffer,"dzR: %2.15f", eval->dzRate );
				SendMessage( g_motorWnd, LB_ADDSTRING, 0, (LPARAM)buffer );
				rawcount[0]++;

						
				ZeroMemory( buffer, T_BUFFER );	SendMessage( g_motorWnd, LB_ADDSTRING, 0, (LPARAM)buffer );	rawcount[0]++;


				// Learning Switches
				sprintf( buffer,"Doya: %2.7f", oscil->transSigDoya );
				SendMessage( g_motorWnd, LB_ADDSTRING, 0, (LPARAM)buffer );
				rawcount[0]++;

				sprintf( buffer,"Alpha: %2.7f", oscil->transSigAlpha );
				SendMessage( g_motorWnd, LB_ADDSTRING, 0, (LPARAM)buffer );
				rawcount[0]++;

				sprintf( buffer,"SenCut: %2.7f", oscil->transSigSensor );
				SendMessage( g_motorWnd, LB_ADDSTRING, 0, (LPARAM)buffer );
				rawcount[0]++;

			
				ZeroMemory( buffer, T_BUFFER );	SendMessage( g_motorWnd, LB_ADDSTRING, 0, (LPARAM)buffer );	rawcount[0]++;


				sprintf( buffer,"z1:%2.4f z2:%2.4f", oscil->osc[oscil->IDX[0][0]]->z , oscil->osc[oscil->IDX[2][0]]->z );
				SendMessage( g_motorWnd, LB_ADDSTRING, 0, (LPARAM)buffer );
				rawcount[0]++;				
		

	SendMessage( g_motorWnd, WM_SETREDRAW, 1, 0 );

}



#define B_BUFFER	50

void SetTextWindow_DX_or_HW( bool isDX )
{
	
		l_isText_AngVelTorq = &GetDXHANDLE()->isText_AngVelTorq;
		l_isText_SenseAdapt = &GetDXHANDLE()->isText_SenseAdapt;
		l_isText_AlphaWeight = &GetDXHANDLE()->isText_AlphaWeight;

		l_alpha_Idx = &GetDXHANDLE()->alpha_Idx;

		g_bodyWnd = GetDXHANDLE()->bodyWnd;
		g_motorWnd = GetDXHANDLE()->motorWnd;
	
}

// Upper Text Window - Fixed
void BodyTextDisplay( void )
{
	SetTextWindow_DX_or_HW( true );
	
	// change Font
	HFONT hFont = CreateFont(15,0,0,0,500,0,0,0,0,0,0,0,0, 0 );
	SendMessage( GetDXHANDLE()->bodyWnd , WM_SETFONT , (WPARAM)hFont , (LPARAM)FALSE );
	SendMessage( GetDXHANDLE()->motorWnd , WM_SETFONT , (WPARAM)hFont , (LPARAM)FALSE );


	char	buffer[B_BUFFER];	
	
	for( int i=0 ; i<rawcount[1] ; i++ )	SendMessage( g_bodyWnd, LB_DELETESTRING, 0, 0 );

	rawcount[1] = 0;


	RIGIDSTATE* rs = g_sim[0]->rs;



	for( int n=0 ; n<rs->NUM_ODE_BODY ; n++ )
	{
		sprintf( buffer,"B%d  R:%2.4f  L:%2.4f",	n, (float)rs->node[n].R , (float)rs->node[n].L );
		SendMessage( g_bodyWnd, LB_ADDSTRING, 0, (LPARAM)buffer );
		rawcount[1]++;	
	}

							
	
		
}



