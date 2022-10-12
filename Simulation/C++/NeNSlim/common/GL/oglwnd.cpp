
/************************************************
												*
  - Common OpenGL Functions -					*
	written by Yoonsik Shim						*
												*
*************************************************/


#include <windows.h>
#include <stdio.h>
#include <malloc.h>

#include "oglwnd.h"



GLHANDLE::GLHANDLE()
{	



		hDC		= NULL;		
		hRC		= NULL;		
		hWnd	= NULL;		
		g_Dlg	= NULL;		
		
		glWidth	= glHeight = 0;
		
		historic_render	= true;
		isPortrait		= false;
		
		hPer			= 1;		
		
		nIdx1 = nIdx2 = 0;
		windowIdx = 0;

		idx1 = 0;	idx2 = 1;	idx3 = 2;

		cam			= NULL;
		lockcam		= true;
		isActive	= false;

		XMIN = XMAX =  0.0;
		YMIN = YMAX =  0.0;
		HMIN = HMAX =  0.0;

		bgR = bgG = bgB = 0.0f;		bgA = 0.5f;

		tscale = 0.0;
		movestep = 0.0;
		clipMIN = clipMAX = 0.0;
		RMIN = RMAX = 0.0;

		isSensor = false;
		isLeadingPoint = isInitPoint = false;

		g_idX = g_idY = g_idZ = NULL;


		// Line
		num_pos = 0;
		num_history = 0;
		NUM_LINE_POS_CURRENT = 0;
		
		HCOUNT3D = 0;

		ppos = NULL;
		cpos = NULL;
		lineColour = NULL;
		lineThick = NULL;
		isDashedLine = NULL;

		
	
		curRadio = 0;
		drawMode = 0;


		num_graph_data = 0;
		for( int g=0 ; g<MAX_GL_GRAPH ; g++ )	graphData[g] = NULL;
		
		
}


void GLHANDLE::Zero( void )
{
	movestep = 0.0;
	RMIN = XMIN;	RMAX = XMAX;

//	HCOUNT = PCOUNT = 0;
	HCOUNT3D = 0;
	for( int g=0 ; g<num_graph_data ; g++ )	graphData[g]->HCOUNT = 0;


	for( int i=0 ; i<num_pos ; i++ )
	{
		for( int h=0 ; h<num_history ; h++ )	ppos[i][h].x = ppos[i][h].y = 0.0;
	}

	
}


GLHANDLE::~GLHANDLE()
{
	if( cam )	delete cam;

	DeletePos( );

	if( g_idX )	free( g_idX );
	if( g_idY )	free( g_idY );
	if( g_idZ )	free( g_idZ );


	for( int g=0 ; g<num_graph_data ; g++ )	delete graphData[g];

}



// This should be replaced... Apr 2017
void GLHANDLE::AllocPos( int num , int hist )
{		
	num_pos = num;
	num_history = hist;
	
	ppos = (VEC3**)malloc( sizeof(VEC3*) * num_pos );
	lineColour = (float**)malloc( sizeof(float*) * num_pos );

	g_idX = (int*)malloc( sizeof(int) * num_pos );
	g_idY = (int*)malloc( sizeof(int) * num_pos );
	g_idZ = (int*)malloc( sizeof(int) * num_pos );

	for( int i=0 ; i<num_pos ; i++ )
	{
		ppos[i] = (VEC3*)malloc( sizeof(VEC3) * num_history );	ZeroMemory( ppos[i] , sizeof(VEC3) * num_history );

		lineColour[i] = (float*)malloc( sizeof(float) * 3 );
	}

	cpos = (VEC3*)malloc( sizeof(VEC3) * num_pos );	ZeroMemory( cpos , sizeof(VEC3)*num_pos );

	isDashedLine = (bool*)malloc( sizeof(bool) * num_pos );
	lineThick = (double*)malloc( sizeof(double) * num_pos );
	
//	HCOUNT = PCOUNT = 0;	
	HCOUNT3D = 0;

	g_idX[0] = 0;	g_idY[0] = 1;	g_idZ[0] = 2;
	
}

void GLHANDLE::DeletePos( void )
{
	for( int i=0 ; i<num_pos ; i++ )
	{
		free( ppos[i] );
		free( lineColour[i] );
	}
	
	if( ppos )	free( ppos );
	if( cpos )	free( cpos );
	if( lineColour )	free( lineColour );
	if( isDashedLine )	free( isDashedLine );
	if( lineThick )		free( lineThick );
}



void GLHANDLE::ReAlloc_Line_History( int newNumH )
{
	if( newNumH < 0 )	return;

	for( int i=0 ; i<num_pos ; i++ )
	{
		free( ppos[i] );
		ppos[i] = (VEC3*)malloc( sizeof(VEC3) * newNumH );	ZeroMemory( ppos[i] , sizeof(VEC3) * newNumH );
	}


	for( int g=0 ; g<num_graph_data ; g++ )
	{
		for( int i=0 ; i<graphData[g]->num_line ; i++ )
		{
			free( graphData[g]->ppos[i] );
			graphData[g]->ppos[i] = (VEC3*)malloc( sizeof(VEC3) * newNumH );
			ZeroMemory( graphData[g]->ppos[i] , sizeof(VEC3) * newNumH );
		}

		graphData[g]->HCOUNT = 0;
	}


	num_history = newNumH;

//	HCOUNT = PCOUNT = 0;
	HCOUNT3D = 0;
}


