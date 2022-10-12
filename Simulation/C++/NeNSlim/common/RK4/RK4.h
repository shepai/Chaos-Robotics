


#ifndef RK4_H
#define RK4_H

#include "../math/vec3.h"



#define MAX_RK4_VAR	5000

class SIMULATION;

class RK4
{
public:
	SIMULATION* sim;
	
	double *x[MAX_RK4_VAR], *y[MAX_RK4_VAR], *v[MAX_RK4_VAR];
	double k1[MAX_RK4_VAR], k2[MAX_RK4_VAR], k3[MAX_RK4_VAR];

	int num_rk4_var;

	double h;
	

	RK4( SIMULATION* );
	~RK4();


	void Add_Var( double* var_x, double* var_y, double* var_v );
	void Add_Var_VEC3( VEC3* X, VEC3* Y, VEC3* V );
	
	void Del_Var( double* var_x );
	void Del_Var_VEC3( VEC3* X );

	void Clean_Var( void );


	void Zero( void );
	void RK4_STEP( int idx );
	void Progress( void );

};








#endif