



#include <windows.h>
#include <stdio.h>
#include <math.h>



#include "../resource.h"

#include "../NeuroUse/neurouse.h"
#include "../Status.h"
#include "oglwnd.h"


char gltBuf[10];



float R,G,B;
void SetMapColor_Type1( float intensity )
{
	if( intensity > 0.0f )
	{
		R = 1.0f;
		B = G = 1.0f-intensity;
	}
	else
	{
		B = 1.0f;
		R = G = 1.0f+intensity;
	}
}

void SetMapColor_Type2( float intensity )
{
	if( intensity < -0.5f )
	{
		R = 1.0f;
		B = 0.0f;
		G = 2.0f*intensity + 2.0f;
	}
	else if( intensity < 0.0f )
	{
		G = 1.0f;
		B = 0.0f;
		R = -2.0f*intensity;
	}
	else if( intensity < 0.5f )
	{
		G = 1.0f;
		R = 0.0f;
		B = 2.0f*intensity;
	}
	else
	{
		B = 1.0f;
		R = 0.0f;
		G = -2.0f*intensity + 2.0f;
	}

}

void SetMapColor_Type3( float intensity )
{
	if( intensity > 0.0f )
	{		
		B = 0.0f;

		G = 1.0f - intensity;
		R = intensity;
	}
	else
	{		
		R = 0.0f;

		G = 1.0f + intensity;
		B = -1.0f*intensity;
	}	

}



double GetBVP_Weight_Doya( int t , int tt , int f , int ff )
{
	return 1.0*g_sim[0]->oscil->osc[t]->weight[tt][f][ff];
}




float wth = 0.0f;

void GLHANDLE::DrawGL_Map2D( void )									// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT);

	

	int num_osc = g_sim[0]->oscil->num_osc;
	int num_grid = num_osc*num_osc;

	wth = (XMAX-XMIN) / (float)num_osc;

	float XMIN2 = XMAX + wth;
	float XMAX2 = XMIN2 + XMAX - XMIN;
		
	glBegin(GL_QUADS);

		double absmax = 0.000001;
		for( int t=0 ; t<num_osc ; t++ )
		{
			for( int tt=0 ; tt<2 ; tt++ )
			{
				for( int f=0 ; f<num_osc ; f++ )
				{
					for( int ff=0 ; ff<2 ; ff++ )
					{
						double val = fabs(GetBVP_Weight_Doya( t , tt , f , ff ));
						if( absmax < val )	absmax = val;
					}
				}
			}
		}

		// excitatory-to-excitatory
		for( int f=0 ; f<num_osc ; f++ )
		{
			for( int t=0 ; t<num_osc ; t++ )
			{
				float x1 = XMIN+(float)f*wth;
				float y1 = YMIN+(float)t*wth;
				float x2 = x1+wth;
				float y2 = y1+wth;
				
				
				float intensity = (float)( GetBVP_Weight_Doya( t , 0 , f , 0 ) );
				
				intensity = intensity / absmax;
				
				SetMapColor_Type2( intensity );				
						
				glColor3f (R, G, B);
				glVertex2f(x1, y1);
				glVertex2f(x2, y1);
				glVertex2f(x2, y2);
				glVertex2f(x1, y2);		

			}
		}

		// excitatory-to-inhibitory
		for( int f=0 ; f<num_osc ; f++ )
		{
			for( int t=0 ; t<num_osc ; t++ )
			{
				float x1 = XMIN2+(float)f*wth;
				float y1 = YMIN+(float)t*wth;
				float x2 = x1+wth;
				float y2 = y1+wth;
				
				
				float intensity = (float)( GetBVP_Weight_Doya( t , 1 , f , 0 ) );
				
				intensity = intensity / absmax;
				
				SetMapColor_Type2( intensity );				
						
				glColor3f (R, G, B);
				glVertex2f(x1, y1);
				glVertex2f(x2, y1);
				glVertex2f(x2, y2);
				glVertex2f(x1, y2);		

			}
		}


	glEnd();


	// Small grid
	glLineWidth (0.5);
	glColor3f (0.5F, 0.5F, 0.5F);
	glBegin (GL_LINES);

		for ( float k = XMIN ; k<=XMAX ; k+=wth ){		glVertex2f (k,YMIN);	glVertex2f (k,YMAX);	}
		for ( float k = YMIN ; k<=YMAX ; k+=wth ){		glVertex2f (XMIN,k);	glVertex2f (XMAX,k);	}

		for ( float k = XMIN2 ; k<=XMAX2 ; k+=wth ){	glVertex2f (k,YMIN);	glVertex2f (k,YMAX);	}
		for ( float k = YMIN ; k<=YMAX ; k+=wth ){		glVertex2f (XMIN2,k);	glVertex2f (XMAX2,k);	}

	glEnd ();

	
	

	// Diagonal axes
	glColor3f (0.0F, 0.0F, 0.0F);
	glLineWidth (1.0);
	glBegin (GL_LINES);

		glVertex2f (XMIN, YMIN);
		glVertex2f (XMAX, YMAX);

		glVertex2f (XMIN2, YMIN);
		glVertex2f (XMAX2, YMAX);

	glEnd ();




	glFlush();


	// Draw GL 3D Font on Map - Too Slow!
