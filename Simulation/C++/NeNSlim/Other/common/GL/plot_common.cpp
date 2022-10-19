





#include <windows.h>
#include <stdio.h>
#include <math.h>



#include "../resource.h"

#include "../NeuroUse/neurouse.h"
#include "../Status.h"
#include "oglwnd.h"

#include "../dynamics/rigidode.h"






void GLHANDLE::Add_Graph_Wnd_Plot_Common( HWND hDlg , HWND hList_select , HWND hCombo_select , int num_dim )
{
	char temp[50] = {0};

	curRadio = SendMessage( hCombo_select , CB_GETCURSEL , 0 , 0 );

	//IDC_EDIT_IDX
	int	 tempIdX = -1, tempIdY = -1, tempIdZ = -1;


	switch( num_dim )
	{
		case 1:
			GetWindowText( GetDlgItem( hDlg , IDC_EDIT_PLOT1D_X ) , temp , 50 );	tempIdX = atoi( temp );
		break;

		case 2:
			GetWindowText( GetDlgItem( hDlg , IDC_EDIT_PLOT2D_X ) , temp , 50 );	tempIdX = atoi( temp );
			GetWindowText( GetDlgItem( hDlg , IDC_EDIT_PLOT2D_Y ) , temp , 50 );	tempIdY = atoi( temp );
		break;

		case 3:
			GetWindowText( GetDlgItem( hDlg , IDC_EDIT_PLOT3D_X ) , temp , 50 );	tempIdX = atoi( temp );
			GetWindowText( GetDlgItem( hDlg , IDC_EDIT_PLOT3D_Y ) , temp , 50 );	tempIdY = atoi( temp );
			GetWindowText( GetDlgItem( hDlg , IDC_EDIT_PLOT3D_Z ) , temp , 50 );	tempIdZ = atoi( temp );
		break;


	}

/*	GetWindowText( GetDlgItem( hDlg , IDC_EDIT_PLOT_X ) , temp , 50 );	tempIdX = atoi( temp );

	if( num_dim > 1 )
	{
		GetWindowText( GetDlgItem( hDlg , IDC_EDIT_PLOT_Y ) , temp , 50 );	tempIdY = atoi( temp );
	}

	if( num_dim > 2 )
	{
		GetWindowText( GetDlgItem( hDlg , IDC_EDIT_PLOT_Z ) , temp , 50 );	tempIdZ = atoi( temp );
	}
*/


	AddGraphData( tempIdX , tempIdY , tempIdZ );

	if( graphData[num_graph_data-1]->hasIndex )
	{
			 if( num_dim == 1 ) sprintf( temp , "[%02d]"			, tempIdX );
		else if( num_dim == 2 )	sprintf( temp , "[%02d-%02d]"		, tempIdX , tempIdY );			
		else if( num_dim == 3 )	sprintf( temp , "[%02d-%02d-%02d]"	, tempIdX , tempIdY , tempIdZ );
		else{}
			
		strcat( graphData[num_graph_data-1]->dataName , temp );
	}

	SendMessage( hList_select , LB_ADDSTRING , 0, (LPARAM)graphData[num_graph_data-1]->dataName );

	SendMessage( hList_select , LB_SETCURSEL , num_graph_data-1 , 0 );

}

void GLHANDLE::Del_Graph_Wnd_Plot_Common( HWND hList_select )
{
	int curList = SendMessage( hList_select , LB_GETCURSEL , 0 , 0 );

	if( curList < 0 )	return;

	DelGraphData( curList );
	

	SendMessage( hList_select , LB_DELETESTRING , curList , 0 );
					

	if( curList < num_graph_data )	SendMessage( hList_select , LB_SETCURSEL , curList , 0 );
	else							SendMessage( hList_select , LB_SETCURSEL , num_graph_data-1 , 0 );

}




void Init_Plot_ComboBox( char** ComboNameList , HWND hCombo_select , int init_cur )
{
	int numComboItem = atoi( ComboNameList[0] ) + 1;
	for( int i=1 ; i<numComboItem ; i++ )
		SendMessage( hCombo_select , CB_ADDSTRING , 0 , (LPARAM)ComboNameList[i] );

	SendMessage (hCombo_select , CB_SETCURSEL , init_cur , 0);	// place cursor to zero position
}



float* Select_Line_Color( HWND hDlg )
{
	static float resCol[3];

	CHOOSECOLOR COL;
	COLORREF crTemp[16];

	memset( &COL , 0 , sizeof(CHOOSECOLOR) );

	COL.lStructSize  = sizeof(CHOOSECOLOR);
	COL.hwndOwner	 = hDlg;
	COL.lpCustColors = crTemp;
	COL.Flags		 = 0;

	if( ChooseColor(&COL) != 0 )
	{
		COLORREF Color = COL.rgbResult;

		resCol[0] = (float)GetRValue( Color ) / 255.0f;
		resCol[1] = (float)GetGValue( Color ) / 255.0f;
		resCol[2] = (float)GetBValue( Color ) / 255.0f;

		InvalidateRect(hDlg, NULL, TRUE);
	}

	return resCol;

}





