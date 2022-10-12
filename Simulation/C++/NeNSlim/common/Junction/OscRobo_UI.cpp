
//****************************************************************************//
//*** Oscillator Driven Robots - Provide Interface for Text & Graphics UI ****//
//****************************************************************************//


#include <windows.h> // This need to include gl.h
#include "../GL/oglwnd.h"
#include "../dynamics/rigidode.h"
#include "../NeuroUse/neurouse.h"



char* GL_DATA_NAME_OSC_PLOT1D[] =	{	"14",
									"All Sensor Scales", 
									"All Sensor Offsets", 
									"EFFICIENCIES",
									"IaL-IaV-Ib",
									"RawSen-SensorVal",
									"Ang-Vel-Torq",
									"Neurons",
									"Neurons All",
									"Neurons Vel",
									"Alpha X",
									"SpCon IbF",
									"Phase Diff",
									"OSC Phase",
									"Teager Energy"
								};

char** Get_GL_Data_Name_OSC_Plot1D( void ){	return GL_DATA_NAME_OSC_PLOT1D;	}



static double no_offset = 0.0;


void GLGRAPHDATA::MakeGraphData_OSC_Plot1D( int arc_kind , int temp_idx )
{

	RIGIDSTATE* l_rs  = g_sim[0]->rs;
	OSCNET*		l_osc = g_sim[0]->oscil;
	EVAL*		eval	= g_sim[0]->eval;
	

	glh->drawMode = 1;	// Lines
	

	int kind = arc_kind;

	

	doUpdate = &glh->is_Normal_Line_Update;


	switch( kind )
	{
		// All sensorScale
		case 0:

			Alloc_GraphData( l_osc->num_osc );

			
			if( l_rs->is_ODE_Robot )
			{
				for( int i=0 ; i<num_line ; i++ )
				{
					if( i < num_line/2 )
					{
						cposY[i] = &l_osc->IaSen[2*i]->sensorScale; // &l_rs->motor[i].sensorScale1;
						isDashedLine[i] = true;
					}
					else
						cposY[i] = &l_osc->IaSen[(2*i-num_line/2)+1]->sensorScale; //&l_rs->motor[(2*i-num_line/2)+1].sensorScale2;
				}
			}

		break;


		// All sensorOffset
		case 1:
			Alloc_GraphData( l_osc->num_osc );

			
			if( l_rs->is_ODE_Robot )
			{
				for( int i=0 ; i<num_line ; i++ )
				{
					if( i < num_line/2 )
					{
						cposY[i] = &l_osc->IaSen[2*i]->sensorOffset;
						isDashedLine[i] = true;
					}
					else
						cposY[i] = &l_osc->IaSen[(2*i-num_line/2)+1]->sensorOffset;
				}
			}
		break;


		// EFFICIENCY, dzRate
		case 2:

			Alloc_GraphData( 3 );

			cposY[0] = &eval->EFFICIENCY;
			cposY[1] = &eval->dzRate;
			cposY[2] = &eval->maxE;

		break;


		// Raw Sensors - IaL, IaV, Ib
		case 3:

			
			if( l_rs->is_ODE_Robot )
			{
				Alloc_GraphData( 3 );

				cposY[0] = &l_rs->motor[temp_idx].IaL1;
				cposY[1] = &l_rs->motor[temp_idx].IaV1;
				cposY[2] = &l_rs->motor[temp_idx].IbF1;
			}

			hasIndex = true;	// different muscle index ?

		break;


		// Mod SensorVal
		case 4:

		//	if( l_rs->spnet )
		//	{
				Alloc_GraphData( 2 );

				cposY[0] = &l_osc->IaSen[temp_idx]->rawSensor_Disp;
				cposY[1] = &l_osc->IaSen[temp_idx]->sensorVal_Disp;
		//	}
		

			hasIndex = true;

		break;


		// Angle & Vel & torque
		case 5:

		
			if( l_rs->is_ODE_Robot )
			{
				Alloc_GraphData( 3 );

				cposY[0] = &l_rs->motor[temp_idx].angle[0];
				cposY[1] = &l_rs->motor[temp_idx].vel[0];
			//	cposY[2] = &l_rs->motor[temp_idx].torque[0];
				cposY[2] = &l_rs->motor[temp_idx].velLF[0];
			}

			hasIndex = true;

		break;


		// Neural Outputs
		case 6:

			Alloc_GraphData( 1 );

			cposY[0] = &l_osc->osc[temp_idx/l_osc->osc[0]->num_node]->x[temp_idx%l_osc->osc[0]->num_node];
		//	cposY[1] = &l_osc->osc[1]->x[0];

			hasIndex = true;

		break;

		// All Neural Outputs
		case 7:

			Alloc_GraphData( l_osc->num_osc );

			for( int i=0 ; i<num_line ; i++ )
			{
				cposY[i] = &l_osc->osc[i]->x[0];
			}

			hasIndex = false;

		break;

		// Neural (OSC) Out - Vel
		case 8:

			Alloc_GraphData( 1 );

			cposY[0] = &l_osc->osc[temp_idx]->v[0];

			hasIndex = true;

		break;

		// Alpha Out - X (Currently no Alpha-V) - Apr 2017
		case 9:

		//	Alloc_GraphData( 1 );
		//	cposY[0] = &l_osc->alpha[temp_idx]->x;

			Alloc_GraphData( l_osc->num_osc );
			
			for( int a=0 ; a<l_osc->num_osc ; a++ )
			{
				cposY[a] = &l_osc->alpha[a]->v;
			}

			hasIndex = true;

		break;


		// Servo F and Fs
		case 10:

		
		break;

		// Antagonistic Spring Phase Diff
		case 11:

			
		
				Alloc_GraphData( l_osc->num_osc-1 );

				for( int i=0 ; i<num_line ; i++ )
				{
					cposY[i] = &l_osc->pDiff_Osc->pCoord[i];
				}
				
			

			lineKind = 1;

			doUpdate = &l_osc->pDiff_Osc->is_pCoord_update;

			hasIndex = false;

		break;


		// Individual OSC Phase
		case 12:

			Alloc_GraphData( l_osc->num_osc );

			for( int i=0 ; i<num_line ; i++ )
			{
				cposY[i] = &l_osc->osc[i]->phase;
			}

			hasIndex = false;

		break;


		// Mass - Teager Energy Operators
		case 13:


			hasIndex = false;

		break;


		


	}


	for( int i=0 ; i<num_line ; i++ )
	{
		cposX[i] = &glh->movestep;	// simtime

	//	cposX_Offset[i] = &no_offset;
	}

	cposX_Offset = &no_offset;


	// Offset from current time instance
	switch( kind )
	{
		case 11:	// Phase Difference New

		//	for( int i=0 ; i<num_line/2 ; i++ )
			if( lineKind == 1 )
			cposX_Offset = &l_osc->pDiff_Osc->glPastTimeAmount;
		

		break;
	}
	


	strcpy( dataName , GL_DATA_NAME_OSC_PLOT1D[kind+1] );

}






