



#include <windows.h>

#include <d3dx8.h>


#include "camera.h"
#include "input.h"
#include "../Status.h"

#include "dxwnd.h"


float	Radian = float(3.141592/180.0);

BYTE	buffer[256];						// keyboard buffer



static VEC3 targDirUD;
static VEC3 targDirLR;







void CAMERA::UpdateKeyBoard( bool LC )		// LC --> lock cam
{
	HRESULT hr;
	
	if FAILED( GetDIDevice()->GetDeviceState(sizeof(buffer), &buffer) )
	{       
		// If it failed, the device has probably been lost. Try to acquire again.
        hr = GetDIDevice()->Acquire();
        
		// Try until acquired
		while( hr == DIERR_INPUTLOST )	hr = GetDIDevice()->Acquire();

        return;
	} 
	

	// Forward / Backward
	if		( buffer[DIK_INSERT] & 0x80 )	{	if( LC )	radius -= LowSpd;	else	Go(  LowSpd,  false );	}
	else if ( buffer[DIK_DELETE] & 0x80 )	{	if( LC )	radius += LowSpd;	else	Go( -LowSpd,  false );	}
	else if ( buffer[DIK_HOME] & 0x80 )		{	if( LC )	radius -= HighSpd;	else	Go(  HighSpd, false );	}
	else if ( buffer[DIK_END] & 0x80 )		{	if( LC )	radius += HighSpd;	else	Go( -HighSpd, false );	}
	else{}
	
	

	// Orientation
	if		( buffer[DIK_UP] & 0x80 )	{	pitch =  angleRate;		PITCH += angleRate;	}
	else if ( buffer[DIK_DOWN] & 0x80 )	{	pitch = -angleRate;		PITCH -= angleRate;	}
	else									pitch = 0.0f;

	if		( buffer[DIK_LEFT] & 0x80 )	{	yaw = -angleRate;	YAW -= angleRate;	}
	else if ( buffer[DIK_RIGHT] & 0x80 ){	yaw =  angleRate;	YAW += angleRate;	}
	else									yaw = 0.0f;



}




VEC3 Get_Targ_Motion( void )
{
	VEC3 tdir = targDirUD + targDirLR;
	tdir.Normalize();

	return tdir;
}



VEC3 GetCenterCAM( DXHANDLE* );

void SetupViewingMatrices( LPDIRECT3DDEVICE8 g_pd3dDevice , DXHANDLE* dxh )
{   
	//dxh->cam , dxh->dxWidth , dxh->dxHeight , dxh->lockcam

	dxh->cam->Pitch( dxh->cam->pitch );
	dxh->cam->Yaw( dxh->cam->yaw );
	dxh->cam->Go( 0.0f, !dxh->lockcam );
	
	D3DXMATRIX matView;

	// Camera Works !!
	if( dxh->lockcam )
	{
		VEC3 At, From, Up;
		VEC3 camAt, camFrom, camDir;
				
		At		= GetCenterCAM( dxh );
		From	= At;
		Up.Set( 0.0f, 1.0f, 0.0f );

		At = GetCenterCAM( dxh );
		Up.Set( dxh->cam->up.x , dxh->cam->up.y , dxh->cam->up.z );
		camFrom.Set( dxh->cam->from.x , dxh->cam->from.y , dxh->cam->from.z );
		camAt.Set( dxh->cam->At.x , dxh->cam->At.y , dxh->cam->At.z );
		camDir = camFrom - camAt;
		camDir.Normalize();
		From = At + camDir * dxh->cam->radius;

		
		D3DXMatrixLookAtLH( &matView, &D3DXVECTOR3( (float)From.x , (float)From.y , (float)From.z ),
									  &D3DXVECTOR3( (float)At.x ,   (float)At.y ,   (float)At.z ),
									  &D3DXVECTOR3( (float)Up.x ,   (float)Up.y ,   (float)Up.z ) );			

		dxh->cam->from.x = From.x;
		dxh->cam->from.y = From.y;
		dxh->cam->from.z = From.z;
			
	}
	else
	{	
		
		D3DXMatrixLookAtLH( &matView, &D3DXVECTOR3( (float)dxh->cam->from.x , (float)dxh->cam->from.y , (float)dxh->cam->from.z ),
									  &D3DXVECTOR3( (float)dxh->cam->At.x ,   (float)dxh->cam->At.y ,   (float)dxh->cam->At.z ),
									  &D3DXVECTOR3( (float)dxh->cam->up.x ,   (float)dxh->cam->up.y ,   (float)dxh->cam->up.z ) );			
	}
    	
	
	g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

			
    D3DXMATRIX matProj;
	double aspect = (double)dxh->dxWidth / (double)dxh->dxHeight;

//	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, (float)aspect , 0.1f, 99999999.0f );
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, (float)aspect , 0.1f, 100000.0f );

    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	
}







