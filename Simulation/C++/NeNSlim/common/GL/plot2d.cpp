





#include <windows.h>
#include <stdio.h>
#include <math.h>



#include "../resource.h"

#include "../NeuroUse/neurouse.h"
#include "../Status.h"
#include "oglwnd.h"

#include "../dynamics/rigidode.h"




bool Convert_PhaseDiff_Torus( VEC3 res[5] , VEC3 tmp_prev , VEC3 tmp_curr );

void SaveGL_OneShot( GLGRAPHDATA* gd , int num_hist );

void GLHANDLE::SaveGL_2D( void )
{

	// Connected to graphData[]->doUpdate; (bit wierd..)
	if( g_Status->SimCount % hPer < 1  )	
		is_Normal_Line_Update = true;
	else									is_Normal_Line_Update = false;


		
		for( int g=0 ; g<num_graph_data ; g++ )
		{
			if( *graphData[g]->doUpdate )
			{

				if( graphData[g]->lineKind == 2 )
				{
					graphData[g]->SaveGL_OneShot( ); // for dz range display
					continue;
				}


				int hcnt = graphData[g]->HCOUNT;

				for( int i=0 ; i<graphData[g]->num_line ; i++ )
				{						
					graphData[g]->ppos[i][hcnt].x =  ( *(graphData[g]->cposX[i]) );
					graphData[g]->ppos[i][hcnt].y = -( *(graphData[g]->cposY[i]) );
					graphData[g]->ppos[i][hcnt].z = 0.0;
				}


				// phase diff line
				if( graphData[g]->lineKind == 1 )
				{
					// Process Torus
					int prev_hcnt = hcnt-1;
					if( prev_hcnt < 0 )	prev_hcnt = num_history-1;

					VEC3 pExpand[5];
					if( Convert_PhaseDiff_Torus( pExpand ,	graphData[g]->ppos[0][prev_hcnt] , 
															graphData[g]->ppos[0][hcnt]		)  )
					{
						for( int h=0 ; h<3 ; h++ )
						{
							graphData[g]->HCOUNT++;
							if( graphData[g]->HCOUNT == num_history )	graphData[g]->HCOUNT = 0;
						}

						int endh = graphData[g]->HCOUNT;
						for( int b=0 ; b<5 ; b++ )
						{
							graphData[g]->ppos[0][endh] = pExpand[4-b];

							endh--;
							if( endh == -1 )	endh = num_history-1;
						}
					}

				}

				graphData[g]->HCOUNT++;
				if( graphData[g]->HCOUNT == num_history )	graphData[g]->HCOUNT = 0;
			}
		}
		


}






static bool isGLClear = true;
bool isGL_2D_Grid = true;

void DelGL( GLHANDLE* glh );
double GetOut( int , int , int , int );



// Oscil-Robot

static HWND hCombo_select = NULL;
static HWND hList_select = NULL;
static char** ComboNameList = NULL;

void Init_Plot_ComboBox( char** , HWND , int );

char** Get_GL_Data_Name_Plot2D( void );	// for combo box

float* Select_Line_Color( HWND hDlg );

static int num_plot2D_dim = 2;

double transUnit = 1.0;

LRESULT GLHANDLE::Plot2D_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	char buf[50];
	RECT pRect;
//	bool isRadioButtonClicked = false;

