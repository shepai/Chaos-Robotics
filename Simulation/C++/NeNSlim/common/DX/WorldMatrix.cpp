
#include <windows.h>

#include <d3dx8.h>
#include "robj.h"


void SetupWorldMatrices( LPDIRECT3DDEVICE8 g_pd3dDevice, ROBJ* robj, int i )
{
	D3DXMATRIX matWorld, matTrans;
	D3DXMatrixIdentity( &matWorld );
	D3DXMatrixIdentity( &matTrans );	
	
	D3DXMatrixTranslation( &matTrans, robj[i].tx, robj[i].ty, robj[i].tz );
	D3DXMatrixMultiply( &matWorld, &robj[i].matRot, &matTrans );
		
    g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );	
}


float Shadow_Height = 0.0001f;

void SetupWorldMatricesShadow( LPDIRECT3DDEVICE8 g_pd3dDevice, ROBJ* robj, int i )
{
	D3DXMATRIX matWorld, matTrans, matShadow;
	D3DXMatrixIdentity( &matWorld );
	D3DXMatrixIdentity( &matShadow );
	D3DXMatrixIdentity( &matTrans );
	
	matShadow._21 = 1.0f;
	matShadow._22 = 0.0f;
	matShadow._23 = 1.0f;
	matShadow._42 = Shadow_Height;
	
	D3DXMatrixTranslation( &matTrans, robj[i].tx, robj[i].ty, robj[i].tz );
	D3DXMatrixMultiply( &matWorld, &robj[i].matRot, &matTrans );	
	D3DXMatrixMultiply( &matWorld, &matWorld, &matShadow );	
	// 18 얘네들은 행벡터로 해서 벡터 X 행렬이였네... 그래서 거꾸로...

    g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );	
}


void SetupWorldMatricesHistoric( LPDIRECT3DDEVICE8 g_pd3dDevice, ROBJ* robj, int i, int count )
{
	D3DXMATRIX matWorld, matTrans;
	D3DXMatrixIdentity( &matWorld );
	D3DXMatrixIdentity( &matTrans );	
	
//	D3DXMatrixTranslation( &matTrans, robj[i].prev_tx[count], robj[i].prev_ty[count], robj[i].prev_tz[count] );

	D3DXMatrixTranslation( &matTrans, robj[i].prev_pos[count].x, 
									  robj[i].prev_pos[count].y, 
									  robj[i].prev_pos[count].z );
	D3DXMatrixMultiply( &matWorld, &robj[i].prev_matRot[count], &matTrans );
	
    g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
	
}






/////// UnUsed !!!! //////////////////////////////////////////////////////////////////////////////////////////

// Currently UnUsed
void SetupWorldMatricesShadowHistoric( LPDIRECT3DDEVICE8 g_pd3dDevice, ROBJ* robj, int i , int c )
{
	D3DXMATRIX matWorld, matTrans, matShadow;
	D3DXMatrixIdentity( &matWorld );
	D3DXMatrixIdentity( &matShadow );
	D3DXMatrixIdentity( &matTrans );
	
	matShadow._21 = 1.0f;
	matShadow._22 = 0.0f;
	matShadow._23 = 1.0f;
	matShadow._42 = Shadow_Height;

	D3DXMatrixTranslation( &matTrans, robj[i].prev_pos[c].x, 
									  robj[i].prev_pos[c].y, 
									  robj[i].prev_pos[c].z );
	D3DXMatrixMultiply( &matWorld, &robj[i].prev_matRot[c], &matTrans );
		
	D3DXMatrixMultiply( &matWorld, &matWorld, &matShadow );	
	// 18 얘네들은 행벡터로 해서 벡터 X 행렬이였네... 그래서 거꾸로...


	
    g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
	
}






