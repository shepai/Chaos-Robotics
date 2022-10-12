


#include "sim.h"

#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <malloc.h>

#include <d3dx8.h>

#include "..\..\common\math\vec3.h"




SIM* ReadSIM( char* name )
{
	SIM* res = NULL;
	res = new SIM();
	ZeroMemory( res, sizeof(SIM) );

	int byteRead = 0;

	FILE* fh;
	
		
	fh = fopen( name, "rb" );


	fread( &res->num_object, sizeof(int), 1, fh );
	
	fread( &res->firstFrame, sizeof(int), 1, fh );
	fread( &res->lastFrame, sizeof(int), 1, fh );
	fread( &res->frameSpeed, sizeof(int), 1, fh );
	fread( &res->ticksPerFrame, sizeof(int), 1, fh );

	fread( &res->num_material, sizeof(int), 1, fh );
			
	res->objMesh = (OBJMESH*)malloc( sizeof OBJMESH * res->num_object );
	res->material = (MATERIAL*)malloc( sizeof MATERIAL * res->num_material );
	ZeroMemory( res->objMesh , sizeof OBJMESH * res->num_object );
	ZeroMemory( res->material , sizeof MATERIAL * res->num_material );

	for( int mat=0 ; mat<res->num_material ; mat++ )
	{
		fread( res->material[mat].texname, 20, 1, fh );
	}


	// GEOMOBJECT		
		
		fread( res->objMesh[0].nodeName, 20, 1, fh );
		fread( res->objMesh[0].parentName, 20, 1, fh );

		fread( res->objMesh[0].tmPos, sizeof(float)*3 , 1, fh );

		fread( &res->objMesh[0].num_vertex, sizeof(int), 1, fh );
		fread( &res->objMesh[0].num_face, sizeof(int), 1, fh );

		res->objMesh[0].fn = (float*)malloc( sizeof(float) * res->objMesh[0].num_face * 3 );

		res->objMesh[0].v = (float*)malloc( sizeof(float) * res->objMesh[0].num_vertex * 3 );
		res->objMesh[0].fi = (int*)malloc( sizeof(int) *  res->objMesh[0].num_face * 3 );

		ZeroMemory( res->objMesh[0].fi , sizeof(int) * res->objMesh[0].num_face * 3 );
		ZeroMemory( res->objMesh[0].fn , sizeof(float) * res->objMesh[0].num_face * 3 );
		ZeroMemory( res->objMesh[0].v , sizeof(float) * res->objMesh[0].num_vertex * 3 );

				
		byteRead = fread( res->objMesh[0].v, sizeof(float), 3* res->objMesh[0].num_vertex, fh );

		byteRead = fread( res->objMesh[0].fi, sizeof(int), res->objMesh[0].num_face * 3, fh );

		fread( &res->objMesh[0].num_tvertex, sizeof(int), 1 , fh );

		res->objMesh[0].tv = (float*)malloc( sizeof(float) *  res->objMesh[0].num_tvertex * 2 );
		ZeroMemory( res->objMesh[0].tv , sizeof(float) * res->objMesh[0].num_tvertex * 2 );

		res->objMesh[0].tf = (int*)malloc( sizeof(int) *  res->objMesh[0].num_face * 3 );
		ZeroMemory( res->objMesh[0].tf , sizeof(int) * res->objMesh[0].num_face * 3 );

		fread( res->objMesh[0].tv, sizeof(float), res->objMesh[0].num_tvertex * 2, fh );
		fread( res->objMesh[0].tf, sizeof(float), res->objMesh[0].num_face * 3, fh );


		fread( &res->objMesh[0].mat_ID, sizeof(int), 1, fh );


	fclose( fh );


		// Compute Face Normals

//	VECTOR3D a,b,c,n;
	VEC3 a,b,c,n;
	
		res->objMesh[0].vn = (VNORM*)malloc( sizeof VNORM * res->objMesh[0].num_face * 3 );
		ZeroMemory( res->objMesh[0].vn , sizeof VNORM * res->objMesh[0].num_face * 3 );		

		for( int f=0; f<res->objMesh[0].num_face ; f++ )
		{
			a.x = res->objMesh[0].v[ res->objMesh[0].fi[f*3] * 3     ];
			a.y = res->objMesh[0].v[ res->objMesh[0].fi[f*3] * 3 + 1 ];
			a.z = res->objMesh[0].v[ res->objMesh[0].fi[f*3] * 3 + 2 ];

			b.x = res->objMesh[0].v[ res->objMesh[0].fi[f*3+1] * 3     ];
			b.y = res->objMesh[0].v[ res->objMesh[0].fi[f*3+1] * 3 + 1 ];
			b.z = res->objMesh[0].v[ res->objMesh[0].fi[f*3+1] * 3 + 2 ];

			c.x = res->objMesh[0].v[ res->objMesh[0].fi[f*3+2] * 3     ];
			c.y = res->objMesh[0].v[ res->objMesh[0].fi[f*3+2] * 3 + 1 ];
			c.z = res->objMesh[0].v[ res->objMesh[0].fi[f*3+2] * 3 + 2 ];		

			n = (a - c)^(b - c);
			n.Normalize();
			
			res->objMesh[0].fn[f*3]		= (float)n.x;
			res->objMesh[0].fn[f*3+1]	= (float)n.y;
			res->objMesh[0].fn[f*3+2]	= (float)n.z;

			
			res->objMesh[0].vn[f*3].index = res->objMesh[0].fi[f*3];
			res->objMesh[0].vn[f*3].x = (float)n.x;
			res->objMesh[0].vn[f*3].y = (float)n.y;
			res->objMesh[0].vn[f*3].z = (float)n.z;
			
			res->objMesh[0].vn[f*3+1].index = res->objMesh[0].fi[f*3+1];
			res->objMesh[0].vn[f*3+1].x = (float)n.x;
			res->objMesh[0].vn[f*3+1].y = (float)n.y;
			res->objMesh[0].vn[f*3+1].z = (float)n.z;
			
			res->objMesh[0].vn[f*3+2].index = res->objMesh[0].fi[f*3+2];
			res->objMesh[0].vn[f*3+2].x = (float)n.x;
			res->objMesh[0].vn[f*3+2].y = (float)n.y;
			res->objMesh[0].vn[f*3+2].z = (float)n.z;
		

		}					
	

	
	return res;

}