/////////////////// FOR PLOT2D GL PLOT DRAWING ///////////////////////////////////////////////////////////////////////


double Get_dzFunc( double x , double min , double max );

void GLGRAPHDATA::SG_dzFunc( void )
{
	RIGIDSTATE* l_rs  = g_sim[0]->rs;
	EVAL*		eval  = g_sim[0]->eval;

	double PI2 = 6.283185307179586;
	int NUM_DATA = 400;
    
    for( int k=0 ; k<NUM_DATA ; k++ )
    {
        double t = (double)k * PI2 / (double)NUM_DATA;
        double E = eval->maxE * 0.7 * (1.0-cos(1.0*t));

		for( int i=0 ; i<num_line ; i++ )
		{
			ppos[i][HCOUNT].x =  E;
			ppos[i][HCOUNT].y = -Get_dzFunc ( E , eval->minE , eval->maxE );
			ppos[i][HCOUNT].z = 0.0;
		}

		HCOUNT++;
	//	if( HCOUNT == glh->num_history )	HCOUNT = 0;
		if( HCOUNT == NUM_DATA )	HCOUNT = 0;
    }
	
}

void GLGRAPHDATA::SG_minmax_E( void )
{
	RIGIDSTATE* l_rs  = g_sim[0]->rs;
	EVAL*		eval	= g_sim[0]->eval;

	double x[2];

	x[0] = eval->minE;
	x[1] = eval->maxE;

	double vertH = 1.5;

	for( int k=0 ; k<2 ; k++ )	// two vertices for each vertical line 2020-09-14
    {
		for( int i=0 ; i<num_line ; i++ )
		{
			ppos[i][HCOUNT].x =  x[i];
			ppos[i][HCOUNT].y = -vertH * (double)k;
			ppos[i][HCOUNT].z = 0.0;
		}

		HCOUNT++;
	//	if( HCOUNT == glh->num_history )	HCOUNT = 0;
		if( HCOUNT == 2 )	HCOUNT = 0;
	}

}



