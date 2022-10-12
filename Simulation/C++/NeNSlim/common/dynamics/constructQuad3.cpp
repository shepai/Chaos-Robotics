


#include "rigidode.h"
#include "../NeuroUse/neurouse.h"




void RIGIDSTATE::SetBody_Quad3( void )
{
	LOG( 0 , "[BGN][END] RIGIDSTATE::SetBody_Quad3(void)" );

	NUM_ODE_BODY = 9;

	double CBDENS	= 100.0;
	double ARMDENS	= 100.0;


	BALLDENSITY = 0.001;
	BALLRAD		= 0.05;

	gainstr		= 2.0;

	
	InitPos.Set( 0.0 , 0.5 , 0.0 );
	
	
	
	int i,j;

	node	= new CAPNODE[NUM_ODE_BODY];
	motor	= new MOTOR[NUM_ODE_BODY-1];
	tupidx	= new TUPLE[NUM_ODE_BODY-1];
	
	node[0].DENSITY = CBDENS;
	node[0].shape	= 1;
	
	node[0].Lx = 0.5;
	node[0].Ly = 0.1;
	node[0].L = 1.0;
	node[0].ox.Set( 1.0 , 0.0 , 0.0 );	
	node[0].oy.Set( 0.0 , 1.0 , 0.0 );
	node[0].oz.Set( 0.0 , 0.0 , 1.0 );
	node[0].pos = InitPos;


	for( i=1 ; i<NUM_ODE_BODY ; i++ )
	{
		node[i].DENSITY = ARMDENS;
		node[i].shape	= 0;			// capped cylinder
	}

	double legR = 0.05;
	double legL = 0.3;

	node[1].R = legR;	node[1].L = legL;	
	node[1].oz.Set( 0.0 , -1.0 , -1.0 );	node[1].oz.Normalize();
	node[1].oy.Set( 0.0 , -1.0 ,  1.0 );	node[1].oy.Normalize();
	node[1].ox.Set( 1.0 ,  0.0 ,  0.0 );	
	node[1].pos = node[0].pos + ( node[0].oz*(0.5*node[0].L) ) + ( node[0].ox*(0.5*node[0].Lx) );
	node[1].pos = node[1].pos + ( node[1].oz*(0.5*node[1].L) );

	node[2].R = legR;	node[2].L = legL;	
	node[2].oz.Set( 0.0 , -1.0 , 1.0 );		node[2].oz.Normalize();
	node[2].oy.Set( 0.0 ,  1.0 , 1.0 );		node[2].oy.Normalize();
	node[2].ox.Set( 1.0 ,  0.0 , 0.0 );
	node[2].pos = node[0].pos + ( node[0].oz*(-0.5*node[0].L) ) + ( node[0].ox*(0.5*node[0].Lx) );
	node[2].pos = node[2].pos + ( node[2].oz*(0.5*node[2].L) );

	node[3].R = legR;	node[3].L = legL;	
	node[3].oz.Set( 0.0 , -1.0 , -1.0 );	node[3].oz.Normalize();
	node[3].oy.Set( 0.0 , -1.0 ,  1.0 );	node[3].oy.Normalize();
	node[3].ox.Set( 1.0 ,  0.0 ,  0.0 );
	node[3].pos = node[0].pos + ( node[0].oz*(0.5*node[0].L) ) + ( node[0].ox*(-0.5*node[0].Lx) );
	node[3].pos = node[3].pos + ( node[3].oz*(0.5*node[3].L) );
	
	node[4].R = legR;	node[4].L = legL;	
	node[4].oz.Set( 0.0 , -1.0 , 1.0 );		node[4].oz.Normalize();
	node[4].oy.Set( 0.0 ,  1.0 , 1.0 );		node[4].oy.Normalize();
	node[4].ox.Set( 1.0 ,  0.0 , 0.0 );
	node[4].pos = node[0].pos + ( node[0].oz*(-0.5*node[0].L) ) + ( node[0].ox*(-0.5*node[0].Lx) );
	node[4].pos = node[4].pos + ( node[4].oz*(0.5*node[4].L) );

	

			
	node[5].R = legR;	node[5].L = legL;	
	node[5].oz.Set( 0.0 , -1.0 , 1.0 );		node[5].oz.Normalize();
	node[5].oy.Set( 0.0 ,  1.0 , 1.0 );		node[5].oy.Normalize();
	node[5].ox.Set( 1.0 ,  0.0 , 0.0 );	
	node[5].pos = node[1].pos + ( node[1].oz*(0.5*node[1].L) );
	node[5].pos = node[5].pos + ( node[5].oz*(0.5*node[5].L) );

	node[6].R = legR;	node[6].L = legL;	
	node[6].oz.Set( 0.0 , -1.0 , -1.0 );	node[6].oz.Normalize();
	node[6].oy.Set( 0.0 , -1.0 ,  1.0 );	node[6].oy.Normalize();
	node[6].ox.Set( 1.0 ,  0.0 ,  0.0 );
	node[6].pos = node[2].pos + ( node[2].oz*(0.5*node[2].L) );
	node[6].pos = node[6].pos + ( node[6].oz*(0.5*node[6].L) );

	node[7].R = legR;	node[7].L = legL;
	node[7].oz.Set( 0.0 , -1.0 , 1.0 );		node[7].oz.Normalize();
	node[7].oy.Set( 0.0 ,  1.0 , 1.0 );		node[7].oy.Normalize();
	node[7].ox.Set( 1.0 ,  0.0 , 0.0 );
	node[7].pos = node[3].pos + ( node[3].oz*(0.5*node[3].L) );
	node[7].pos = node[7].pos + ( node[7].oz*(0.5*node[7].L) );
	
	node[8].R = legR;	node[8].L = legL ;
	node[8].oz.Set( 0.0 , -1.0 , -1.0 );	node[8].oz.Normalize();
	node[8].oy.Set( 0.0 , -1.0 ,  1.0 );	node[8].oy.Normalize();
	node[8].ox.Set( 1.0 ,  0.0 ,  0.0 );
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
	motor[0].orien[0]	= node[0].ox * 1.0;
		
	motor[1].dof		=  1;
	motor[1].anchor		= node[2].pos + ( node[2].oz*(-0.5*node[2].L) );	
	motor[1].orien[0]	= node[0].ox * -1.0;
	
	motor[2].dof		=  1;
	motor[2].anchor		= node[3].pos + ( node[3].oz*(-0.5*node[3].L) );	
	motor[2].orien[0]	= node[0].ox * 1.0;
	
	motor[3].dof		=  1;
	motor[3].anchor		= node[4].pos + ( node[4].oz*(-0.5*node[4].L) );	
	motor[3].orien[0]	= node[0].ox * -1.0;
	
	
	motor[4].dof		=  1;
	motor[4].anchor		= node[5].pos + ( node[5].oz*(-0.5*node[5].L) );	
	motor[4].orien[0]	= node[5].ox * -1.0;
	
	motor[5].dof		=  1;
	motor[5].anchor		= node[6].pos + ( node[6].oz*(-0.5*node[6].L) );	
	motor[5].orien[0]	= node[6].ox * 1.0;
	
	motor[6].dof		=  1;
	motor[6].anchor		= node[7].pos + ( node[7].oz*(-0.5*node[7].L) );	
	motor[6].orien[0]	= node[7].ox * -1.0;
	
	motor[7].dof		=  1;
	motor[7].anchor		= node[8].pos + ( node[8].oz*(-0.5*node[8].L) );	
	motor[7].orien[0]	= node[8].ox * 1.0;
	
	

	double	QPI = 1.570796326794896;		//  90 degree
	for( j=0 ; j<4 ; j++ )
	{
		motor[j].Lstop[0] = -QPI*0.2;
		motor[j].Hstop[0] =  QPI*0.2;
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



	/////// Simulator & Motors ////////////////////////////////////////////////////////////////

//	SetBody_Walkers_Common( 800.0 , true );		mu_coeff = 1.0;

}




void RIGIDSTATE::SetSensor_Quad3( void )
{
	SetSensor_CE_Walker_Common( );
}


void RIGIDSTATE::AfterConstruct_Quad3( void )
{
	SetBody_CE_Walkers_Common( 800.0 , true );		mu_coeff = 1.0;

	for( int i=1 ; i<NUM_ODE_BODY ; i++ )
	{
	//	if( i<=(NUM-1)/2 )	node[i].inertia = 1.0 * ( node[i].inertia + m.mass*node[i].L*node[i].L );
	//	if( i<=(NUM-1)/2 )	node[i].inertia = 2.0 * node[i].inertia;
		if( i<=(NUM_ODE_BODY-1)/2 )	node[i].inertia *= 1.5;
	}


	// Robot Specific CE Paramameters
	sim->eval->mdz = 0.32;

}










