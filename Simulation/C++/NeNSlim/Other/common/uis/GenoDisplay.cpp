
#include <stdio.h>
#include <math.h>

#include "../Status.h"

#include "../NeuroUse/neurouse.h"






void OSCTextDisplay( void );


// Realtime Display
void TextDisplay( void )
{

	SendMessage( g_Status->NN_Status_Wnd1, LB_RESETCONTENT, 0, 0 );
	SendMessage( g_Status->NN_Status_Wnd3, LB_RESETCONTENT, 0, 0 );


	OSCTextDisplay( );

	
	char buffer[50];
	ZeroMemory( buffer, 50 );
	SendMessage( g_Status->NN_Status_Wnd1, LB_ADDSTRING, 0, (LPARAM)buffer );
	g_Status->rowCount[0]++;

	
	// Simulation Count & Simulation Time (sec)
	sprintf( buffer,"Step: %d  Time: %2.4fs", g_Status->SimCount , g_sim[0]->GetSimTime() );
	SendMessage( g_Status->NN_Status_Wnd1, LB_ADDSTRING, 0, (LPARAM)buffer );
	g_Status->rowCount[0]++;	


///////////////////////////////////////////////////////////////////////////////////////////////////////////////


	SendMessage( g_Status->NN_Status_Wnd1, WM_SETREDRAW, 1, 0 );
	SendMessage( g_Status->NN_Status_Wnd3, WM_SETREDRAW, 1, 0 );


}





void Init_OSC_TextDisplay( void );

void RefreshFixedText( void )
{
	for( int i=0 ; i<g_Status->rowCount[1] ; i++ )	SendMessage( g_Status->NN_Status_Wnd2, LB_DELETESTRING, 0, 0 );
	g_Status->rowCount[1] = 0;
	
	for( int i=0 ; i<g_Status->rowCount[3] ; i++ )	SendMessage( g_Status->NN_Status_Wnd4, LB_DELETESTRING, 0, 0 );
	g_Status->rowCount[3] = 0;
	

	char	buffer[50];
	sprintf( buffer ,"NUM-RK4: %d", g_sim[0]->rk4->num_rk4_var );
	SendMessage( g_Status->NN_Status_Wnd2, LB_ADDSTRING, 0, (LPARAM)buffer );
	g_Status->rowCount[1]++;


	Init_OSC_TextDisplay( );
	

	SendMessage( g_Status->NN_Status_Wnd2, WM_SETREDRAW, 1, 0 );
	SendMessage( g_Status->NN_Status_Wnd4, WM_SETREDRAW, 1, 0 );	
}

void InitTextDisplay( void )
{
//	tStat = GetStatus();

	HFONT hFont = CreateFont( 15 , 0 , 0 , 0 , 500 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );
	SendMessage( g_Status->NN_Status_Wnd1 , WM_SETFONT , (WPARAM)hFont , (LPARAM)FALSE );
	SendMessage( g_Status->NN_Status_Wnd2 , WM_SETFONT , (WPARAM)hFont , (LPARAM)FALSE );
	SendMessage( g_Status->NN_Status_Wnd3 , WM_SETFONT , (WPARAM)hFont , (LPARAM)FALSE );
	SendMessage( g_Status->NN_Status_Wnd4 , WM_SETFONT , (WPARAM)hFont , (LPARAM)FALSE );


	RefreshFixedText( );
	
}