//	int	 tempIdX, tempIdY;
	char temp[50] = {0};

	// Color Select
	float* colRGB;

	// for wide/small double
	double wdthX,wdthY, cnptX,cnptY;	
	double halfWidth;

	
	switch (message)
	{
		case WM_INITDIALOG:
		
			hWnd = GetDlgItem( hDlg , IDC_EDIT_RENDER );
			
			InitGL( this );
			

			// Select Drawing Data from ComboBox
			hCombo_select = GetDlgItem( hDlg , IDC_COMBO_SELECT );
			hList_select  = GetDlgItem( hDlg , IDC_LIST_DATA );

			
			SetWindowText( GetDlgItem( hDlg , IDC_STATIC_LINETRACK_STATUS ) , "ON" );			

			CheckRadioButton(hDlg , IDC_RADIO_Y, IDC_RADIO_FR, IDC_RADIO_Y );
		//	CheckRadioButton(hDlg , IDC_RADIO_Y, IDC_RADIO_FR, IDC_RADIO_DY );


					
			// Update edit boxes
			SetWindowText( GetDlgItem( hDlg , IDC_EDIT_PLOT2D_X )		, "0" );
			SetWindowText( GetDlgItem( hDlg , IDC_EDIT_PLOT2D_Y )		, "1" );
			
			CheckDlgButton( hDlg, IDC_CHECK_HISTORIC, BST_CHECKED );
			CheckDlgButton( hDlg, IDC_CHECK_GL_CLEAR, BST_CHECKED );
			CheckDlgButton( hDlg, IDC_CHECK_GL_GRID , BST_CHECKED );





			hPer	= 10;			// how much sample datapoint ?
			num_history = 1000;

			SetWindowText( GetDlgItem( hDlg , IDC_EDIT_PLOT_MAX )	, _itoa(num_history, buf,10) );


			
			// Fill combo Text & Set Cursor to init position
			ComboNameList = Get_GL_Data_Name_Plot2D( );
			curRadio = 0;
			Init_Plot_ComboBox( ComboNameList , hCombo_select , curRadio );


			// This time we want to initially add below two graphs . . .			
			SendMessage (hCombo_select , CB_SETCURSEL , 7 , 0);
			Add_Graph_Wnd_Plot_Common( hDlg , hList_select , hCombo_select , num_plot2D_dim );

			SendMessage (hCombo_select , CB_SETCURSEL , 9 , 0);
			Add_Graph_Wnd_Plot_Common( hDlg , hList_select , hCombo_select , num_plot2D_dim );
			
			isLeadingPoint = true;

			XMIN = -2.0;	XMAX = 8.0;
			YMIN = -8.0;	YMAX = 2.0;


			// Adjust Screen Size & Pos Again
			SendMessage (hDlg , WM_SIZE , 0 , 0);

			g_Status->isPlot2d = true;

		break;

		
		case WM_COMMAND:

			// Radio Buttons - Just for switching to phase diff - Apr 2017
			if (HIWORD(wParam) == BN_CLICKED )
			{				
				switch (LOWORD(wParam)) 
				{ 
					case IDC_RADIO_Y:	// normal graphs

						SendMessage (hDlg , WM_COMMAND , IDC_BUTTON_VIEW_C , 0);	// Center Screen

						while( num_graph_data > 0 ){	Del_Graph_Wnd_Plot_Common( hList_select );	}

					break;
					
					case IDC_RADIO_DY:	// phase diff

						isLeadingPoint = true;

						XMIN = -2.0;	XMAX = 8.0;
						YMIN = -8.0;	YMAX = 2.0;

					//	while( num_graph_data > 0 ){	Del_Graph_Wnd_Plot_Common( hList_select );	}

						SendMessage (hCombo_select , CB_SETCURSEL , 7 , 0);
						Add_Graph_Wnd_Plot_Common( hDlg , hList_select , hCombo_select , 2 );

					//	SendMessage (hCombo_select , CB_SETCURSEL , 0 , 0);

					break;	


					case IDC_CHECK_HISTORIC:
						if( IsDlgButtonChecked(hDlg, IDC_CHECK_HISTORIC) )		historic_render = true;
						else													historic_render = false;						
					break;

					case IDC_CHECK_GL_CLEAR:
						if( IsDlgButtonChecked(hDlg, IDC_CHECK_GL_CLEAR) )		isGLClear = true;
						else													isGLClear = false;						
					break;

					case IDC_CHECK_GL_GRID:
						if( IsDlgButtonChecked(hDlg, IDC_CHECK_GL_GRID) )		isGL_2D_Grid = true;
						else													isGL_2D_Grid = false;						
					break;
					
				}				
			}


			// Push Buttons ////////////////////////////////////////////////////////////////'

			switch (LOWORD(wParam))
			{			
			
				case IDC_BUTTON_IS_SENSOR:			isSensor = !isSensor;				break;
				case IDC_BUTTON_IS_LEADINGPOINT:	isLeadingPoint = !isLeadingPoint;	break;
				case IDC_BUTTON_IS_INITPOINT:		isInitPoint = !isInitPoint;			break;
			

				case IDC_BUTTON_EXIT:
				
					g_Status->isPlot2d = false;
					DelGL( this );			
					EndDialog(hDlg, LOWORD(wParam));
				
				return TRUE;
				
				
				case IDC_BUTTON_VIEW_WIDE:
					XMAX += 1.0;	XMIN -= 1.0;	YMAX += 1.0;	YMIN -= 1.0;
				break;
				
				case IDC_BUTTON_VIEW_SMALL:
					XMAX -= 1.0;	XMIN += 1.0;	YMAX -= 1.0;	YMIN += 1.0;
				break;

				
				
				case IDC_BUTTON_VIEW_WIDE_2:
					wdthX = (XMAX-XMIN)*1.0;
					cnptX = (XMAX+XMIN)*0.5;
					wdthY = (YMAX-YMIN)*1.0;
					cnptY = (YMAX+YMIN)*0.5;

					XMAX = cnptX+wdthX;	XMIN = cnptX-wdthX;	YMAX = cnptY+wdthY;	YMIN =cnptY-wdthY;

					halfWidth = (XMAX-XMIN)*0.5;
					transUnit = 1.0;
					if( halfWidth > 19.99 )	transUnit = halfWidth/10.0;
					if( halfWidth <  1.01 )	transUnit = halfWidth/5.0;
				break;

				case IDC_BUTTON_VIEW_SMALL_2:
					wdthX = (XMAX-XMIN)*0.25;
					cnptX = (XMAX+XMIN)*0.5;
					wdthY = (YMAX-YMIN)*0.25;
					cnptY = (YMAX+YMIN)*0.5;

					XMAX = cnptX+wdthX;	XMIN = cnptX-wdthX;	YMAX = cnptY+wdthY;	YMIN =cnptY-wdthY;

					halfWidth = (XMAX-XMIN)*0.5;
					transUnit = 1.0;
					if( halfWidth > 19.99 )	transUnit = halfWidth/10.0;
					if( halfWidth <  1.01 )	transUnit = halfWidth/5.0;
				break;



				case IDC_BUTTON_VIEW_U:			YMAX -= transUnit;	YMIN -= transUnit;	break;
				case IDC_BUTTON_VIEW_D:			YMAX += transUnit;	YMIN += transUnit;	break;
				case IDC_BUTTON_VIEW_L:			XMAX -= transUnit;	XMIN -= transUnit;	break;
				case IDC_BUTTON_VIEW_R:			XMAX += transUnit;	XMIN += transUnit;	break;

				case IDC_BUTTON_VIEW_C:			XMAX = 2.0;	XMIN = -2.0;	YMAX = 2.0;	YMIN = -2.0;	break;


				
				
				
				case IDC_BUTTON_SETXY:
					Del_Graph_Wnd_Plot_Common( hList_select );							// delete current listBoxed graph							
					Add_Graph_Wnd_Plot_Common( hDlg , hList_select , hCombo_select , 2 );	// Add current comboBoxed graph
				break;				

				case IDC_BUTTON_ADDXY:
					Add_Graph_Wnd_Plot_Common( hDlg , hList_select , hCombo_select , 2 );

					if( curRadio == 10 )
					{
						SendMessage (hCombo_select , CB_SETCURSEL , 11 , 0);
						Add_Graph_Wnd_Plot_Common( hDlg , hList_select , hCombo_select , 2 );

						SendMessage (hCombo_select , CB_SETCURSEL , 12 , 0);
						Add_Graph_Wnd_Plot_Common( hDlg , hList_select , hCombo_select , 2 );
					}
				break;				

				case IDC_BUTTON_DELXY:
					Del_Graph_Wnd_Plot_Common( hList_select );
				break;


				// Choose & Apply Colour to the Graphs !! - Apr 2017
				// http://soen.kr/lecture/win32api/lec8/lec8-5-2.htm
				case IDC_BUTTON_COLOR:

					colRGB = Select_Line_Color( hDlg );

					for( int c=0 ; c<3 ; c++ )
					graphData[SendMessage( hList_select , LB_GETCURSEL , 0 , 0 )]->lineColour[0][c] = colRGB[c];


				break;


				// Show all OSC output
				case IDC_BUTTON_ADD_ALL:

					while( num_graph_data > 0 ){	Del_Graph_Wnd_Plot_Common( hList_select );	}

					SendMessage (hCombo_select , CB_SETCURSEL , 2 , 0);

					Add_Graph_Wnd_Plot_Common( hDlg , hList_select , hCombo_select , 2 );

				break;

				// Show OSC & Alpha
				case IDC_BUTTON_OSC_AND_ALPHA:

					while( num_graph_data > 0 ){	Del_Graph_Wnd_Plot_Common( hList_select );	}

					SendMessage (hCombo_select , CB_SETCURSEL , 8 , 0);

					Add_Graph_Wnd_Plot_Common( hDlg , hList_select , hCombo_select , 2 );

				break;



				case IDC_BUTTON_SET_MAXPLOT:	// change num_history
					GetWindowText( GetDlgItem( hDlg , IDC_EDIT_PLOT_MAX ) , buf , 50 );
					ReAlloc_Line_History( atoi(buf) );
				break;
			}

			
		break;


		case WM_SIZE:			
			GetClientRect( hDlg , &pRect );
		//	glWidth  = pRect.right - pRect.left - 0;
			glWidth  = pRect.right - pRect.left - 172;
			glHeight = pRect.bottom - pRect.top - 70;
			SetWindowPos( hWnd , NULL , 0, 70-0, glWidth, glHeight, 0 );

			
			SetWindowPos( hList_select , NULL , glWidth+2, 70-0, 170, glHeight/2, 0 );

			SetWindowPos( hCombo_select , NULL , glWidth+2, 4, 0, 0, SWP_NOSIZE );
			
		break;

		
	}
    return FALSE;

}







