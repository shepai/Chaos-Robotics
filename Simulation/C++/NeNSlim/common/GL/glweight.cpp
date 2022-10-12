

#include <windows.h>
#include <stdio.h>
#include <math.h>

#include "../resource.h"

#include "../NeuroUse/neurouse.h"
#include "../Status.h"
#include "oglwnd.h"








bool Convert_PhaseDiff_Torus_Multi( VEC3 res[50][5] , int num_line , VEC3** ppos , int hcnt , int prev_hcnt );

void GLHANDLE::SaveGL_MultiOUT( void )
{
	movestep = g_sim[0]->GetSimTime();



		if( g_Status->SimCount % hPer < 1  )	is_Normal_Line_Update = true;
		else									is_Normal_Line_Update = false;


		for( int g=0 ; g<num_graph_data ; g++ )
		{

			if( *graphData[g]->doUpdate )
			{
				int hcnt = graphData[g]->HCOUNT;

				for( int i=0 ; i<graphData[g]->num_line ; i++ )
				{				
					graphData[g]->ppos[i][hcnt].x =    *(graphData[g]->cposX[i]) - *(graphData[g]->cposX_Offset);
					graphData[g]->ppos[i][hcnt].y = -( *(graphData[g]->cposY[i]) + graphData[g]->offsetY );
					graphData[g]->ppos[i][hcnt].z = 0.0;
				}


				if( graphData[g]->lineKind == 1 )
				{
					int prev_hcnt = hcnt-1;
					if( prev_hcnt < 0 )	prev_hcnt = num_history-1;

					VEC3 pExpand[50][5];
					if( Convert_PhaseDiff_Torus_Multi( pExpand , graphData[g]->num_line,
																		graphData[g]->ppos, hcnt, prev_hcnt )  )
					{
						for( int h=0 ; h<3 ; h++ )
						{
							graphData[g]->HCOUNT++;
							if( graphData[g]->HCOUNT == num_history )	graphData[g]->HCOUNT = 0;
						}

						int endh = graphData[g]->HCOUNT;
						for( int b=0 ; b<5 ; b++ )
						{
							for( int i=0 ; i<graphData[g]->num_line ; i++ )
								graphData[g]->ppos[i][endh] = pExpand[i][4-b];

							endh--;
							if( endh == -1 )	endh = num_history-1;
						}
									
					}
				}

				graphData[g]->HCOUNT++;
				if( graphData[g]->HCOUNT == num_history )	graphData[g]->HCOUNT = 0;

			} // end *doUpdate	


		} // end g<graph data
	

}




//double GetSingular( int n );		// un-comment if used

