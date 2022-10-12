



#include <windows.h>
#include <stdio.h>
#include <math.h>



#include "../resource.h"

#include "../NeuroUse/neurouse.h"
#include "../Status.h"
#include "oglwnd.h"
#include "../DX/input.h"





bool Convert_PhaseDiff_Torus( VEC3 res[5] , VEC3 tmp_prev , VEC3 tmp_curr );

void GLHANDLE::SaveGL_3D( void )
{

	if( g_Status->SimCount % hPer < 1  )	is_Normal_Line_Update = true;
	else									is_Normal_Line_Update = false;


		
		for( int g=0 ; g<num_graph_data ; g++ )
		{
			if( *graphData[g]->doUpdate )
			{
				int hcnt = graphData[g]->HCOUNT;

				for( int i=0 ; i<graphData[g]->num_line ; i++ )
				{						
					graphData[g]->ppos[i][hcnt].x =  ( *(graphData[g]->cposX[i]) );
					graphData[g]->ppos[i][hcnt].y = -( *(graphData[g]->cposY[i]) );
					graphData[g]->ppos[i][hcnt].z = -( *(graphData[g]->cposZ[i]) ) * graphData[g]->cposZ_Scale[i];
				}


				if( graphData[g]->lineKind == 1 )
				{
					// Process Torus
					VEC3 pExpand[5];
					if( Convert_PhaseDiff_Torus( pExpand ,	graphData[g]->ppos[0][hcnt-1] , 
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


/*	
	if( g_Status->SimCount % hPer < 1 &&  historic_render )
	{

		if( HCOUNT3D > num_history-1 )
		{
			for( int h=0 ; h<HCOUNT3D-1 ; h++ )
			{
				ppos[0][h].x = ppos[0][h+1].x;
				ppos[0][h].y = ppos[0][h+1].y;
				ppos[0][h].z = ppos[0][h+1].z;
			}

			ppos[0][HCOUNT3D-1].x =  OutFunc( idx1 , curRadio , 0 , 0 );
			ppos[0][HCOUNT3D-1].y =  OutFunc( idx2 , curRadio , 0 , 0 );
			ppos[0][HCOUNT3D-1].z =  OutFunc( idx3 , curRadio , 0 , 0 );
		}
		else
		{
			ppos[0][HCOUNT3D].x =  OutFunc( idx1 , curRadio , 0 , 0 );
			ppos[0][HCOUNT3D].y =  OutFunc( idx2 , curRadio , 0 , 0 );
			ppos[0][HCOUNT3D].z =  OutFunc( idx3 , curRadio , 0 , 0 );

			HCOUNT3D++;
		}

	}
*/
	
}








GLvoid GLHANDLE::ResizeGL_3D( void )		// Resize And Initialize The GL Window
{
	int width  = glWidth;
	int height = glHeight;

	if (height==0)	height=1;

	
	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
	
	
	cam->Pitch( cam->pitch );
	cam->Yaw( cam->yaw );
	cam->Go( 0.0f, !lockcam );

	// Camera Works !!
	if( lockcam )
	{
		VEC3 At, From, Up;
		VEC3 camAt, camFrom, camDir;
				
		At.Set( 0.0 , 0.0 , 0.0 );
		From	= At;
		Up.Set( 0.0f, 1.0f, 0.0f );

		At.Set( 0.0 , 0.0 , 0.0 );
		Up.Set( cam->up.x , cam->up.y , cam->up.z );
		camFrom.Set( cam->from.x , cam->from.y , cam->from.z );
		camAt.Set( cam->At.x , cam->At.y , cam->At.z );
		camDir = camFrom - camAt;
		camDir.Normalize();
		From = At + camDir*cam->radius;

				
		gluLookAt(  (float)From.x ,	-(float)From.y ,	(float)From.z , 
					(float)At.x ,	-(float)At.y ,		(float)At.z , 
					(float)Up.x ,	-(float)Up.y ,		(float)Up.z		);		

		cam->from.x = From.x;
		cam->from.y = From.y;
		cam->from.z = From.z;
			
	}
	else
	{	
		gluLookAt(  (float)cam->from.x ,	-(float)cam->from.y ,	(float)cam->from.z ,
					(float)cam->At.x ,		-(float)cam->At.y ,		(float)cam->At.z , 
					(float)cam->up.x ,		-(float)cam->up.y ,		(float)cam->up.z		);
					
	}

	
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix

	
}



static bool isGLClear = true;

void DelGL( GLHANDLE* glh );
double GetOut( int , int , int , int );


static HWND hCombo_select = NULL;
static HWND hList_select = NULL;
static char** ComboNameList = NULL;

void Init_Plot_ComboBox( char** , HWND , int );

char** Get_GL_Data_Name_Plot3D( void );	// for combo box

float* Select_Line_Color( HWND hDlg );




LRESULT GLHANDLE::Plot3D_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	char buf[50];
//	RECT pRect;

	// Color Select
	float* colRGB;
	
	switch (message)
	{
		case WM_INITDIALOG:
			
		//	AllocPos( 1 , 10000 );
			
		//	OutFunc			= GetOut;

			hWnd = GetDlgItem( hDlg , IDC_EDIT_PLOT_3D );

			InitGL( this );


			SetWindowText( GetDlgItem( hDlg , IDC_EDIT_PLOT3D_X ) , _itoa(idx1, buf,10) );
			SetWindowText( GetDlgItem( hDlg , IDC_EDIT_PLOT3D_Y ) , _itoa(idx2, buf,10) );
			SetWindowText( GetDlgItem( hDlg , IDC_EDIT_PLOT3D_Z ) , _itoa(idx3, buf,10) );


			CheckRadioButton(hDlg , IDC_RADIO_Y, IDC_RADIO_FR, IDC_RADIO_FR );	

			SetRange( -10.0f , 10.0f , -10.0f , 10.0f , -10.0f , 10.0f );

			
			// Initialize Camera
			cam = new CAMERA(true);

			cam->angleRate	= 0.008f;
			cam->HighSpd	= 0.3f;		// 0.05
			cam->LowSpd		= 0.1f;		// 0.025

			
			CheckDlgButton( hDlg, IDC_CHECK_HISTORIC, BST_CHECKED );
			CheckDlgButton( hDlg, IDC_CHECK_GL_CLEAR, BST_CHECKED );
			CheckDlgButton( hDlg, IDC_CHECK_LOCK_CAM, BST_CHECKED );



				// Init Combo & List Boxes
				hPer	= 10;			// how much sample datapoint ?
				num_history = 1000;

				
						
				ComboNameList = Get_GL_Data_Name_Plot3D( );

				curRadio = 0;	// "Neuron Output" as default

			SetWindowText( GetDlgItem( hDlg , IDC_EDIT_PLOT_MAX ) , _itoa(num_history, buf,10) );
			

			// Select Drawing Data from ComboBox
			hCombo_select = GetDlgItem( hDlg , IDC_COMBO_SELECT );
			hList_select  = GetDlgItem( hDlg , IDC_LIST_DATA );

			Init_Plot_ComboBox( ComboNameList , hCombo_select , curRadio );
			
			Add_Graph_Wnd_Plot_Common( hDlg , hList_select , hCombo_select , 3 );


			// Adjust Screen Size & Pos Again
			SendMessage (hDlg , WM_SIZE , 0 , 0);

			g_Status->isPlot3d = true;
	
		break;

		case WM_ACTIVATE:

			if( wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE )
			{
				DI_Term();
				InitDInput(hDlg);
				isActive = true;
			}

			if( wParam == WA_INACTIVE )		isActive = false;

		break;

		
		case WM_COMMAND:

			if (HIWORD(wParam) == BN_CLICKED )
			{				
				switch (LOWORD(wParam)) 
				{ 
					case IDC_RADIO_Y:
						curRadio = 0;
						sprintf( buf ,"NeuronomicoN Plot3D - Y" );
						SetWindowText( hDlg , buf );
					break;

					case IDC_RADIO_DY:
						curRadio = 1;
						sprintf( buf ,"NeuronomicoN Plot3D - DY" );
						SetWindowText( hDlg , buf );
					break;
					
					case IDC_RADIO_FR:
						curRadio = 2;
						sprintf( buf ,"NeuronomicoN Plot3D - FR" );
						SetWindowText( hDlg , buf );
					break;


					case IDC_CHECK_HISTORIC:
						if( IsDlgButtonChecked(hDlg, IDC_CHECK_HISTORIC) )		historic_render = true;
						else													historic_render = false;						
					break;

					case IDC_CHECK_GL_CLEAR:
						if( IsDlgButtonChecked(hDlg, IDC_CHECK_GL_CLEAR) )		isGLClear = true;
						else													isGLClear = false;						
					break;

					case IDC_CHECK_LOCK_CAM:
						if( IsDlgButtonChecked(hDlg, IDC_CHECK_LOCK_CAM) )		lockcam = true;
						else													lockcam = false;						
					break;

				}
				
			}


			switch (LOWORD(wParam))
			{			
				case IDC_BUTTON_EXIT:
					
					g_Status->isPlot3d = false;


					DI_Term();
									
					DelGL( this );
					EndDialog(hDlg, LOWORD(wParam));

				return TRUE;


				case IDC_BUTTON_VIEW_WIDE:
					XMAX += 1.0;	XMIN -= 1.0;
					YMAX += 1.0;	YMIN -= 1.0;
					HMAX += 1.0;	HMIN -= 1.0;
				break;

				case IDC_BUTTON_VIEW_SMALL:
					XMAX -= 1.0;	XMIN += 1.0;
					YMAX -= 1.0;	YMIN += 1.0;
					HMAX -= 1.0;	HMIN += 1.0;
				break;


				case IDC_BUTTON_SETXY:
					Del_Graph_Wnd_Plot_Common( hList_select );
					Add_Graph_Wnd_Plot_Common( hDlg , hList_select , hCombo_select , 3 );
				break;


				case IDC_BUTTON_SET_PLOTMAX:
					GetWindowText( GetDlgItem( hDlg , IDC_EDIT_PLOT_MAX ) , buf , 50 );
					ReAlloc_Line_History( atoi(buf) );
				break;


				case IDC_BUTTON_ADDXYZ:
					Add_Graph_Wnd_Plot_Common( hDlg , hList_select , hCombo_select , 3 );
				break;				

				case IDC_BUTTON_DELXYZ:
					Del_Graph_Wnd_Plot_Common( hList_select );
				break;



				case IDC_BUTTON_IS_LEADINGPOINT:	isLeadingPoint = !isLeadingPoint;	break;


				// Choose & Apply Colour to the Graphs !! - Apr 2017
				// http://soen.kr/lecture/win32api/lec8/lec8-5-2.htm
				case IDC_BUTTON_COLOR:

					colRGB = Select_Line_Color( hDlg );

					for( int c=0 ; c<3 ; c++ )
					graphData[SendMessage( hList_select , LB_GETCURSEL , 0 , 0 )]->lineColour[0][c] = colRGB[c];


				break;



			}

		break;


		// Edit this for 3D window
		case WM_SIZE:

/*			GetClientRect( hDlg , &pRect );
		//	glWidth  = pRect.right - pRect.left - 0;
			glWidth  = pRect.right - pRect.left - 172;
			glHeight = pRect.bottom - pRect.top - 70;
			SetWindowPos( hWnd , NULL , 0, 70-0, glWidth, glHeight, 0 );

			
			SetWindowPos( hList_select , NULL , glWidth+2, 70-0, 170, glHeight/2, 0 );

			SetWindowPos( hCombo_select , NULL , glWidth+2, 4, 0, 0, SWP_NOSIZE );
*/			
		break;


		

		
	}
    return FALSE;
}




void GLHANDLE::DrawGL_3D( void )									// Here's Where We Do All The Drawing
{
			
	if( isGLClear )	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	
	// Medium Grid
	glLineWidth (0.7);
	glColor3f (0.5F, 0.5F, 0.5F);
	glBegin (GL_LINES);

		for ( double i = XMIN ; i < XMAX ; i += 1.0 )
		{
			glVertex3f ( i , 0.0 , YMIN );
			glVertex3f ( i , 0.0 , YMAX );
		}
		
		for ( int i = YMIN ; i<YMAX ; i += 1.0 )
		{
			glVertex3f ( XMIN , 0.0 , i );
			glVertex3f ( XMAX , 0.0 , i );
		}

	glEnd ();


	// Bounding Box & Edge quad
	glColor3f (1.0F, 0.52F, 0.0F);
	glLineWidth (1.2);
	glBegin (GL_LINE_LOOP);
		glVertex3f ( XMIN, 0.0, YMIN );
		glVertex3f ( XMIN, 0.0, YMAX );
		glVertex3f ( XMAX, 0.0, YMAX );
		glVertex3f ( XMAX, 0.0, YMIN );
	glEnd ();
	glBegin (GL_LINE_LOOP);
		glVertex3f ( XMIN, HMIN, YMIN );
		glVertex3f ( XMIN, HMIN, YMAX );
		glVertex3f ( XMAX, HMIN, YMAX );
		glVertex3f ( XMAX, HMIN, YMIN );
	glEnd ();
	glBegin (GL_LINE_LOOP);
		glVertex3f ( XMIN, HMAX, YMIN );
		glVertex3f ( XMIN, HMAX, YMAX );
		glVertex3f ( XMAX, HMAX, YMAX );
		glVertex3f ( XMAX, HMAX, YMIN );
	glEnd ();
	glBegin (GL_LINES);
		glVertex3f ( XMIN, HMAX, YMIN );
		glVertex3f ( XMIN, HMIN, YMIN );
		glVertex3f ( XMAX, HMAX, YMAX );
		glVertex3f ( XMAX, HMIN, YMAX );
		glVertex3f ( XMIN, HMAX, YMAX );
		glVertex3f ( XMIN, HMIN, YMAX );
		glVertex3f ( XMAX, HMAX, YMIN );
		glVertex3f ( XMAX, HMIN, YMIN );
	glEnd ();


	// Main axes
	glColor3f (0.7F, 0.7F, 1.0F);
	glLineWidth (2);
	glBegin (GL_LINES);
		glVertex3f (XMIN, 0.0, 0.0);
		glVertex3f (0.0, 0.0, 0.0);
		glVertex3f (0.0, 0.0, -YMIN);
		glVertex3f (0.0, 0.0, 0.0);
		glVertex3f (0.0, -HMIN ,0.0);
		glVertex3f (0.0, 0.0 ,0.0);
	glEnd ();


	// Indicating Plus Axes
	glColor3f (1.0F, 1.0F, 0.0F);
	glLineWidth (2);
	glBegin (GL_LINES);
		glVertex3f (0.0, 0.0, 0.0);
		glVertex3f (XMAX, 0.0, 0.0);
		glVertex3f (0.0, 0.0, 0.0);
		glVertex3f (0.0, 0.0, -YMAX);
		glVertex3f (0.0, 0.0 ,0.0);
		glVertex3f (0.0, -HMAX ,0.0);
	glEnd ();



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
							glVertex3f ( graphData[g]->ppos[i][f].x , graphData[g]->ppos[i][f].y , graphData[g]->ppos[i][f].z );
							glVertex3f ( graphData[g]->ppos[i][t].x , graphData[g]->ppos[i][t].y , graphData[g]->ppos[i][t].z );
						}

						if( h%10 < 1 )	beDraw = !beDraw;	// dashed line
					}
				}
			}

		glEnd ();

	}


	// Leading Point
	if( isLeadingPoint )
	{
		for( int g=0 ; g<num_graph_data ; g++ )
		{
			for( int i=0 ; i<graphData[g]->num_line ; i++ )
			{
				double cPosX =  (*graphData[g]->cposX[i]);
				double cPosY = -(*graphData[g]->cposY[i]);
				double cPosZ = -(*graphData[g]->cposZ[i]);
				
				glColor3f ( graphData[g]->lineColour[i][0] , graphData[g]->lineColour[i][1] , graphData[g]->lineColour[i][2] );
				glPointSize(10.0);
				glBegin (GL_POINTS);
					glVertex3f ( cPosX , cPosY , cPosZ );
				glEnd ();
			}
		}
	}

	// Leading Point
