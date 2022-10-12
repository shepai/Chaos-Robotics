

#ifndef MESHES_H
#define MESHES_H

#include <d3dx8.h>

#include "..\..\common\math\vec3.h"
#include "sim.h"


#define D3DFVF_CUSTOMVERTEX ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
#define D3DFVF_LINEVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE )

//#define D3DFVF_POINTVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_PSIZE )
//#define D3DFVF_CUSTOMVERTEX_DIFFUSE ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE )


struct CUSTOMVERTEX
{
    D3DXVECTOR3 position , n;			// The position and vertex normal
	DWORD	    diffuseColor;			// diffuse color
	FLOAT       tu, tv;					// The texture coordinates	
};



struct LINEVERTEX
{
    D3DXVECTOR3 position;				// The position and vertex normal
    DWORD	    diffuseColor;			// diffuse color
};


/*
struct CUSTOMVERTEX_DIFFUSE
{
    D3DXVECTOR3 position , n;			// The position and vertex normal
	DWORD	    diffuseColor;			// diffuse color
};
*/

/*
struct POINTVERTEX
{
    D3DXVECTOR3 position;				// The position and vertex normal
	float		psize;
    DWORD	    diffuseColor;			// diffuse color	
};
*/





#endif