// Actually Draw Spikes
void GLHANDLE::DrawGL_MultiOUT( void )
{
	clipMIN = XMIN + movestep - 0.8*XMAX;
	clipMAX = XMAX + movestep - 0.8*XMAX;

	gluOrtho2D ( clipMIN, clipMAX, YMAX, YMIN );

	glClear(GL_COLOR_BUFFER_BIT);



	// Begin Draw Lines !
	glBegin (GL_LINES);
	

		// Medium Grid -- Old version using tScale can be found at the bottom of this file...
		glColor3f (0.3F, 0.3F, 0.3F);	glLineWidth (0.5);		
		for ( int i=(int)clipMIN ; i<=(int)clipMAX ; i++ )
		{
			glVertex2f ( i , YMIN );
			glVertex2f ( i , YMAX );
		}


		
		// for continuous lines /////////////////////////////////////////////////////
		
		
			// Horizontal minor axes
			glColor3f (0.3F, 0.3F, 0.3F);	glLineWidth (0.5);
			for( float y=YMIN ; y<YMAX ; y+=1.0f )
			{	glVertex2f (clipMIN, y);	glVertex2f (clipMAX, y);	}
			
			// Horizontal Zero axes
			glColor3f (1.0F, 0.5F, 0.0F);	glLineWidth (1.0);
			glVertex2f (clipMIN, 0.0f);	glVertex2f (clipMAX, 0.0f);

			// Line of Singular Value
		/*	double sing = -GetSingular( 0 );
			glColor3f (1.0F, 0.0F, 0.0F);	glLineWidth (1.0);
			glVertex2f (clipMIN, sing);	glVertex2f (clipMAX, sing);
		*/

			// Historic Line Render	
			for( int g=0 ; g<num_graph_data ; g++ )
			{
				for( int i=0 ; i<graphData[g]->num_line ; i++ )
				{
					// their own zero axes
					glColor3f (1.0F, 1.0F, 0.0F);	glLineWidth (1.0);
					glVertex2f (clipMIN, -graphData[g]->offsetY);
					glVertex2f (clipMAX, -graphData[g]->offsetY);
				}

				int h = graphData[g]->HCOUNT;

				for( int i=0 ; i<graphData[g]->num_line ; i++ )
				{
					// main draw
					glColor3f ( graphData[g]->lineColour[i][0] , graphData[g]->lineColour[i][1] , graphData[g]->lineColour[i][2] );
					glLineWidth (graphData[g]->lineThick[i]);

					
					bool beDraw = true;

					int c = h;
				//	if( graphData[g]->lineKind == 1 )	c = PCOUNT;

					for( int h=0 ; h<num_history-1 ; h++ )
					{
						int f = c;	c++;			
						if( c == num_history )	c = 0;
						int t = c;

						// For phaseDiff on Torus - Apr 2017
						bool skipPhaseDiff = true;
						if( graphData[g]->lineKind == 1 )
						if( graphData[g]->ppos[i][f].y < -90.0  ||  graphData[g]->ppos[i][t].y < -90.0 )
							skipPhaseDiff = false;

						if( skipPhaseDiff )
						if( beDraw || !graphData[g]->isDashedLine[i] )
						{
							
						/*	double Lx = graphData[g]->ppos[i][f].x - graphData[g]->ppos[i][t].x;
							double Ly = graphData[g]->ppos[i][f].y - graphData[g]->ppos[i][t].y;

							double leng = sqrt( Lx*Lx + Ly*Ly );

							if( leng > 100.0 )
								int gjgj = 0;
						*/

							glVertex2f ( graphData[g]->ppos[i][f].x , graphData[g]->ppos[i][f].y );
							glVertex2f ( graphData[g]->ppos[i][t].x , graphData[g]->ppos[i][t].y );
						}

						if( h%10 < 1 )	beDraw = !beDraw;	// dashed line
					}


				}
			}

		



	
	glEnd ();


	glFlush();


	// Draw GL Font for each data --> Just provisional! Fix this later~
/*	glLoadIdentity();

	char* txt[3];

	txt[0] = "E";
	txt[1] = "CH";
	txt[2] = "Ed";
	
	for( int g=0 ; g<num_graph_data ; g++ )
	{
		for( int i=0 ; i<graphData[g]->num_line ; i++ )
		{
			glColor3f ( graphData[g]->lineColour[i][0] , graphData[g]->lineColour[i][1] , graphData[g]->lineColour[i][2] );

			double val = ( *(graphData[g]->cposY[i]) + graphData[g]->offsetY );
			val = val / YMAX;

			glRasterPos2f( 0.83f , val );	glPrint( txt[i] );
		}	
	}
*/

	
}


/*
	switch( GetNETKIND() )
	{
		case CTRNN:		break;
		case SNN:		break;

		case PNN:

		break;
			
		case OSCIL:


		break;
	}
*/


GLvoid GLHANDLE::ResizeGL_MultiOUT( void )		// Resize And Initialize The GL Window
{
	int width  = glWidth;
	int height = glHeight;

	if (height==0)	height=1;	

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glViewport (0, 0, width, height);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
}




static HWND hCombo_select = NULL;
static HWND hList_select = NULL;
static char** ComboNameList = NULL;

void Init_Plot_ComboBox( char** , HWND , int );

char** Get_GL_Data_Name_Plot1D( void );	// for combo box
char** Get_GL_Data_Name_PNN( void );




