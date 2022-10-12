


#include "input.h"
#include "../Status.h"

LPDIRECTINPUT8				g_pDI = NULL;							// Direct Input ( Add )
LPDIRECTINPUTDEVICE8		g_KDIDev;								// Keyboard Device ( Add )


LPDIRECTINPUTDEVICE8 GetDIDevice( void )
{
	return g_KDIDev;
}


HRESULT InitDInput( HWND hWnd )
{

	HRESULT hr;

    // Create Direct Input
    hr = DirectInput8Create(g_Status->g_hinstance, DIRECTINPUT_VERSION, 
							IID_IDirectInput8, (void**)&g_pDI, NULL); 
	
	if FAILED(hr) 	return E_FAIL; 


    // Create The Keyboard Device
    hr = g_pDI->CreateDevice(GUID_SysKeyboard, &g_KDIDev, NULL); 
	
	if FAILED(hr) 
	{ 
		DI_Term(); 
		return E_FAIL; 
	} 

	// Set Data Format
    hr = g_KDIDev->SetDataFormat(&c_dfDIKeyboard); 

	if FAILED(hr) 
	{ 
		DI_Term(); 
		return E_FAIL; 
	} 


	hr = g_KDIDev->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); 
	
	if FAILED(hr) 
	{ 
		DI_Term(); 
		return E_FAIL; 
	} 

		
	hr = GetDIDevice()->Acquire();
    // Try until acquired
	while( hr == DIERR_INPUTLOST )	hr = GetDIDevice()->Acquire();

        

    return S_OK;
}






//-----------------------------------------------------------------------------
// Name: InitDInput(), DI_Term()
// Desc: Initializes and Terminate DirectInput
//-----------------------------------------------------------------------------
void DI_Term( void )
{
    if (g_pDI)
    {
        if (g_KDIDev)
        {
            g_KDIDev->Unacquire();
            g_KDIDev->Release();
            g_KDIDev = NULL;
        }

        g_pDI->Release();
        g_pDI = NULL;
    }
}

