

#include "neurouse.h"	// All network types are declared


SIMULATION* g_sim[NUM_MAX_EA_THR] = {0,};

int NUM_EA_THR = 1;
int Get_NUM_EA_THR( void ){		return NUM_EA_THR;		}
void Set_NUM_EA_THR( int nThr ){	NUM_EA_THR = nThr;	}




void Init_SIMULATION( ENUM_ROBOT rKind )
{
	LOG( +1 , "[BGN] void Init_SIMULATION( %s )",	ENUM_ROBOT_NAME[rKind] 	);


	for( int T=0 ; T<NUM_EA_THR ; T++ )
	{
		g_sim[T] = new SIMULATION();

		g_sim[T]->threadIdx = T;

		g_sim[T]->robotKind = rKind;
		
		g_sim[T]->Init();
	}

	LOG( -1 , "[END] void Init_SIMULATION(,,,)" );

}

void Delete_SIMULATION( void )
{
	for( int T=0 ; T<NUM_EA_THR ; T++ )	delete g_sim[T];
}





/// ///////////////////////////////////////////////////////////////////////////
SIMULATION::SIMULATION()
{
	threadIdx = -1;

	oscil = NULL;

	rs = NULL;

	rk4_for_Init = rk4 = NULL;

	eval = NULL;

	
	world = NULL;	space = NULL;	contactgroup = NULL;	ground = NULL;

	for( int i=0 ; i<50 ; i++ )
	for( int d=0 ; d<3 ; d++ )
	{
		effIdx1[i][d] = effIdx2[i][d] = -1;
	}

	nrp.sim = this;
}

void SIMULATION::Init( void )
{	
	rk4 = new RK4(this);
	rk4_for_Init = rk4;

	InitNNET();
	ZeroNN( );

	InitEval();
}

void SIMULATION::Zero( void )
{

}



SIMULATION::~SIMULATION()
{
	CleanNNET();

	if( rk4 )
	{
		rk4_for_Init = NULL;
		delete rk4;	
	}

	EndRS();

	EndEval();
}




void SIMULATION::AllocNNET( void )		// Only Allocation
{

	LOG( 0 , "[BGN][END] SIMULATION::AllocNNET() for oscil" );

		
	oscil = new OSCNET(this);
				
}




void SIMULATION::InitNNET( void )		// Only Randomize | HandCraft
{
	AllocNNET( );

	oscil->ManualNet();
}



void SIMULATION::CleanNNET( void )
{
	delete oscil;	oscil = 0x00000000l;
	
}


// OSC-CTRNN
void SIMULATION::Reload_CTRNN_for_OSCIL( void )
{
	
}