SIM* MakeBox( char* name )
{
	SIM* res = NULL;
	res = new SIM();
	ZeroMemory( res, sizeof(SIM) );

	
	res->num_object = 1;
		
	res->num_material = 1;
			
	res->objMesh = (OBJMESH*)malloc( sizeof OBJMESH * res->num_object );
	res->material = (MATERIAL*)malloc( sizeof MATERIAL * res->num_material );
	ZeroMemory( res->objMesh , sizeof OBJMESH * res->num_object );
	ZeroMemory( res->material , sizeof MATERIAL * res->num_material );

	for( int mat=0 ; mat<res->num_material ; mat++ )
	{
		sprintf( res->material[mat].texname , "%s" , name );
		
	}


	// GEOMOBJECT		
	
		res->objMesh[0].num_vertex	=  8;
		res->objMesh[0].num_face	= 12;

		// allocation
		res->objMesh[0].v = (float*)malloc( sizeof(float) * res->objMesh[0].num_vertex * 3 );
		res->objMesh[0].fn = (float*)malloc( sizeof(float) * res->objMesh[0].num_face * 3 );		
		res->objMesh[0].fi = (int*)malloc( sizeof(int) *  res->objMesh[0].num_face * 3 );

		// numbers
		ZeroMemory( res->objMesh[0].fi , sizeof(int) * res->objMesh[0].num_face * 3 );
		ZeroMemory( res->objMesh[0].fn , sizeof(float) * res->objMesh[0].num_face * 3 );
		ZeroMemory( res->objMesh[0].v , sizeof(float) * res->objMesh[0].num_vertex * 3 );

		
		float HL = 0.5;
		
		res->objMesh[0].v[0]  =  HL;	res->objMesh[0].v[1]  =  HL;	res->objMesh[0].v[2]  =  HL;
		res->objMesh[0].v[3]  =  HL;	res->objMesh[0].v[4]  =  HL;	res->objMesh[0].v[5]  = -HL;
		res->objMesh[0].v[6]  = -HL;	res->objMesh[0].v[7]  =  HL;	res->objMesh[0].v[8]  = -HL;
		res->objMesh[0].v[9]  = -HL;	res->objMesh[0].v[10] =  HL;	res->objMesh[0].v[11] =  HL;
		res->objMesh[0].v[12] =  HL;	res->objMesh[0].v[13] = -HL;	res->objMesh[0].v[14] =  HL;
		res->objMesh[0].v[15] =  HL;	res->objMesh[0].v[16] = -HL;	res->objMesh[0].v[17] = -HL;
		res->objMesh[0].v[18] = -HL;	res->objMesh[0].v[19] = -HL;	res->objMesh[0].v[20] = -HL;
		res->objMesh[0].v[21] = -HL;	res->objMesh[0].v[22] = -HL;	res->objMesh[0].v[23] =  HL;

		res->objMesh[0].fi[0]  = 0;		res->objMesh[0].fi[1]  = 1;		res->objMesh[0].fi[2]  = 3;
		res->objMesh[0].fi[3]  = 3;		res->objMesh[0].fi[4]  = 1;		res->objMesh[0].fi[5]  = 2;
		res->objMesh[0].fi[6]  = 0;		res->objMesh[0].fi[7]  = 4;		res->objMesh[0].fi[8]  = 1;
		res->objMesh[0].fi[9]  = 1;		res->objMesh[0].fi[10] = 4;		res->objMesh[0].fi[11] = 5;
		res->objMesh[0].fi[12] = 4;		res->objMesh[0].fi[13] = 7;		res->objMesh[0].fi[14] = 5;
		res->objMesh[0].fi[15] = 7;		res->objMesh[0].fi[16] = 6;		res->objMesh[0].fi[17] = 5;
		res->objMesh[0].fi[18] = 3;		res->objMesh[0].fi[19] = 2;		res->objMesh[0].fi[20] = 7;
		res->objMesh[0].fi[21] = 2;		res->objMesh[0].fi[22] = 6;		res->objMesh[0].fi[23] = 7;
		res->objMesh[0].fi[24] = 1;		res->objMesh[0].fi[25] = 5;		res->objMesh[0].fi[26] = 6;
		res->objMesh[0].fi[27] = 1;		res->objMesh[0].fi[28] = 6;		res->objMesh[0].fi[29] = 2;
		res->objMesh[0].fi[30] = 0;		res->objMesh[0].fi[31] = 3;		res->objMesh[0].fi[32] = 4;
		res->objMesh[0].fi[33] = 3;		res->objMesh[0].fi[34] = 7;		res->objMesh[0].fi[35] = 4;



		res->objMesh[0].num_tvertex = 12;

		res->objMesh[0].tv = (float*)malloc( sizeof(float) *  res->objMesh[0].num_tvertex * 2 );
		ZeroMemory( res->objMesh[0].tv , sizeof(float) * res->objMesh[0].num_tvertex * 2 );
		res->objMesh[0].tf = (int*)malloc( sizeof(int) *  res->objMesh[0].num_face * 3 );
		ZeroMemory( res->objMesh[0].tf , sizeof(int) * res->objMesh[0].num_face * 3 );

		res->objMesh[0].tv[0]  = 1.0;	res->objMesh[0].tv[1]  = 1.0;
		res->objMesh[0].tv[2]  = 1.0;	res->objMesh[0].tv[3]  = 0.0;
		res->objMesh[0].tv[4]  = 0.0;	res->objMesh[0].tv[5]  = 0.0;
		res->objMesh[0].tv[6]  = 0.0;	res->objMesh[0].tv[7]  = 1.0;

		res->objMesh[0].tv[8]  = 1.0;	res->objMesh[0].tv[9]  = 0.0;
		res->objMesh[0].tv[10] = 1.0;	res->objMesh[0].tv[11] = 1.0;
		res->objMesh[0].tv[12] = 0.0;	res->objMesh[0].tv[13] = 1.0;
		res->objMesh[0].tv[14] = 0.0;	res->objMesh[0].tv[15] = 0.0;

		res->objMesh[0].tv[16] = 0.0;	res->objMesh[0].tv[17] = 1.0;
		res->objMesh[0].tv[18] = 0.0;	res->objMesh[0].tv[19] = 0.0;
		res->objMesh[0].tv[20] = 1.0;	res->objMesh[0].tv[21] = 1.0;
		res->objMesh[0].tv[22] = 1.0;	res->objMesh[0].tv[23] = 0.0;

		
		res->objMesh[0].tf[0]  = 0;		res->objMesh[0].tf[1]  = 1;		res->objMesh[0].tf[2]  = 3;
		res->objMesh[0].tf[3]  = 3;		res->objMesh[0].tf[4]  = 1;		res->objMesh[0].tf[5]  = 2;
		res->objMesh[0].tf[6]  = 0;		res->objMesh[0].tf[7]  = 8;		res->objMesh[0].tf[8]  = 1;
		res->objMesh[0].tf[9]  = 1;		res->objMesh[0].tf[10] = 8;		res->objMesh[0].tf[11] = 9;
		res->objMesh[0].tf[12] = 4;		res->objMesh[0].tf[13] = 7;		res->objMesh[0].tf[14] = 5;
		res->objMesh[0].tf[15] = 7;		res->objMesh[0].tf[16] = 6;		res->objMesh[0].tf[17] = 5;
		res->objMesh[0].tf[18] = 3;		res->objMesh[0].tf[19] = 2;		res->objMesh[0].tf[20] = 10;
		res->objMesh[0].tf[21] = 2;		res->objMesh[0].tf[22] = 11;	res->objMesh[0].tf[23] = 10;
		res->objMesh[0].tf[24] = 1;		res->objMesh[0].tf[25] = 5;		res->objMesh[0].tf[26] = 6;
		res->objMesh[0].tf[27] = 1;		res->objMesh[0].tf[28] = 6;		res->objMesh[0].tf[29] = 2;
		res->objMesh[0].tf[30] = 0;		res->objMesh[0].tf[31] = 3;		res->objMesh[0].tf[32] = 4;
		res->objMesh[0].tf[33] = 3;		res->objMesh[0].tf[34] = 7;		res->objMesh[0].tf[35] = 4;

			
		res->objMesh[0].mat_ID = 1;


	
		// Compute Face Normals


	VEC3 a,b,c,n;
	
		res->objMesh[0].vn = (VNORM*)malloc( sizeof VNORM * res->objMesh[0].num_face * 3 );
		ZeroMemory( res->objMesh[0].vn , sizeof VNORM * res->objMesh[0].num_face * 3 );		

		for( int f=0; f<res->objMesh[0].num_face ; f++ )
		{
			a.x = res->objMesh[0].v[ res->objMesh[0].fi[f*3] * 3     ];
			a.y = res->objMesh[0].v[ res->objMesh[0].fi[f*3] * 3 + 1 ];
			a.z = res->objMesh[0].v[ res->objMesh[0].fi[f*3] * 3 + 2 ];

			b.x = res->objMesh[0].v[ res->objMesh[0].fi[f*3+1] * 3     ];
			b.y = res->objMesh[0].v[ res->objMesh[0].fi[f*3+1] * 3 + 1 ];
			b.z = res->objMesh[0].v[ res->objMesh[0].fi[f*3+1] * 3 + 2 ];

			c.x = res->objMesh[0].v[ res->objMesh[0].fi[f*3+2] * 3     ];
			c.y = res->objMesh[0].v[ res->objMesh[0].fi[f*3+2] * 3 + 1 ];
			c.z = res->objMesh[0].v[ res->objMesh[0].fi[f*3+2] * 3 + 2 ];		

			n = (a - c)^(b - c);
			n.Normalize();
			
			res->objMesh[0].fn[f*3]		= (float)n.x;
			res->objMesh[0].fn[f*3+1]	= (float)n.y;
			res->objMesh[0].fn[f*3+2]	= (float)n.z;

			
			res->objMesh[0].vn[f*3].index = res->objMesh[0].fi[f*3];
			res->objMesh[0].vn[f*3].x = (float)n.x;
			res->objMesh[0].vn[f*3].y = (float)n.y;
			res->objMesh[0].vn[f*3].z = (float)n.z;
			
			res->objMesh[0].vn[f*3+1].index = res->objMesh[0].fi[f*3+1];
			res->objMesh[0].vn[f*3+1].x = (float)n.x;
			res->objMesh[0].vn[f*3+1].y = (float)n.y;
			res->objMesh[0].vn[f*3+1].z = (float)n.z;
			
			res->objMesh[0].vn[f*3+2].index = res->objMesh[0].fi[f*3+2];
			res->objMesh[0].vn[f*3+2].x = (float)n.x;
			res->objMesh[0].vn[f*3+2].y = (float)n.y;
			res->objMesh[0].vn[f*3+2].z = (float)n.z;
		

		}					
	

	
	return res;

}


