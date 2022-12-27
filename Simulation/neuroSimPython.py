import numpy as np
from copy import deepcopy
TC_INV  = 0.0;
W_RATE  = 0.0;
B_RATE  = 0.0;
FB_GAIN = 0.0;

g_Bias_One = 1.0;


class DOYALEARN:
    def __init__(self):
        self.num_weight=0

        #time varying params
        self.TCI=[]
        self.wRate=[]
        self.bRate=[]
        self.fbGain=[]			# TC, learning rates & feedback gain

        # variables --> only y and v variables for RK4
        self.w=[[]] 
        self.dw=[[]]

        # reference pointers
        self.x=[]
        self.exin=[]
        self.inp=[]
    def Add_Weight_Ref(self,a,b,c):
        pass
    def Learn(self):
        pass

class OSCILLATOR:
    def __init__(self):
        self.net=[] #OSCNET* ;
        self.index=0
        self.isAlive=False
        self.type_=0
        self.num_node=0
            
        self.TCI=0						# common param - TC
        self.Trans = np.array((2))
        self.Center = np.array((2))	# common param - Shift
        self.CenterNumeric = np.array((2))
        self.doya = [] # DOYALEARN 
        # BVP params
        self.z=0
        self.z1=0
        self.z2=0
        self.baseZ=0
        # Hopf params
        self.hopf_mu=0
        self.hopf_freq=0

        # variables
        self.x=[]
        self.v=[]
        self.y=[]
        
        self.exin_hand=[]
        self.exin_sensor=[]
        self.exin_con=[]
        self.exin_con_disp=[];
        
        # for display init cond. - 2018-02-20
        self.xInit=[];

        # freq adaptation
        self.exinNorm=[];

        # phase
        self.kpi=0
        self.phase=0
        self.prev_phase=0
        self.prev_Y=0

        # period
        self.prev_time=0
        self.prev_VX=0
        self.period=0
        
        # Sensor Bin
        self.senDesired=0
        self.senLearned=0
        self.senAvrg=0
        
        # Doya Learning
        self.vLearned=np.array((4))
        self.vFeedback=np.array((4))

        self.xAvrg=[]
        self.xAvrg_V=[]
        self.weight=[[[]]]
        self.weight_V=[[[]]]
        self.Bias=[]
        self.wBias_V=[]

        # RK4	
        self.yA=[]#, *ka1, *ka2, *ka3;#, *ka4;
        self.yW=[[[]]]#, ***kw1, ***kw2, ***kw3;#, ***kw4;
        self.yWb=[]#, *kwb1, *kwb2, *kwb3;#, *kwb4;

        # Doya Learning for Env Sensors
        self.num_sen=0
        self.weight_sen=[[]]
        self.weight_sen_V=[[]]
        self.ySW=[[]]#, **ksw1, **ksw2, **ksw3;#, **ksw4;



class ALPHANEURON:
    def __init__(self):
        self.net=[];
        self.index=0;

        self.isAlive=False;
        
        self.num_weight=0;

        self.x=0
        self.y=0
        self.v=0
        self.exin=0

        self.weight=[]
        self.yW=[]
        self.weight_V=[]

        self.doya=None #DOYALEARN

        # pointers to learning variables both for RK4 and DirectProcessing... June 10th 2017
        self.pAlpha=[]	#if receive from other alpha
        self.pOsc=[]		#if receive from CPG

class IASENSOR:
    def __init__(self):
        self.net=[]
        self.index=0

        self.isAlive=False	# damaged or not ?
        self.IaL_y=0
        self.IaV_y=0
        self.IbF_y=0	# being set by robotic system (currently for ODE-robots & spnets)
        

        # Composition from Proprioceptor
        self.rawSensor_y=0
        
        self.rawSensor_Disp=0
        self.sensorVal_Disp=0

        # Param -- Sensor Adapt Init Condition
        self.sensorScaleInit=0
        self.sensorOffsetInit=0

        # RK4 for Homeo SAM
        self.sigPow=0
        self.sigAvrg=0
        self.rawAvrg=0
        self.y_sigPow=0
        self.y_sigAvrg=0
        self.y_rawAvrg=0
        self.sigPowV=0
        self.sigAvrgV=0
        self.rawAvrgV=0
        
        self.sensorScale=0
        self.sensorOffset=0
        self.y_sensorScale=0
        self.y_sensorOffset=0
        self.sensorScaleV=0
        self.sensorOffsetV=0

