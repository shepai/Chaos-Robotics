


#include "meshes.h"
#include "..\..\common\NeuroUse\neurouse.h"
#include "../Status.h"
#include "dxwnd.h"


#include "sim.h"
#include "meshes.h"

#include "robj.h"




#define XCOL_WHITE		D3DCOLOR_XRGB(255,255,255)
#define XCOL_YELLOW		D3DCOLOR_XRGB(255,255,0)

// Once at Initialisation
HRESULT Set_Init_SIM_Data_with_Shadow( LPDIRECT3DVERTEXBUFFER8 g_pVB, SIM* g_sim, bool is_shadow )
{
	OBJMESH* obj = &g_sim->objMesh[0];
	
	CUSTOMVERTEX* pVertices = NULL;
	
	
		if( FAILED( g_pVB->Lock( 0, 0, (BYTE**)&pVertices, 0 ) ) )		return E_FAIL;
		
			for( int f=0; f<obj->num_face ; f++ )
			{
				for(int w = 0; w < 3; w++)
				{
					int idx = f*3 + w;

					pVertices[idx].position = D3DXVECTOR3(	obj->v[ obj->fi[idx] * 3	 ],
															obj->v[ obj->fi[idx] * 3 + 1 ],
															obj->v[ obj->fi[idx] * 3 + 2 ]  );
					
					if( is_shadow )
					{
						pVertices[idx].n = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
						pVertices[idx].tu = pVertices[idx].tv = 0.0f;
					}
					else
					{
						pVertices[idx].n = D3DXVECTOR3( obj->vn[idx].x , obj->vn[idx].y , obj->vn[idx].z );
			
						pVertices[idx].tu = obj->tv[ obj->tf[idx] * 2	  ];
						pVertices[idx].tv = obj->tv[ obj->tf[idx] * 2 + 1 ];
					}

					// dummy color must be set
					pVertices[idx].diffuseColor = XCOL_WHITE;
				}
			}

		if( FAILED( g_pVB->Unlock() ) )		return E_FAIL;

	

	return S_OK;

}





// Put my vertices into DX structure

HRESULT Set_Realtime_Triangle_Data( LPDIRECT3DVERTEXBUFFER8 g_pVB , TRISURFACE* trList , int num_triangle )
{
	CUSTOMVERTEX* pVertices = NULL;
		
		if( FAILED( g_pVB->Lock( 0, 0, (BYTE**)&pVertices, 0 ) ) )	return E_FAIL;
		

			for( int f=0; f<num_triangle ; f++ )
			{
			
				VEC3 norm = trList[f].surfnorm;

				for(int w = 0; w < 3; w++)
				{		
					pVertices[f*3+w].position = D3DXVECTOR3( (float)trList[f].vtx[w].pos.x , 
															 (float)trList[f].vtx[w].pos.y , 
															 (float)trList[f].vtx[w].pos.z		);

					pVertices[f*3+w].n = D3DXVECTOR3( (float)norm.x , (float)norm.y , (float)norm.z );

					pVertices[f*3+w].tu = trList[f].vtx[w].u;
					pVertices[f*3+w].tv = trList[f].vtx[w].v;

					pVertices[f*3+w].diffuseColor = D3DCOLOR_XRGB( trList[f].col[0] , trList[f].col[1] , trList[f].col[2] );
				}
		
			}

		
		if( FAILED( g_pVB->Unlock() ) )		return E_FAIL; 		
			

	return S_OK;

}





static double wsHeight = 0.00001;

