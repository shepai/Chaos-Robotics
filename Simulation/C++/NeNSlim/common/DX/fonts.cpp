


#include <stdio.h>

#include "fonts.h"
#include "../Status.h"

#include "dxwnd.h"
#include "..\..\common\dynamics\rigidode.h"




ID3DXFont* InitFonts( LPDIRECT3DDEVICE8 g_pd3dDevice )
{

	ID3DXFont *g_DXfont;

	HRESULT hr;

	LOGFONT log_font={ 16,								// height
						0,								// width; 
						0,								// lfEscapement; 
						0,								// lfOrientation; 
						FW_BOLD,						// lfWeight; 
						FALSE,							// lfItalic; 
						FALSE,							// lfUnderline; 
						FALSE,							// lfStrikeOut; 
						DEFAULT_CHARSET,				// lfCharSet; 
						OUT_DEFAULT_PRECIS,				// lfOutPrecision; 
						CLIP_DEFAULT_PRECIS,			// lfClipPrecision; 
						ANTIALIASED_QUALITY,			// lfQuality; 
						DEFAULT_PITCH,					// lfPitchAndFamily; 
						"Arial"							// lfFaceName[LF_FACESIZE]; 
					  };

	
	hr = D3DXCreateFontIndirect( g_pd3dDevice ,&log_font, &g_DXfont );
	
	
	if(FAILED(hr))
		return NULL;	  
	
	return g_DXfont;

}



RECT	font_rect={0,0,9000,9000};
char	buffer[500];
char*	blendSrcBuff;
char*	blendDestBuff;


int		leftside	= 10;
int		middleside	= 250;
int		rightside	= 420;





void D3DTexts( ID3DXFont *g_DXfont , DXHANDLE* dxh )
{
	RIGIDSTATE* l_rs = g_sim[0]->rs;

	DWORD color;
	//color = D3DCOLOR_XRGB(255,158,0);
	color = D3DCOLOR_XRGB(255,255,255);
		
	g_DXfont->Begin();

		
		font_rect.top=10;
		font_rect.left=middleside;
		if( dxh->lockcam )	sprintf( buffer , "Cam: LOCK" );
		else				sprintf( buffer , "Cam: FLY" );
		g_DXfont->DrawText( buffer ,-1,&font_rect,DT_LEFT,color);

		font_rect.top=30;
		font_rect.left=middleside;
		sprintf(buffer,"StepSize: %1.4f", l_rs->STEPSIZE );
		g_DXfont->DrawText( buffer ,-1,&font_rect,DT_LEFT,color);
		
		
		
		font_rect.top=10;
		font_rect.left=rightside;

		sprintf(buffer,"Environment: Terrestrial" );

		
		g_DXfont->DrawText( buffer ,-1,&font_rect,DT_LEFT,color);
		
		font_rect.top=30;
		font_rect.left=rightside;
		sprintf(buffer,"Step: %d", g_Status->SimCount );
		g_DXfont->DrawText( buffer ,-1,&font_rect,DT_LEFT,color);

		font_rect.top=50;
		font_rect.left=rightside;
		sprintf(buffer,"Time: %4.3f sec", l_rs->robotStep );
		g_DXfont->DrawText( buffer ,-1,&font_rect,DT_LEFT,color);
	



		font_rect.top=10;
		font_rect.left=leftside;
		sprintf(buffer,"GetTimes: %d / %d", dxh->startTime , dxh->endTime );
		g_DXfont->DrawText( buffer ,-1,&font_rect,DT_LEFT,color);
		
	/*	font_rect.top=30;
		font_rect.left=leftside;		
		sprintf(buffer,"End Time : %d", dxh->endTime );
		g_DXfont->DrawText( buffer ,-1,&font_rect,DT_LEFT,color);
	*/

		font_rect.top=30;
		font_rect.left=leftside;
		sprintf(buffer,"Flip Count: %d", dxh->flipCount );		
		g_DXfont->DrawText( buffer ,-1,&font_rect,DT_LEFT,color);

		font_rect.top=50;
		font_rect.left=leftside;
		sprintf(buffer,"Frame Rate: %d fps", dxh->frameRate );
		g_DXfont->DrawText( buffer ,-1,&font_rect,DT_LEFT,color);


		
      
    g_DXfont->End();

	

}