/*	glColor3f (1.0F, 0.0F, 0.0F);
	glPointSize(10.0);
	glBegin (GL_POINTS);
		glVertex3f ( OutFunc( idx1, curRadio, 0, 0 ), -OutFunc( idx2, curRadio, 0, 0 ), -OutFunc( idx3, curRadio, 0, 0 ) );
	glEnd ();
*/


	glFlush();


	// Draw GL 3D Font
	glLoadIdentity();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f,0.0f,0.0f);	
	glRasterPos2f( XMAX , 0.0f );	glPrint("X");
	glRasterPos2f( 0.0f , -YMAX );	glPrint("Y");
	glTranslatef(0.0,0.0f,-HMAX);	
	glRasterPos2f( 0.0f , 0.0f );	glPrint("Z");
	
}





// Small grid
/*	glLineWidth (0.5);
	glColor3f (0.35F, 0.35F, 0.35F);
	glBegin (GL_LINES);
	for ( double i = glh->XMIN ; i <= glh->XMAX ; i += 0.1 )
	{
		glVertex3f ( i , 0.0 , glh->YMIN );
		glVertex3f ( i , 0.0 , glh->YMAX );
	}

	for ( i = glh->YMIN ; i <= glh->YMAX ; i += 0.1 )
	{
		glVertex3f ( glh->XMIN , 0.0 , i );
		glVertex3f ( glh->XMAX , 0.0 , i );
	};
	glEnd ();
*/
