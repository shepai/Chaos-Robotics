//-----------------------------------------------------------------------------
// File: DXGraphics.cpp
//
// Coded By Jeff Shim 2002.01.22.TUE.
//-----------------------------------------------------------------------------



#include <d3dx8.h>

#include <mmsystem.h>
#include "../resource.h"


#include "fonts.h"
//#include "Move.h"
#include "meshes.h"
//#include "..\Status.h"

#include "dxwnd.h"
#include "robj.h"





HRESULT		hr;											// Globally used HRESULT variable


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

LPDIRECT3D8					g_pD3D			= NULL;		// Used to create the D3DDevice
LPDIRECT3DDEVICE8			g_pd3dDevice	= NULL;		// Our rendering device

LPDIRECT3DDEVICE8	GetDXDevice( void ){	return g_pd3dDevice;	}

LPDIRECT3DVERTEXBUFFER8		g_pVB_grid		= NULL;		// Buffer to hold grid vertices
LPDIRECT3DVERTEXBUFFER8		g_pVB_floor		= NULL;		// Buffer to hold floor vertices

LPDIRECT3DVERTEXBUFFER8*	g_pVB			= NULL;		// All Cylinders

LPDIRECT3DVERTEXBUFFER8*	g_pSVB			= NULL;		// Cylinder Shadow 


LPDIRECT3DTEXTURE8*			g_pTexture		= NULL;		// Cylinder Tex
LPDIRECT3DTEXTURE8			g_pWTexture		= NULL;		// Wing Tex


ID3DXFont *g_DXfont = NULL;		// Fonts
ID3DXFont* GetID3DFont( void ){		return g_DXfont;	}



// Shadow Colour
DWORD shadow_color = D3DCOLOR_XRGB(20,20,20);


// Grids -- this is set elsewhere !!
int			MAX_WIDTH	= 1000; //10000;						// Overall Grid Width
int			width		=   20; //200;							// Width per line

void SetDXGridScale( int size, int wd )
{
	MAX_WIDTH = size;
	width = wd;
}


// to target
DWORD		targ_color1	= D3DCOLOR_XRGB(255,255,0);
DWORD		targ_color2	= D3DCOLOR_XRGB(255,0,0);

// Floor
int			NUM_FLOOR = 20;								// half width number of floor tiles
float		TILE_WID  = 1000.0f;



HRESULT Set_Init_SIM_Data_with_Shadow( LPDIRECT3DVERTEXBUFFER8, SIM* , bool );


HRESULT SetGridData( LPDIRECT3DVERTEXBUFFER8 , int , int , DXHANDLE* );
HRESULT SetFloorData( LPDIRECT3DVERTEXBUFFER8, int , float );


HRESULT Set_Realtime_Triangle_Data( LPDIRECT3DVERTEXBUFFER8 g_pVB , TRISURFACE* trList , int num_triangle );
HRESULT Set_RT_Shadow_Data( LPDIRECT3DVERTEXBUFFER8 g_pVB , TRISURFACE* trList , int num_triangle );

HRESULT Set_Voxel_Line_Data( LPDIRECT3DVERTEXBUFFER8 g_pVB , DXVTX* vList , int num_line );

HRESULT SetTargetData( LPDIRECT3DVERTEXBUFFER8 , DWORD , DWORD , DXHANDLE* );
HRESULT SetWindArrowData( LPDIRECT3DVERTEXBUFFER8 );
HRESULT SetTargArrowData( LPDIRECT3DVERTEXBUFFER8 ,  DXHANDLE* );

HRESULT SetWingTipDataHistoric( LPDIRECT3DVERTEXBUFFER8 , int , DXHANDLE* );

HRESULT SetSpringLineData( LPDIRECT3DVERTEXBUFFER8 , DWORD , DXHANDLE* );
HRESULT SetSpringLineShadowData( LPDIRECT3DVERTEXBUFFER8 , DWORD , DXHANDLE* );



ID3DXFont* InitFonts( LPDIRECT3DDEVICE8 g_pd3dDevice );
void D3DTexts( ID3DXFont *g_DXfont , DXHANDLE* );

void SetupWorldMatrices( LPDIRECT3DDEVICE8, ROBJ*, int );
void SetupWorldMatricesShadow( LPDIRECT3DDEVICE8, ROBJ*, int );
void SetupWorldMatricesHistoric( LPDIRECT3DDEVICE8, ROBJ*, int, int );

