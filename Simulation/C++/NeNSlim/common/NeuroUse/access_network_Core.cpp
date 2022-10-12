

////////////////////////////////////////////////////////////
////													////
////	These Functions are shared with EA Process		////
////													////
////////////////////////////////////////////////////////////


#include "neurouse.h"

#include "../Status.h"



// This is called from threaded EA


void SIMULATION::ZeroNN( void )
{

	LOG( 0 , "[BGN][END] SIMULATION::ZeroNN()" );

		oscil->Zero();
		oscil->RandY();
}



///////////// Initialisation Functions ////////////////////////////////////////////

// Connect Neuro-Activation to Motors
double* SIMULATION::Get_Neuro_Effector_Pointer( int i , bool is_X )
{	
	
	double*	AX = &oscil->alpha[i]->x;
	double*	AY = &oscil->alpha[i]->y;
	
	// x: real value, y: rk4 variable
	if( is_X )	return AX;
	else		return AY;

}


int SIMULATION::GetEffIdx( int n , int d , int idx )
{	
	if( idx == 1 )		return effIdx1[n][d];	// osc idx for flexor
	else if( idx == 2 )	return effIdx2[n][d];	// osc idx for extensor
	else				return -1;

}





///////////// Realtime Functions ////////////////////////////////////////////



void SIMULATION::SetSensor_Proprioceptor( int n , double IaL , double IaV , double IbF )	// sensor for alpha neurons
{
			oscil->IaSen[n]->IaL_y = IaL;
			oscil->IaSen[n]->IaV_y = IaV;
			oscil->IaSen[n]->IbF_y = IbF;
			
}



void SIMULATION::SetAsaiZ_DZ_Rate_Same_y( void )
{
	
	if( eval->dzRate_y < eval->zEpsilon )	oscil->zRate = 0.0;
	else									oscil->zRate = eval->dzRate_y;

	
	double g_z = oscil->baseZ - oscil->zRate * eval->mdz;

	for( int i=0 ; i<oscil->num_osc ; i++ )		oscil->osc[i]->z = g_z;



}

void SIMULATION::SetAsaiZ_DZ_Rate_Same( int thrId , RIGIDSTATE* rs )
{
	
	if( eval->dzRate < eval->zEpsilon )	oscil->zRate = 0.0;
	else								oscil->zRate = eval->dzRate;


	for( int i=0 ; i<oscil->num_osc ; i++ )
	{
		oscil->osc[i]->z = oscil->baseZ - oscil->zRate * eval->mdz;

	//	oscil->osc[i]->z = rs->baseZ - 0.2;
	}




}
