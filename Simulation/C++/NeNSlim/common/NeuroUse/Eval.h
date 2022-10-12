


#ifndef EVALUATION_H
#define EVALUATION_H


class SIMULATION;

class EVAL
{
public:
	SIMULATION* sim;

	// Oscillator Params
	double mdz;

	// Time Consts
	double vMeasureT, effUpdateT;

	// Time Consts - CE
	double maxET_Inc, maxET_Dec;
	double dzT;

	double	zEpsilon;
	double	instant_Eff;	// Instantaneous measurement from physics OR pCoords

	int	fix_dzRate_Mode;

		
	// Exploration & Learning RK4
	VEC3	locospd, locospd_y, locospd_v;		// Moving toward arbitrary direction -- 2021.05.31

	double	dzRate, dzRate_y, dzRate_v;
	double  EFFICIENCY, EFFICIENCY_y, EFFICIENCY_v;
	double	maxE, maxE_y, maxE_v;
	double	minE, minE_y, minE_v;

	

	EVAL( SIMULATION* );
	~EVAL();

	void Init_Eval( void );
	void Zero( void );

	// Instant Perf
	void Performance_ODE_Robots( void );
	
	
	

	// CE
	void Add_Eval_RK4_Var_No_RS( void );
	void Del_Eval_RK4_Var_No_RS( void );
	
	void Add_Eval_RK4_Var_RS( void );
	void Del_Eval_RK4_Var_RS( void );

	void Evaluate_CE( void );
	double Set_MaxE_CE_dt( void );
	double Set_MinE_CE_dt( void );
	double Set_dz_dt( void );



};





#endif