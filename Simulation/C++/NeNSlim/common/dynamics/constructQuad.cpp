


#include "rigidode.h"





void RIGIDSTATE::SetBody_Quad( void )
{
	LOG( 0 , "[BGN][END] RIGIDSTATE::SetBody_Quad(void)" );

	NUM_ODE_BODY = 9;

//	double CBDENS	= 200.0;
//	double ARMDENS	= 100.0;

	double CBDENS	= 400.0;
	double ARMDENS	= 100.0;


	
	BALLDENSITY = 0.001;
	BALLRAD		= 0.05;

	gainstr		= 2.0;

	
	InitPos.Set( 0.0 , 0.22 , 0.0 );
	
	
	
	int i,j;

	node	= new CAPNODE[NUM_ODE_BODY];
	motor	= new MOTOR[NUM_ODE_BODY-1];
	tupidx	= new TUPLE[NUM_ODE_BODY-1];
	
	node[0].DENSITY = CBDENS;
	node[0].shape	= 0;
	for( i=1 ; i<NUM_ODE_BODY ; i++ )
	{
		node[i].DENSITY = ARMDENS;
		node[i].shape	= 0;			// capped cylinder
	}

	node[0].R = 0.05;
	node[0].L = 0.9;
	node[0].ox.Set( 1.0 , 0.0 , 0.0 );
	node[0].oz.Set( 0.0 , 0.0 , 1.0 );
	node[0].oy.Set( 0.0 , 1.0 , 0.0 );
	node[0].pos = InitPos;


//	double legR = 0.05;		double legL = 0.3;

	double legR[9],		legL[9];

	for( i=1 ; i<NUM_ODE_BODY ; i++ ){	legR[i] = 0.05;		legL[i] = 0.3;	}

	// Irregular sized Legs
/*	legR[1] = 0.05;		legL[1] = 0.3;
	legR[2] = 0.07;		legL[2] = 0.2;
	legR[3] = 0.04;		legL[3] = 0.4;
	legR[4] = 0.05;		legL[4] = 0.5;
	legR[5] = 0.03;		legL[5] = 0.43;
	legR[6] = 0.05;		legL[6] = 0.6;
	legR[7] = 0.07;		legL[7] = 0.5;
	legR[8] = 0.04;		legL[8] = 0.4;
*/

	node[1].R = legR[1];
	node[1].L = legL[1];	
	node[1].oz.Set( 1.0 , 0.0 , 0.0 );
	node[1].oy.Set( 0.0 , 1.0 , 0.0 );
	node[1].ox.Set( 0.0 , 0.0 , -1.0 );	
	node[1].pos = node[0].pos + ( node[0].oz*(0.5*node[0].L) );
	node[1].pos = node[1].pos + ( node[1].oz*(0.5*node[1].L) );

	node[2].R = legR[2];
	node[2].L = legL[2];	
	node[2].oz.Set( 1.0 , 0.0 , 0.0 );
	node[2].oy.Set( 0.0 , 1.0 , 0.0 );
	node[2].ox.Set( 0.0 , 0.0 , -1.0 );
	node[2].pos = node[0].pos + ( node[0].oz*(-0.5*node[0].L) );
	node[2].pos = node[2].pos + ( node[2].oz*(0.5*node[2].L) );

	node[3].R = legR[3];
	node[3].L = legL[3];	
	node[3].oz.Set( -1.0 , 0.0 , 0.0 );
	node[3].oy.Set( 0.0 , 1.0 , 0.0 );
	node[3].ox.Set( 0.0 , 0.0 , 1.0 );
	node[3].pos = node[0].pos + ( node[0].oz*(0.5*node[0].L) );
	node[3].pos = node[3].pos + ( node[3].oz*(0.5*node[3].L) );
	
	node[4].R = legR[4];
	node[4].L = legL[4];	
	node[4].oz.Set( -1.0 , 0.0 , 0.0 );
	node[4].oy.Set( 0.0 , 1.0 , 0.0 );
	node[4].ox.Set( 0.0 , 0.0 , 1.0 );
	node[4].pos = node[0].pos + ( node[0].oz*(-0.5*node[0].L) );
	node[4].pos = node[4].pos + ( node[4].oz*(0.5*node[4].L) );

	
	NUM_TIP = 4;
	for( i=5 ; i<NUM_ODE_BODY ; i++ )	node[i].isFoot = true;

	double bend = 0.6;
	
	node[5].R = legR[5];
	node[5].L = legL[5];	
	node[5].oz.Set( 1.0 , -bend , 0.0 );	node[5].oz.Normalize();
	node[5].oy.Set( bend , 1.0 , 0.0 );		node[5].oy.Normalize();
	node[5].ox.Set( 0.0 , 0.0 , -1.0 );	
	node[5].pos = node[1].pos + ( node[1].oz*(0.5*node[1].L) );
	node[5].pos = node[5].pos + ( node[5].oz*(0.5*node[5].L) );

	node[6].R = legR[6];
	node[6].L = legL[6];	
	node[6].oz.Set( 1.0 , -bend , 0.0 );	node[6].oz.Normalize();
	node[6].oy.Set( bend , 1.0 , 0.0 );		node[6].oy.Normalize();
	node[6].ox.Set( 0.0 , 0.0 , -1.0 );
	node[6].pos = node[2].pos + ( node[2].oz*(0.5*node[2].L) );
	node[6].pos = node[6].pos + ( node[6].oz*(0.5*node[6].L) );

	node[7].R = legR[7];
	node[7].L = legL[7];
	node[7].oz.Set( -1.0 , -bend , 0.0 );	node[7].oz.Normalize();
	node[7].oy.Set( -bend , 1.0 , 0.0 );	node[7].oy.Normalize();
	node[7].ox.Set( 0.0 , 0.0 , 1.0 );
	node[7].pos = node[3].pos + ( node[3].oz*(0.5*node[3].L) );
	node[7].pos = node[7].pos + ( node[7].oz*(0.5*node[7].L) );
	

	node[8].R = legR[8];
	node[8].L = legL[8];

	node[8].oz.Set( -1.0 , -bend , 0.0 );	node[8].oz.Normalize();
	node[8].oy.Set( -bend , 1.0 , 0.0 );	node[8].oy.Normalize();
	node[8].ox.Set( 0.0 , 0.0 , 1.0 );
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
	motor[0].orien[0]	= node[0].oy;
		
	motor[1].dof		=  1;
	motor[1].anchor		= node[2].pos + ( node[2].oz*(-0.5*node[2].L) );	
	motor[1].orien[0]	= node[0].oy;
	
	motor[2].dof		=  1;
	motor[2].anchor		= node[3].pos + ( node[3].oz*(-0.5*node[3].L) );	
	motor[2].orien[0]	= node[0].oy * -1.0;
	
	motor[3].dof		=  1;
	motor[3].anchor		= node[4].pos + ( node[4].oz*(-0.5*node[4].L) );	
	motor[3].orien[0]	= node[0].oy * -1.0;
	
	
	motor[4].dof		=  1;
	motor[4].anchor		= node[5].pos + ( node[5].oz*(-0.5*node[5].L) );	
	motor[4].orien[0]	= node[5].ox;
	
	motor[5].dof		=  1;
	motor[5].anchor		= node[6].pos + ( node[6].oz*(-0.5*node[6].L) );	
	motor[5].orien[0]	= node[6].ox;
	
	motor[6].dof		=  1;
	motor[6].anchor		= node[7].pos + ( node[7].oz*(-0.5*node[7].L) );	
	motor[6].orien[0]	= node[7].ox;
	
	motor[7].dof		=  1;
	motor[7].anchor		= node[8].pos + ( node[8].oz*(-0.5*node[8].L) );	
	motor[7].orien[0]	= node[8].ox;
	
	

	double	QPI = 1.570796326794896;		//  90 degree
	for( j=0 ; j<4 ; j++ )
	{
		motor[j].Lstop[0] = -QPI*0.3;
		motor[j].Hstop[0] =  QPI*0.3;
	}

	for( j=4 ; j<8 ; j++ )
	{
		motor[j].Lstop[0] = -QPI*0.2;
		motor[j].Hstop[0] =  QPI*0.2;
	}

	tupidx[0][0] = 1;	tupidx[0][1] = 0;
	tupidx[1][0] = 2;	tupidx[1][1] = 0;
	tupidx[2][0] = 3;	tupidx[2][1] = 0;
	tupidx[3][0] = 4;	tupidx[3][1] = 0;

	tupidx[4][0] = 5;	tupidx[4][1] = 1;
	tupidx[5][0] = 6;	tupidx[5][1] = 2;
	tupidx[6][0] = 7;	tupidx[6][1] = 3;
	tupidx[7][0] = 8;	tupidx[7][1] = 4;

}




void RIGIDSTATE::SetSensor_Quad( void )
{
	SetSensor_CE_Walker_Common( );	
}


void RIGIDSTATE::AfterConstruct_Quad( void )
{
	SetBody_CE_Walkers_Common( 700.0 , true );		mu_coeff = 1.0;

	
	// Robot Specific CE Paramameters
	sim->eval->mdz = 0.32;
}




