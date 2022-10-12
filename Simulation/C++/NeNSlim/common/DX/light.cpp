


#include <d3dx8.h>



void Set_Directional_Lights( LPDIRECT3DDEVICE8 g_pd3dDevice , D3DXVECTOR3 l_dir )
{       
	D3DMATERIAL8 mtrl;
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL8) );
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;

	mtrl.Specular.r = 1.0f;
    mtrl.Specular.g = 1.0f;
    mtrl.Specular.b = 1.0f;
    mtrl.Specular.a = 1.0f;
	mtrl.Power		= 50.0f;
    g_pd3dDevice->SetMaterial( &mtrl );
	

// LIGHT !!
	D3DLIGHT8 light;
	ZeroMemory(&light, sizeof(D3DLIGHT8));

	float gSpecular = 1.0f;

	// Directional Light
    D3DXVECTOR3 vecDir;
    light.Type       = D3DLIGHT_DIRECTIONAL;
    light.Diffuse.r  = 1.0f;
    light.Diffuse.g  = 1.0f;
    light.Diffuse.b  = 1.0f;
	light.Specular.r  = gSpecular;
    light.Specular.g  = gSpecular;
    light.Specular.b  = gSpecular;
	vecDir = l_dir;
    D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );

	
/*
	// Set up a white point light.
	light.Type = D3DLIGHT_POINT;
	light.Diffuse.r  = 1.0f;
	light.Diffuse.g  = 1.0f;
	light.Diffuse.b  = 1.0f;
	light.Ambient.r  = 1.0f;
	light.Ambient.g  = 1.0f;
	light.Ambient.b  = 1.0f;
	light.Specular.r = 1.0f;
	light.Specular.g = 1.0f;
	light.Specular.b = 1.0f;
	
	light.Position.x = -10.0f;
	light.Position.y = 100.0f;
	light.Position.z = -10.0f;

	light.Attenuation0 = 1.0f; 
	light.Attenuation1 = 0.4f; 
	light.Range        = 2000.0f;
*/


	// Set Light
	g_pd3dDevice->SetLight(0, &light);
	g_pd3dDevice->LightEnable( 0, TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );


    // Finally, turn on some ambient light.
	g_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0x00202020 );
	
}
