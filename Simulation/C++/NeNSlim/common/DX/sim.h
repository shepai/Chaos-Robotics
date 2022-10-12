


#ifndef SIM_H
#define SIM_H

#include <d3dx8.h>




struct MATERIAL
{
	char texname[20];
};


struct VNORM
{
	int index;
	float x,y,z;
};

struct OBJMESH
{
	int	num_vertex;
	int	num_face;
	int num_tvertex;

	int	mat_ID;
	int hasTex;

	char nodeName[20];
	char parentName[20];

	float			tmPos[3];		// pivot

	float			*v;				// vertex position
	int				*fi;			// face's vertex index

	float			*tv;			// tex coord of tVertex
	int				*tf;			// tFace index

	VNORM			*vn;
	float			*fn;			// face normal

	

};


class SIM
{
public:
	int			num_object;
	int			num_material;

	int			firstFrame;
	int			lastFrame;
	int			frameSpeed;
	int			ticksPerFrame;

	OBJMESH*	objMesh;
	MATERIAL*	material;

//	int			isPicked;

	BOOL		isSound;
	int			sound_index;

	SIM()
	{
		num_object = 0;
		num_material = 0;
		firstFrame = 0;
		lastFrame = 0;
		frameSpeed = 0;
		ticksPerFrame = 0;

		objMesh = NULL;
		material = NULL;

		isSound = false;
		sound_index = 0;
	}

	~SIM()
	{
		free( objMesh[0].fi );
		free( objMesh[0].fn );
		free( objMesh[0].vn );
		free( objMesh[0].v );
		free( objMesh[0].tf );
		free( objMesh[0].tv );
		
		free( material );
		free( objMesh );	
	}

};




SIM* ReadSIM( char* name );

SIM* MakeBox( char* );
SIM* MakeCylinder( char* );

void get_v_normal( SIM* sim );



#endif