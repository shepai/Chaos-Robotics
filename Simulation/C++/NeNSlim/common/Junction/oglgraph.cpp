
/************************************************
												*
  - Common OpenGL Functions -					*
	written by Yoonsik Shim						*
												*
*************************************************/


#include <windows.h>
#include <stdio.h>
#include <malloc.h>

#include "../GL/oglwnd.h"
#include "../dynamics/rigidode.h"
#include "../NeuroUse/neurouse.h"



GLGRAPHDATA::GLGRAPHDATA()
{
	glh = NULL;

	index = -1;

	idX = idY = idZ = NULL;

	num_line = 0;

	offsetX = offsetY = offsetZ = 0.0;
	
	ppos = NULL;
	
	cposX = cposY = cposZ = NULL;

	initPosX = initPosY = initPosZ = NULL;

	cposX_Scale = cposY_Scale = cposZ_Scale = NULL;

	isDashedLine = NULL;
	lineThick = NULL;
	
	lineColour = NULL;

}

GLGRAPHDATA::~GLGRAPHDATA()
{
	for( int i=0 ; i<num_line ; i++ )
	{
		free( ppos[i] );
		free( lineColour[i] );
	}
	
	if( ppos )	free( ppos );

	if( cposX )	free( cposX );		if( cposY )	free( cposY );		if( cposZ )	free( cposZ );

	if( initPosX ) free( initPosX );	if( initPosY ) free( initPosY );	if( initPosZ ) free( initPosZ );

	if( cposX_Scale ) free( cposX_Scale );		if( cposY_Scale ) free( cposY_Scale );		if( cposZ_Scale ) free( cposZ_Scale );

	if( idX ) free( idX );		if( idY ) free( idY );		if( idZ ) free( idZ );

	if( lineColour )	free( lineColour );
	if( isDashedLine )	free( isDashedLine );
	if( lineThick )		free( lineThick );
}



void HUE2RGB( float H, float& R, float& G, float& B );

void GLGRAPHDATA::Set_Line_Colour_Gradient( int NUMLINE )
{
	float colourAngleDiv = 360.0f / (NUMLINE-1);

	for( int i=0 ; i<NUMLINE ; i++ )
	{		
		if( i < 1 )		lineColour[i][0] = lineColour[i][1] = lineColour[i][2] = 1.0f;
		else			HUE2RGB( colourAngleDiv*(float)(i-1) , lineColour[i][0], lineColour[i][1], lineColour[i][2] );
	}
}



void GLGRAPHDATA::Alloc_GraphData( int num )
{
	lineKind = 0;	// 0: normal line
					// 1: phase diff

	hasIndex = false;

	num_line = num;

	cposX = (double**)malloc( sizeof(double*) * num_line );
	cposY = (double**)malloc( sizeof(double*) * num_line );
	cposZ = (double**)malloc( sizeof(double*) * num_line );

	initPosX = (double**)malloc( sizeof(double*) * num_line );
	initPosY = (double**)malloc( sizeof(double*) * num_line );
	initPosZ = (double**)malloc( sizeof(double*) * num_line );

	cposX_Scale = (double*)malloc( sizeof(double) * num_line );
	cposY_Scale = (double*)malloc( sizeof(double) * num_line );
	cposZ_Scale = (double*)malloc( sizeof(double) * num_line );

	idX = (int*)malloc( sizeof(int) * num_line );
	idY = (int*)malloc( sizeof(int) * num_line );
	idZ = (int*)malloc( sizeof(int) * num_line );

	isDashedLine = (bool*)	malloc( sizeof(bool)	* num_line );
	lineThick	 = (double*)malloc( sizeof(double)	* num_line );

	lineColour	 = (float**)malloc( sizeof(float*)	* num_line );

	ppos = (VEC3**)malloc( sizeof(VEC3*) * num_line );

	for( int i=0 ; i<num_line ; i++ )
	{
		ppos[i] = (VEC3*)calloc( glh->num_history , sizeof(VEC3) );

		lineColour[i] = (float*)malloc( sizeof(float) * 3 );

		cposX_Scale[i] = cposY_Scale[i] = cposZ_Scale[i] = 1.0;

		initPosX[i] = initPosY[i] = initPosZ[i] = NULL;

		cposX[i] = cposY[i] = cposZ[i] = NULL;
	}

	
	// Initialise some values
	for( int i=0 ; i<num_line ; i++ )
	{
		isDashedLine[i] = false;
		lineThick[i] = 2.0;
	}

	Set_Line_Colour_Gradient( num_line );

}








////////// GLHANDLE Func ////////////////////////////////////////



char** Get_GL_Data_Name_OSC_Plot1D( void );
char** Get_GL_Data_Name_OSC_Plot2D( void );
char** Get_GL_Data_Name_OSC_Plot3D( void );


char** Get_GL_Data_Name_Plot1D( void )
{
	return Get_GL_Data_Name_OSC_Plot1D();
}

char** Get_GL_Data_Name_Plot2D( void )
{
	return Get_GL_Data_Name_OSC_Plot2D();
}

char** Get_GL_Data_Name_Plot3D( void )
{
	
	return Get_GL_Data_Name_OSC_Plot3D();
	
}


void GLHANDLE::AddGraphData( int temp_idx1 , int temp_idx2 , int temp_idx3 )
{
	graphData[num_graph_data] = new GLGRAPHDATA;

	graphData[num_graph_data]->glh = this;
	graphData[num_graph_data]->index = num_graph_data;

	

			if( temp_idx3 > 0 )
				graphData[num_graph_data]->MakeGraphData_OSC_Plot3D( curRadio , temp_idx1 , temp_idx2 , temp_idx3 );
			else if( temp_idx2 > 0 )
				graphData[num_graph_data]->MakeGraphData_OSC_Plot2D( curRadio , temp_idx1 , temp_idx2 );
			else
				graphData[num_graph_data]->MakeGraphData_OSC_Plot1D( curRadio , temp_idx1 );			

	

	graphData[num_graph_data]->HCOUNT = 0;

	num_graph_data++;

//	HCOUNT = PCOUNT = 0;
}


void GLHANDLE::DelGraphData( int idx )
{
	if( num_graph_data > 0 )
	{
		delete graphData[idx];

		for( int i=idx ; i<num_graph_data-1 ; i++ )
		{
			graphData[i] = graphData[i+1];
		}

		num_graph_data--;
	}

}