SIM* MakeCylinder( char* name )
{
	SIM* res = NULL;
	res = new SIM();
	ZeroMemory( res, sizeof(SIM) );

	
	res->num_object = 1;
		
	res->num_material = 1;
			
	res->objMesh = (OBJMESH*)malloc( sizeof OBJMESH * res->num_object );
	res->material = (MATERIAL*)malloc( sizeof MATERIAL * res->num_material );
	ZeroMemory( res->objMesh , sizeof OBJMESH * res->num_object );
	ZeroMemory( res->material , sizeof MATERIAL * res->num_material );

	for( int mat=0 ; mat<res->num_material ; mat++ )
	{
		sprintf( res->material[mat].texname , "%s" , name );
		
	}


	// GEOMOBJECT
		int NUM_PIE = 8;
	
		res->objMesh[0].num_vertex	= NUM_PIE*2 + 2;	// including 2 centre points
		res->objMesh[0].num_face	= NUM_PIE*4;

		// allocation
		res->objMesh[0].v = (float*)malloc( sizeof(float) * res->objMesh[0].num_vertex * 3 );
		res->objMesh[0].fn = (float*)malloc( sizeof(float) * res->objMesh[0].num_face * 3 );		
		res->objMesh[0].fi = (int*)malloc( sizeof(int) *  res->objMesh[0].num_face * 3 );

		// numbers
		ZeroMemory( res->objMesh[0].fi , sizeof(int) * res->objMesh[0].num_face * 3 );
		ZeroMemory( res->objMesh[0].fn , sizeof(float) * res->objMesh[0].num_face * 3 );
		ZeroMemory( res->objMesh[0].v , sizeof(float) * res->objMesh[0].num_vertex * 3 );

		
		double R = 1.0;

		VEC3* cyl = (VEC3*)malloc( sizeof(VEC3) * (NUM_PIE*2) );

		cyl[0].Set( R , 0.0 , 0.0 );

		double PI2_Frag = 6.283185307179586476925286766559 / (double)NUM_PIE;
		VEC3 rotAxs;	rotAxs.Set( 0.0 , 0.0 , 1.0 );

		for( int i=0 ; i<NUM_PIE ; i++ )
		{
			cyl[i].Set( R , 0.0 , 0.0 );
			if( i>0 )	cyl[i].RotateABS( rotAxs , PI2_Frag*(double)i );

			cyl[i] = cyl[i+NUM_PIE];
			
			cyl[i].z		 =  0.5;
			cyl[i+NUM_PIE].z = -0.5;
		}

		VEC3 upCent, downCent;
		upCent.Set( 0.0 , 0.0 , 0.5 );
		downCent.Set( 0.0 , 0.0 , -0.5 );

		
		for( int i=0 ; i<NUM_PIE ; i++ )
		{
			res->objMesh[0].v[3*i+0] = (float)cyl[i].x;
			res->objMesh[0].v[3*i+1] = (float)cyl[i].y;
			res->objMesh[0].v[3*i+2] = (float)cyl[i].z;

			res->objMesh[0].v[3*(i+NUM_PIE)+0] = (float)cyl[i+NUM_PIE].x;
			res->objMesh[0].v[3*(i+NUM_PIE)+1] = (float)cyl[i+NUM_PIE].y;
			res->objMesh[0].v[3*(i+NUM_PIE)+2] = (float)cyl[i+NUM_PIE].z;
		}

		free( cyl );

		int upIdx   = NUM_PIE*2;
		int downIdx = NUM_PIE*2+1;

		res->objMesh[0].v[3*upIdx+0] = (float)upCent.x;
		res->objMesh[0].v[3*upIdx+1] = (float)upCent.y;
		res->objMesh[0].v[3*upIdx+2] = (float)upCent.z;

		res->objMesh[0].v[3*downIdx+0] = (float)downCent.x;
		res->objMesh[0].v[3*downIdx+1] = (float)downCent.y;
		res->objMesh[0].v[3*downIdx+2] = (float)downCent.z;


		// face idx
		for( int i=0 ; i<NUM_PIE ; i++ )
		{
			int ii = i+1;	if( ii==NUM_PIE )	ii=0;

			// up disk
			res->objMesh[0].fi[3*i+0] = upIdx;
			res->objMesh[0].fi[3*i+1] = ii;
			res->objMesh[0].fi[3*i+2] = i;

			// down disk
			res->objMesh[0].fi[3*(i+NUM_PIE)+0] = downIdx;
			res->objMesh[0].fi[3*(i+NUM_PIE)+1] = i+NUM_PIE;
			res->objMesh[0].fi[3*(i+NUM_PIE)+2] = ii+NUM_PIE;
		}

		// side
		for( int i=0 ; i<NUM_PIE ; i++ )
		{
			int ii = i+1;	if( ii==NUM_PIE )	ii=0;

			res->objMesh[0].fi[3*(i+NUM_PIE*2)+0] = i;
			res->objMesh[0].fi[3*(i+NUM_PIE*2)+1] = ii;
			res->objMesh[0].fi[3*(i+NUM_PIE*2)+2] = i+NUM_PIE;

			res->objMesh[0].fi[3*(i+NUM_PIE*3)+0] = ii;
			res->objMesh[0].fi[3*(i+NUM_PIE*3)+1] = ii+NUM_PIE;
			res->objMesh[0].fi[3*(i+NUM_PIE*3)+2] = i+NUM_PIE;
		}

		
/*		res->objMesh[0].v[0]  =  HL;	res->objMesh[0].v[1]  =  HL;	res->objMesh[0].v[2]  =  HL;
		res->objMesh[0].v[3]  =  HL;	res->objMesh[0].v[4]  =  HL;	res->objMesh[0].v[5]  = -HL;
		res->objMesh[0].v[6]  = -HL;	res->objMesh[0].v[7]  =  HL;	res->objMesh[0].v[8]  = -HL;
		res->objMesh[0].v[9]  = -HL;	res->objMesh[0].v[10] =  HL;	res->objMesh[0].v[11] =  HL;
		res->objMesh[0].v[12] =  HL;	res->objMesh[0].v[13] = -HL;	res->objMesh[0].v[14] =  HL;
		res->objMesh[0].v[15] =  HL;	res->objMesh[0].v[16] = -HL;	res->objMesh[0].v[17] = -HL;
		res->objMesh[0].v[18] = -HL;	res->objMesh[0].v[19] = -HL;	res->objMesh[0].v[20] = -HL;
		res->objMesh[0].v[21] = -HL;	res->objMesh[0].v[22] = -HL;	res->objMesh[0].v[23] =  HL;

		res->objMesh[0].fi[0]  = 0;		res->objMesh[0].fi[1]  = 1;		res->objMesh[0].fi[2]  = 3;
		res->objMesh[0].fi[3]  = 3;		res->objMesh[0].fi[4]  = 1;		res->objMesh[0].fi[5]  = 2;
		res->objMesh[0].fi[6]  = 0;		res->objMesh[0].fi[7]  = 4;		res->objMesh[0].fi[8]  = 1;
		res->objMesh[0].fi[9]  = 1;		res->objMesh[0].fi[10] = 4;		res->objMesh[0].fi[11] = 5;
		res->objMesh[0].fi[12] = 4;		res->objMesh[0].fi[13] = 7;		res->objMesh[0].fi[14] = 5;
		res->objMesh[0].fi[15] = 7;		res->objMesh[0].fi[16] = 6;		res->objMesh[0].fi[17] = 5;
		res->objMesh[0].fi[18] = 3;		res->objMesh[0].fi[19] = 2;		res->objMesh[0].fi[20] = 7;
		res->objMesh[0].fi[21] = 2;		res->objMesh[0].fi[22] = 6;		res->objMesh[0].fi[23] = 7;
		res->objMesh[0].fi[24] = 1;		res->objMesh[0].fi[25] = 5;		res->objMesh[0].fi[26] = 6;
		res->objMesh[0].fi[27] = 1;		res->objMesh[0].fi[28] = 6;		res->objMesh[0].fi[29] = 2;
		res->objMesh[0].fi[30] = 0;		res->objMesh[0].fi[31] = 3;		res->objMesh[0].fi[32] = 4;
		res->objMesh[0].fi[33] = 3;		res->objMesh[0].fi[34] = 7;		res->objMesh[0].fi[35] = 4;
*/


		res->objMesh[0].num_tvertex = 12;

		res->objMesh[0].tv = (float*)malloc( sizeof(float) *  res->objMesh[0].num_tvertex * 2 );
		ZeroMemory( res->objMesh[0].tv , sizeof(float) * res->objMesh[0].num_tvertex * 2 );
		res->objMesh[0].tf = (int*)malloc( sizeof(int) *  res->objMesh[0].num_face * 3 );
		ZeroMemory( res->objMesh[0].tf , sizeof(int) * res->objMesh[0].num_face * 3 );

		res->objMesh[0].tv[0]  = 1.0;	res->objMesh[0].tv[1]  = 1.0;
		res->objMesh[0].tv[2]  = 1.0;	res->objMesh[0].tv[3]  = 0.0;
		res->objMesh[0].tv[4]  = 0.0;	res->objMesh[0].tv[5]  = 0.0;
		res->objMesh[0].tv[6]  = 0.0;	res->objMesh[0].tv[7]  = 1.0;

		res->objMesh[0].tv[8]  = 1.0;	res->objMesh[0].tv[9]  = 0.0;
		res->objMesh[0].tv[10] = 1.0;	res->objMesh[0].tv[11] = 1.0;
		res->objMesh[0].tv[12] = 0.0;	res->objMesh[0].tv[13] = 1.0;
		res->objMesh[0].tv[14] = 0.0;	res->objMesh[0].tv[15] = 0.0;

		res->objMesh[0].tv[16] = 0.0;	res->objMesh[0].tv[17] = 1.0;
		res->objMesh[0].tv[18] = 0.0;	res->objMesh[0].tv[19] = 0.0;
		res->objMesh[0].tv[20] = 1.0;	res->objMesh[0].tv[21] = 1.0;
		res->objMesh[0].tv[22] = 1.0;	res->objMesh[0].tv[23] = 0.0;

		
		res->objMesh[0].tf[0]  = 0;		res->objMesh[0].tf[1]  = 1;		res->objMesh[0].tf[2]  = 3;
		res->objMesh[0].tf[3]  = 3;		res->objMesh[0].tf[4]  = 1;		res->objMesh[0].tf[5]  = 2;
		res->objMesh[0].tf[6]  = 0;		res->objMesh[0].tf[7]  = 8;		res->objMesh[0].tf[8]  = 1;
		res->objMesh[0].tf[9]  = 1;		res->objMesh[0].tf[10] = 8;		res->objMesh[0].tf[11] = 9;
		res->objMesh[0].tf[12] = 4;		res->objMesh[0].tf[13] = 7;		res->objMesh[0].tf[14] = 5;
		res->objMesh[0].tf[15] = 7;		res->objMesh[0].tf[16] = 6;		res->objMesh[0].tf[17] = 5;
		res->objMesh[0].tf[18] = 3;		res->objMesh[0].tf[19] = 2;		res->objMesh[0].tf[20] = 10;
		res->objMesh[0].tf[21] = 2;		res->objMesh[0].tf[22] = 11;	res->objMesh[0].tf[23] = 10;
		res->objMesh[0].tf[24] = 1;		res->objMesh[0].tf[25] = 5;		res->objMesh[0].tf[26] = 6;
		res->objMesh[0].tf[27] = 1;		res->objMesh[0].tf[28] = 6;		res->objMesh[0].tf[29] = 2;
		res->objMesh[0].tf[30] = 0;		res->objMesh[0].tf[31] = 3;		res->objMesh[0].tf[32] = 4;
		res->objMesh[0].tf[33] = 3;		res->objMesh[0].tf[34] = 7;		res->objMesh[0].tf[35] = 4;

			
		res->objMesh[0].mat_ID = 1;


	
		// Compute Face Normals


	VEC3 a,b,c,n;
	
		res->objMesh[0].vn = (VNORM*)malloc( sizeof VNORM * res->objMesh[0].num_face * 3 );
		ZeroMemory( res->objMesh[0].vn , sizeof VNORM * res->objMesh[0].num_face * 3 );		

		for( int f=0; f<res->objMesh[0].num_face ; f++ )
		{
			a.x = res->objMesh[0].v[ res->objMesh[0].fi[f*3] * 3     ];
			a.y = res->objMesh[0].v[ res->objMesh[0].fi[f*3] * 3 + 1 ];
			a.z = res->objMesh[0].v[ res->objMesh[0].fi[f*3] * 3 + 2 ];

			b.x = res->objMesh[0].v[ res->objMesh[0].fi[f*3+1] * 3     ];
			b.y = res->objMesh[0].v[ res->objMesh[0].fi[f*3+1] * 3 + 1 ];
			b.z = res->objMesh[0].v[ res->objMesh[0].fi[f*3+1] * 3 + 2 ];

			c.x = res->objMesh[0].v[ res->objMesh[0].fi[f*3+2] * 3     ];
			c.y = res->objMesh[0].v[ res->objMesh[0].fi[f*3+2] * 3 + 1 ];
			c.z = res->objMesh[0].v[ res->objMesh[0].fi[f*3+2] * 3 + 2 ];		

			n = (a - c)^(b - c);
			n.Normalize();
			
			res->objMesh[0].fn[f*3]		= (float)n.x;
			res->objMesh[0].fn[f*3+1]	= (float)n.y;
			res->objMesh[0].fn[f*3+2]	= (float)n.z;

			
			res->objMesh[0].vn[f*3].index = res->objMesh[0].fi[f*3];
			res->objMesh[0].vn[f*3].x = (float)n.x;
			res->objMesh[0].vn[f*3].y = (float)n.y;
			res->objMesh[0].vn[f*3].z = (float)n.z;
			
			res->objMesh[0].vn[f*3+1].index = res->objMesh[0].fi[f*3+1];
			res->objMesh[0].vn[f*3+1].x = (float)n.x;
			res->objMesh[0].vn[f*3+1].y = (float)n.y;
			res->objMesh[0].vn[f*3+1].z = (float)n.z;
			
			res->objMesh[0].vn[f*3+2].index = res->objMesh[0].fi[f*3+2];
			res->objMesh[0].vn[f*3+2].x = (float)n.x;
			res->objMesh[0].vn[f*3+2].y = (float)n.y;
			res->objMesh[0].vn[f*3+2].z = (float)n.z;
		

		}					
	

	
	return res;

}