int Get_PNN_Spike_Draw_Num( void );		// get number of total spiking nodes

void DelGL( GLHANDLE* glh );

float* Select_Line_Color( HWND hDlg );

LRESULT GLHANDLE::PlotMultiOUT_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int  tempIdx = 0;
	char temp[50] = {0};
	char listText[50] = {0};

	RECT pRect;	
	
	int curList;

	// Color Select
	float* colRGB;
	
	switch (message)
	{		
		case WM_INITDIALOG:

			

			hWnd = GetDlgItem( hDlg , IDC_EDIT_OUTPUT );
				
			InitGL( this );			

			
			
			tscale = 1.0;
			
			XMIN = -5.0;	XMAX =  5.0;
			YMIN = -4.0;	YMAX =  4.0;

			
					hPer	= 10;			// how much sample datapoint ?
					num_history = 7000;	// Max number of sampled datapoint for line drawing

					XMAX =  40.0;	XMIN = -XMAX;

					ComboNameList = Get_GL_Data_Name_Plot1D( );

					curRadio = 6;	// Neuron Output as default

					SetWindowText( GetDlgItem( hDlg , IDC_EDIT_PLOT1D_X ) , "0" );



			RMIN = XMIN;	RMAX = XMAX;


			// Select Drawing Data from ComboBox
			hCombo_select = GetDlgItem( hDlg , IDC_COMBO_SELECT );
			hList_select  = GetDlgItem( hDlg , IDC_LIST_DATA );

			Init_Plot_ComboBox( ComboNameList , hCombo_select , curRadio );

			Add_Graph_Wnd_Plot_Common( hDlg , hList_select , hCombo_select , 1 );


			SetWindowText( GetDlgItem( hDlg , IDC_EDIT_PLOT_MAX )	, _itoa(num_history, temp, 10) );
	

			// Adjust Screen Size Again
			SendMessage (hDlg , WM_SIZE , 0 , 0);


			g_Status->isPlotMultiOUT = true;

		break;

		
		case WM_COMMAND:
					
			// Radio Buttons - UnUsed
			if (HIWORD(wParam) == BN_CLICKED )
			{				
				switch( LOWORD(wParam) )
				{
					case IDC_RADIO1:	break;
					case IDC_RADIO2:	break;
					case IDC_RADIO3:	break;
				}
			}

			// Press Buttons
			switch( LOWORD(wParam) )
			{
				// select button from ComboBox
				case IDC_BUTTON_SELECT_ADD:
					
						Add_Graph_Wnd_Plot_Common( hDlg , hList_select , hCombo_select , 1 );

	
				break;


				case IDC_BUTTON_SELECT_DEL:
										
					Del_Graph_Wnd_Plot_Common( hList_select );
					

				break;


				case IDC_BUTTON_COLOR:

					colRGB = Select_Line_Color( hDlg );

					for( int c=0 ; c<3 ; c++ )
					graphData[SendMessage( hList_select , LB_GETCURSEL , 0 , 0 )]->lineColour[0][c] = colRGB[c];


				break;


				case IDC_BUTTON_GRAPH_UP:

					curList = SendMessage( hList_select , LB_GETCURSEL , 0 , 0 );

					graphData[curList]->offsetY++;

					
					strcpy( listText , graphData[curList]->dataName );
					sprintf( temp , "%+1.1f" , graphData[curList]->offsetY );
					strcat( listText , temp );

					SendMessage( hList_select , LB_DELETESTRING , curList , 0 );
					SendMessage( hList_select , LB_INSERTSTRING , curList , (LPARAM)listText );
					SendMessage( hList_select , LB_SETCURSEL	, curList , 0 );

				break;

				case IDC_BUTTON_GRAPH_DOWN:

					curList = SendMessage( hList_select , LB_GETCURSEL , 0 , 0 );

					graphData[curList]->offsetY--;


					strcpy( listText , graphData[curList]->dataName );
					sprintf( temp , "%+1.1f" , graphData[curList]->offsetY );
					strcat( listText , temp );

					SendMessage( hList_select , LB_DELETESTRING , curList , 0 );
					SendMessage( hList_select , LB_INSERTSTRING , curList , (LPARAM)listText );
					SendMessage( hList_select , LB_SETCURSEL	, curList , 0 );

				break;



				case IDC_BUTTON_VIEW_Y_WIDE:
					if( YMAX < 1.0 ){	YMAX *= 2.0;	YMIN *= 2.0;	}else{	YMAX += 1.0;	YMIN -= 1.0;	}				
				break;

				case IDC_BUTTON_VIEW_Y_SMALL:
					if( YMAX > 1.5 ){	YMAX -= 1.0;	YMIN += 1.0;	}else{	YMAX *= 0.5;	YMIN *= 0.5;	}
				break;

				case IDC_BUTTON_VIEW_X_WIDE:
					if( XMAX < 1.0 ){	XMAX *= 2.0;	XMIN *= 2.0;	}else{	XMAX += 1.0;	XMIN -= 1.0;	}
					RMIN = XMIN;	RMAX = XMAX;				
				break;

				case IDC_BUTTON_VIEW_X_SMALL:
					if( XMAX > 1.5 ){	XMAX -= 1.0;	XMIN += 1.0;	}else{	XMAX *= 0.5;	XMIN *= 0.5;	}
					RMIN = XMIN;	RMAX = XMAX;				
				break;

				case IDC_BUTTON_VIEW_UP:		YMAX -= 1.0;	YMIN -= 1.0;		break;
				case IDC_BUTTON_VIEW_DOWN:		YMAX += 1.0;	YMIN += 1.0;		break;


				case IDC_BUTTON_SET_MAXPLOT:	// change num_history
					GetWindowText( GetDlgItem( hDlg , IDC_EDIT_PLOT_MAX ) , temp , 50 );
					ReAlloc_Line_History( atoi(temp) );
				break;

			
				case IDC_BUTTON_EXIT:
					g_Status->isPlotMultiOUT = false;
					DelGL( this );
					EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
	
		break;


		case WM_SIZE:
			GetClientRect( hDlg , &pRect );
			glWidth  = pRect.right - pRect.left-150;
			glHeight = pRect.bottom - pRect.top-30;
			SetWindowPos( hWnd , NULL , 0, 30, glWidth, glHeight, 0 );

			SetWindowPos( GetDlgItem(hDlg,IDC_LIST_DATA) , NULL , glWidth, 30, 150, glHeight, 0 );
			SetWindowPos( GetDlgItem(hDlg,IDC_BUTTON_EXIT) , NULL , glWidth, 2, 0, 0, SWP_NOSIZE );
		break;

	}

    return FALSE;
}





