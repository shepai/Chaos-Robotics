



#ifndef INPUT_H
#define INPUT_H

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>


void DI_Term(void);
HRESULT InitDInput( HWND );
LPDIRECTINPUTDEVICE8 GetDIDevice( void );


#endif