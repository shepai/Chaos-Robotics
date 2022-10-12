

#ifndef ENUMERATIONS_H
#define ENUMERATIONS_H


enum ENUM_ROBOT
{	
	
	WALKER_START = 0,
	
		WALKER_QUAD = WALKER_START,
		WALKER_QUAD2,
		WALKER_QUAD3,
		WALKER_HEX,
		WALKER_TRI,
		STAR_ROBOT,
		
	WALKER_END,

	END_ENUM_ROBOT = WALKER_END
};

static char* ENUM_ROBOT_NAME[] =	{	

								"Walk-Quad I",
								"Walk-Quad II",
								"Walk-Quad III",
								"Walk-Hex",
								"Walk-Tri",
								"Walk-Star"
							};


char*	GetRobotName( void );

#endif