/*	for( int i=0 ; i<num_osc ; i++ )
	{
		for( int j=0 ; j<num_osc ; j++ )
		{
			float x1 = XMIN+(float)i*wth;
			float y1 = YMIN+(float)j*wth;
			float x2 = x1+wth;
			float y2 = y1+wth;
		
			glColor3f ( 0.0f , 0.0f , 0.0f );
							
			glRasterPos2f( x1+0.1f*wth, y1+0.4f*wth );
			sprintf( gltBuf ,"%d%d-%d%d", i/2 , i%2 , j/2 , j%2 );
			glPrint(gltBuf);
		}
	}
*/
	
}



void GLHANDLE::ResizeGL_Map2D( void )		// Resize And Initialize The GL Window
{
	int width  = glWidth;
	int height = glHeight;

	if (height==0)	height=1;

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glViewport (0, 0, width, height);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();	

//	gluOrtho2D ( XMIN*1.1, XMAX*1.1,  YMAX*1.1, YMIN*1.1 );	
	gluOrtho2D ( XMIN-wth, 2.0f*(XMAX+wth),  YMAX+wth, YMIN-wth );	

}





void DelGL( GLHANDLE* glh );

double GetOut( int , int , int , int );




LRESULT GLHANDLE::Map2D_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
//	char buf[50];
	RECT pRect;
	
	switch (message)
	{
		case WM_INITDIALOG:
			
			AllocPos( 1 , 10 );

			g_Status->isPlot2dmap = true;
		
				
			//OutFunc			= GetBVP_Weight_Doya;


			hWnd = GetDlgItem( hDlg , IDC_EDIT_RENDER );
			

			bgR = bgG = bgB = 1.0f;
			bgA = 0.5f;

			InitGL( this );
								

		//	XMIN = -4.0;	XMAX =  0.0;
		//	YMIN = -2.0;	YMAX =  2.0;

			XMIN = 0.0;	XMAX =  4.0;
			YMIN = 0.0;	YMAX =  4.0;

		break;


		case WM_KEYDOWN:
			switch (wParam)
            {
                case VK_ESCAPE:
					PostMessage(hDlg, WM_COMMAND , IDC_BUTTON_EXIT, 0 );
                break;
            }			
		break;

		
		case WM_COMMAND:
			
			if (LOWORD(wParam) == IDC_BUTTON_EXIT) 
			{
				g_Status->isPlot2dmap = false;

				DelGL( this );			
				EndDialog(hDlg, LOWORD(wParam));
			
				return TRUE;
			}


			if (LOWORD(wParam) == IDC_BUTTON_VIEW_WRITE )
			{
				
			}

			
			if (LOWORD(wParam) == IDC_BUTTON_VIEW_WIDE )
			{		
			//	XMAX += 1.0;	XMIN -= 1.0;
			//	YMAX += 1.0;	YMIN -= 1.0;
			}

			if (LOWORD(wParam) == IDC_BUTTON_VIEW_SMALL )
			{	
			//	XMAX -= 1.0;	XMIN += 1.0;
			//	YMAX -= 1.0;	YMIN += 1.0;							
			}

			
		break;


		case WM_SIZE:			
			GetClientRect( hDlg , &pRect );

			glWidth  = pRect.right - pRect.left;
			glHeight = pRect.bottom - pRect.top - 30;

			if( glWidth > 2*glHeight )	glWidth = 2*glHeight;
			else						glHeight = glWidth/2;

			SetWindowPos( hWnd , NULL , 0, 0, glWidth, glHeight, SWP_NOMOVE );

			
			SetWindowPos( GetDlgItem(hDlg,IDC_BUTTON_EXIT) , NULL , glWidth-70, 6, 0, 0, SWP_NOSIZE );
			SetWindowPos( GetDlgItem(hDlg,IDC_BORDER_QUIT) , NULL , glWidth-74, 2, 0, 0, SWP_NOSIZE );

		break;

		
	}
    return FALSE;

}