void get_v_normal( SIM* sim )
{
	int t=0;
	D3DXVECTOR3 tm,
				fm,
				*dupe = NULL,
				*newdupe = NULL;
	tm.x = 0.0f; tm.y = 0.0f; tm.z = 0.0f;
	int num_dupe = 0;
	int *isIn = NULL;
	
	int **temp = NULL;
	
	
	for( int obj=0 ; obj<sim->num_object ; obj++ )
	{
		for( int f=0 ; f<sim->objMesh[obj].num_face ; f++ )
		{
			fm = D3DXVECTOR3( sim->objMesh[obj].fn[f*3],
							  sim->objMesh[obj].fn[f*3+1],
 							  sim->objMesh[obj].fn[f*3+2]  );
			D3DXVec3Normalize( (D3DXVECTOR3*)&fm, &fm );

			sim->objMesh[obj].fn[f*3]	= fm.x;
			sim->objMesh[obj].fn[f*3+1]	= fm.y;
 			sim->objMesh[obj].fn[f*3+2] = fm.z;
			
		}

		temp = (int**)malloc( sizeof (int*) * sim->objMesh[obj].num_vertex );

		for( int tt=0 ; tt<sim->objMesh[obj].num_vertex ; tt++ )
		{
	//		temp[tt] = new int[sim->objMesh[obj].num_face*3];
			temp[tt] = (int*)malloc( sizeof(int) * sim->objMesh[obj].num_face*3 );
		}

		for(int pp=0 ; pp<sim->objMesh[obj].num_vertex ; pp++ )
		for(int qq=0 ; qq<sim->objMesh[obj].num_face*3 ; qq++ )
			temp[pp][qq] = -1;
		

		for( int i=0 ; i<sim->objMesh[obj].num_vertex ; i++ )
		{
			t=0;

			for( int j=0 ; j<sim->objMesh[obj].num_face*3 ; j++ )
			{			
				if( sim->objMesh[obj].vn[j].index == i )
				{
					temp[i][t] = j;
					t++;
				}
			}
		}

		////////////////////
				
		for( int p=0 ; p<sim->objMesh[obj].num_vertex ; p++ )
		{
			num_dupe = 0;

			for( int q=0 ; q<sim->objMesh[obj].num_face*3 ; q++ )
			{
				if( temp[p][q] != -1 )
				{
					num_dupe++;
				}
			}

			dupe = new D3DXVECTOR3[num_dupe];
			newdupe = new D3DXVECTOR3[num_dupe];
			isIn = new int[num_dupe];
			ZeroMemory( dupe , sizeof D3DXVECTOR3 * num_dupe );
			ZeroMemory( newdupe , sizeof D3DXVECTOR3 * num_dupe );
			t=0;

			for( int q=0 ; q<sim->objMesh[obj].num_face*3 ; q++ )
			{
				if( temp[p][q] != -1 )
				{
					dupe[t].x = sim->objMesh[obj].vn[temp[p][q]].x;
					dupe[t].y = sim->objMesh[obj].vn[temp[p][q]].y;
					dupe[t].z = sim->objMesh[obj].vn[temp[p][q]].z;
					t++;
				}
			}

						
			for( int dv=0 ; dv<num_dupe ; dv++ )
			{
				for( int dvv=0 ; dvv<num_dupe ; dvv++ )
				{
					if( (dupe[dv].x == newdupe[dvv].x)&&
						(dupe[dv].y == newdupe[dvv].y)&&
						(dupe[dv].z == newdupe[dvv].z)   )
					{
						isIn[dv] = 0;
						break;
					}
					else
					{
						isIn[dv] = 1;
					//	break;
					}
				}					
						
				if( isIn[dv] == 1 )
				{
					newdupe[dv].x = dupe[dv].x;
					newdupe[dv].y = dupe[dv].y;
					newdupe[dv].z = dupe[dv].z;
				}																	
										
			}

			delete [] dupe;
			delete [] isIn;

			tm.x = 0.0f;	tm.y = 0.0f;	tm.z = 0.0f;
			for( int dv=0 ; dv<num_dupe ; dv++ )
			{
				tm.x = tm.x + newdupe[dv].x;
				tm.y = tm.y + newdupe[dv].y;
				tm.z = tm.z + newdupe[dv].z;
			}

			delete [] newdupe;
			
			
			for( int v=0 ; v<sim->objMesh[obj].num_face*3 ; v++ )
			{
				if( temp[p][v] != -1 )
				{
					sim->objMesh[obj].vn[temp[p][v]].x = tm.x;
					sim->objMesh[obj].vn[temp[p][v]].y = tm.y;
					sim->objMesh[obj].vn[temp[p][v]].z = tm.z;
					
					D3DXVECTOR3 norm;
					norm.x = sim->objMesh[obj].vn[temp[p][v]].x;
					norm.y = sim->objMesh[obj].vn[temp[p][v]].y;
					norm.z = sim->objMesh[obj].vn[temp[p][v]].z;

					//norm = Normalize( norm );
					D3DXVec3Normalize( (D3DXVECTOR3*)&norm, &norm );

					sim->objMesh[obj].vn[temp[p][v]].x = norm.x;
					sim->objMesh[obj].vn[temp[p][v]].y = norm.y;
					sim->objMesh[obj].vn[temp[p][v]].z = norm.z;
				}

			}
		
		}

		
		for( int tt=0 ; tt<sim->objMesh[obj].num_vertex ; tt++ )
			free(temp[tt]);
		free( temp );
		
	}
		
}