char glbuf[50];



double GetOscDrawPoint( int n )
{
	OSCNET* oscil = g_sim[0]->oscil;

	
	return oscil->osc[oscil->IDX[n][0]]->v[oscil->IDX[n][1]];
	
}


void GLHANDLE::DrawGL_2D( void )									// Here's Where We Do All The Drawing
{
	if( isGLClear )	glClear(GL_COLOR_BUFFER_BIT);

/*	// Small grid
	glLineWidth (0.5);
	glColor3f (0.2F, 0.2F, 0.2F);
	glBegin (GL_LINES);
	for ( double i = XMIN ; i <= XMAX ; i += 0.1 )
	{
		glVertex2f (i,YMIN);
		glVertex2f (i,YMAX);
	}

	for ( i = YMIN ; i <= YMAX ; i += 0.1 )
	{
		glVertex2f (XMIN,i);
		glVertex2f (XMAX,i);
	};

	glEnd ();
*/
	
	// Medium Grid
	if( isGL_2D_Grid )
	{
		glLineWidth (0.7);
		glColor3f (0.4F, 0.4F, 0.4F);
		glBegin (GL_LINES);

			for ( float i = XMIN ; i < XMAX ; i += 1.0f )
			{
				glVertex2f (i, YMIN);
				glVertex2f (i, YMAX);
			}
			
			for ( int i = YMIN ; i < YMAX ; i += 1.0f )
			{
				glVertex2f (XMIN, i);
				glVertex2f (XMAX, i);
			};

		glEnd ();
	}


	// Edge quad
	glColor3f (1.0F, 1.0F, 1.0F);
	glLineWidth (1.2);
	glBegin (GL_LINE_LOOP);
		glVertex2f (XMIN, YMIN);
		glVertex2f (XMIN, YMAX);
		glVertex2f (XMAX, YMAX);
		glVertex2f (XMAX, YMIN);
	glEnd ();


	// Main axes
	glColor3f (1.0F, 1.0F, 1.0F);
	glLineWidth (1.2);
	glBegin (GL_LINES);
		glVertex2f (XMIN, 0.0);
		glVertex2f (XMAX, 0.0);
		glVertex2f (0.0, YMIN);
		glVertex2f (0.0, YMAX);
	glEnd ();



	// Center of Rotation
	if( isGL_2D_Grid )
	{
		int sIdX,sIdY;
		if( num_graph_data > 0 ){	sIdX = graphData[0]->idX[0];	sIdY = graphData[0]->idY[0];	}
		else					{	sIdX = 0;						sIdY = 1;						}

		double Sx = g_sim[0]->GetSingular( sIdX );
		double Sy = g_sim[0]->GetSingular( sIdY );

		glColor3f (0.0F, 1.0F, 1.0F);
		glLineWidth (1.2);
		glBegin (GL_LINES);
			glVertex2f (XMIN, -Sy );
			glVertex2f (XMAX, -Sy );
			glVertex2f (Sx, YMIN);
			glVertex2f (Sx, YMAX);
		glEnd ();
	}


	// Historic Render
	if( historic_render )
	{
		glBegin (GL_LINES);

		// Historic Line Render	
			for( int g=0 ; g<num_graph_data ; g++ )
			{
				int hcnt = graphData[g]->HCOUNT;

				for( int i=0 ; i<graphData[g]->num_line ; i++ )
				{
					// main draw
					glLineWidth (graphData[g]->lineThick[i]);
					glColor3f ( graphData[g]->lineColour[i][0] , graphData[g]->lineColour[i][1] , graphData[g]->lineColour[i][2] );

					bool beDraw = true;
					int c = hcnt;
					for( int h=0 ; h<num_history-1 ; h++ )
					{
						int f = c;	c++;			
						if( c == num_history )	c = 0;
						int t = c;

						bool skipPhaseDiff = true;
						if( graphData[g]->lineKind == 1 )
						if( graphData[g]->ppos[i][f].x < -90.0  ||  graphData[g]->ppos[i][t].x < -90.0 )
							skipPhaseDiff = false;

						if( skipPhaseDiff )
						if( beDraw || !graphData[g]->isDashedLine[i] )
						{
							glVertex2f ( graphData[g]->ppos[i][f].x , graphData[g]->ppos[i][f].y );
							glVertex2f ( graphData[g]->ppos[i][t].x , graphData[g]->ppos[i][t].y );
						}

						if( h%10 < 1 )	beDraw = !beDraw;	// dashed line
					}
				}
			}

		glEnd ();

	}



	if( isLeadingPoint )
	{		
		for( int g=0 ; g<num_graph_data ; g++ )
		{
			for( int i=0 ; i<graphData[g]->num_line ; i++ )
			{
				if( graphData[g]->cposX[i] )
				{
					double cPosX = *graphData[g]->cposX[i];
					double cPosY = -(*graphData[g]->cposY[i]);
					
					glColor3f ( graphData[g]->lineColour[i][0] , graphData[g]->lineColour[i][1] , graphData[g]->lineColour[i][2] );
					glPointSize(10.0);
					glBegin (GL_POINTS);
						glVertex2f ( cPosX , cPosY );
					glEnd ();
				}
			}
		}
	}


	// Init Conditions - 2018-02-22 THU
	if( isInitPoint )
	{
		for( int g=0 ; g<num_graph_data ; g++ )
		{
			for( int i=0 ; i<graphData[g]->num_line ; i++ )
			{
				double iPosX = 0.0;
				double iPosY = 0.0;

				if( graphData[g]->initPosX[i] )
				{
					iPosX =  *graphData[g]->initPosX[i];
					iPosY = -(*graphData[g]->initPosY[i]);
				}
				
				glColor3f ( graphData[g]->lineColour[i][0] , graphData[g]->lineColour[i][1] , graphData[g]->lineColour[i][2] );
				glPointSize(10.0);
				glBegin (GL_POINTS);
					glVertex2f ( iPosX , iPosY );
				glEnd ();
			}
		}
	}


	if( isSensor )
	{
		double drawScale = 10.0;

		glLineWidth (2.0);

		for( int g=0 ; g<num_graph_data ; g++ )
		{
			for( int i=0 ; i<graphData[g]->num_line ; i++ )
			{
				double cPosX = *graphData[g]->cposX[i];
				double cPosY = -(*graphData[g]->cposY[i]);

				int indX = graphData[g]->idX[i];
				int indY = graphData[g]->idY[i];
		
				// Incoming Sensor Vectors
				glColor3f (0.0F, 1.0F, 0.0F);		
				glBegin (GL_LINES);
					glVertex2f ( cPosX , cPosY );
					glVertex2f ( cPosX + drawScale*g_sim[0]->GetExinSensor( indX ) , cPosY - drawScale*g_sim[0]->GetExinSensor( indY ) );
				glEnd ();

				
				// additional vector line draw - Currently the velocity vectors	
				glColor3f (1.0F, 0.0F, 0.0F);
				glBegin (GL_LINES);	
					glVertex2f ( cPosX , cPosY );
					glVertex2f ( cPosX + GetOscDrawPoint( indX ) , cPosY - GetOscDrawPoint( indY ) );
				glEnd ();
				
				// Total Incoming Vectors
				glColor3f (1.0F, 1.0F, 0.0F);
				glBegin (GL_LINES);
					glVertex2f ( cPosX , cPosY );
					glVertex2f ( cPosX + drawScale*g_sim[0]->GetExinTotal( indX ) , cPosY - drawScale*g_sim[0]->GetExinTotal( indY ) );			
				glEnd ();

				// Incoming Connection Vectors
				glColor3f (0.5F, 0.5F, 1.0F);
				glBegin (GL_LINES);
					glVertex2f ( cPosX , cPosY );
					glVertex2f ( cPosX + drawScale*g_sim[0]->GetExinCon( indX ) , cPosY - drawScale*g_sim[0]->GetExinCon( indY ) );	
				glEnd ();

			}
		}
		
	}

	glFlush();



	// Draw GL 3D Font
	glLoadIdentity();
	
	int tot_text = 0;
	for( int g=0 ; g<num_graph_data ; g++ )
	{
		for( int i=0 ; i<graphData[g]->num_line ; i++ )
		{
			glColor3f ( graphData[g]->lineColour[i][0] , graphData[g]->lineColour[i][1] , graphData[g]->lineColour[i][2] );
			
			glRasterPos2f( -0.95f + 0.1f*(float)tot_text , -1.0f );
			sprintf( glbuf,"%d", graphData[g]->idX[i] );	glPrint(glbuf);

			glRasterPos2f( -1.0f , -0.95f + 0.1f*(float)tot_text );
			sprintf( glbuf,"%d", graphData[g]->idY[i] );	glPrint(glbuf);

			tot_text++;
		}
	}

	
}