/*
void (*SaveGL_OneShot_FuncName) ( GLGRAPHDATA* , int );
void SaveGL_OneShot( GLGRAPHDATA* gd , int nHist )
{
	SaveGL_OneShot_FuncName( gd , nHist );
}
*/

char* GL_DATA_NAME_OSC_PLOT2D[] = {	"13",
									"BVP Out-x",			// 0
									"BVP Out-v",			// 1
									"All BVP Output",		// 2
									"Alpha Out",			// 3
									"BVP x0-v0",			// 4
									"SpNet Mass",			// 5
									"Gottwald P,Q",			// 6
									"Phase Diff 2D Osc",	// 7 - init
									"Phase Diff 2D Alpha",	// 8
									"All & Alpha's",		// 9 - init
									
									"Eval Stat",			// 10, 11, 12 (manual call in Plot2DProc) 2020-08-24
									"Eval dzFunc",
									"Eval minmaxE"
								  };

char** Get_GL_Data_Name_OSC_Plot2D( void ){	return GL_DATA_NAME_OSC_PLOT2D;	}



void GLGRAPHDATA::MakeGraphData_OSC_Plot2D( int cCursor , int temp_idx1 , int temp_idx2 )
{

	RIGIDSTATE* l_rs  = g_sim[0]->rs;
	OSCNET*		l_osc = g_sim[0]->oscil;
	EVAL*		eval	= g_sim[0]->eval;
	

	glh->drawMode = 0;	// Lines
	

	doUpdate = &glh->is_Normal_Line_Update;


	int tmp_num_line, mul;


	// convert to node indices
	int nNode = l_osc->osc[0]->num_node;

	int oIdx1 = temp_idx1/nNode;	int oIdx2 = temp_idx2/nNode;
	int nIdx1 = temp_idx1%nNode;	int nIdx2 = temp_idx2%nNode;

	switch( cCursor )
	{
		// BVP Outputs x[]
		case 0:

			Alloc_GraphData( 1 );	hasIndex = true;

			cposX[0] = &l_osc->osc[oIdx1]->x[nIdx1];
			cposY[0] = &l_osc->osc[oIdx2]->x[nIdx2];

			initPosX[0] = &l_osc->osc[oIdx1]->xInit[nIdx1];
			initPosY[0] = &l_osc->osc[oIdx2]->xInit[nIdx2];

			idX[0] = temp_idx1;
			idY[0] = temp_idx2;

		break;


		// BVP Outputs v[]
		case 1:

			Alloc_GraphData( 1 );	hasIndex = true;

			cposX[0] = &l_osc->osc[oIdx1]->v[nIdx1];
			cposY[0] = &l_osc->osc[oIdx2]->v[nIdx2];

			idX[0] = temp_idx1;
			idY[0] = temp_idx2;
		
		break;


		// All BVP outputs
		case 2:

			hasIndex = false;

			
			tmp_num_line = l_rs->NUM_ODE_BODY-2;
			mul = 2;
		

			Alloc_GraphData( tmp_num_line );

			for( int i=0 ; i<tmp_num_line ; i++ )
			{
				int yIdx = (i+1)*mul;

				cposX[i] = &l_osc->osc[0]->x[0];
				cposY[i] = &l_osc->osc[yIdx]->x[0];

				idX[i] = 0;
				idY[i] = yIdx*2;
			}			
			
		break;


		// Alpha Neuron
		case 3:

			Alloc_GraphData( 1 );	hasIndex = true;

			cposX[0] = &l_osc->alpha[temp_idx1]->x;
			cposY[0] = &l_osc->alpha[temp_idx2]->x;

			idX[0] = temp_idx1;
			idY[0] = temp_idx2;
		
		break;


		// BVP x and v
		case 4:

			Alloc_GraphData( 1 );	hasIndex = true;

			cposX[0] = &l_osc->osc[temp_idx1]->x[0];
			cposY[0] = &l_osc->osc[temp_idx1]->v[0];

			idX[0] = temp_idx1;
			idY[0] = temp_idx2;

		break;


		// SpNet V-F May 12 2017
		case 5:

			
		break;


		// Gottwald Test
		case 6:

		
		break;



		// Phase Diffs ////////////////////////////////
		case 7:
		
			Alloc_GraphData( 1 );	hasIndex = false;

			cposX[0] = &l_osc->pDiff_Osc->pCoord[0];
			cposY[0] = &l_osc->pDiff_Osc->pCoord[1];

			idX[0] = 0;
			idY[0] = 1;

			lineKind = 1;

			doUpdate = &l_osc->pDiff_Osc->is_pCoord_update;

		
		break;


		// Phase Diffs R-Motors ////////////////////////////////
		case 8:

			Alloc_GraphData( 1 );	hasIndex = false;

			cposX[0] = &l_osc->pDiff_Alpha->pCoord[0];
			cposY[0] = &l_osc->pDiff_Alpha->pCoord[1];

			idX[0] = 0;
			idY[0] = 1;

			lineKind = 1;

			doUpdate = &l_osc->pDiff_Alpha->is_pCoord_update;

		
		break;


		// All osc's & Alpha's
		case 9:

			hasIndex = false;

			
			tmp_num_line = l_rs->NUM_ODE_BODY-2;
			mul = 2;

			
			int num_line_mult, idx_mult_alpha;

		//	if( l_osc->___num_inter_neuron > 0 )
		//	{
		//		num_line_mult = 3;
		//		idx_mult_alpha = 2;
		//	}
		//	else
		//	{
				num_line_mult = 2;
				idx_mult_alpha = 1;
		//	}


			Alloc_GraphData( num_line_mult * tmp_num_line );

			for( int i=0 ; i<tmp_num_line ; i++ )
			{
				int yIdx = (i+1)*mul;

				cposX[i] = &l_osc->osc[0]->x[0];
				cposY[i] = &l_osc->osc[yIdx]->x[0];

				idX[i] = 0;		idY[i] = yIdx*2;

				lineColour[i][0] = 1.0f;	lineColour[i][1] = 1.0f;	lineColour[i][2] = 1.0f;



/*				if( l_osc->num_inter_neuron > 0 )
				{
					int b = i+tmp_num_line;

					cposX[b] = &l_osc->alpha[0+l_osc->num_osc]->x;
					cposY[b] = &l_osc->alpha[yIdx+l_osc->num_osc]->x;

					idX[b] = 0	  + l_osc->num_osc;
					idY[b] = yIdx + l_osc->num_osc;

					lineColour[b][0] = 0.0f;	lineColour[b][1] = 1.0f;	lineColour[b][2] = 0.0f;
				}
*/


				int a = i+idx_mult_alpha*tmp_num_line;

				if( l_osc->alpha )
				{
					cposX[a] = &l_osc->alpha[0]->x;
					cposY[a] = &l_osc->alpha[yIdx]->x;
				}
				else
				{
					cposX[a] = &no_offset;
					cposY[a] = &no_offset;
				}

				idX[a] = 0;		idY[a] = yIdx;
				lineColour[a][0] = 1.0f;	lineColour[a][1] = 0.5f;	lineColour[a][2] = 0.0f;		

			}
		
		break;

		

		/******* 2020-08-23 SUN *******/
		// Following three Graphs 10,11,12 are always called together
		// dzRate, tdz Fixed, minE/maxE fixed -- refreshed in realtime...

		// (1) dzRate
		case 10:

			Alloc_GraphData( 1 );

			cposX[0] = &eval->EFFICIENCY;
			cposY[0] = &eval->dzRate;

			hasIndex = false;

		break;

		// (2) background curve for tdz
		case 11:

			Alloc_GraphData( 1 );	// now num_line is 1. 2020-09-13 SUN

			hasIndex = false;

			lineKind = 2;

			SaveGL_OneShot_FuncName = &GLGRAPHDATA::SG_dzFunc;
			
		break;

		// (3) minE & maxE, two vertical lines
		case 12:

			Alloc_GraphData( 2 );

			hasIndex = false;

			lineKind = 2;

			SaveGL_OneShot_FuncName = &GLGRAPHDATA::SG_minmax_E;

		break;


	}
	

//	if( cCursor < 0 )	strcpy( dataName , "Phase Diff 2D" );
//	else				
	
	strcpy( dataName , GL_DATA_NAME_OSC_PLOT2D[cCursor+1] );

}