//void SetupViewingMatrices( LPDIRECT3DDEVICE8, CAMERA*, int, int, bool );
void SetupViewingMatrices( LPDIRECT3DDEVICE8, DXHANDLE* );

VOID Set_Directional_Lights( LPDIRECT3DDEVICE8 , D3DXVECTOR3 );





//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
//HRESULT InitD3D( HWND hWnd , int dxWidth , int dxHeight )
HRESULT InitD3D( DXHANDLE* dxh )
{

    // Create the D3D object.
    if( NULL == ( g_pD3D = Direct3DCreate8( D3D_SDK_VERSION ) ) )	return E_FAIL;

    // Get the current desktop display mode, so we can set up a back buffer of the same format
    D3DDISPLAYMODE d3ddm;
    if( FAILED( g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )		return E_FAIL;

    // Set up the structure used to create the D3DDevice. Since we are now
    // using more complex geometry, we will create a device with a zbuffer.
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );    
	
	// Associate with a render window
	d3dpp.Windowed					= TRUE;
	d3dpp.hDeviceWindow				= dxh->hWnd;
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferCount			= 1;
	d3dpp.BackBufferFormat			= d3ddm.Format;
	d3dpp.BackBufferWidth			= dxh->dxWidth;
	d3dpp.BackBufferHeight			= dxh->dxHeight;
	d3dpp.EnableAutoDepthStencil	= TRUE;

	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;

//	d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleType			= D3DMULTISAMPLE_8_SAMPLES;
	
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;



    // Create the D3DDevice
	if( FAILED( g_pD3D->CreateDevice(	D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, dxh->hWnd,
										D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE ,
										&d3dpp, 
										&g_pd3dDevice ) ) )
    {	return E_FAIL;    }


	
    // Culling
	g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	
    // Turn on the zbuffer
    g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE  );
	g_pd3dDevice->SetRenderState(D3DRS_ZBIAS, 1);
	

	

//	g_pd3dDevice->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, TRUE );		// Full Scene	Anti Aliasing
	g_pd3dDevice->SetRenderState( D3DRS_EDGEANTIALIAS, TRUE );		// Edge			Anti Aliasing
	
	
//	g_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );				// Dithering & Specular
//	g_pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, TRUE);
//	g_pd3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);


	// Enable Point Render
	float fPointSize = 1.0f ,fPointScaleB = 1.0f;
	g_pd3dDevice->SetRenderState(D3DRS_POINTSPRITEENABLE,true);
	g_pd3dDevice->SetRenderState(D3DRS_POINTSCALEENABLE,true);
	g_pd3dDevice->SetRenderState(D3DRS_POINTSIZE,*((DWORD*)&fPointSize));
	g_pd3dDevice->SetRenderState(D3DRS_POINTSCALE_B,*((DWORD*)&fPointScaleB));
	

	// Texture State
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );		
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
	

	
	// Initialize Fonts System
	g_DXfont = InitFonts( g_pd3dDevice );
	if( g_DXfont == NULL )	return E_FAIL;	  
	

	D3DXVECTOR3 lightDir = D3DXVECTOR3( 1.0f, -1.0f, 1.0f );
	Set_Directional_Lights( g_pd3dDevice , lightDir );

	
    return S_OK;
}







D3DXMATRIX g_mat_Identity;

