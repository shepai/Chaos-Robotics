

#include "./neurouse.h"






EVAL::EVAL( SIMULATION* sss )
{
	sim = sss;

	zEpsilon = 0.0;

	fix_dzRate_Mode = -1;	// -1: dynamic, 0: stable, 1: chaos

	// Set differently by each robot
	mdz = 0.0;
}


EVAL::~EVAL()
{


}



double Init_dzFunc( void );

void EVAL::Init_Eval( void )
{
	double oscTe = sim->oscil->oscT;

	vMeasureT = oscTe * 5.0;		// TC for immediate EFF - not the EFFICIENCY integration !!
	effUpdateT = oscTe * 1.0;

	maxET_Inc = oscTe *  1.0;
	maxET_Dec = oscTe *  50.0;
			
	dzT	= oscTe * 0.2;

	zEpsilon = Init_dzFunc( );

	Add_Eval_RK4_Var_No_RS( );	// No RS

	Zero();
	
}

void EVAL::Zero( void )		// called in rs->ReserVariable()
{

	LOG( 0 , "[BGN][END] EVAL::Zero()" );


	instant_Eff = 0.0;


	// RK4 Initialisation ~!!
	locospd.Zero();
	locospd_y = locospd;
	locospd_v.Zero();	

	dzRate = dzRate_y = 0.5;		dzRate_v = 0.0;

	EFFICIENCY = EFFICIENCY_y = EFFICIENCY_v = 0.0;

	maxE = 0.01;	maxE_y = maxE_v = 0.0;
	minE = minE_y = minE_v = 0.0;

}



void EVAL::Add_Eval_RK4_Var_No_RS( void )
{	
	
			sim->rk4_for_Init->Add_Var( &dzRate , &dzRate_y , &dzRate_v );

			sim->rk4_for_Init->Add_Var( &EFFICIENCY , &EFFICIENCY_y , &EFFICIENCY_v );

			sim->rk4_for_Init->Add_Var( &maxE , &maxE_y , &maxE_v );
			sim->rk4_for_Init->Add_Var( &minE , &minE_y , &minE_v );

}

void EVAL::Del_Eval_RK4_Var_No_RS( void )
{	
	
			sim->rk4_for_Init->Add_Var( &dzRate , &dzRate_y , &dzRate_v );

			sim->rk4_for_Init->Add_Var( &EFFICIENCY , &EFFICIENCY_y , &EFFICIENCY_v );

			sim->rk4_for_Init->Add_Var( &maxE , &maxE_y , &maxE_v );
			sim->rk4_for_Init->Add_Var( &minE , &minE_y , &minE_v );

}


void EVAL::Add_Eval_RK4_Var_RS( void )
{	
	if( sim->rs->is_use_CG )
	{
		sim->rk4_for_Init->Add_Var( &locospd.x , &locospd_y.x , &locospd_v.x );
		sim->rk4_for_Init->Add_Var( &locospd.z , &locospd_y.z , &locospd_v.z );
		
	}
}

void EVAL::Del_Eval_RK4_Var_RS( void )
{	
	if( sim->rs->is_use_CG )
	{
		sim->rk4_for_Init->Del_Var( &locospd.x );
		sim->rk4_for_Init->Del_Var( &locospd.z );
	}
}



// Neuro Access Functions

void EVAL::Evaluate_CE( void )
{
		 
	if( sim->rs )
	{
		if ( sim->rs->is_ODE_Robot )	Performance_ODE_Robots( );
		else{}
	}
	else{}


	

				EFFICIENCY_v = ( instant_Eff - EFFICIENCY ) / effUpdateT;

				// Set maxE - Goal Performance
				maxE_v = Set_MaxE_CE_dt( );

				// Set minE
				minE_v = Set_MinE_CE_dt( );
	

				dzRate_v = Set_dz_dt( );


				// not used yet - delete..
				//double dzRate_Trend_dt = ( dzRate - dzRate_Trend ) / maxET_Dec;
		
		
				switch( fix_dzRate_Mode )
				{
					case  0:	dzRate = dzRate_y =	0.0;	dzRate_v = 0.0; 	break;
					case  1:	dzRate = dzRate_y = 1.0;	dzRate_v = 0.0; 	break;

					case -1:	break;
				}


				sim->SetAsaiZ_DZ_Rate_Same_y( );

		

}




