

#include "./neurouse.h"

#include "../dynamics/rigidode.h"



// Just Simply Mesure Forward Locomotion
void EVAL::Performance_ODE_Robots( void )
{
	// Calculate EFFICIENCY
	VEC3 SPD = sim->rs->CGv;
	
	locospd_v = ( SPD - locospd_y ) / vMeasureT;
	
	
	instant_Eff = 10.0 * locospd_y.Mag();
}










static double PI2 = 6.283185307179586;

static double targetPhaseX[4], targetPhaseY[4];
static double distsq[4], variance[4], maxAmp[4];


// 16/03/2017 - tested with mdz=0.31 without learning
void Eval_1D_Phase_Diff( OSCNET* sp_osc , EVAL* eval )
{	
	int num_peak = 2;

	targetPhaseX[0] =  2.5;		variance[0] = 0.3;	maxAmp[0] = 1.0;
	targetPhaseX[1] =  4.0;		variance[1] = 0.3;	maxAmp[1] = 0.8;


	for( int i=0 ; i<num_peak ; i++ )	// 2 peaks
	{
		double distX = fabs(sp_osc->pDiff_Osc->pCoord[0] - targetPhaseX[i]);

		// distance on wrapped torus
		double distX2 = PI2-distX;		if( distX > distX2 )	distX = distX2;

		distsq[i] = distX*distX;
	}
			
	if( sp_osc->pDiff_Osc->is_pCoord_update )
	{
		double gaussVal = 0.0;
		for( int i=0 ; i<num_peak ; i++ )
			gaussVal += maxAmp[i] * exp( -distsq[i] / (2.0*variance[i]*variance[i]) );	// gaussian fitness

		eval->instant_Eff = gaussVal;
	}

}




//static double distsq_Old[4];

void Eval_2D_Phase_Diff( OSCNET* sp_osc , EVAL* eval )
{
	// mixture of 4 2d-gaussians
	int num_peak = 4;
	
	double gMax = 1.0 / (0.64*25.0*0.694444444444444);
	double var0 = 0.3;

	maxAmp[0] = 1.0;	maxAmp[1] = 0.8;	maxAmp[2] = 0.6;	maxAmp[3] = 0.4;
//	maxAmp[0] = 1.0;	maxAmp[1] = 0.9;	maxAmp[2] = 0.8;	maxAmp[3] = 0.7;


//	for( int i=0 ; i<num_peak ; i++ )	variance[i] = var0/sqrt(maxAmp[i]);
	for( int i=0 ; i<num_peak ; i++ )	variance[i] = var0;

	
	
	// works for M2_S3_Serial
/*	targetPhaseX[0] = 4.9;	targetPhaseY[0] = 5.0;
	targetPhaseX[1] = 4.4;	targetPhaseY[1] = 1.5;
	targetPhaseX[2] = 1.5;	targetPhaseY[2] = 4.4;
	targetPhaseX[3] = 1.8;	targetPhaseY[3] = 1.8;
*/

	targetPhaseX[0] = 4.4;	targetPhaseY[0] = 1.5;
	targetPhaseX[1] = 1.5;	targetPhaseY[1] = 4.4;
	targetPhaseX[2] = 1.8;	targetPhaseY[2] = 1.8;
	targetPhaseX[3] = 4.9;	targetPhaseY[3] = 5.0;


	for( int i=0 ; i<num_peak ; i++ )
	{
		double distX = fabs(sp_osc->pDiff_Osc->pCoord[0] - targetPhaseX[i]);
		double distY = fabs(sp_osc->pDiff_Osc->pCoord[1] - targetPhaseY[i]);

		// distance on wrapped torus
		double distX2 = PI2-distX;		if( distX > distX2 )	distX = distX2;
		double distY2 = PI2-distY;		if( distY > distY2 )	distY = distY2;

		distsq[i] = distX*distX + distY*distY;
	}
			
	if( sp_osc->pDiff_Osc->is_pCoord_update )
	{
		// gaussian fitness
		double gaussVal = 0.0;
		for( int i=0 ; i<num_peak ; i++ )
		{
		//	gaussVal += gMax * exp( -distsq[i] / (2.0*variance[i]*variance[i]) ) / (variance[i]*variance[i]);

			gaussVal += maxAmp[i] * exp( -distsq[i] / (2.0*variance[i]*variance[i]) );
		}

		eval->instant_Eff = gaussVal;
	}


}




///// Perlin Noise Terrain - Add smooth noise to the Gaussian Hills /////////////////////////////////////////////

// Do when necessary - May 2017

static VEC3 PerlinGrid2D[100][100];

void Init_Perlin_2D( void )
{
	int num_bin = 100;
}

double Get_Perlin_2D( double x , double y )
{


	return 0.0;
}





// From ODE Robots

/*
// Just Simply Mesure Forward Locomotion
void RIGIDSTATE::Performance_ODE_Robots( void )
{
	// Calculate EFFICIENCY
	VEC3 SPD;
	if( GetRKIND() == WALKER_RS_HW )	SPD = wsVel;
	else								SPD = CGv;
	
	VEC3 dotV = ( SPD-target ) / vMeasureT;
	target = target + dotV*STEPSIZE;
	
	if( GetENV() != AQUATIC )	target.y = 0.0;


	// phase based/
//	if( GetOSCIL(0)->pCoord[0] > -1000.0 )
	
	instant_Eff = 10.0 * target.Mag();
}

*/






