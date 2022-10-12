


#include "rigidode.h"
#include "../NeuroUse/neurouse.h"






void RIGIDSTATE::SetBody_Quad2( void )
{
	LOG( 0 , "[BGN][END] RIGIDSTATE::SetBody_Quad2(void)" );
	
	NUM_ODE_BODY = 9;
	double CBDENS	= 5.0;
	double ARMDENS	= 5.0;

	gainstr = 2.0;

	
	BALLDENSITY = 0.001;

	InitPos.Set( 0.0 , 0.5 , 0.0 );
	BALLRAD = 0.05;
	
	
	int i,j;

	node	= new CAPNODE[NUM_ODE_BODY];
	motor	= new MOTOR[NUM_ODE_BODY-1];
	tupidx	= new TUPLE[NUM_ODE_BODY-1];
	
	node[0].DENSITY = CBDENS;
	node[0].shape	= 0;
	for( i=1 ; i<NUM_ODE_BODY ; i++ )
	{
		node[i].DENSITY = ARMDENS;
		node[i].shape	= 0;	// capped cylinder
	}

	node[0].R = 0.25;
	node[0].L = 0.01;
	node[0].ox.Set( 1.0 , 0.0 , 0.0 );
	node[0].oz.Set( 0.0 , 1.0 , 0.0 );
	node[0].oy.Set( 0.0 , 0.0 , -1.0 );
	node[0].pos = InitPos;


	double legR = 0.08;
	double legL = 0.56;

	node[1].R = legR;
	node[1].L = legL;	
	node[1].oz.Set( 1.0 , 0.0 , 0.0 );
	node[1].oy.Set( 0.0 , 1.0 , 0.0 );
	node[1].ox.Set( 0.0 , 0.0 , -1.0 );	
	node[1].pos = node[0].pos + ( node[0].ox*(0.5*node[0].R) );
	node[1].pos = node[1].pos + ( node[1].oz*(0.5*node[1].L) );

	node[2].R = legR;
	node[2].L = legL;	
	node[2].oz.Set( 0.0 , 0.0 , 1.0 );
	node[2].oy.Set( 0.0 , 1.0 , 0.0 );
	node[2].ox.Set( 1.0 , 0.0 , 0.0 );
	node[2].pos = node[0].pos + ( node[0].oy*(-0.5*node[0].R) );
	node[2].pos = node[2].pos + ( node[2].oz*(0.5*node[2].L) );

	node[3].R = legR;
	node[3].L = legL;	
	node[3].oz.Set( -1.0 , 0.0 , 0.0 );
	node[3].oy.Set( 0.0 , 1.0 , 0.0 );
	node[3].ox.Set( 0.0 , 0.0 , 1.0 );
	node[3].pos = node[0].pos + ( node[0].ox*(-0.5*node[0].R) );
	node[3].pos = node[3].pos + ( node[3].oz*(0.5*node[3].L) );
	
	node[4].R = legR;
	node[4].L = legL;	
	node[4].oz.Set( 0.0 , 0.0 , -1.0 );
	node[4].oy.Set( 0.0 , 1.0 , 0.0 );
	node[4].ox.Set( -1.0 , 0.0 , 0.0 );
	node[4].pos = node[0].pos + ( node[0].oy*(0.5*node[0].R) );
	node[4].pos = node[4].pos + ( node[4].oz*(0.5*node[4].L) );

	

	NUM_TIP = 4;
	for( i=5 ; i<NUM_ODE_BODY ; i++ )	node[i].isFoot = true;




	// For MuJoCo XML style (degree)
	double upperRange1 = -30.0;
	double upperRange2 =  30.0;

	double lowerRange1 = 30.0;	// 50 degree (0.87 rad) is centre position
	double lowerRange2 = 70.0;


	// Convert to radians
	double PI = 3.141592653589793;

	double uRange = 30.0 * PI / 180.0;
	double lRange = 20.0 * PI / 180.0;



	double bend = tan( 50.0*PI/180.0 );

	
	node[5].R = legR;
	node[5].L = legL;	
	node[5].oz.Set( 1.0 , -bend , 0.0 );	node[5].oz.Normalize();
	node[5].oy.Set( bend , 1.0 , 0.0 );		node[5].oy.Normalize();
	node[5].ox.Set( 0.0 , 0.0 , -1.0 );	
	node[5].pos = node[1].pos + ( node[1].oz*(0.5*node[1].L) );
	node[5].pos = node[5].pos + ( node[5].oz*(0.5*node[5].L) );

	node[6].R = legR;
	node[6].L = legL;	
	node[6].oz.Set( 0.0 , -bend , 1.0 );	node[6].oz.Normalize();
	node[6].oy.Set( 0.0 , 1.0 , bend );		node[6].oy.Normalize();
	node[6].ox.Set( 1.0 , 0.0 , 0.0 );
	node[6].pos = node[2].pos + ( node[2].oz*(0.5*node[2].L) );
	node[6].pos = node[6].pos + ( node[6].oz*(0.5*node[6].L) );

	node[7].R = legR;
	node[7].L = legL;
	node[7].oz.Set( -1.0 , -bend , 0.0 );	node[7].oz.Normalize();
	node[7].oy.Set( -bend , 1.0 , 0.0 );	node[7].oy.Normalize();
	node[7].ox.Set( 0.0 , 0.0 , 1.0 );
	node[7].pos = node[3].pos + ( node[3].oz*(0.5*node[3].L) );
	node[7].pos = node[7].pos + ( node[7].oz*(0.5*node[7].L) );
	
	node[8].R = legR;
	node[8].L = legL;
	node[8].oz.Set( 0.0 , -bend , -1.0 );	node[8].oz.Normalize();
	node[8].oy.Set( 0.0 , 1.0 , -bend );	node[8].oy.Normalize();
	node[8].ox.Set( -1.0 , 0.0 , 0.0 );
	node[8].pos = node[4].pos + ( node[4].oz*(0.5*node[4].L) );
	node[8].pos = node[8].pos + ( node[8].oz*(0.5*node[8].L) );

	for( i=0 ; i<NUM_ODE_BODY ; i++ )
	{
		node[i].ox.Normalize();
		node[i].oy.Normalize();
		node[i].oz.Normalize();
	}
	


	motor[0].dof		=  1;
	motor[0].anchor		= node[1].pos + ( node[1].oz*(-0.5*node[1].L) );	
	motor[0].orien[0]	= node[1].oy;
	
	motor[1].dof		=  1;
	motor[1].anchor		= node[2].pos + ( node[2].oz*(-0.5*node[2].L) );	
	motor[1].orien[0]	= node[2].oy;
	
	motor[2].dof		=  1;
	motor[2].anchor		= node[3].pos + ( node[3].oz*(-0.5*node[3].L) );	
	motor[2].orien[0]	= node[3].oy;
	
	motor[3].dof		=  1;
	motor[3].anchor		= node[4].pos + ( node[4].oz*(-0.5*node[4].L) );	
	motor[3].orien[0]	= node[4].oy;
	

	motor[4].dof		=  1;
	motor[4].anchor		= node[5].pos + ( node[5].oz*(-0.5*node[5].L) );	
	motor[4].orien[0]	= node[5].ox * 1.0;
	
	motor[5].dof		=  1;
	motor[5].anchor		= node[6].pos + ( node[6].oz*(-0.5*node[6].L) );
	motor[5].orien[0]	= node[6].ox * 1.0;
	
	motor[6].dof		=  1;
	motor[6].anchor		= node[7].pos + ( node[7].oz*(-0.5*node[7].L) );	
	motor[6].orien[0]	= node[7].ox * 1.0;
	
	motor[7].dof		=  1;
	motor[7].anchor		= node[8].pos + ( node[8].oz*(-0.5*node[8].L) );	
	motor[7].orien[0]	= node[8].ox * 1.0;
	

	
	

	for( j=0 ; j<4 ; j++ )
	{
		motor[j].Lstop[0] = -uRange;
		motor[j].Hstop[0] =  uRange;
	}

	for( j=4 ; j<8 ; j++ )
	{
		motor[j].Lstop[0] = -lRange;
		motor[j].Hstop[0] =  lRange;
	}

	tupidx[0][0] = 1;	tupidx[0][1] = 0;
	tupidx[1][0] = 2;	tupidx[1][1] = 0;
	tupidx[2][0] = 3;	tupidx[2][1] = 0;
	tupidx[3][0] = 4;	tupidx[3][1] = 0;

	tupidx[4][0] = 5;	tupidx[4][1] = 1;
	tupidx[5][0] = 6;	tupidx[5][1] = 2;
	tupidx[6][0] = 7;	tupidx[6][1] = 3;
	tupidx[7][0] = 8;	tupidx[7][1] = 4;

	


	/////// Simulator & Motors ////////////////////////////////////////////////////////////////

//	SetBody_Walkers_Common( 300.0 , true );		mu_coeff = 1.0;

}




void RIGIDSTATE::SetSensor_Quad2( void )
{
	SetSensor_CE_Walker_Common( );
}


void RIGIDSTATE::AfterConstruct_Quad2( void )
{
	SetBody_CE_Walkers_Common( 300.0 , true );		mu_coeff = 1.0;


	// Robot Specific CE Paramameters
	sim->eval->mdz = 0.315;
}