/////////////////// FOR PLOT3D GL PLOT DRAWING ///////////////////////////////////////////////////////////////////////

char* GL_DATA_NAME_OSC_PLOT3D[] = {	"6",
									"BVP Out-x",
									"Phase Diffs 3D",
									"Phase Diffs 2D zRate",
									"BVP+sen",
									"BVP+sen 2D",
									"BVP+sen 3D"
								  };

char** Get_GL_Data_Name_OSC_Plot3D( void ){	return GL_DATA_NAME_OSC_PLOT3D;	}


void GLGRAPHDATA::MakeGraphData_OSC_Plot3D( int cCursor , int temp_idx1 , int temp_idx2 , int temp_idx3 )
{

	RIGIDSTATE* l_rs  = g_sim[0]->rs;
	OSCNET*		l_osc = g_sim[0]->oscil;
	
	

	glh->drawMode = 0;	// Lines
	

	doUpdate = &glh->is_Normal_Line_Update;



	// convert to node indices
	int numOscNode = l_osc->osc[0]->num_node;
	int oIdx1 = temp_idx1/numOscNode;	int oIdx2 = temp_idx2/numOscNode;	int oIdx3 = temp_idx3/numOscNode;
	int nIdx1 = temp_idx1%numOscNode;	int nIdx2 = temp_idx2%numOscNode;	int nIdx3 = temp_idx3%numOscNode;


	if( oIdx3 >= l_osc->num_osc )	oIdx3 = 0;

	switch( cCursor )
	{
		// BVP Outputs x[]
		case 0:

			Alloc_GraphData( 1 );	hasIndex = true;

			cposX[0] = &l_osc->osc[oIdx1]->x[nIdx1];
			cposY[0] = &l_osc->osc[oIdx2]->x[nIdx2];
			cposZ[0] = &l_osc->osc[oIdx3]->x[nIdx3];

			idX[0] = temp_idx1;
			idY[0] = temp_idx2;
			idZ[0] = temp_idx3;

		break;


		

		// Phase Diffs ////////////////////////////////
		case 1:

			Alloc_GraphData( 1 );	hasIndex = false;

			cposX[0] = &l_osc->pDiff_Osc->pCoord[0];
			cposY[0] = &l_osc->pDiff_Osc->pCoord[1];
			cposZ[0] = &l_osc->pDiff_Osc->pCoord[2];

			idX[0] = 0;
			idY[0] = 1;
			idZ[0] = 2;

			lineKind = 1;

			doUpdate = &l_osc->pDiff_Osc->is_pCoord_update;

		
		break;


		case 2:

			Alloc_GraphData( 1 );	hasIndex = false;

			cposX[0] = &l_osc->pDiff_Osc->pCoord[0];
			cposY[0] = &l_osc->pDiff_Osc->pCoord[1];
			cposZ[0] = &l_osc->zRate;		// multiply by ex:6.0 for clearer visualisation...

			cposZ_Scale[0] = 6.0;

			idX[0] = 0;
			idY[0] = 1;
			idZ[0] = 2;

			lineKind = 1;

			doUpdate = &l_osc->pDiff_Osc->is_pCoord_update;

		
		break;



		// BVP+sen

		case 3:

			Alloc_GraphData( 1 );	hasIndex = true;

			cposX[0] = &l_osc->osc[temp_idx1]->x[0];
			cposY[0] = &l_osc->osc[temp_idx1]->x[1];
		
			cposZ_Scale[0] = 1.0;

			idX[0] = temp_idx1;
			idY[0] = -1;
			idZ[0] = -1;
		
		break;


		case 4:

			
		break;

		case 5:

			
		break;


	}
	

//	if( cCursor < 0 )	strcpy( dataName , "Phase Diff 3D" );
//	else				
	
	strcpy( dataName , GL_DATA_NAME_OSC_PLOT3D[cCursor+1] );

}