static double PI2 = 6.283185307179586;

#define MAX_NUM_PHASE_DIFF	50

bool Convert_PhaseDiff_Torus_Multi( VEC3 res[MAX_NUM_PHASE_DIFF][5] , int num_line , VEC3** ppos , int hcnt , int prev_hcnt )
{

	double sepMargin = 3.141592653589793;

	bool isExpand = false;


	VEC3 prev[MAX_NUM_PHASE_DIFF] , curr[MAX_NUM_PHASE_DIFF];
	for( int i=0 ; i<num_line ; i++ )
	{
		prev[i] = ppos[i][prev_hcnt];
		curr[i] = ppos[i][hcnt  ];
	}


	int idx1[MAX_NUM_PHASE_DIFF], idx2[MAX_NUM_PHASE_DIFF];		// idx1[]: lines out-ranged to be processed
	int numIdx1=0, numIdx2=0;		// idx2[]: lines not out-ranged

	for( int i=0 ; i<num_line ; i++ )
	{
		prev[i].y = -prev[i].y;
		curr[i].y = -curr[i].y;

		if( fabs( prev[i].y-curr[i].y ) > sepMargin )
		{
			isExpand = true;

			idx1[numIdx1] = i;
			numIdx1++;
		}
		else
		{
			idx2[numIdx2] = i;
			numIdx2++;
		}
	}



	if( !isExpand )	return false;	/////////////////////////////////////////////


	// initially fill all positions
	for( int i=0 ; i<num_line ; i++ )
	{
		for( int h=0 ; h<5 ; h++ )	res[i][h].Set( -99.0, -99.0, -99.0 );

		res[i][0] = prev[i];
		res[i][4] = curr[i];
	}



	// Apr 22 2017 -- TODO: no need to be 5 --> change to 3 !!

	
	// lines to process
	for( int l=0 ; l<numIdx1 ; l++ )
	{
		int i = idx1[l];


		VEC3 currNew = curr[i];	// outer range points
			
		if( curr[i].y-prev[i].y < 0.0 )
		{
			currNew.y = curr[i].y + PI2;
			res[i][1].y = PI2;
			res[i][3].y = 0.0;
		}
		else
		{
			currNew.y = curr[i].y - PI2;
			res[i][1].y = 0.0;
			res[i][3].y = PI2;
		}
		
		res[i][1].x = prev[i].x  +  (currNew.x-prev[i].x) * (res[i][1].y-prev[i].y) / (currNew.y - prev[i].y);

		res[i][3].x = res[i][1].x;

	}


	// lines not to process
	for( int l=0 ; l<numIdx2 ; l++ )
	{
		int i = idx2[l];

		res[i][1] = res[i][3] = prev[i];
	}


	// negate y for GL draw
	for( int i=0 ; i<num_line ; i++ )
	{
		res[i][0].y = -res[i][0].y;
		res[i][1].y = -res[i][1].y;
		res[i][3].y = -res[i][3].y;
		res[i][4].y = -res[i][4].y;
	}

	
	return true;
}