class PDIFF:
    def __init__(self):
        self.sourceKind=0
        self.sim_time=[]

        self.numOsc=0

        # Var per Osc
        self.v=[[]]
        self.has_Revolute=[]
        self.prev_Vel=[]
        self.phaseNew=[]
        self.revTime=[]
        self.prev_revTime=[]	# current & last time of one revolution

        # phase diff
        self.is_pCoord_Has_Revolute=False
        self.is_pCoord_update=False
        self.measureTime=0
        self.glPastTimeAmount=0	# for GL draw

        self.pCoord=[]		# max 12 DoF --> 24-1 oscillators
        self.pCoord_Anal=[]	# added for Hopf system 2017 Feb

        self.revIdx=[]	# indices of osc revTimes -- size of num_osc --> used as a cue for phaseDiff
        self.revIdxSend=[]		# used for ShareMem


class OSCNET:
    def __init__(self,NDOF,NSEN):
        self.sim=None
        # running stepsize
        self.StepSize=0
        # preset TC
        self.tConst=0
        # number of oscillator
        self.num_osc=0
        # total number of oscillator DoFs
        self.num_node=0
        self.num_con=0
        
        self.osc=[[]] #holding osccilators

        # index mapper from node to osc
        self.IDX=np.array((NDOF,2))
        # sensors
        self.senDisp=np.array((NSEN))
        # motor neurons
        self.num_alpha_neuron=0	# same as num_muscle
        self.num_Ia_sensor=0		# mainly - twice the muscles: length & velocity of a muscle
        
        self.num_proprioceptor=0	# how many Ia per alpha neuron? -- 2 or 3 (if IbF)

        self.alpha=[[]]
        self.IaSen=[[]]



        self.is_Alpha_Reset=False
        self.is_Alpha_Integrate=False

        self.fb_Str_Alpha=0

        self.pDiff_Osc=[]
        self.pDiff_Alpha=[]

        # simulation ime in second
        self.simtime=0
        # common variable for CE learning
        self.oscT, self.baseZ=0
        self.learnT, self.transT=0
        self.zRate=0

        # Transition Signal
        self.isDoyaLearn,	self.isDoyaAlphaLearn,	self.isSensorCut=False
        self.transSigDoya,	self.transSigAlpha,		self.transSigSensor=0
        self.transSigDoya_y,	self.transSigAlpha_y,	self.transSigSensor_y=0
        self.transSigDoya_v,	self.transSigAlpha_v,	self.transSigSensor_v=0

        self.doyaStr=np.array((2))

        # NoRobot lRate -- just value container (not part of rk4)
        self.transNoRobot_y, self.transNoRobot_disp=0

        # parameter 2017 Jan. for Doya OSC Learning with Bias
        self.num_tt, self.num_ff=0
            
        # phaseDiff sum
        self.pdv,self.phaseDiff, self.prevPhaseDiff=np.array((50))

        # sensor bin learning weight	
        self.weight, self.weight2=np.array((50,50))

        # Loaded Initial Points
        self.initPI=np.array((100))
    def LearnOSC_Doya_Node_RK4():
        pass
    def FunctionV_RK4():
        pass
    def dFunction_RK4(self):
        # SAM included !!
        # 0.013 & 0.022 weight included !!
        for j in range(0,self.num_Ia_sensor):
            self.IaSen[j]=self.FunctionV_RK4();

        # Osc's and Doya Learning
        self.LearnOSC_Doya_Node_RK4( );	# If NoRobot, this uses mu_Y from above.

        for n in range(0,self.num_osc):
            self.osc[n]=FunctionV_RK4()

        # Alpha neurons & Afferent Weights
        LearnOSC_Doya_Alpha_RK4( );
            
        # If RK4 alpha update
        if( is_Alpha_Integrate )
        for( int i=0 ; i<num_alpha_neuron ; i++ )	alpha[i]->FunctionV_RK4( );


    }
    def Init_Doya_Learn_Wrapper_OSC(self):
        TC_INV = 1.0 / (self.learnT * 1.0);
        doya=deepcopy(self.osc)
        for t in range(0,self.num_osc):
            for tt in range(0,self.num_tt):
                doya[tt].TCI=TC_INV
                doya[tt].wRate=W_RATE
                doya[tt].bRate=B_RATE
                doya[tt].bRate=B_RATE
                doya[tt].fbGain=FB_GAIN

                #target var & feedback container
                doya[tt].x=y[tt]

                #presynaptic osc
                for f in range(0,self.num_osc):
                    if( t != f ):			
                        for ff in range(0,self.num_ff):
                            doya[tt].Add_Weight_Ref(doya[tt].yW[tt][f][ff],doya[tt].weight_V[tt][f][ff],doya[tt].y[ff])
                #bias weight
                doya[tt].Add_Weight_Ref(doya[tt].yWb[tt],doya[tt].wBias_V[tt],g_Bias_One);
                self.osc[t]=deepcopy(doya[tt])
        #int hjhj = 0;
