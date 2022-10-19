

#ifndef OSCILLATOR_H
#define OSCILLATOR_H



enum ENUM_OSC_TYPE
{
	BVP,
	BVP2,

	BVP_NO_ROBOT,
	HOPF,

	BVP_COUPLED,

	ROSSLER,
	KAUFMAN,

	SPROTT_TANH,
	SPROTT_K,
	SPROTT_Q,

	END_ENUM_OSC_TYPE
};

static char* ENUM_OSC_NAME[] =	{	"BVP", 
									"BVP2", 

									"BVP_NO_ROBOT", 
									"HOPF", 

									"BVP_Coupled",

									"ROSSLER", 
									"KAUFMAN", 

									"SPROTT_TANH", 
									"SPROTT_K", 
									"SPROTT_Q"
								};



// Wrapper: per node !! -- input from several sources (incl. other nodes & sensors etc...)
class DOYALEARN
{
public:
	
	// Fixed params
	int num_weight;
	
	

	// time varying params
	double *TCI, *wRate, *bRate, *fbGain;			// TC, learning rates & feedback gain

	// variables --> only y and v variables for RK4
	double **w, **dw;

	// reference pointers
	double *x, *exin;
	double **inp;

	DOYALEARN();
	~DOYALEARN();

	void Add_Weight_Ref( double* , double* , double* );
	
	void Learn( void );


	

};


class OSCNET;

class OSCILLATOR
{
public:
	OSCNET* net;

	int index;

	bool isAlive;

	int		type;
	int		num_node;
		
	double	TCI;						// common param - TC
	double	Trans[2], Center[2];	// common param - Shift
	double	CenterNumeric[2];
	
	DOYALEARN doya[2];	
	
	
	// BVP params
	double  z ,z1,z2, baseZ;

	// Hopf params
	double hopf_mu, hopf_freq;

	


	
	// variables
	double	*x, *v, *y;
	
	double  *exin_hand, *exin_sensor, *exin_con;
	double	*exin_con_disp;
	
	// for display init cond. - 2018-02-20
	double	*xInit;

	// freq adaptation
	double	*exinNorm;

	// phase
	double kpi, phase, prev_phase, prev_Y;

	// period
	double prev_time, prev_VX, period;

	
	
	
	// Sensor Bin
	double senDesired, senLearned;
	double senAvrg;
	
	// Doya Learning
	double vLearned[4], vFeedback[4];

	double *xAvrg, *xAvrg_V;
	double ***weight, ***weight_V;
	double *wBias, *wBias_V;

	
	// RK4	
	double *yA;//, *ka1, *ka2, *ka3;//, *ka4;
	double ***yW;//, ***kw1, ***kw2, ***kw3;//, ***kw4;
	double *yWb;//, *kwb1, *kwb2, *kwb3;//, *kwb4;


	// Doya Learning for Env Sensors
	int num_sen;
	double **weight_sen, **weight_sen_V;
	double **ySW;//, **ksw1, **ksw2, **ksw3;//, **ksw4;


	OSCILLATOR( );
	OSCILLATOR( int , int , double , OSCNET* );
	~OSCILLATOR();
	
	void	AllocWeightDoya( void );
	void	Zero( void );
	
	// Integrators	
	void	FunctionV_RK4( void );

	// Auxiliary per oscillator calcs
	void	AuxCalc( void );
		

	// Equation function pointer
	void (OSCILLATOR::*EquationFuncName)( double* );	
	void Equation( double* y ) { (this->*EquationFuncName)( y ); }


	// functions
	void	Equation_Asai_BVP( double* );
	void	Equation_Asai_BVP2( double* );

	void	Equation_Asai_BVP_NoRobot( double* );
	void	Equation_Asai_BVP_NoRobot_2( double* );
	void	Equation_Hopf( double* );

	void	Equation_Coupled_BVP( double* );

	
	void	Equation_Sprott_Tanh( double* );
	void	Equation_Rossler( double* );

	void	Equation_Sprott_K( double* );
	void	Equation_Sprott_Q( double* );

	void	Equation_Kaufman( double* );
	

	// Calculate Phases
	void	Calculate_Phase_BVP( void );
	void	Calculate_Period_BVP( void );

	// Frequency Adaptation
	void	Frequency_Adaptation( double );



};






#endif