bool Convert_PhaseDiff_Torus( VEC3 res[5] , VEC3 tmp_prev , VEC3 tmp_curr )
{
	bool isExpand = false;

	VEC3 prev = tmp_prev;
	VEC3 curr = tmp_curr;

	prev.y = -prev.y;	prev.z = -prev.z;
	curr.y = -curr.y;	curr.z = -curr.z;


	double sepMarginX = 3.141592653589793;
	double sepMarginY = 3.141592653589793;
	double sepMarginZ = 3.141592653589793;

	
	if( fabs( prev.x-curr.x ) > sepMarginX || 
		fabs( prev.y-curr.y ) > sepMarginY ||
		fabs( prev.z-curr.z ) > sepMarginZ		)
	{
		isExpand = true;

		for( int i=0 ; i<5 ; i++ )	res[i].Set( -99.0, -99.0, -99.0 );

		res[0] = prev;
		res[4] = curr;
							
		// [HC-1][ HC ]
		// ..changes into like..
		// [HC-4][HC-3][HC-2][HC-1][ HC ]
		// [prev][PI20][-99 ][PI20][curr]

		VEC3 currNew = curr;	// outer range points

		VEC3 prevNew = prev;



		bool isIntersect[3][2];		// isIntersect[x or y or z][big or small]

		

		for( int i=0 ; i<3 ; i++ )
			for( int j=0 ; j<2 ; j++ )
			{
				isIntersect[i][j] = false;
			}

				
		if( fabs( prev.x-curr.x ) > sepMarginX )
		{
			isIntersect[0][0] = true;

			if( curr.x-prev.x < 0.0 )
			{
				currNew.x = curr.x + PI2;
				prevNew.x = prev.x - PI2;

				isIntersect[0][1] = true;

			//	res[1].x = PI2;
			//	res[3].x = 0.0;
			}
			else
			{
				currNew.x = curr.x - PI2;
				prevNew.x = prev.x + PI2;

				isIntersect[0][1] = false;

			//	res[1].x = 0.0;
			//	res[3].x = PI2;
			}
		}
		

		if( fabs( prev.y-curr.y ) > sepMarginY )
		{
			isIntersect[1][0] = true;

			if( curr.y-prev.y < 0.0 )
			{
				currNew.y = curr.y + PI2;
				prevNew.y = prev.y - PI2;

				isIntersect[1][1] = true;

			//	res[1].y = PI2;
			//	res[3].y = 0.0;
			}
			else
			{
				currNew.y = curr.y - PI2;
				prevNew.y = prev.y + PI2;

				isIntersect[1][1] = false;

			//	res[1].y = 0.0;
			//	res[3].y = PI2;
			}
		}

		if( fabs( prev.z-curr.z ) > sepMarginZ )
		{
			isIntersect[2][0] = true;

			if( curr.z-prev.z < 0.0 )
			{
				currNew.z = curr.z + PI2;
				prevNew.z = prev.z - PI2;

				isIntersect[2][1] = true;

				res[1].z = PI2;
				res[3].z = 0.0;
			}
			else
			{
				currNew.z = curr.z - PI2;
				prevNew.z = prev.z + PI2;

				isIntersect[2][1] = false;

			//	res[1].z = 0.0;
			//	res[3].z = PI2;
			}
		}




		VEC3 norm[3][2];
		double D[3][2];

		norm[0][0].Set(  1.0 ,  0.0 ,  0.0 );		D[0][0] = PI2;
		norm[0][1].Set( -1.0 ,  0.0 ,  0.0 );		D[0][1] = 0.0;

		norm[1][0].Set(  0.0 ,  1.0 ,  0.0 );		D[1][0] = PI2;
		norm[1][1].Set(  0.0 , -1.0 ,  0.0 );		D[1][1] = 0.0;

		norm[2][0].Set(  0.0 ,  0.0 ,  1.0 );		D[2][0] = PI2;
		norm[2][1].Set(  0.0 ,  0.0 , -1.0 );		D[2][1] = 0.0;

		
		
		VEC3 toNew = currNew - prev;

		double T[3];
		
		for( int i=0 ; i<3 ; i++ )
		{
			T[i] = -76.0;	// init

			if( isIntersect[i][0] )
			{
				int k;
				if( isIntersect[i][1] )	k = 0;				
				else					k = 1;

				T[i] = (  D[i][k] - ( norm[i][k]*prev )  )  /  ( norm[i][k]*toNew );
			}
		}


		
		// Get min & max T
		double minT = 1.1, maxT = -0.1;

		for( int i=0 ; i<3 ; i++ )
		{
			if( T[i] > -70.0 )
			{
				if( T[i] < minT )	minT = T[i];

				if( T[i] > maxT )	maxT = T[i];
			}
		}


	
		// Get res[1] & res[3]
		res[1] = prev + toNew*minT;

		res[3] = prevNew + toNew*maxT;



		




		// interpolate X
/*		if( res[1].x < -90.0 )
		{
			res[1].x = prev.x  +  (currNew.x-prev.x) * (res[1].y-prev.y) / (currNew.y - prev.y);

			res[3].x = res[1].x;
		}

		// interpolate Y
		if( res[1].y < -90.0 )
		{
			res[1].y = prev.y  +  (currNew.y-prev.y) * (res[1].x-prev.x) / (currNew.x - prev.x);

			res[3].y = res[1].y;
		}

		// interpolate Z
		if( res[1].z < -90.0 )
		{
			res[1].z = prev.z  +  (currNew.z-prev.z) * (res[1].x-prev.x) / (currNew.x - prev.x);

			res[3].z = res[1].y;
		}
*/


		// This should be removed when using 3D
		res[0].y = -res[0].y;	res[0].z = -res[0].z;
		res[1].y = -res[1].y;	res[1].z = -res[1].z;
		res[3].y = -res[3].y;	res[3].z = -res[3].z;
		res[4].y = -res[4].y;	res[4].z = -res[4].z;


	}

	return isExpand;
}


