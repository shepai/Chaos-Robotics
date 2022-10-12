

#ifndef OSCIL_RANDOM_NET_H
#define OSCIL_RANDOM_NET_H


#define NDOF	100		// max number of total node in oscillator net = num_osc * num_node_in_a_osc
#define NSEN	50


#include "oscillator.h"
#include "../NeuroUse/neurouse.h"


class ALPHANEURON
{
public:

	OSCNET* net;

	int index;

	bool isAlive;
	
	int num_weight;

	double x, y, v;
	double exin;


	double *weight, *yW, *weight_V;

	DOYALEARN doya;

	void Init_Doya_Learn_Wrapper_Alpha( void );


	// pointers to learning variables both for RK4 and DirectProcessing... June 10th 2017
	ALPHANEURON* pAlpha;	// if receive from other alpha
	OSCILLATOR*  pOsc;		// if receive from CPG

	
	ALPHANEURON();
	~ALPHANEURON();

	ALPHANEURON( OSCNET* , int , int );

	void Zero( void );

	void FunctionV_RK4( void );
	void Progress_Alpha_Direct( void );

	void Damage( int damIdx );

};

class IASENSOR
{
public:

	OSCNET* net;

	int index;

	bool isAlive;	// damaged or not ?


	double IaL_y, IaV_y, IbF_y;	// being set by robotic system (currently for ODE-robots & spnets)
	

	// Composition from Proprioceptor
	double rawSensor_y;
	
	double rawSensor_Disp;
	double sensorVal_Disp;


	// Param -- Sensor Adapt Init Condition
	double sensorScaleInit, sensorOffsetInit;

	// RK4 for Homeo SAM
	double sigPow, sigAvrg, rawAvrg;
	double y_sigPow, y_sigAvrg, y_rawAvrg;
	double sigPowV, sigAvrgV, rawAvrgV;
	
	double sensorScale, sensorOffset;
	double y_sensorScale, y_sensorOffset;	
	double sensorScaleV, sensorOffsetV;


	
	~IASENSOR();

	IASENSOR( OSCNET* );

	void Zero( void );


	void FunctionV_RK4( void );

	void Set_Disp_Var( void );



};


class PDIFF
{
public:

	int sourceKind;

	double* sim_time;

	int numOsc;

	// Var per Osc
	double  **v;
	bool	*has_Revolute;
	double	*prev_Vel;
	double	*phaseNew;
	double	*revTime, *prev_revTime;	// current & last time of one revolution


	// phase diff
	bool	is_pCoord_Has_Revolute, is_pCoord_update;
	double	measureTime;
	double	glPastTimeAmount;	// for GL draw

	double	*pCoord;		// max 12 DoF --> 24-1 oscillators
	double	*pCoord_Anal;	// added for Hopf system 2017 Feb

	int *revIdx;			// indices of osc revTimes -- size of num_osc --> used as a cue for phaseDiff
	int *revIdxSend;		// used for ShareMem


	PDIFF();
	PDIFF( OSCNET* , int );
	~PDIFF();

	void Zero();

	void Calc_Phase_Diffs( void );
};




class SIMULATION;

class NRPARAM;

class OSCNET
{
public:
	SIMULATION* sim;


	// running stepsize
	double StepSize;

	// preset TC
	double tConst;


	// number of oscillator
	int				num_osc;

	// total number of oscillator DoFs
	int				num_node, num_con;
	
	OSCILLATOR**	osc;
	


	// index mapper from node to osc
	int		IDX[NDOF][2];
	
	// sensors
	double	senDisp[NSEN];

		

	// motor neurons
	int num_alpha_neuron;	// same as num_muscle
	int num_Ia_sensor;		// mainly - twice the muscles: length & velocity of a muscle
	
	int num_proprioceptor;		// how many Ia per alpha neuron? -- 2 or 3 (if IbF)

	ALPHANEURON** alpha;
	IASENSOR**	  IaSen;



	bool is_Alpha_Reset;
	bool is_Alpha_Integrate;

	double fb_Str_Alpha;
	void Toggle_fb_Str_Alpha( void );	// for DX Wnd UI
	

	PDIFF* pDiff_Osc;
	PDIFF* pDiff_Alpha;


	// simulation time in second
	double simtime;


	// common variable for CE learning
	double oscT, baseZ;
	double learnT, transT;
	double zRate;

	// Transition Signal
	bool	isDoyaLearn,	isDoyaAlphaLearn,	isSensorCut;
	double	transSigDoya,	transSigAlpha,		transSigSensor;
	double	transSigDoya_y,	transSigAlpha_y,	transSigSensor_y;
	double	transSigDoya_v,	transSigAlpha_v,	transSigSensor_v;

	double doyaStr[2];

	// NoRobot lRate -- just value container (not part of rk4)
	double transNoRobot_y, transNoRobot_disp;



	// parameter 2017 Jan. for Doya OSC Learning with Bias
	int num_tt, num_ff;
		
	// phaseDiff sum
	double pdv[50],phaseDiff[50], prevPhaseDiff[50];

	
	
	
	// sensor bin learning weight	
	double weight[50][50], weight2[50][50];

	// Loaded Initial Points
	int initPI[100];


	OSCNET( SIMULATION* );
   ~OSCNET();


	void SetController( void );
	
	void AddNode( int , double );
		
	void DeleteNode( OSCILLATOR* );
	void DeleteNode( int );
		
	void Zero( void );
	void RandY( void );
	
	
	
	// RK4
	void dFunction_RK4( void );
	


	void Init_Doya_Learn_Wrapper_OSC( void );

	void LearnOSC_Doya_Node_RK4( void );
	void LearnOSC_Doya_Alpha_RK4( void );

	void LearnOSC_Doya_Single_Output_RK4( int );

	


	// Auxillary --> Just for displaying osc's exin_con_disp[]
	void Aux_Fill_Exin_Con_Disp( void );

	

	// Learning Switches
	void SetTransSigs( void );
	
	
	// Oscillators w/o Robots
	void ManualNet( void );
	

	// CE Robots & SpNets
	void ManualNet_CE_Common( void );	// Using NRPARAM
	



};







#endif
