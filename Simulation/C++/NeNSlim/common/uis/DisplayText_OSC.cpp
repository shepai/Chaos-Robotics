
#include <windows.h>
#include <stdio.h>

#include "../Status.h"
#include "../NeuroUse/neurouse.h"


#define	BUFFSIZE	50


void OSCTextDisplay( void )
{

	char buffer[BUFFSIZE];

	OSCNET*	oscil = g_sim[0]->oscil;
	
	if( g_Status->isOutputDisp )	// wnd-2
	{
		for( int i=0 ; i<oscil->num_osc ; i++ )
		{
			for( int j=0 ; j<oscil->osc[i]->num_node ; j++ )
			{
				sprintf( buffer,"[%d-%d] y:%+2.4f TCI:%2.4f P:%1.7f", i, j, 
				oscil->osc[i]->x[j], oscil->osc[i]->TCI , oscil->osc[i]->period );
				SendMessage( g_Status->NN_Status_Wnd3, LB_ADDSTRING, 0, (LPARAM)buffer );
				g_Status->rowCount[2]++;
			}
		}
	}

				
	if( g_Status->isInputDisp )	// wnd-1
	{
				
		
			for( int n=0 ; n<oscil->num_osc/2 ; n++ )
			{
				sprintf( buffer,"s[%d]: %2.7f %2.7f", n, oscil->senDisp[2*n] , oscil->senDisp[2*n+1] );
				SendMessage( g_Status->NN_Status_Wnd1, LB_ADDSTRING, 0, (LPARAM)buffer );
				g_Status->rowCount[0]++;
			}


			ZeroMemory( buffer, BUFFSIZE );
			SendMessage( g_Status->NN_Status_Wnd1, LB_ADDSTRING, 0, (LPARAM)buffer );
			g_Status->rowCount[0]++;
					
			// External Input Sum
			for( int n=0 ; n<oscil->num_node ; n++ )
			{
			//	sprintf( buffer,"Ext[%d]: %2.7f %2.10f", n, GetExinTotal_Display(n) , GetSensorCon_Display(n) );
				
				sprintf( buffer,"Ext[%d]: %2.7f %2.10f", n, 

						// total sum of inputs = sensor + weight-in
						oscil->osc[oscil->IDX[n][0]]->exin_sensor[oscil->IDX[n][1]] + 
						oscil->osc[oscil->IDX[n][0]]->exin_con_disp[oscil->IDX[n][1]],

						// weight-in sum
						oscil->osc[oscil->IDX[n][0]]->exin_con_disp[oscil->IDX[n][1]] );

				SendMessage( g_Status->NN_Status_Wnd1, LB_ADDSTRING, 0, (LPARAM)buffer );
				g_Status->rowCount[0]++;
			}

			ZeroMemory( buffer, BUFFSIZE );
			SendMessage( g_Status->NN_Status_Wnd1, LB_ADDSTRING, 0, (LPARAM)buffer );
			g_Status->rowCount[0]++;


			// External Input Sum
			for( int i=0 ; i<oscil->num_alpha_neuron ; i++ )
			{
				sprintf( buffer,"alphaExin[%d]: %2.7f", i, oscil->alpha[i]->exin );

				SendMessage( g_Status->NN_Status_Wnd1, LB_ADDSTRING, 0, (LPARAM)buffer );
				g_Status->rowCount[0]++;

			}

			ZeroMemory( buffer, BUFFSIZE );
			SendMessage( g_Status->NN_Status_Wnd1, LB_ADDSTRING, 0, (LPARAM)buffer );
			g_Status->rowCount[0]++;

		}


		// phase diff
		for( int i=0 ; i<oscil->num_osc-1 ; i++ )
		{
			if( oscil->pDiff_Alpha )
				sprintf( buffer,"pCoord[%d]: osc:%2.7f alpha:%2.7f", i, oscil->pDiff_Osc->pCoord[i] , oscil->pDiff_Alpha->pCoord[i] );
			else
				sprintf( buffer,"pCoord[%d]: %1.7f = %1.7f pi", i, oscil->pDiff_Osc->pCoord[i] , oscil->pDiff_Osc->pCoord[i] / 3.141592 );

			SendMessage( g_Status->NN_Status_Wnd1, LB_ADDSTRING, 0, (LPARAM)buffer );
			g_Status->rowCount[0]++;
		}

	

}



void Init_OSC_TextDisplay( void )
{
//	STATUS* g--Stat = GetStatus();

	char	buffer[BUFFSIZE];

	OSCNET*	oscil = g_sim[0]->oscil;

	sprintf( buffer ,"StepSize: %1.4f", oscil->StepSize );
	SendMessage( g_Status->NN_Status_Wnd2, LB_ADDSTRING, 0, (LPARAM)buffer );
	g_Status->rowCount[1]++;

		
	if		( g_Status->repeatMode == 0 )	sprintf( buffer ,"Loop Mode: Forever" );
	else if	( g_Status->repeatMode == 1 )	sprintf( buffer ,"Loop Mode: Stop at Duration" );
	else if	( g_Status->repeatMode == 2 )	sprintf( buffer ,"Loop Mode: Repeat" );

	SendMessage( g_Status->NN_Status_Wnd2, LB_ADDSTRING, 0, (LPARAM)buffer );
	g_Status->rowCount[1]++;


	if( g_Status->repeatMode > 0 )
	{
		sprintf( buffer ,"Duration: %d steps", g_Status->duration );
		SendMessage( g_Status->NN_Status_Wnd2, LB_ADDSTRING, 0, (LPARAM)buffer );
		g_Status->rowCount[1]++;
	}
	

	// OSC init file name
	SendMessage( g_Status->NN_Status_Wnd2, LB_ADDSTRING, 0, (LPARAM)g_Status->oscInitFileName );
	g_Status->rowCount[1]++;


}


// when just ResetNN was pressed
void RefreshFixedText( void ); // delete screen
void Reset_OSC_Text_Fixed( int tmp_pCoord_count , int total_repeat )
{
	RefreshFixedText( );

//	STATUS* g_Status = GetStatus();

	char	buffer[BUFFSIZE];

	sprintf( buffer ,"init count: %d of %d", tmp_pCoord_count , total_repeat );
	SendMessage( g_Status->NN_Status_Wnd4, LB_ADDSTRING, 0, (LPARAM)buffer );	g_Status->rowCount[3]++;
}





/*	
		*/