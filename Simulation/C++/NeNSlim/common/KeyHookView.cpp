
//-------------------------------------------------------------------------------------------------
// Name: KeyboardProc()
// Desc: Keyboard Hooking
//-------------------------------------------------------------------------------------------------





#include "Status.h"

#include "..\common\dynamics\rigidode.h"







HHOOK hhook;


void Zero_System( bool );
void RecomposeNN( char* );


LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	RIGIDSTATE* rs = g_sim[0]->rs;
	
	if( lParam & 0x80000000 && nCode>=0)
	{
		// General
		switch (wParam)
        {
			case VK_F1:
				LOG( -2 , "\n\n-------[F1 Pressed]----------------------------\n\n" );
				RecomposeNN( NULL );
			break;

			case VK_F2:		g_Status->goStep = !g_Status->goStep;		break;
			
			case VK_F3:
				LOG( -2 , "\n\n-------[F3 Pressed]----------------------------\n\n" );
				Zero_System( false );
			break;
			
			case VK_F4:		g_Status->oneStep = true;		            break;

			case VK_F5:		Zero_System( true );							break;
		}
	

		
		
		
	}
		
    return CallNextHookEx(hhook, nCode, wParam, lParam);
} 



void InitKeyHook( void )
{
	hhook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, NULL, GetCurrentThreadId());
}

void EndKeyHook( void )
{
	UnhookWindowsHookEx(hhook);
	hhook = NULL;
}