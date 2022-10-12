
// This functions are for realtime graph & text display


#include "neurouse.h"





double SIMULATION::GetExinTotal( int n )	// GL plot2d
{
	return oscil->osc[oscil->IDX[n][0]]->exin_sensor[oscil->IDX[n][1]]+
								oscil->osc[oscil->IDX[n][0]]->exin_con_disp[oscil->IDX[n][1]];
}

double SIMULATION::GetExinSensor( int n )	// GL plot2d
{
	return oscil->osc[oscil->IDX[n][0]]->exin_sensor[oscil->IDX[n][1]];
}

double SIMULATION::GetExinCon( int n )	// GL plot2d
{
	return oscil->osc[oscil->IDX[n][0]]->exin_con_disp[oscil->IDX[n][1]];
}


double SIMULATION::GetOut( int n , int kind , int dummy1 , int dummy2 )		// used in GL Plots - gloutput, plot2d/3d , plot2dmap
{
	
			switch( kind )
			{
				case 0:		return oscil->osc[oscil->IDX[n][0]]->x[oscil->IDX[n][1]];
				case 1:		return oscil->osc[oscil->IDX[n][0]]->v[oscil->IDX[n][1]];
				case 2:		return oscil->alpha[oscil->IDX[n][0]]->x;
			}

		return 0.0;
}





double SIMULATION::GetSimTime( void )
{
	return oscil->simtime;
}



int SIMULATION::GetNumNode( void )
{
	return oscil->num_node;
}



// Realtime-analytically calculated unstable fixed point of BVP Oscillator

double SIMULATION::GetSingular( int n )	// currently used in GL plot2d
{
	int a = oscil->IDX[n][0];
	int b = oscil->IDX[n][1];

	return	oscil->osc[a]->Center[b] - oscil->osc[a]->Trans[b];	
}