double EVAL::Set_MaxE_CE_dt( void )
{
	double maxE_tau;
	double eRate;
		
	if( EFFICIENCY_y < maxE_y )
	{
		maxE_tau = maxET_Dec;
	//	eRate = 1.0-dzRate;
		eRate = dzRate;
	}
	else
	{
		maxE_tau = maxET_Inc;
		eRate = 1.0;
	}


	return	eRate * ( EFFICIENCY_y - maxE_y ) / maxE_tau;
			
}


  
double EVAL::Set_MinE_CE_dt( void )
{
	double minE_tau;
	double eRate;
		
	if( EFFICIENCY_y > minE_y )
	{
		minE_tau = maxET_Dec;
	//	eRate = dzRate;
		eRate = 1.0-dzRate;
	}
	else
	{
		minE_tau = maxET_Inc;
		eRate = 1.0;
	}
	

	return	eRate * ( EFFICIENCY - minE ) / minE_tau;

}



// Various functions for [E,dz] behaviours

// Non-Hysteresis functions
double dzFunc_Sigmoid( double, double, double );
double dzFunc_Gompertz( double, double, double );
double dzFunc_Polynomial( double, double, double );
double dzFunc_Cosine( double, double, double );

// hysteresis in E-dz behav
double dzFunc_Binary( double, double, double );
double dzFunc_Hist( double, double, double );


// Initialisation
double (*dzFunc) ( double, double, double );
double Init_dzFunc( void )
{
	// initialise using this for non-hystereses functions
	dzFunc = dzFunc_Sigmoid;
	return dzFunc( 1.0, 0.0, 1.0 );	// for setting zEpsilon
	

	// use this for hysteresis functions
//	dzFunc = dzFunc_Hist;	
//	return 0.00001;	// for setting zEpsilon	
}

// In loop
double EVAL::Set_dz_dt( void )
{
	double tdz = dzFunc	( EFFICIENCY_y , minE_y , maxE_y );

	return ( tdz - dzRate_y ) / dzT;
}



// dz Functions
double dzFunc_Sigmoid( double x , double min , double max )
{
	return 1.0 / (  1.0 + exp(  (16.0/(max-min)) * (x-0.5*(max+min))  )   );
}

double dzFunc_Gompertz( double x , double min , double max )
{
	double e = 2.718281828459045;
	double a = 1.0;
//	double b = -exp(-10.0);	double c =  1.6*exp(2.0);
	double b = -exp(-5.0);	double c =  1.0*exp(2.0);
	
	return a * exp( b*exp( c*(x-min)/(max-min) ) );
}

double dzFunc_Polynomial( double x , double min , double max )
{
	if( x < max )
	{
		if( x > min )
		{
			return 2.0 * (x-min)*(x-min)*(x-min) / ((max-min)*(max-min)*(max-min)) 
				 - 3.0 * (x-min)*(x-min)		 / ((max-min)*(max-min))
				 + 1.0;
		}
		else
		{
			return 1.0;
		}
	}
	else
	{
		return 0.0;
	}

}

double dzFunc_Cosine( double x , double min , double max )
{
	if( x < max )
	{
		if( x > min )	return 0.5 * ( cos( 3.141592653589793*(x-min)/(max-min) ) + 1.0 );
		else			return 1.0;
	}
	else
	{
		return 0.0;
	}
}



// Below two are for adding hysteresis in E-dz behav

int histIdx = 0;	// if set to 1 by Get_dzFunc(), hState[1] is used for plot2D drawing mode

double dzFunc_Binary( double x , double , double max )
{
	static bool hState[2] = {true,true};

	if( hState[histIdx] )
	{
		if( x > 1.1*max ){		hState[histIdx] = false;	return 0.0;		}
		else			 {		hState[histIdx] = true;		return 1.0;		}
	}
	else
	{
		if( x < 0.9*max ){		hState[histIdx] = true;		return 1.0;		}
		else		 	 {		hState[histIdx] = false;	return 0.0;		}
	}

}



double dzFunc_Hist( double x , double min , double max )
{
	static bool hState[2] = {true,true};

	if( hState[histIdx] )
	{
        if( x > max )	hState[histIdx] = false;
        
        double new_max = max - 0.25*(max-min);
        return dzFunc_Polynomial( x , min , new_max );
    }
    else
    {
        if( x < min )   hState[histIdx] = true;
     
        double new_min = min + 0.25*(max-min);
        return dzFunc_Polynomial( x , new_min , max );
    }

}



// For Plot2D
double Get_dzFunc( double x , double min , double max )
{
	histIdx = 1;	// drawing mode
	double res = dzFunc( x , min , max );
	histIdx = 0;	// calc mode

	return res;
}