/*	if( RMIN < clipMIN )
		{	
			double tIncrm = (double)((int)(clipMIN-RMIN)+1);
			RMIN += tIncrm;	RMAX += tIncrm;

			// tScale unused !!
		}
	*/
		
	/*	RMIN = (double)((int)clipMIN);
		RMAX = (double)((int)clipMAX);

		for ( double i=RMIN ; i<RMAX ; i+=tscale ){		glVertex2f ( i , YMIN );	glVertex2f ( i , YMAX );		}
	*/



/*	case IDC_COMBO_SELECT:
					switch (HIWORD(wParam))
					{
						case CBN_SELCHANGE:
							curRadio = SendMessage( hCombo_select , CB_GETCURSEL , 0 , 0 );					
						break;
					}
				break;
*/

/*

			for( int i=half_num_pos ; i<num_pos ; i++ )
			{
				int d = (i-half_num_pos)%8;
				glColor3f ( lineColour[d][0] , lineColour[d][1] , lineColour[d][2] );

				glLineWidth (1.5);
				glBegin (GL_LINES);

					for( int c=0 ; c<HCOUNT ; c++ )
					{
						int p = c-1;	if( p < 0 )	p = 0;
						glVertex2f ( ppos[i][p].x , ppos[i][p].y );
						glVertex2f ( ppos[i][c].x , ppos[i][c].y );
					}
				
				glEnd ();
			}
*/