HRESULT Set_RT_Shadow_Data( LPDIRECT3DVERTEXBUFFER8 g_pVB , TRISURFACE* trList , int num_triangle )
{
	
	CUSTOMVERTEX* pVertices = NULL;
	
	
		if( FAILED( g_pVB->Lock( 0, 0, (BYTE**)&pVertices, 0 ) ) )		return E_FAIL;
		

			for( int f=0; f<num_triangle ; f++ )
			{			
				for(int w = 0; w < 3; w++)
				{
					VEC3 wvtx = trList[f].vtx[w].pos;
				
					VEC3 svtx;
					svtx.Set( wvtx.x+wvtx.y-wsHeight , wsHeight , wvtx.z+wvtx.y-wsHeight );


					pVertices[f*3+w].position = D3DXVECTOR3( (float)svtx.x , (float)svtx.y , (float)svtx.z  );

					pVertices[f*3+w].n = D3DXVECTOR3( 0.0f , 1.0f , 0.0f );

					pVertices[f*3+w].tu = pVertices[f*3+w].tv = 0.0f;

					pVertices[f*3+w].diffuseColor = D3DCOLOR_XRGB(255,255,255);
				}

			}


		
		if( FAILED( g_pVB->Unlock() ) )		return E_FAIL; 		

	

	return S_OK;

}





HRESULT SetFloorData( LPDIRECT3DVERTEXBUFFER8 g_pVB , int NUM_FLOOR , float TILE_WID )
{
	
	CUSTOMVERTEX* pVertices = NULL;
	
	int count = 0;
		
	if( FAILED( g_pVB->Lock( 0, 0, (BYTE**)&pVertices, 0 ) ) )
		return E_FAIL;
		
		for( int y=0 ; y<2*NUM_FLOOR ; y++ )
		{
			for( int x=0 ; x<2*NUM_FLOOR ; x++ )
			{
				pVertices[count].position = D3DXVECTOR3( -TILE_WID*(float)NUM_FLOOR + (float)x*TILE_WID , 0.0f , -TILE_WID*(float)NUM_FLOOR + (float)y*TILE_WID );
				pVertices[count].n = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
				pVertices[count].tu = 0.0f;
				pVertices[count].tv = 0.0f;
				pVertices[count].diffuseColor = XCOL_WHITE;
				
				count++;

				pVertices[count].position = D3DXVECTOR3( -TILE_WID*(float)NUM_FLOOR + (float)x*TILE_WID , 0.0f , -TILE_WID*(float)NUM_FLOOR + (float)y*TILE_WID + TILE_WID );
				pVertices[count].n = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
				pVertices[count].tu = 0.0f;
				pVertices[count].tv = 1.0f;
				pVertices[count].diffuseColor = XCOL_WHITE;

				count++;

				pVertices[count].position = D3DXVECTOR3( -TILE_WID*(float)NUM_FLOOR + (float)x*TILE_WID + TILE_WID , 0.0f , -TILE_WID*(float)NUM_FLOOR + (float)y*TILE_WID );
				pVertices[count].n = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
				pVertices[count].tu = 1.0f;
				pVertices[count].tv = 0.0f;
				pVertices[count].diffuseColor = XCOL_WHITE;

				count++;

				pVertices[count].position = D3DXVECTOR3( -TILE_WID*(float)NUM_FLOOR + (float)x*TILE_WID + TILE_WID , 0.0f , -TILE_WID*(float)NUM_FLOOR + (float)y*TILE_WID );
				pVertices[count].n = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
				pVertices[count].tu = 1.0f;
				pVertices[count].tv = 0.0f;
				pVertices[count].diffuseColor = XCOL_WHITE;

				count++;				

				pVertices[count].position = D3DXVECTOR3( -TILE_WID*(float)NUM_FLOOR + (float)x*TILE_WID , 0.0f , -TILE_WID*(float)NUM_FLOOR + (float)y*TILE_WID + TILE_WID );
				pVertices[count].n = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
				pVertices[count].tu = 0.0f;
				pVertices[count].tv = 1.0f;
				pVertices[count].diffuseColor = XCOL_WHITE;

				count++;

				pVertices[count].position = D3DXVECTOR3( -TILE_WID*(float)NUM_FLOOR + (float)x*TILE_WID + TILE_WID , 0.0f , -TILE_WID*(float)NUM_FLOOR + (float)y*TILE_WID + TILE_WID );
				pVertices[count].n = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
				pVertices[count].tu = 1.0f;
				pVertices[count].tv = 1.0f;
				pVertices[count].diffuseColor = XCOL_WHITE;

				count++;
				
			}
		}

	if( FAILED( g_pVB->Unlock() ) )
		return E_FAIL; 		
		

	return S_OK;

}