/*
#include "../Status.h"

// Repeated Run
void RIGIDSTATE::Evaluate2_CE( void )
{
	double avrT = oscTr*10.0;

	// Calculate EFFICIENCY
	VEC3 SPD = CGv;

	// just for measurement
	double VxV2 = ( SPD.x - Vx2 ) / avrT;
	double VzV2 = ( SPD.z - Vz2 ) / avrT;

	Vx2 = Vx2 + STEPSIZE*VxV2;
	Vz2 = Vz2 + STEPSIZE*VzV2;
	
	double EFF2 = 10.0 * sqrt( Vx2*Vx2 + Vz2*Vz2 );
	

	double denom = (double)( g_Status->SimCount + 1 );

	if( dzRate > 0.0001 )	EFF2 = 0.0;

//	AVRG = AVRG*((denom-1.0)/denom) + EFF2/denom;

}
*/



/*
	// Temp Comparison -- 26/07/2017
	for( int i=0 ; i<num_peak ; i++ )
	{
		double distX = fabs(sp_osc->pCoordOld[0] - targetPhaseX[i]);
		double distY = fabs(sp_osc->pCoordOld[1] - targetPhaseY[i]);

		// distance on wrapped torus
		double distX2 = PI2-distX;		if( distX > distX2 )	distX = distX2;
		double distY2 = PI2-distY;		if( distY > distY2 )	distY = distY2;

		distsq_Old[i] = distX*distX + distY*distY;
	}
			
	if( sp_osc->is_pCoord_update_Old )
	{
		double gaussVal = 0.0;
		for( int i=0 ; i<num_peak ; i++ )
			gaussVal += gMax * exp( -distsq_Old[i] / (2.0*variance[i]*variance[i]) ) / (variance[i]*variance[i]);	// gaussian fitness

		rs->effV_Old = gaussVal;
	}
*/




// Removed May 07 2017 -- 2D PhaseDiff Eval

		/*	targetPhaseX[0] = -1.4;	targetPhaseY[0] = -1.3;		variance[0] = 0.4;	maxAmp[0] = 1.0;
			targetPhaseX[1] = -1.9;	targetPhaseY[1] =  1.5;		variance[1] = 0.4;	maxAmp[1] = 0.3;
			targetPhaseX[2] =  1.5;	targetPhaseY[2] = -1.9;		variance[2] = 0.4;	maxAmp[2] = 0.2;
			targetPhaseX[3] =  1.8;	targetPhaseY[3] =  1.8;		variance[3] = 0.4;	maxAmp[3] = 0.1;


			for( int i=0 ; i<4 ; i++ )
			{
				double distX = fabs(sp_osc->pCoord[0] - targetPhaseX[i]);
				double distY = fabs(sp_osc->pCoord[1] - targetPhaseY[i]);

				// distance on wrapped torus
				double distX2 = 2.0*PI-distX;		if( distX > distX2 )	distX = distX2;
				double distY2 = 2.0*PI-distY;		if( distY > distY2 )	distY = distY2;

				distsq[i] = distX*distX + distY*distY;
			}
			
			if( sp_osc->pCoord[0] > -1000.0 )
			{
				double gaussVal = 0.0;
				for( int i=0 ; i<4 ; i++ )
					gaussVal += maxAmp[i] * exp( -distsq[i] / (2.0*variance[i]*variance[i]) );	// gaussian fitness

				effV = gaussVal;
			}

			targetPhaseX[0] = PI2-1.4;	targetPhaseY[0] = PI2-1.3;
			targetPhaseX[1] = PI2-1.9;	targetPhaseY[1] =     1.5;
			targetPhaseX[2] =     1.5;	targetPhaseY[2] = PI2-1.9;
			targetPhaseX[3] =     1.8;	targetPhaseY[3] =     1.8;
		*/




// Removed Nov 2016 --> without alphaOut

/*	spnet->SetEffectors( );			// change k & d according to nOut / Bring osc variable.
	spnet->SetSensors( );			// send processed sensor values to OSC before SensorAdapt() is forward-updated.

//	spnet->SetEffectors( );			// change k & d according to nOut / Bring osc variable.


	if( sp_osc->is_CTRNN ){		}	// if ctrnn higher controller exists


	sp_osc->ForwardStep_RK4_LLC( );		// oscillator x+=h*v !!

	
	// Sensor Adaptation - x+=h*v is being done but the values are pre-used above
	for( int c=0 ; c<spnet->num_muscle ; c++ )	spnet->con[c]->SensorAdapt( this );

	// SpringNet x+=h*v inside!!
	spnet->ForwardStepRK4( );
*/





/*	// Calculate Velocities !! ////////////////////////////////////////////////////////////////////////////////

	spnet->SetEffectors( );			// change k & d according to nOut Bring osc variable.

	spnet->SetSensors( );
	sp_osc->ForwardStep_RK4( );		// "is_SpringNet" flag must be set !!

	// Sensor Adaptation
	for( int c=0 ; c<spnet->num_muscle ; c++ )	spnet->con[c]->SensorAdapt_V_Only( this );
	



	// Progress One Step All Together !! //////////////////////////////////////////////////////////////////////

	target = target + dotV*STEPSIZE;	target.y = 0.0;
	maxE += STEPSIZE*maxEV;
	dzRate += STEPSIZE*dzV;		
//	if( dzRate < 0.000001 )	dzRate = 0.0;

	sp_osc->ProgressStep_LLE( );

	for( int c=0 ; c<spnet->num_muscle ; c++ )	spnet->con[c]->ProgressStep_SensorAdapt( this );

	spnet->ForwardStepRK4( );		// This do x+=h*v inside!! --> must be last called
*/