void GLHANDLE::ResizeGL_2D( void )
{
	int width  = glWidth;
	int height = glHeight;

	if( height == 0 )	height = 1;		// Prevent A Divide By Zero By Making Height Equal One

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glViewport (0, 0, width, height);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();	
	gluOrtho2D ( XMIN*1.05, XMAX*1.05,  YMAX*1.05, YMIN*1.05 );	

}







		
/*		if( drawMode == -1 )
		{
			int g = 0;

			if( *graphData[g]->doUpdate )
			{
				int hcnt = graphData[g]->HCOUNT;

				for( int i=0 ; i<graphData[g]->num_line ; i++ )
				{
					// Provess Torus
					VEC3 pExpand[5];
					if( Convert_PhaseDiff_Torus( pExpand ,	graphData[g]->ppos[i][hcnt-1] , 
															graphData[g]->ppos[i][hcnt]		)  )
					{
						for( int h=0 ; h<3 ; h++ )
						{
							graphData[g]->HCOUNT++;
							if( graphData[g]->HCOUNT == num_history )	graphData[g]->HCOUNT = 0;
						}

						int endh = graphData[g]->HCOUNT;
						for( int b=0 ; b<5 ; b++ )
						{
							graphData[g]->ppos[i][endh] = pExpand[4-b];

							endh--;
							if( endh == -1 )	endh = num_history-1;
						}
					}
				}

				graphData[g]->HCOUNT++;
				if( graphData[g]->HCOUNT == num_history )	graphData[g]->HCOUNT = 0;

			}
		}
*/