HRESULT SetGridData( LPDIRECT3DVERTEXBUFFER8 g_pVB , int MAX_WIDTH , int width , DXHANDLE* dxh )
{
	ROBJ* rb = dxh->robj;
	float cx = (float)( (int)rb[0].tx / width ) * (float)width;
	float cz = (float)( (int)rb[0].tz / width ) * (float)width;

	
	// Setting Grid vertex into VBuffer
	LINEVERTEX* p_grid;
	if( FAILED( g_pVB->Lock( 0, 0, (BYTE**)&p_grid, 0 ) ) )
		return E_FAIL;
		
		float grid_Y = -0.0000001f;
		for( int f=0; f<(MAX_WIDTH/width+1) ; f++ )
		{
			p_grid[f*2].position		= D3DXVECTOR3( cx + (-1.0f*(float)MAX_WIDTH/2)+(f*width), grid_Y, cz-1.0f*(float)MAX_WIDTH/2 );
			p_grid[f*2].diffuseColor	= dxh->grid_color;
			
			p_grid[f*2+1].position		= D3DXVECTOR3( cx + (-1.0f*(float)MAX_WIDTH/2)+(f*width), grid_Y, cz+(float)MAX_WIDTH/2 );
			p_grid[f*2+1].diffuseColor	= dxh->grid_color;
			
		}

		for( int f=0; f<(MAX_WIDTH/width+1) ; f++ )
		{
			p_grid[((MAX_WIDTH/width+1)*2)+f*2].position  = 
				D3DXVECTOR3( cx-1.0f*(float)MAX_WIDTH/2, grid_Y, cz + (-1.0f*(float)MAX_WIDTH/2)+(f*width) );
			p_grid[((MAX_WIDTH/width+1)*2)+f*2].diffuseColor = dxh->grid_color;
			
			p_grid[((MAX_WIDTH/width+1)*2)+f*2+1].position = 
				D3DXVECTOR3( cx+(float)MAX_WIDTH/2, grid_Y, cz + (-1.0f*(float)MAX_WIDTH/2)+(f*width) );
			p_grid[((MAX_WIDTH/width+1)*2)+f*2+1].diffuseColor = dxh->grid_color;
			
		}

	if( FAILED( g_pVB->Unlock() ) )
		return E_FAIL; 

	
	
	return S_OK;
}







///////////////////// UnUsed //////////////////////////////////////////////////////////////////////////////////


#ifdef OLD_MESH


HRESULT SetWingData( LPDIRECT3DVERTEXBUFFER8 g_pVB , DXHANDLE* dxh )
{
	WOBJ* wobj = dxh->wobj;


	CUSTOMVERTEX* pVertices = NULL;
		
		if( FAILED( g_pVB->Lock( 0, 0, (BYTE**)&pVertices, 0 ) ) )	return E_FAIL;
		

			for( int f=0; f<wobj->num_tri ; f++ )
			{
			
				VEC3 norm = wobj->render_wtr[f].surfnorm;

				for(int w = 0; w < 3; w++)
				{		
					pVertices[f*3+w].position = D3DXVECTOR3(	(float)wobj->render_wtr[f].vtx[w].pos.x , 
																(float)wobj->render_wtr[f].vtx[w].pos.y , 
																(float)wobj->render_wtr[f].vtx[w].pos.z		);


					pVertices[f*3+w].n = D3DXVECTOR3( (float)norm.x , (float)norm.y , (float)norm.z );


					pVertices[f*3+w].tu = wobj->render_wtr[f].vtx[w].u;
					pVertices[f*3+w].tv = wobj->render_wtr[f].vtx[w].v;


					pVertices[f*3+w].diffuseColor = XCOL_WHITE;

				}
		
			}

		
		if( FAILED( g_pVB->Unlock() ) )		return E_FAIL; 		
			

	return S_OK;

}