//-----------------------------------------------------------------------------
// Name: InitGeometry()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT InitGeometry( DXHANDLE* dxh )
{	
	char temp[80];
  	
//	NUM_BODY	= dxh->NUM_OBJ;
//	robj		= GetROBJ();
//	wobj		= GetWOBJ();
	
	// Texture Creation	
	g_pVB	= new LPDIRECT3DVERTEXBUFFER8[dxh->NUM_OBJ];
	g_pSVB	= new LPDIRECT3DVERTEXBUFFER8[dxh->NUM_OBJ-2];
	
	g_pTexture	= new LPDIRECT3DTEXTURE8[dxh->NUM_OBJ];
		
	for( int t=0 ; t<dxh->NUM_OBJ ; t++ )
		g_pTexture[t] = NULL;
	
	
	// Go through all the materials
	for(int i = 0; i < dxh->NUM_OBJ ; i++)
	{
		// Check to see if there is a file name to load in this material
		if(strlen(dxh->robj[i].sim->material[0].texname) > 0)
		{			
			ZeroMemory( temp , sizeof (char) * 80 );
			strcat( temp, "../media/model/" );
			strcat( temp, dxh->robj[i].sim->material[0].texname );
			if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, temp , &g_pTexture[i] ) ) )
				return E_FAIL;
		}		
	}


	
	// Obtain Vertex Normals
	if( GetDXHANDLE()->isGoraud )
	{
		g_pd3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		for( int i=0 ; i<dxh->NUM_OBJ ; i++ )	get_v_normal( dxh->robj[i].sim );
	}
	
	
	
    // Create the vertex buffer for a file's meshes		
	for( int i=0 ; i<dxh->NUM_OBJ ; i++ )
	{		
		if( FAILED( g_pd3dDevice->CreateVertexBuffer( 3*dxh->robj[i].sim->objMesh[0].num_face*sizeof(CUSTOMVERTEX),
													  0, 
													  D3DFVF_CUSTOMVERTEX,
													  D3DPOOL_DEFAULT, &g_pVB[i] ) ) )	return E_FAIL;		

		if( FAILED( Set_Init_SIM_Data_with_Shadow( g_pVB[i], dxh->robj[i].sim , false ) ) )		return E_FAIL;
		
	}

	// SIM Obj Shadow
	for( int i=0 ; i<dxh->NUM_OBJ-2 ; i++ )
	{
		// Shadow
		if( FAILED( g_pd3dDevice->CreateVertexBuffer( 3*dxh->robj[i].sim->objMesh[0].num_face*sizeof(CUSTOMVERTEX),
												  0, 
												  D3DFVF_CUSTOMVERTEX,
												  D3DPOOL_DEFAULT, &g_pSVB[i] ) ) )		return E_FAIL;

		if( FAILED( Set_Init_SIM_Data_with_Shadow( g_pSVB[i], dxh->robj[i].sim , true ) ) )		return E_FAIL;
	}



	
	// Create VB for grids
	if( FAILED( g_pd3dDevice->CreateVertexBuffer( ((MAX_WIDTH/width + 1)*4) * sizeof(LINEVERTEX),
													  0, D3DFVF_LINEVERTEX,
													  D3DPOOL_DEFAULT, &g_pVB_grid ) ) )
		return E_FAIL;

	

	// Create VB for floor
	if( FAILED( g_pd3dDevice->CreateVertexBuffer( 24 * NUM_FLOOR * NUM_FLOOR * sizeof(CUSTOMVERTEX),
													  0, D3DFVF_CUSTOMVERTEX,
													  D3DPOOL_DEFAULT, &g_pVB_floor ) ) )
		return E_FAIL;
	
	// Create Floor Buffer and Set Data
	if( FAILED( SetFloorData( g_pVB_floor , NUM_FLOOR , TILE_WID ) ) )
		return E_FAIL;



	





	// Render State Fixed
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, 11 );
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, 10 );


	D3DXMatrixIdentity( &g_mat_Identity );

	return S_OK;
}



//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------

VOID CleanupGeometry( DXHANDLE* dxh )
{
	for(int mat=0 ; mat<dxh->NUM_OBJ ; mat++)
	{		
		if( g_pTexture[mat] != NULL )
			g_pTexture[mat]->Release();
	}
	
	delete [] g_pTexture;
	
	
	// Release vertex
	for( int i=0 ; i<dxh->NUM_OBJ   ; i++ )		if( g_pVB[i] != NULL )	g_pVB[i]->Release();
	for( int i=0 ; i<dxh->NUM_OBJ-2 ; i++ )		if( g_pSVB[i] != NULL )	g_pSVB[i]->Release();
	
	delete [] g_pVB;
	delete [] g_pSVB;

	g_pVB_grid->Release();
	g_pVB_floor->Release();	
		
	
	
}


VOID CleanupGraphics( void )
{
	if( g_pd3dDevice != NULL )	g_pd3dDevice->Release();

    if( g_pD3D != NULL )        g_pD3D->Release();

	if(g_DXfont)
	{
		g_DXfont->Release();
		g_DXfont=NULL;
	}

}




//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------