/*

void HUE2RGB( float H, float& R, float& G, float& B );


int GetNumNode( void );	// osc node num
int GetNumDoF( void );	// robot dof


int Show_All_MotorNeurons_OscRobo( GLHANDLE* glh )
{
	int nlhist = -1;
	int mul = 0;


	int NUMLINE = 0;
	if( GetRKIND() == NO_ROBOT )
	{
		NUMLINE = GetNumNode()/2;
		mul = 2;
		nlhist = 4000;
	}
	else
	{
		NUMLINE = GetNumDoF();	// This is differently set between robot & springNets

		// IF SpNets
		if( GetRS(0)->spnet )
		{
			mul = 2;
			nlhist = 4000;
		}
		// IF Robots
		else
		{
			mul = 4;
			nlhist = 300;
		}

	}


	glh->g_idX[0] = 0;
	glh->g_idY[0] = mul;

	glh->NUM_LINE_POS_CURRENT = 1;
	for( int i=2 ; i<NUMLINE ; i++ )
	{
		glh->g_idX[glh->NUM_LINE_POS_CURRENT] = 0;
		glh->g_idY[glh->NUM_LINE_POS_CURRENT] = i*mul;

		glh->NUM_LINE_POS_CURRENT++;
	}
	

	return nlhist;

}



void Init_Alloc_Plot2D_OscRobo( GLHANDLE* glh )
{
	int NUMLINE = 0;
	if( GetRKIND() == NO_ROBOT )	NUMLINE = GetNumNode();
	else							NUMLINE = GetNumDoF();

	glh->AllocPos( NUMLINE , 30000 );
					
	glh->NUM_LINE_POS_CURRENT = 1;

	float colourAngleDiv = 360.0f / glh->num_pos-1;
	for( int i=0 ; i<glh->num_pos ; i++ )
	{
		glh->isDashedLine[i] = false;
		glh->lineThick[i] = 1.0;

		if( i < 1 )		glh->lineColour[i][0] = glh->lineColour[i][1] = glh->lineColour[i][2] = 1.0f;
		else			HUE2RGB( colourAngleDiv*(float)(i-1) , glh->lineColour[i][0], glh->lineColour[i][1], glh->lineColour[i][2] );
	}

}
*/
