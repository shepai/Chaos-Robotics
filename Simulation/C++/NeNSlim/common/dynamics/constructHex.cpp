


#include "rigidode.h"
#include "../NeuroUse/neurouse.h"




void RIGIDSTATE::SetBody_Hex( void )
{
	LOG( 0 , "[BGN][END] RIGIDSTATE::SetBody_Hex(void)" );

	NUM_ODE_BODY = 13;
	double CBDENS	= 400.0;
	double ARMDENS	= 100.0;

	gainstr = 3.0;

	
	BALLDENSITY = 0.001;

	InitPos.Set( 0.0 , 0.22 , 0.0 );
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

	node[0].R = 0.05;
	node[0].L = 1.8;
	node[0].ox.Set( 1.0 , 0.0 , 0.0 );
	node[0].oz.Set( 0.0 , 0.0 , 1.0 );
	node[0].oy.Set( 0.0 , 1.0 , 0.0 );
	node[0].pos = InitPos;


	double legR = 0.05;
	double legL = 0.3;

	// front-right
	node[1].R = legR;
	node[1].L = legL;	
	node[1].oz.Set( 1.0 , 0.0 , 0.0 );
	node[1].oy.Set( 0.0 , 1.0 , 0.0 );
	node[1].ox.Set( 0.0 , 0.0 , -1.0 );	
	node[1].pos = node[0].pos + ( node[0].oz*(0.5*node[0].L) );
	node[1].pos = node[1].pos + ( node[1].oz*(0.5*node[1].L) );

	// middle-right
	node[2].R = legR;
	node[2].L = legL;	
	node[2].oz.Set( 1.0 , 0.0 , 0.0 );
	node[2].oy.Set( 0.0 , 1.0 , 0.0 );
	node[2].ox.Set( 0.0 , 0.0 , -1.0 );	
	node[2].pos = node[0].pos;
	node[2].pos = node[2].pos + ( node[2].oz*(0.5*node[2].L) );

	// rear-right
	node[3].R = legR;
	node[3].L = legL;	
	node[3].oz.Set( 1.0 , 0.0 , 0.0 );
	node[3].oy.Set( 0.0 , 1.0 , 0.0 );
	node[3].ox.Set( 0.0 , 0.0 , -1.0 );
	node[3].pos = node[0].pos + ( node[0].oz*(-0.5*node[0].L) );
	node[3].pos = node[3].pos + ( node[3].oz*(0.5*node[3].L) );

		
	// front-left
	node[4].R = legR;
	node[4].L = legL;	
	node[4].oz.Set( -1.0 , 0.0 , 0.0 );
	node[4].oy.Set( 0.0 , 1.0 , 0.0 );
	node[4].ox.Set( 0.0 , 0.0 , 1.0 );
	node[4].pos = node[0].pos + ( node[0].oz*(0.5*node[0].L) );
	node[4].pos = node[4].pos + ( node[4].oz*(0.5*node[4].L) );
	
	// middle-left
	node[5].R = legR;
	node[5].L = legL;	
	node[5].oz.Set( -1.0 , 0.0 , 0.0 );
	node[5].oy.Set( 0.0 , 1.0 , 0.0 );
	node[5].ox.Set( 0.0 , 0.0 , 1.0 );
	node[5].pos = node[0].pos;
	node[5].pos = node[5].pos + ( node[5].oz*(0.5*node[5].L) );

	// rear-left
	node[6].R = legR;
	node[6].L = legL;	
	node[6].oz.Set( -1.0 , 0.0 , 0.0 );
	node[6].oy.Set( 0.0 , 1.0 , 0.0 );
	node[6].ox.Set( 0.0 , 0.0 , 1.0 );
	node[6].pos = node[0].pos + ( node[0].oz*(-0.5*node[0].L) );
	node[6].pos = node[6].pos + ( node[6].oz*(0.5*node[6].L) );

	



	double bend = 0.6;
	legL = 0.3;
	
	node[7].R = legR;
	node[7].L = legL;	
	node[7].oz.Set( 1.0 , -bend , 0.0 );	node[7].oz.Normalize();
	node[7].oy.Set( bend , 1.0 , 0.0 );		node[7].oy.Normalize();
	node[7].ox.Set( 0.0 , 0.0 , -1.0 );	
	node[7].pos = node[1].pos + ( node[1].oz*(0.5*node[1].L) );
	node[7].pos = node[7].pos + ( node[7].oz*(0.5*node[7].L) );

	node[8].R = legR;
	node[8].L = legL;	
	node[8].oz.Set( 1.0 , -bend , 0.0 );	node[8].oz.Normalize();
	node[8].oy.Set( bend , 1.0 , 0.0 );		node[8].oy.Normalize();
	node[8].ox.Set( 0.0 , 0.0 , -1.0 );	
	node[8].pos = node[2].pos + ( node[2].oz*(0.5*node[2].L) );
	node[8].pos = node[8].pos + ( node[8].oz*(0.5*node[8].L) );

	node[9].R = legR;
	node[9].L = legL;	
	node[9].oz.Set( 1.0 , -bend , 0.0 );	node[9].oz.Normalize();
	node[9].oy.Set( bend , 1.0 , 0.0 );		node[9].oy.Normalize();
	node[9].ox.Set( 0.0 , 0.0 , -1.0 );
	node[9].pos = node[3].pos + ( node[3].oz*(0.5*node[3].L) );
	node[9].pos = node[9].pos + ( node[9].oz*(0.5*node[9].L) );

	node[10].R = legR;
	node[10].L = legL;	
	node[10].oz.Set( -1.0 , -bend , 0.0 );	node[10].oz.Normalize();
	node[10].oy.Set( -bend , 1.0 , 0.0 );	node[10].oy.Normalize();
	node[10].ox.Set( 0.0 , 0.0 , 1.0 );
	node[10].pos = node[4].pos + ( node[4].oz*(0.5*node[4].L) );
	node[10].pos = node[10].pos + ( node[10].oz*(0.5*node[10].L) );

	node[11].R = legR;
	node[11].L = legL;	
	node[11].oz.Set( -1.0 , -bend , 0.0 );	node[11].oz.Normalize();
	node[11].oy.Set( -bend , 1.0 , 0.0 );	node[11].oy.Normalize();
	node[11].ox.Set( 0.0 , 0.0 , 1.0 );
	node[11].pos = node[5].pos + ( node[5].oz*(0.5*node[5].L) );
	node[11].pos = node[11].pos + ( node[11].oz*(0.5*node[11].L) );
	
	node[12].R = legR;
	node[12].L = legL;	
	node[12].oz.Set( -1.0 , -bend , 0.0 );	node[12].oz.Normalize();
	node[12].oy.Set( -bend , 1.0 , 0.0 );	node[12].oy.Normalize();
	node[12].ox.Set( 0.0 , 0.0 , 1.0 );
	node[12].pos = node[6].pos + ( node[6].oz*(0.5*node[6].L) );
	node[12].pos = node[12].pos + ( node[12].oz*(0.5*node[12].L) );

	for( i=0 ; i<NUM_ODE_BODY ; i++ )
	{
		node[i].ox.Normalize();
		node[i].oy.Normalize();
		node[i].oz.Normalize();
	}


	


	motor[0].dof		=  1;
	motor[0].anchor		= node[1].pos + ( node[1].oz*(-0.5*node[1].L) );	
	motor[0].orien[0]	= node[0].oy;

	motor[1].dof		=  1;
	motor[1].anchor		= node[2].pos + ( node[2].oz*(-0.5*node[2].L) );	
	motor[1].orien[0]	= node[1].oy;
	
	motor[2].dof		=  1;
	motor[2].anchor		= node[3].pos + ( node[3].oz*(-0.5*node[3].L) );	
	motor[2].orien[0]	= node[2].oy;

	motor[3].dof		=  1;
	motor[3].anchor		= node[4].pos + ( node[4].oz*(-0.5*node[4].L) );	
	motor[3].orien[0]	= node[3].oy * -1.0;
	
	motor[4].dof		=  1;
	motor[4].anchor		= node[5].pos + ( node[5].oz*(-0.5*node[5].L) );	
	motor[4].orien[0]	= node[4].oy * -1.0;
	
	motor[5].dof		=  1;
	motor[5].anchor		= node[6].pos + ( node[6].oz*(-0.5*node[6].L) );	
	motor[5].orien[0]	= node[5].oy * -1.0;
		
///////////
	
	motor[6].dof		=  1;
	motor[6].anchor		= node[7].pos + ( node[7].oz*(-0.5*node[7].L) );	
	motor[6].orien[0]	= node[7].ox;
	
	motor[7].dof		=  1;
	motor[7].anchor		= node[8].pos + ( node[8].oz*(-0.5*node[8].L) );	
	motor[7].orien[0]	= node[8].ox;
	
	motor[8].dof		=  1;
	motor[8].anchor		= node[9].pos + ( node[9].oz*(-0.5*node[9].L) );	
	motor[8].orien[0]	= node[9].ox;
	
	motor[9].dof		=  1;
	motor[9].anchor		= node[10].pos + ( node[10].oz*(-0.5*node[10].L) );	
	motor[9].orien[0]	= node[10].ox * 1.0;
	
	motor[10].dof		=  1;
	motor[10].anchor	= node[11].pos + ( node[11].oz*(-0.5*node[11].L) );	
	motor[10].orien[0]	= node[11].ox * 1.0;
	
	motor[11].dof		=  1;
	motor[11].anchor	= node[12].pos + ( node[12].oz*(-0.5*node[12].L) );	
	motor[11].orien[0]	= node[12].ox * 1.0;
	
	

	double	QPI = 1.570796326794896;		//  90 degree

	for( j=0 ; j<6 ; j++ )
	{
		motor[j].Lstop[0] = -QPI*0.3;
		motor[j].Hstop[0] =  QPI*0.3;
	}

	for( j=6 ; j<12 ; j++ )
	{
		motor[j].Lstop[0] = -QPI*0.2;
		motor[j].Hstop[0] =  QPI*0.2;
	}


	// attach
	tupidx[0][0] = 1;	tupidx[0][1] = 0;
	tupidx[1][0] = 2;	tupidx[1][1] = 0;
	tupidx[2][0] = 3;	tupidx[2][1] = 0;
	tupidx[3][0] = 4;	tupidx[3][1] = 0;
	tupidx[4][0] = 5;	tupidx[4][1] = 0;
	tupidx[5][0] = 6;	tupidx[5][1] = 0;

	tupidx[6][0] = 7;	tupidx[6][1] = 1;
	tupidx[7][0] = 8;	tupidx[7][1] = 2;
	tupidx[8][0] = 9;	tupidx[8][1] = 3;
	tupidx[9][0] = 10;	tupidx[9][1] = 4;
	tupidx[10][0] = 11;	tupidx[10][1] = 5;
	tupidx[11][0] = 12;	tupidx[11][1] = 6;



	/////// Simulator & Motors ////////////////////////////////////////////////////////////////

//	SetBody_Walkers_Common( 800.0 , true );		mu_coeff = 1.0;

}




void RIGIDSTATE::SetSensor_Hex( void )
{	
	SetSensor_CE_Walker_Common( );
}


void RIGIDSTATE::AfterConstruct_Hex( void )
{	
	SetBody_CE_Walkers_Common( 800.0 , true );		mu_coeff = 1.0;


	// Robot Specific CE Paramameters
	sim->eval->mdz = 0.30;
}