VOID Render( DXHANDLE* dxh )
{	
	int i;


	// Present Scene and time measuring	
	if( dxh->flipCount == 0 )	dxh->startTime = timeGetTime();

	
	// Clear the backbuffer and the zbuffer
	g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, dxh->bg_color, 1.0f, 0 );

    // Begin the scene
    hr = g_pd3dDevice->BeginScene();


	SetupViewingMatrices( g_pd3dDevice , dxh );






	g_pd3dDevice->SetTransform( D3DTS_WORLD, &g_mat_Identity );

	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	if( !dxh->isBackGround )
	{
		SetGridData( g_pVB_grid , MAX_WIDTH , width , dxh );
		g_pd3dDevice->SetVertexShader( D3DFVF_LINEVERTEX );
		g_pd3dDevice->SetStreamSource( 0, g_pVB_grid, sizeof LINEVERTEX  );
		g_pd3dDevice->SetTexture( 0, NULL );
		g_pd3dDevice->DrawPrimitive( D3DPT_LINELIST, 0, (MAX_WIDTH/width+1)*2 );		
	}
	else
	{
		int gnd_Idx = dxh->NUM_OBJ-1;
		int sky_Idx = dxh->NUM_OBJ-2;

		// Ground Texture
		g_pd3dDevice->SetVertexShader( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->SetStreamSource( 0, g_pVB_floor, sizeof CUSTOMVERTEX  );
		g_pd3dDevice->SetTexture( 0, g_pTexture[gnd_Idx] );
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, NUM_FLOOR*NUM_FLOOR*8 );

		// sky location follows object
		dxh->robj[sky_Idx].ty = 0.0f;
		dxh->robj[sky_Idx].tx = dxh->robj[0].tx;
		dxh->robj[sky_Idx].tz = dxh->robj[0].tz;
		SetupWorldMatrices(g_pd3dDevice, dxh->robj, sky_Idx);
		g_pd3dDevice->SetTexture( 0, g_pTexture[sky_Idx] );
		g_pd3dDevice->SetStreamSource( 0, g_pVB[sky_Idx], sizeof CUSTOMVERTEX  );
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dxh->robj[sky_Idx].sim->objMesh[0].num_face );
	}



	

	
	// Cull ON & Light ON ///////////////////////////////////////////////////////////////////////////////////
	g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	
	
	
	g_pd3dDevice->SetVertexShader( D3DFVF_CUSTOMVERTEX );


	// sim object
	for( i=0 ; i<dxh->NUM_OBJ-2 ; i++ )
	{
		// render objects
		SetupWorldMatrices(g_pd3dDevice, dxh->robj, i);
		g_pd3dDevice->SetTexture( 0, g_pTexture[i] );
		g_pd3dDevice->SetStreamSource( 0, g_pVB[i], sizeof CUSTOMVERTEX  );
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dxh->robj[i].sim->objMesh[0].num_face );

		// render snapshots
		if( dxh->isSnapShot )
		{
			for( int h=0 ; h<dxh->SS_COUNT ; h++ )
			{
				SetupWorldMatricesHistoric(g_pd3dDevice, dxh->robj, i, h);
				g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dxh->robj[i].sim->objMesh[0].num_face );
			}				
		}
	}


	// sim Shadow
	if( dxh->is_bg_color )	// This should be tuned for robots
	{
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE );
	
		g_pd3dDevice->SetTexture( 0, NULL );
		for( i=0 ; i<dxh->NUM_OBJ-2 ; i++ )
		{		
			SetupWorldMatricesShadow(g_pd3dDevice, dxh->robj, i);		
			g_pd3dDevice->SetStreamSource( 0, g_pSVB[i], sizeof CUSTOMVERTEX  );
			g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dxh->robj[i].sim->objMesh[0].num_face );
		}	
		
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE );

	}



	/////// Identity World ///////////////////////////
	g_pd3dDevice->SetTransform( D3DTS_WORLD, &g_mat_Identity );






	////////////////////////// Here comes Voxel Rendering --> ON Cull & ON Light ///////////////////////////////////////

	//// TO DO ////////


	
	// OFF Cull but Light ON /////////////////////////////////////////////////////////////////////////////////////
	g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );




	// No Cull & No Light //////////////////////////////////////////////////////////////////////////////////////////
	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	

	// No Texture used from here...
	g_pd3dDevice->SetTexture( 0, NULL );


	

	


	// Render text
	if( dxh->isDXText )	D3DTexts( g_DXfont , dxh );

    // End the scene
    g_pd3dDevice->EndScene();
	
		
    
	
    // Present the backbuffer contents to the display
/**/	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );	/////////////////////////////



	dxh->flipCount++;
	dxh->endTime = timeGetTime();
		
	if( dxh->endTime - dxh->startTime >= 1000 )
	{
		dxh->startTime = dxh->endTime;
		dxh->frameRate = dxh->flipCount;
		dxh->flipCount = 0;
	}
	
}


