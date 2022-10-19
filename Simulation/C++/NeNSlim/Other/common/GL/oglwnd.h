

#ifndef OGLWND_H
#define OGLWND_H


/*
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#include <gl\glut.h>
*/

#include <glut.h>

#include "..\..\common\math\vec3.h"
#include "../../common/DX\camera.h"


#include "../Status.h"


#define MAX_GL_GRAPH	100



class GLHANDLE;

class GLGRAPHDATA
{
public:

	GLHANDLE*	glh;

	int		lineKind;

	bool* doUpdate;
	int	HCOUNT;

	



	int		index;
	bool	hasIndex;
	char	dataName[50];

	// currently unused but leave it be
	int *idX, *idY, *idZ;	// specific neural/motor indices upto 3-dimension plot
	
	
	int			num_line;			// number of lines
	
	double		offsetX, offsetY, offsetZ;		// display offset

	VEC3		**ppos;		// num_line X num_history
	
	double		**cposX, **cposY, **cposZ;		// num_line of addresses
	double		**initPosX, **initPosY, **initPosZ;		// num_line of addresses

	double		*cposX_Scale, *cposY_Scale, *cposZ_Scale;		// num_line of addresses
	double		*cposX_Offset;	// Mar. 2017 -- specifically used for new pCoord[] delay

	bool		*isDashedLine;
	double		*lineThick;
	
	float		**lineColour;

	GLGRAPHDATA();
	~GLGRAPHDATA();

	void Alloc_GraphData( int );		// size of num_line
	void Set_Line_Colour_Gradient( int );


	void MakeGraphData_OSC_Plot1D( int , int );
	void MakeGraphData_OSC_Plot2D( int , int , int );
	void MakeGraphData_OSC_Plot3D( int , int , int , int );

	void (GLGRAPHDATA::*SaveGL_OneShot_FuncName) ( void );
	void SaveGL_OneShot( void )
	{
		(this->*SaveGL_OneShot_FuncName)( );
	}

	void SG_dzFunc( void );
	void SG_minmax_E( void );

};



class GLHANDLE
{
public:
	HDC			hDC;
	HGLRC		hRC;									// Permanent Rendering Context
	HWND		hWnd, g_Dlg;
		
	int			glWidth, glHeight;

	bool		lockcam, isActive, historic_render, isPortrait, isSensor, isLeadingPoint, isInitPoint;
	CAMERA*		cam;
	

	int curRadio;	// which radio button is clicked?	
	int	drawMode;	// spike OR line ?

	// for drawing lines
	int num_history;
	int hPer;

	int HCOUNT3D;	// PROVISIONAL! - this should be removed (May 24th 2017)





	bool is_Normal_Line_Update;


	
	// 2015 Feb 09 ///////////////////////////////////

	int num_graph_data;
	GLGRAPHDATA *graphData[MAX_GL_GRAPH];

	void AddGraphData( int , int , int );
	void DelGraphData( int );

	void Add_Graph_Wnd_Plot_Common( HWND , HWND , HWND , int );
	void Del_Graph_Wnd_Plot_Common( HWND );


	//////////////////////////////////////////////////



	int			num_pos;
	int			NUM_LINE_POS_CURRENT;
	VEC3		**ppos;
	VEC3		*cpos;
	bool		*isDashedLine;
	double		*lineThick;	
	float		**lineColour;
	

	
	// An individual Output window
	int			nIdx1, nIdx2;		// neuron index
	int			windowIdx;			// outwindow index
	
	int			idx1, idx2, idx3;	// x,y,z axis idx
	int			*g_idX, *g_idY, *g_idZ;	// x,y,z axis idx multi
	
	float		XMIN, XMAX, YMIN, YMAX, HMIN, HMAX;		// X,Y,H(Z) display space size

	float		bgR, bgG, bgB, bgA;		// background colour of drawing surface


	
	double  movestep;
	double	clipMIN, clipMAX;


	double	tscale;		// This should be removed
	double	RMIN, RMAX;	// This should be removed
	

	GLHANDLE();
	~GLHANDLE();

	void Zero( void );

	void AllocPos( int , int );		void DeletePos( void );
	
	void ReAlloc_Line_History( int );

	
	// For different GL windows -- seems unused (May 25 2017)
	void (*DrawFunc)( GLHANDLE* );
	void (*ResizeFunc)( int , int );	


	// Plot Weight
	void SaveGL_MultiOUT( void );		void DrawGL_MultiOUT( void );		void ResizeGL_MultiOUT( void );
	LRESULT PlotMultiOUT_Proc( HWND, UINT, WPARAM, LPARAM );

	// Plot 2D
	void SaveGL_2D( void );		void DrawGL_2D( void );		void ResizeGL_2D( void );
	LRESULT Plot2D_Proc( HWND, UINT, WPARAM, LPARAM );

	// Plot 3D
	void SaveGL_3D( void );		void DrawGL_3D( void );		void ResizeGL_3D( void );
	LRESULT Plot3D_Proc( HWND, UINT, WPARAM, LPARAM );



	// Plot 2D Map
	void DrawGL_Map2D( void );		void ResizeGL_Map2D( void );
	LRESULT Map2D_Proc( HWND, UINT, WPARAM, LPARAM );


	void	SetOutKind( int );	// currently for just setting window name
	void SetRange( float, float, float, float, float, float );


	
	
};



bool InitGL( GLHANDLE* );
void SetGLSize( GLHANDLE* );
GLvoid glPrint(const char* , ...);

int CheckGLError( void );


#endif