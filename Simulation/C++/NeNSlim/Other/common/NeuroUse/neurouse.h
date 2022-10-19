


#ifndef NEUROUSE_H
#define NEUROUSE_H



#include "../neurolib/oscil_random_net.h"

#include "../RK4/RK4.h"
#include "../dynamics/rigidode.h"

#include "./Eval.h"


#define NUM_MAX_EA_THR	128


class NRPARAM
{
public:
	SIMULATION* sim;

	int DoF;

	bool IS_ALPHA_RK4;
	bool IS_OSC_FED_TO_ALPHA;

	double STEP_SIZE;

	int OSC_KIND;
	double TIME_CONST;

	

	NRPARAM();
	~NRPARAM(){}

	void Select_NRPARAM( int );
	
};


enum ENUM_ROBOT;




class SIMULATION
{
public:
	int threadIdx;

	NRPARAM nrp;

	
	ENUM_ROBOT	 robotKind;
	
	


	OSCNET*		oscil;


	RIGIDSTATE* rs;

	RK4* rk4_for_Init;
	RK4* rk4;

	EVAL* eval;


	// ODE Worlds
	dWorldID		world;
	dSpaceID		space;
	dJointGroupID	contactgroup;
	dGeomID			ground;


	// NeuroIdx for Effectors
	int effIdx1[50][3], effIdx2[50][3];


	SIMULATION();
	~SIMULATION();

	void Init( void );
	void Zero( void );


	void AllocNNET( void );
	void InitNNET( void );
	void CleanNNET( void );

	void Reload_CTRNN_for_OSCIL( void );


	void InitRS( void );
	void EndRS( void );


	void InitEval( void );
	void EndEval( void );


	// access_network_Core.cpp
	void ZeroNN( void );


	// Connect Activation-Effector & NeuroInput-Sensors
	double* Get_Neuro_Effector_Pointer( int , bool );


	int GetEffIdx( int n , int d , int idx );


	void SetSensor_Proprioceptor( int n , double IaL , double IaV , double IbF  );
	

	void SetAsaiZ_DZ_Rate_Same_y( void );
	void SetAsaiZ_DZ_Rate_Same( int thrId , RIGIDSTATE* rs );


	// access_network.cpp
	
	double GetExinTotal( int n );

	double GetExinSensor( int n );
	double GetExinCon( int n );
	
	
	
	
	double GetOut( int n , int kind , int dummy1 , int dummy2 );

	double	GetSimTime( void );

	// for uis.cpp
	int GetNumNode( void );
	
	
	
	// plot2d
	double GetSingular( int n );
	
	
};


extern SIMULATION* g_sim[NUM_MAX_EA_THR];


void LOG( int , const char* , ... );



#endif