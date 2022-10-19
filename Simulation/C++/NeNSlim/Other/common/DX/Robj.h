


#ifndef ROBJ_H
#define ROBJ_H




#include <d3dx8.h>
#include "sim.h"
#include "..\..\common\math\vec3.h"

#define MAX_DX_HISTORIC	1000


class DXHANDLE;


class DXVTX
{
public:
    VEC3	pos;			// The position	
	float   u, v;			// The texture coordinates

	DXVTX()
	{
		pos.Zero();
		u = v = 0.0f;
	}
};

class TRISURFACE
{
public:
	DXVTX	vtx[3];

	VEC3	surfnorm;
	int		col[3];		// diffuse color

	TRISURFACE()
	{
		surfnorm.Zero();
		col[0] = col[1] = col[2] = 0;
	}

	void Calc_Surface_Norm( void )
	{
		surfnorm = (vtx[1].pos - vtx[0].pos) ^ (vtx[2].pos - vtx[0].pos);	
		surfnorm.Normalize();
	}

};


class WOBJ
{
public:
	
	DXHANDLE* dxh;

	VEC3 target;

	// Wing patagia
	int			num_tri;
	TRISURFACE		*render_wtr;
	bool		isTail;			// tail is separated because it does not have flexible bend

	// wingtip render
//	VEC3 prev_tip[10][NUM_PREV];
	
	WOBJ( DXHANDLE* ddd , int NUMTRI, bool it )
	{
		dxh = ddd;
		num_tri = NUMTRI;
	
		isTail = it;
		target.Zero();

		render_wtr = (TRISURFACE*)malloc( sizeof(TRISURFACE) * num_tri );
	//	render_wtr = new TRISURFACE[num_tri];
	}

	~WOBJ()
	{		
		free( render_wtr );
	//	delete [] render_wtr;
	}

	void Update_Wing_Triangle( void );

};



class ROBJ
{
public:
	
	DXHANDLE* dxh;

	D3DXMATRIX	matRot;
	float		tx , ty , tz;

	// Capped Cylinder
//	float		R , L;

	// Box
//	float		Lx , Ly , Lz;

	// afterimages
	D3DXMATRIX	prev_matRot[MAX_DX_HISTORIC];
//	float		prev_tx[NUM_PREV] , prev_ty[NUM_PREV] , prev_tz[NUM_PREV];
	D3DVECTOR prev_pos[MAX_DX_HISTORIC];

	// Geometry
	SIM*		sim;

	ROBJ();
	~ROBJ();
	
	void ReadShape( char* );
	void ReadBox( char* );
	void ReadCylinder( char* );
	
	void SetScale( double , double, double  );

	void SetTranslate( double , double, double  );


	//	void SetRL( double, double );
	//	void SetLxLyLz( double , double , double );		

};



void AllocROBJ( DXHANDLE* );





#endif