/*
		res->objMesh[0].num_tvertex = 8;

		res->objMesh[0].tv = (float*)malloc( sizeof(float) *  res->objMesh[0].num_tvertex * 2 );
		ZeroMemory( res->objMesh[0].tv , sizeof(float) * res->objMesh[0].num_tvertex * 2 );
		res->objMesh[0].tf = (int*)malloc( sizeof(int) *  res->objMesh[0].num_face * 3 );
		ZeroMemory( res->objMesh[0].tf , sizeof(int) * res->objMesh[0].num_face * 3 );

		res->objMesh[0].tv[0]  = 1.0;	res->objMesh[0].tv[1]  = 1.0;
		res->objMesh[0].tv[2]  = 0.0;	res->objMesh[0].tv[3]  = 1.0;
		res->objMesh[0].tv[4]  = 0.0;	res->objMesh[0].tv[5]  = 0.0;
		res->objMesh[0].tv[6]  = 1.0;	res->objMesh[0].tv[7]  = 0.0;

		res->objMesh[0].tv[8]  = 1.0;	res->objMesh[0].tv[9]  = 1.0;
		res->objMesh[0].tv[10] = 0.0;	res->objMesh[0].tv[11] = 1.0;
		res->objMesh[0].tv[12] = 0.0;	res->objMesh[0].tv[13] = 0.0;
		res->objMesh[0].tv[14] = 1.0;	res->objMesh[0].tv[15] = 0.0;

		
		res->objMesh[0].tf[0]  = 0;		res->objMesh[0].tf[1]  = 1;		res->objMesh[0].tf[2]  = 3;
		res->objMesh[0].tf[3]  = 3;		res->objMesh[0].tf[4]  = 1;		res->objMesh[0].tf[5]  = 2;
		res->objMesh[0].tf[6]  = 0;		res->objMesh[0].tf[7]  = 4;		res->objMesh[0].tf[8]  = 1;
		res->objMesh[0].tf[9]  = 1;		res->objMesh[0].tf[10] = 4;		res->objMesh[0].tf[11] = 5;
		res->objMesh[0].tf[12] = 4;		res->objMesh[0].tf[13] = 7;		res->objMesh[0].tf[14] = 5;
		res->objMesh[0].tf[15] = 7;		res->objMesh[0].tf[16] = 6;		res->objMesh[0].tf[17] = 5;
		res->objMesh[0].tf[18] = 3;		res->objMesh[0].tf[19] = 2;		res->objMesh[0].tf[20] = 7;
		res->objMesh[0].tf[21] = 2;		res->objMesh[0].tf[22] = 6;		res->objMesh[0].tf[23] = 7;
		res->objMesh[0].tf[24] = 1;		res->objMesh[0].tf[25] = 5;		res->objMesh[0].tf[26] = 6;
		res->objMesh[0].tf[27] = 1;		res->objMesh[0].tf[28] = 6;		res->objMesh[0].tf[29] = 2;
		res->objMesh[0].tf[30] = 0;		res->objMesh[0].tf[31] = 3;		res->objMesh[0].tf[32] = 4;
		res->objMesh[0].tf[33] = 3;		res->objMesh[0].tf[34] = 7;		res->objMesh[0].tf[35] = 4;
*/	