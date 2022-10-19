
#include "../Status.h"

#include "../NeuroUse/neurouse.h"
#include "../dynamics/rigidode.h"






void ResetGLPlot( void );

void InitSimulation( int );
void EndSimulation( int );

void TextDisplay( void );


// includes checking rk4 var num
void RefreshFixedText( void );


// if press F3
// This resets simulation of the current robot
void Zero_System( bool doRandomise )
{

	LOG( +1 , "[BGN] Zero_System(%s)" , doRandomise ? "true" : "false" );


	// Pause simulation
	if( g_Status->goStep )
	{
		g_Status->goStep = false;
		Sleep(10);
	}
	

	// Zero all Neural Controllers
	g_sim[0]->ZeroNN( );
		
	// Zero All Physics
	if( g_sim[0]->rs )
	{
		EndSimulation(0);
		InitSimulation(0);	// rs->ResetVariables() is included... which is same as rs->Zero()

		
	}

	if( g_sim[0]->eval )	g_sim[0]->eval->Zero();
	
	g_sim[0]->rk4->Zero();



	// One-time refresh of all zeroed texts
	TextDisplay( );

	// Zero all graphs
	ResetGLPlot( );

	RefreshFixedText( );

	// global counter
	g_Status->SimCount = 0;	


	LOG( -1 , "[END] Zero_System(%s)" , doRandomise ? "true" : "false" );

}





// This re-builds all Neural controllers and Robots
// Used when chaging to different robot

void InitTextDisplay( void );
void Refresh_GL_One_Frame( void );


void RecomposeNN( char* name )
{

	LOG( +1 , "[BGN] RecomposeNN(%s)" , name );


	// Pause simulation
	if( g_Status->goStep )
	{
		g_Status->goStep = false;
		Sleep(10);
	}


	if( g_Status->isPlotOut )
	{
		MessageBox( g_Status->g_hwnd , "Close All Output Windows!" , "Caution" , NULL );
		return;
	}

	
	g_sim[0]->rk4->Clean_Var();
		
	// ReCompose NNETs
	g_sim[0]->CleanNNET();

	g_sim[0]->EndEval();



	if( name ){		}
	else
	{		
		g_sim[0]->InitNNET();
	}


	g_sim[0]->InitEval();


	
	Zero_System( false );

	

	// Texts
	InitTextDisplay( );

	
	LOG( -1 , "[END] RecomposeNN(%s)" , name );


}