// Wing Shape
static float texMargin = 0.07f;
static float texMargin2 = 0.0f;
HRESULT SetWingData( LPDIRECT3DVERTEXBUFFER8 g_pVB , DXHANDLE* dxh )
{
	WOBJ* wobj = dxh->wobj;

//	VEC3 lookAt;
//	lookAt.Set( GetDXHANDLE()->cam->at.x , GetDXHANDLE()->cam->at.y , GetDXHANDLE()->cam->at.z );


	CUSTOMVERTEX* pVertices = NULL;
	
	
		if( FAILED( g_pVB->Lock( 0, 0, (BYTE**)&pVertices, 0 ) ) )
			return E_FAIL;
		
		for( int f=0; f<wobj->num_tri ; f++ )
		{
		//	VEC3 norm = (wobj->render_wtr[f].vtx[1].pos - wobj->render_wtr[f].vtx[0].pos) ^ 
		//				(wobj->render_wtr[f].vtx[2].pos - wobj->render_wtr[f].vtx[0].pos);
		//	norm.Normalize();
		

		//	wobj->render_wtr[f].Calc_Surface_Norm( );
		//	VEC3 norm = wobj->render_wtr[f].surfnorm;

			// No-Culling --> only for thin layer wing patagium
		//	if( lookAt*norm > 0.0 )		norm.Negate();

			
			VEC3 norm = wobj->render_wtr[f].surfnorm;

			for(int w = 0; w < 3; w++)
			{		
				pVertices[f*3+w].position = 
				D3DXVECTOR3( (float)wobj->render_wtr[f].vtx[w].pos.x , 
							 (float)wobj->render_wtr[f].vtx[w].pos.y , 
							 (float)wobj->render_wtr[f].vtx[w].pos.z  );


				pVertices[f*3+w].n = D3DXVECTOR3( (float)norm.x , (float)norm.y , (float)norm.z );


				pVertices[f*3+w].tu = wobj->render_wtr[f].vtx[w].u;
				pVertices[f*3+w].tv = wobj->render_wtr[f].vtx[w].v;


				pVertices[f*3+w].diffuseColor = XCOL_WHITE;
			}

		/*	pVertices[f*3  ].tu = texMargin;
			pVertices[f*3  ].tv = 0.0f;
			
			pVertices[f*3+1].tu = 1.0f;
			pVertices[f*3+1].tv = 0.0f;

			pVertices[f*3+2].tu = 1.0f;
			pVertices[f*3+2].tv = 1.0f-texMargin;
		*/


		}


		// texture for tail separately
	/*	for( int f=wobj->num_tri-4; f<wobj->num_tri ; f++ )
		{			
			pVertices[f*3  ].tu = texMargin2;
			pVertices[f*3  ].tv = 0.0f;
			
			pVertices[f*3+1].tu = 1.0f;
			pVertices[f*3+1].tv = 0.0f;

			pVertices[f*3+2].tu = 1.0f;
			pVertices[f*3+2].tv = 1.0f-texMargin2;
		}
*/
		// dummy color must be set
	/*	for( int f=0; f<wobj->num_tri ; f++ )
		{
			pVertices[f*3  ].diffuseColor = 
			pVertices[f*3+1].diffuseColor = 
			pVertices[f*3+2].diffuseColor = XCOL_WHITE;
		}
	*/
		
		if( FAILED( g_pVB->Unlock() ) )
			return E_FAIL; 		

	

	return S_OK;

}

#endif