// Used in Plot3D
void GLHANDLE::SetRange( float x1, float x2, float y1, float y2, float h1, float h2 )
{
	XMIN = x1;		XMAX = x2;
	YMIN = y1;		YMAX = y2;
	HMIN = h1;		HMAX = h2;		
}












/// OpenGL Common ////////////////////////////////////////////////////////////////////////////////////

GLvoid BuildFont( HDC hDC );
GLvoid KillFont(GLvoid);
GLvoid glPrint(const char* , ...);


void SetGLSize( GLHANDLE* glh )
{
	RECT cRect;
	GetClientRect( glh->hWnd , &cRect );
	glh->glWidth  = cRect.right - cRect.left;
	glh->glHeight = cRect.bottom - cRect.top;
	
}


int MySetPixelFormat(HDC hdc)
{

	PIXELFORMATDESCRIPTOR pfd = { 
	    sizeof(PIXELFORMATDESCRIPTOR),    // size of this pfd 
	    1,                                // version number 
	    PFD_DRAW_TO_WINDOW |              // support window 
	    PFD_SUPPORT_OPENGL |              // support OpenGL 
	    PFD_DOUBLEBUFFER,                 // double buffered 
	    PFD_TYPE_RGBA,                    // RGBA type 
	    24,                               // 24-bit color depth 
	    0, 0, 0, 0, 0, 0,                 // color bits ignored 
	    0,                                // no alpha buffer 
	    0,                                // shift bit ignored 
	    0,                                // no accumulation buffer 
	    0, 0, 0, 0,                       // accum bits ignored 
	    32,                               // 32-bit z-buffer     
	    0,                                // no stencil buffer 
	    0,                                // no auxiliary buffer 
	    PFD_MAIN_PLANE,                   // main layer 
	    0,                                // reserved 
	    0, 0, 0                           // layer masks ignored 
	}; 
	
	int  iPixelFormat; 
 
	// get the device context's best, available pixel format match 
	if((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
	{
		MessageBox(NULL, "ChoosePixelFormat Failed", NULL, MB_OK);
		return 0;
	}
	 
	// make that match the device context's current pixel format 
	if(SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE)
	{
		MessageBox(NULL, "SetPixelFormat Failed", NULL, MB_OK);
		return 0;
	}

	return 1;
}



bool InitGL( GLHANDLE* glh )
{

	int err = CheckGLError();
	
	glh->hDC = GetDC( glh->hWnd );
		
	MySetPixelFormat( glh->hDC );

	glh->hRC = wglCreateContext( glh->hDC );

	wglMakeCurrent( glh->hDC , glh->hRC );

	SetGLSize( glh );

	glShadeModel(GL_SMOOTH);
	glClearColor( glh->bgR, glh->bgG, glh->bgB, glh->bgA );
	
	
	if( glh->cam )
	{
		glClearDepth(1.0f);	
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}
	else
		glDisable(GL_DEPTH_TEST);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	BuildFont( glh->hDC );
	

//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	


	glEnable(GL_POINT_SMOOTH);
		
	return TRUE;
}



void DelGL( GLHANDLE* glh )
{
	KillFont();
	ReleaseDC( glh->hWnd , glh->hDC ) ; 
	wglDeleteContext( glh->hRC ); 
}





GLuint	base;				// Base Display List For The Font Set


GLvoid BuildFont( HDC hDC )								// Build Our Bitmap Font
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists(96);								// Storage For 96 Characters
		
	font = CreateFont(	-18,							// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_BOLD,						// Font Weight
					//	0,								// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						"Courier New");					// Font Name

	oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
	wglUseFontBitmaps(hDC, 32, 96, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}

GLvoid KillFont(GLvoid)									// Delete The Font List
{
	glDeleteLists(base, 96);							// Delete All 96 Characters
}

GLvoid glPrint(const char *fmt, ...)					// Custom GL "Print" Routine
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	    vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base - 32);								// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}



int CheckGLError( void )
{
	int error = glGetError();

	if( error == GL_NO_ERROR )
		return 1;
	else if( error == GL_INVALID_ENUM )
		return 2;
	else if( error == GL_INVALID_VALUE )
		return 3;
	else if( error == GL_INVALID_OPERATION )
		return 4;
	else if( error == GL_STACK_OVERFLOW )
		return 5;
	else if( error == GL_STACK_UNDERFLOW )
		return 6;
	else if( error == GL_OUT_OF_MEMORY  )
		return 7;
	else
		return 0;
}



/*
void GLHANDLE::AddIdx( int x , int y , int z )
{
	idX = (int*)realloc( idX, sizeof(int)*(num_pos+1) );
	idY = (int*)realloc( idY, sizeof(int)*(num_pos+1) );
	idZ = (int*)realloc( idZ, sizeof(int)*(num_pos+1) );
		
	idX[num_pos] = x;
	idY[num_pos] = y;
	idZ[num_pos] = z;

	num_pos++;
}

void GLHANDLE::DelIdx( int id )
{
	num_pos--;

	for( int i=id ; i<num_pos ; i++ )
	{
		idX[i] = idX[i+1];
		idY[i] = idY[i+1];
		idZ[i] = idZ[i+1];
	}

	idX = (int*)realloc( idX, sizeof(int)*num_pos );
	idY = (int*)realloc( idY, sizeof(int)*num_pos );
	idZ = (int*)realloc( idZ, sizeof(int)*num_pos );
}
*/
