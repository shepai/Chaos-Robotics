


#ifndef ODE
#define ODE


#include <ode/ode.h>
#include "..\math\vec3.h"

class RIGIDSTATE;

#include "../Junction/Enumerations.h"

#include "../NeuroUse/neurouse.h"




#define		G_LENGTH_SCALE	1.0
#define		MASS_SCALE		1.0



class CAPNODE
{
public:

	RIGIDSTATE* rs;
	int index;

	bool	isFoot;

	int		shape;

	double	DENSITY;
	double	mass , inertia;

	VEC3		pos;
	VEC3		ox , oy , oz;
		
	double		R, L;		// Capped Cylinder
	double		Lx, Ly;		// Box


	// realtime varying
	VEC3		tpos, lv, av;
	VEC3		tox , toy , toz;

	
	// extra value for probe
	double probeVal;
		
	CAPNODE();
	~CAPNODE();

	void Zero( void );
};


// This should be separated into joint & muscle classes... 2021.07.13
class MOTOR
{
public:
	RIGIDSTATE* rs;
	
	int index;

	int			dof;
	VEC3		anchor;
	VEC3		orien[3];	
	double		Lstop[3], Hstop[3];

	// Neuro Output
	double *neuroAct1_x[3], *neuroAct1_y[3];
	double *neuroAct2_x[3], *neuroAct2_y[3];

	// muscle param
	double		alpha[3], beta[3], gamma[3], delta[3];


	
	double		angle[3], vel[3], torque[3], force[3], activation1[3], activation2[3];
	double		velLF[3];

	double		prevAngle[3], prevVel[3];


	dJointFeedback	fback[3];

	// for Servo-Muscle Emul
	double factor;
	double modelF, realF;
	double F, Fs;


	bool	contact;	// is foot is in contact with ground?


	double ACCEL;


	// for alpha neuron
	double IaL1, IaL2;
	double IaV1, IaV2;
	double IbF1, IbF2;

	double IaL1_Vel, IaL2_Vel;
	double IaV1_Vel, IaV2_Vel;
	double IbF1_Vel, IbF2_Vel;



	// rCoord New
	bool has_Revolute;
	double prev_Vel;
	double revTime, prev_revTime;
	double phaseNew;

			
	MOTOR();
	~MOTOR();

	void Zero();


	void Calculate_Reflex( void );
	void Progress_ODE_Robot_Reflex( void );

	void Calculate_TwoTorque_Alpha( void );



};






typedef int	TUPLE[2];

class SIMULATION;

class RIGIDSTATE
{
public:
	
	SIMULATION* sim;

	bool	is_ODE_Alive;	// include SpNet Dummy ODE bodies
	bool	is_ODE_Robot;	// Only Real ODE Robots

	double	STEPSIZE;

	int		NUM_ODE_BODY;
	int		NUM_ODE_BODY_ORIGIN;	// for data writing

	double	BALLRAD, BALLDENSITY, gainstr;
	VEC3	InitPos;
	

	
	CAPNODE*	node;
	MOTOR*		motor;

	// joining index wrappers
	TUPLE*		tupidx;

	
	// ODE
	dBodyID*		body;
	dBodyID**		ball;
	dJointID**		joint;
	dGeomID*		bodygeom;
		


	// Foot Tip Traj
	int NUM_TIP;
	VEC3 footTip[10];
	
	
	// time varying
	double	robotStep;
	bool	isBlow;

	bool	is_use_CG;
	VEC3	CG, CGv;
	VEC3	rkCG, rkCGv;

	
	void Calc_CG( void );
	


	VEC3	avrCG; // just for DX camera work
	void Calc_avrCG_for_DX_Cam( void );

	double oscTr;	// Get from OSCNET



	
	// body contact ?
	double footVel[20];
	double footHeight[20];


	// friction mode
	bool	isCoulomb;
	double	mu_coeff;

	

	// k-scale for servo control - related to motor stop (motor range)
	double kScale, signal_portion;


	
	// rCoord New
	bool is_rCoord_Has_Revolute;
	bool is_rCoord_update;

	int revIdx[30];

	double rCoord[32];	// max 12 DoF --> 24-1 oscillators
	double measureTime;
	double glPastTimeAmount;




	

	//RIGIDSTATE();
	RIGIDSTATE( SIMULATION* );
	~RIGIDSTATE();


	// Initial Setting
	
	void	Destroy_ODE_Body( void );
	void	ResetVariable( void );
	
	void	Construct_Body_ODE( dWorldID, dSpaceID );
	void	CombineJoint( int , int , int , dWorldID );

	
	// Realtime Setting
	void	SetTransform_ODE( void );
	
	double	GetTotalPower( void );
	void	MeasureJointForce( void );
	void	MeasureJointForceQuad( void );
	

	
	
	// Construct Robots  /  After Constructions  /  Set Sensors   /   Set Effectors
	void (RIGIDSTATE::*SetBodyFuncName)( void );
	void Set_Body_RS( void );

	void (RIGIDSTATE::*SetSensorFuncName)( void );
	void SetSensor( void );

	void (RIGIDSTATE::*AfterConstructFuncName)( void );
	void AfterConstruct( void );
	
	void (RIGIDSTATE::*SetEffectorFuncName)( void );
	void SetEffector( void );



	// Progress IaL IaV IbF . . .Reflex integration 2021-05-27
	void Progress_Reflex_ODE_Robot( void );
	
	
	// Different Actuators
	void Set_Effectors_Servo_Single( void );
	void Set_Effectors_Servo_Double( void );
	void Set_Effectors_Two_Torque( void );	
	


	
	void SetRobotFuncName( void );	// This Function will Set below sets of three initialisations



	// CE Controller Global Common -> Both ODE robots & SpNets !!
	void Set_CE_Params_Global( void );
	
	// Muscle Params & Moment of Inertia -- Global
	void Set_Muscle_and_Inertia( double );

	

	// Terrestrials ///////////////////////////////////////////////////////////////////////////////////////////////////
	void SetBody_CE_Walkers_Common( double , bool );		// Common Walkers properties
	void SetSensor_CE_Walker_Common( void );

	void SetBody_Quad( void );		void SetSensor_Quad( void );	void AfterConstruct_Quad( void );
	void SetBody_Hex( void );		void SetSensor_Hex( void );		void AfterConstruct_Hex( void );
	void SetBody_Quad2( void );		void SetSensor_Quad2( void );	void AfterConstruct_Quad2( void );
	void SetBody_Quad3( void );		void SetSensor_Quad3( void );	void AfterConstruct_Quad3( void );
	void SetBody_Tri( void );		void SetSensor_Tri( void );		void AfterConstruct_Tri( void );
	void SetBody_Star( void );		void SetSensor_Star( void );	void AfterConstruct_Star( void );
	

	

};




#endif