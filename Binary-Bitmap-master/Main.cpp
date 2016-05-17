// ============================================================================
//
// This file contains the actions, conditions and expressions your object uses
// 
// ============================================================================

#include "common.h"

#define PARAM_OPERATOR _T("Operator (+  -  *  /  **  %%  <  >  &  |  ^  <<  >>  =)")
#define PARAM_OPERATOR_OPTIONAL _T("Operator (Empty: None, +  -  *  /  **  %%  <  >  &  |  ^  <<  >>  =)")

// ============================================================================
//
// CONDITIONS
// 
// ============================================================================

// ============================================================================
//
// ACTIONS
// 
// ============================================================================

void __cdecl SurfSaveImage(LPRDATA rdPtr)
{
	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	CImageFilterMgr* pImgMgr = rhPtr->rh4.rh4Mv->mvImgFilterMgr;

	if(ExportImage(pImgMgr,rdPtr->ioFile,TargetImg,rdPtr->ioFilter))
	{
		if(rdPtr->ioHandle)
			rdPtr->rRd->PushEvent(5);
		else
			rdPtr->rRd->GenerateEvent(5);
	}
	else
	{
		if(rdPtr->ioHandle)
			rdPtr->rRd->PushEvent(3);
		else
			rdPtr->rRd->GenerateEvent(3);
	}
	//Free stuff again
	free(rdPtr->ioFile);
	rdPtr->ioFile = 0;
	rdPtr->ioHandle = 0;
}

void __cdecl SurfLoadImage(LPRDATA rdPtr)
{
	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	CImageFilterMgr* pImgMgr = rhPtr->rh4.rh4Mv->mvImgFilterMgr;

	cSurface* psf = new cSurface, *tmp = TargetImg;
	COLORREF transCol = tmp->GetTransparentColor();

	if(ImportImage(pImgMgr,rdPtr->ioFile,psf,0,0))
	{
		//Clone image
		if(tmp->IsValid())
			tmp->Delete();
		cSurface* proto = 0;
		GetSurfacePrototype(&proto,rdPtr->depth,ST_MEMORYWITHDC,SD_DIB);
		tmp->Create(psf->GetWidth(),psf->GetHeight(),proto);
		psf->Blit(*tmp,0,0,BMODE_OPAQUE,BOP_COPY,0,BLTF_COPYALPHA);
		tmp->SetTransparentColor(transCol);

		rdPtr->targetValid = true;
		if Current(rdPtr->targetId)
		{
			UpdateHotspot(rdPtr);
			ImageChanged();
		}
		
		//Success
		if(rdPtr->threadedIO)
			rdPtr->rRd->PushEvent(4);
		else
			rdPtr->rRd->GenerateEvent(4);
		
		if Current(rdPtr->targetId)
			ImageChanged();
		
	}
	//Load failed
	else
	{
		if(rdPtr->threadedIO)
			rdPtr->rRd->PushEvent(2);
		else
			rdPtr->rRd->GenerateEvent(2);
	}
	//Delete temp
	delete psf;
	free(rdPtr->ioFile);
	rdPtr->ioFile = 0;
	rdPtr->ioHandle = 0;
}

ACTION(
	/* ID */			0,
	/* Name */			_T("%o: Create alpha channel, keep transparency = %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Keep transparency? (0: No, 1: Yes)"))
) {
	TargetExists();
	COLORREF transp = TargetImg->GetTransparentColor();
	if(!TargetImg->HasAlpha())
	{
		TargetImg->CreateAlpha();
		//Keep transparency
		if(param1)
		{
			cSurface* alpha = TargetImg->GetAlphaSurface();
			for(int x=0;x<TargetImg->GetWidth();x++)
				for(int y=0;y<TargetImg->GetHeight();y++)
					if(TargetImg->GetPixelFast(x,y)!=transp)
						alpha->SetPixel(x,y,(COLORREF)0xffffffff);
			 TargetImg->ReleaseAlphaSurface(alpha);
		}
		else
		{
			if Current(rdPtr->targetId)
				ImageChanged();	
			
		}
	}
	else if(param1)
	{
		cSurface* alpha = TargetImg->GetAlphaSurface();
		for(int x=0;x<TargetImg->GetWidth();x++)
			for(int y=0;y<TargetImg->GetHeight();y++)
				if(TargetImg->GetPixelFast(x,y)!=transp)
					alpha->SetPixel(x,y,255);
		TargetImg->ReleaseAlphaSurface(alpha);
	}
	return 0;
}

ACTION(
	/* ID */			1,
	/* Name */			_T("%o: Force redraw"),
	/* Flags */			0,
	/* Params */		(0)
) {
	ImageChanged();
	
	return 0;
}

ACTION(
	/* ID */			2,
	/* Name */			_T("%o: Skip redraw"),
	/* Flags */			0,
	/* Params */		(0)
) {
	rdPtr->rc.rcChanged = false; 
	return 0;
}

ACTION(
	/* ID */			3,
	/* Name */			_T("%o: Set transparent to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Display transparent? (0: No, 1: Yes)"))
) {
	if(param1)
		rdPtr->rs.rsEffect |= EFFECTFLAG_TRANSPARENT;
	else
		rdPtr->rs.rsEffect &= ~EFFECTFLAG_TRANSPARENT;
	return 0;
}

ACTION(
	/* ID */			4,
	/* Name */			_T("Set scale to %0, quality = %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Scale value (1.0: Normal dimension)"),PARAM_NUMBER,_T("Quality (0: Speed, 1: Quality)"))
) {
	long p1temp = CNC_GetFloatParameter(rdPtr);
	float p1 = *(float*) &p1temp;
	int p2 = Param(TYPE_INT) != 0;
	rdPtr->rc.rcScaleX = p1;
	rdPtr->rc.rcScaleY = p1;
	rdPtr->rs.rsFlags = (rdPtr->rs.rsFlags & ~RSFLAG_SCALE_RESAMPLE) | (p2 ? RSFLAG_SCALE_RESAMPLE : 0);

	UpdateHotspot(rdPtr);
	ImageChanged();

	return 0;
}

ACTION(
	/* ID */			5,
	/* Name */			_T("%o: Set X scale to %0, quality = %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Scale value (1.0: Normal dimension)"),PARAM_NUMBER,_T("Quality (0: Speed, 1: Quality)"))
) {
	long p1temp = CNC_GetFloatParameter(rdPtr);
	float p1 = *(float*) &p1temp;
	int p2 = Param(TYPE_INT) != 0;
	rdPtr->rc.rcScaleX = p1;
	rdPtr->rs.rsFlags = (rdPtr->rs.rsFlags & ~RSFLAG_SCALE_RESAMPLE) | (p2 ? RSFLAG_SCALE_RESAMPLE : 0);
	
	UpdateHotspot(rdPtr);
	ImageChanged();
	
	return 0;
}

ACTION(
	/* ID */			6,
	/* Name */			_T("%o: Set Y scale to %0, quality = %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Scale value (1.0: Normal dimension)"),PARAM_NUMBER,_T("Quality (0: Speed, 1: Quality)"))
) {
	long p1temp = CNC_GetFloatParameter(rdPtr);
	float p1 = *(float*) &p1temp;
	int p2 = Param(TYPE_INT) != 0;

	rdPtr->rc.rcScaleY = p1;
	rdPtr->rs.rsFlags = (rdPtr->rs.rsFlags & ~RSFLAG_SCALE_RESAMPLE) | (p2 ? RSFLAG_SCALE_RESAMPLE : 0);

	UpdateHotspot(rdPtr);
	ImageChanged();

	return 0;

}

ACTION(
	/* ID */			7,
	/* Name */			_T("%o: Set transparent color to %0, replace old color = %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_COLOUR,_T("Transparent color"),PARAM_NUMBER,_T("Replace old color? (0: No, 1: Yes)"))
) {
	//Oh my, what a crappy workaround
	rdPtr->transCol = param1;
	TargetExists();
	rdPtr->transCol = 0;
	//Replace old color if wanted
	if(param2)
		TargetImg->ReplaceColor(param1,TargetImg->GetTransparentColor());
	TargetImg->SetTransparentColor(param1);

	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}

ACTION(
	/* ID */			8,
	/* Name */			_T("%o: Set hot spot to (%0,%1) px"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Hot spot X"),PARAM_NUMBER,_T("Hot spot Y"))
) {
	TargetExists();
	ImageS(rdPtr->targetId)->hotX = param1;
	ImageS(rdPtr->targetId)->hotY = param2;
	if Current(rdPtr->targetId)
	{
		UpdateHotspot(rdPtr);
		ImageChanged();
	}
	return 0;
}

ACTION(
	/* ID */			9,
	/* Name */			_T("%o: Set hot spot to (%0,%1) percent"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Hot spot X (Percent)"),PARAM_NUMBER,_T("Hot spot Y (Percent)"))
) {

	TargetExists();
	param1 = param1/100.0*TargetImg->GetWidth();
	param2 = param2/100.0*TargetImg->GetHeight();
	ImageS(rdPtr->targetId)->hotX = param1;
	ImageS(rdPtr->targetId)->hotY = param2;
	if Current(rdPtr->targetId)
	{
		UpdateHotspot(rdPtr);
		ImageChanged();
	}
	return 0;
}

ACTION(
	/* ID */			10,
	/* Name */			_T("%o: Load image from file %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_FILENAME2,_T("File"))
) {
	ImageInRange(rdPtr->targetId);
	TCHAR* file = GetStr();

	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	CImageFilterMgr* pImgMgr = rhPtr->rh4.rh4Mv->mvImgFilterMgr;

	if (rdPtr->ioFile)
		free(rdPtr->ioFile);
	rdPtr->ioFile = _tcsdup(file);

	if(rdPtr->threadedIO)
	{
		if (rdPtr->ioHandle)
		{
			rdPtr->rRd->GenerateEvent(2);
			return 0;
		}

		rdPtr->ioSave = false;
		rdPtr->ioHandle = rdPtr->rRd->StartThread(SurfLoadImage);
	}
	else
	{
		SurfLoadImage(rdPtr);
	}

	return 0;
}

ACTION(
	/* ID */			11,
	/* Name */			_T("%o: Save image to file %0, override extension = %1"),//, compression level = %2,
	/* Flags */			0,
	/* Params */		(2,PARAM_FILENAME,_T("File"),PARAM_STRING,_T("Override extension (Specify if path does not include an extension like \"png\""))//,PARAM_NUMBER,_T("Compression level (0-100, -1: Ignore)"))
) {
	TargetExists();
	TCHAR* file = GetStr();
	TCHAR* ext = GetStr();

	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	CImageFilterMgr* pImgMgr = rhPtr->rh4.rh4Mv->mvImgFilterMgr;
	DWORD filter = *ext?FindFilter(pImgMgr,ext,true):FindFilter(pImgMgr,file);
	//No filter
	if(!filter)
	{
		rdPtr->rRd->GenerateEvent(3);
		return 0;
	}

	if (rdPtr->ioFile)
		free(rdPtr->ioFile);
	rdPtr->ioFile = _tcsdup(file);
	rdPtr->ioFilter = filter;

	if(rdPtr->threadedIO)
	{
		if (rdPtr->ioHandle)
		{
			rdPtr->rRd->GenerateEvent(3);
			return 0;
		}

		rdPtr->ioSave = true;
		rdPtr->ioHandle = rdPtr->rRd->StartThread(SurfSaveImage);
	}
	else
	{
		SurfSaveImage(rdPtr);
	}

	return 0;
}

ACTION(
	/* ID */			12,
	/* Name */			_T("%o: Load image from the clipboard"),
	/* Flags */			0,
	/* Params */		(0)
) {
	ImageInRange(rdPtr->targetId);

	if(IsClipboardFormatAvailable(CF_DIB)&&OpenClipboard(NULL))
	{
		COLORREF transCol = TargetImg->GetTransparentColor();
		HANDLE handle = GetClipboardData(CF_DIB);
		BITMAPINFO* bmp = (BITMAPINFO*)GlobalLock(handle);
		//Load DIB
		cSurface* tmp = TargetImg;
		if(tmp->IsValid())
			tmp->Delete();
		cSurface* proto = 0;
		GetSurfacePrototype(&proto,rdPtr->depth,ST_MEMORYWITHDC,SD_DIB);
		tmp->Create(bmp->bmiHeader.biWidth,bmp->bmiHeader.biHeight,proto);
		if(tmp->IsValid()&&tmp->LoadImage(bmp,GetDIBBitmap(bmp)))
		{
			//Sucess! Update
			tmp->SetTransparentColor(transCol);
			rdPtr->targetValid = true;
			if Current(rdPtr->targetId)
			{
				rdPtr->rHo.hoImgWidth = TargetImg->GetWidth();
				rdPtr->rHo.hoImgHeight = TargetImg->GetHeight();
				UpdateHotspot(rdPtr);
				ImageChanged();
			}
			
			rdPtr->rRd->GenerateEvent(4);
		}
		//Fail
		else
		{
			rdPtr->rRd->GenerateEvent(2);
			tmp->Delete();
		}

		GlobalUnlock(handle);
		CloseClipboard();
	}
	//Fail
	else
	{
		rdPtr->rRd->GenerateEvent(2);
	}
	return 0;
}
ACTION(
	/* ID */			13,
	/* Name */			_T("%o: Save image to the clipboard"),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();
	if(OpenClipboard(NULL))
	{
		EmptyClipboard();
		cSurface* tmp = TargetImg;
		//Allocate buffer
		HGLOBAL cb = GlobalAlloc(GMEM_MOVEABLE,tmp->GetDIBSize());
		BITMAPINFO* bmp = (BITMAPINFO*)GlobalLock(cb);
		//Copy image
		tmp->SaveImage(bmp,(BYTE*)(bmp+1)-4);
		SetClipboardData(CF_DIB,bmp);
		bmp = bmp;
		//Done
		GlobalUnlock(cb);
		CloseClipboard();
		rdPtr->rRd->GenerateEvent(5);
	}
	//Fail
	else
	{
		rdPtr->rRd->GenerateEvent(3);
	}
	return 0;
}

ACTION(
	/* ID */			14,
	/* Name */			_T("%o: Lock buffer"),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();
	if(!rdPtr->buffer)
	{
		rdPtr->buffer = TargetImg->LockBuffer();
		rdPtr->buffPitch = TargetImg->GetPitch();
		rdPtr->buffSurf = TargetImg;
		if(rdPtr->buffPitch < 0)
		{
			rdPtr->buffPitch *= -1;
			rdPtr->buffer -= rdPtr->buffPitch*(TargetImg->GetHeight()-1);
		}
	}
	return 0;
}

ACTION(
	/* ID */			15,
	/* Name */			_T("%o: Unlock buffer"),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();
	if(rdPtr->buffer&&rdPtr->buffPitch>0)
	{
		rdPtr->buffSurf->UnlockBuffer(rdPtr->buffer);
		rdPtr->buffSurf = 0;
		rdPtr->buffPitch = 0;
		rdPtr->buffer = 0;
		ImageChanged();
		
	}
	return 0;
}


ACTION(
	/* ID */			16,
	/* Name */			_T("%o: Load raw image from %0  with alpha channel %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Source address"),PARAM_NUMBER,_T("Select (0 - 24bpp RGB, 1 - 32 bpp RGBA, 2 - 32 bpp alpha channel)"))
) {
	DWORD src = GetInt();
	char RGBA = GetInt();
	bool hasAlpha = RGBA;

	const int height = TargetImg->GetHeight();
	const int width = TargetImg->GetWidth();

	ActionFunc0(rdPtr, 1, 0); //Create alpha channel

	cSurface* alpha = TargetImg->GetAlphaSurface();
	BYTE pixel[4] = { 0, 0, 0, 255 };

	for ( int y=0; y<height; y++ )
		for ( int x=0; x<width; x++ )
		{
			memcpy( &pixel, (DWORD*)(src+(y*width+x)*(3+hasAlpha)), 3+hasAlpha );

			switch (RGBA)
			{
				case 0:
					TargetImg->SetPixel( x, y, pixel[0],pixel[1],pixel[2] );
				break;
				case 1:
				{
					TargetImg->SetPixel( x, y, pixel[0],pixel[1],pixel[2] );
					alpha->SetPixel( x, y, pixel[3] );
				}
				break;
				case 2:
				{
					TargetImg->SetPixel( x, y, pixel[3],pixel[3],pixel[3] );
					alpha->SetPixel( x, y, 0xFF );
				}
				break;
			}
		}

	if (!RGBA)
		alpha->Fill(0xFF);
	TargetImg->ReleaseAlphaSurface(alpha);
	if Current(rdPtr->targetId)
		ImageChanged();

	return 0;
}


ACTION(
	/* ID */			17,
	/* Name */			_T("%o: Create alpha channel, keep transparency = %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Keep transparency? (0: No, 1: Yes)"))
) {
	TargetExists();
	COLORREF transp = TargetImg->GetTransparentColor();
	if(!TargetImg->HasAlpha())
	{
		TargetImg->CreateAlpha();
		//Keep transparency
		if(param1)
		{
			cSurface* alpha = TargetImg->GetAlphaSurface();
			for(int x=0;x<TargetImg->GetWidth();x++)
				for(int y=0;y<TargetImg->GetHeight();y++)
					if(TargetImg->GetPixelFast(x,y)!=transp)
						alpha->SetPixel(x,y,(COLORREF)0xffffffff);
			 TargetImg->ReleaseAlphaSurface(alpha);
		}
		else
		{
			if Current(rdPtr->targetId)
				ImageChanged();	
			
		}
	}
	else if(param1)
	{
		cSurface* alpha = TargetImg->GetAlphaSurface();
		for(int x=0;x<TargetImg->GetWidth();x++)
			for(int y=0;y<TargetImg->GetHeight();y++)
				if(TargetImg->GetPixelFast(x,y)!=transp)
					alpha->SetPixel(x,y,255);
		TargetImg->ReleaseAlphaSurface(alpha);
	}
	return 0;
}

ACTION(
	/* ID */			18,
	/* Name */			_T("%o: Remove alpha channel"),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();
	COLORREF transp = TargetImg->GetTransparentColor();
	if(TargetImg->HasAlpha())
	{	
		cSurface tmp;
		tmp.Clone(*TargetImg);
		TargetImg->Delete();
		TargetImg->Create(tmp.GetWidth(),tmp.GetHeight(),&tmp);
		tmp.Blit(*TargetImg,0,0,BMODE_OPAQUE);
		if Current(rdPtr->targetId)
			ImageChanged();
		
	}
	return 0;
}

ACTION(
	/* ID */			19,
	/* Name */			_T("%o: Clear alpha with %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Alpha (0-255)"))
) {
	TargetExists();
	ActionFunc0(rdPtr, 1, 0);
	if(TargetImg->HasAlpha())
	{
		cSurface* alpha = TargetImg->GetAlphaSurface();
		alpha->Fill(param1);
		TargetImg->ReleaseAlphaSurface(alpha);
		if Current(rdPtr->targetId)
			ImageChanged();
		
	}
	return 0;
}

ACTION(
	/* ID */			20,
	/* Name */			_T("%o: Set alpha at (%0,%1) to %2"),
	/* Flags */			0,
	/* Params */		(3,PARAM_NUMBER,_T("X"),PARAM_NUMBER,_T("Y"),PARAM_NUMBER,_T("Alpha (0-255)"))
) {
	TargetExists();
	int x = GetXPos();
	int y = GetYPos();
	int a = GetInt();
	ActionFunc0(rdPtr, 1, 0);
	if(TargetImg->HasAlpha())
	{
		cSurface* alpha = TargetImg->GetAlphaSurface();
		alpha->SetPixel(x,y,a);
		TargetImg->ReleaseAlphaSurface(alpha);
		if Current(rdPtr->targetId)
			RectChanged(x,y,x+1,y+1);
		
	}
	return 0;
}

ACTION(
	/* ID */            21,
	/* Name */          _T("%o: Resize image to (%0,%1)"),
	/* Flags */         0,
	/* Params */        (2,PARAM_NUMBER,_T("Width"),PARAM_NUMBER,_T("Height"))
) {
        ImageInRange(rdPtr->targetId);
        int w = GetInt();
        int h = GetInt();
        if(w>0&&h>0)
        { 
                //Resize by cloning
                if(TargetImg->IsValid())
                {
                        int ow = TargetImg->GetWidth();
                        int oh = TargetImg->GetHeight();
                        cSurface tmp;
                        tmp.Clone(*TargetImg);
                        //Create from prototype and Blit
                        cSurface* proto = 0;
                        GetSurfacePrototype(&proto,rdPtr->depth,TargetImg->GetType(),TargetImg->GetDriver());
                        TargetImg->Delete();
                        TargetImg->Create(w,h,proto);
                        int flag = STRF_COPYALPHA|(rdPtr->rs.rsEffect&EFFECTFLAG_ANTIALIAS?STRF_RESAMPLE:0);
                        tmp.Stretch(*TargetImg,0,0,w,h,0,0,ow,oh,BMODE_OPAQUE,BOP_COPY,0,flag);
                }
                //Invalid, create new from prototype
                else {
                        cSurface* proto = 0;
                        GetSurfacePrototype(&proto,rdPtr->depth,ST_MEMORYWITHDC,SD_DIB);
                        TargetImg->Create(w,h,proto);
                        TargetImg->Fill(BLACK);
                        rdPtr->targetValid = true;
                }
                //Update display image
                if Current(rdPtr->targetId)
                {
                        CurrentImg = TargetImg;
                        UpdateHotspot(rdPtr);
                        ImageChanged();
                }
				
        }
        else if(TargetImg->IsValid())
        {
                TargetImg->Delete();
                rdPtr->targetValid = false;
                if Current(rdPtr->targetId)
                {
                        rdPtr->rHo.hoImgWidth = 0;
                        rdPtr->rHo.hoImgHeight = 0;
						rdPtr->rHo.hoImgXSpot = 0;
						rdPtr->rHo.hoImgYSpot = 0;
                        ImageChanged();
                }
				
        }
        return 0;
}

ACTION(
	/* ID */			22,
	/* Name */			_T("%o: Resize canvas to (%0, %1, %2, %3)"),
	/* Flags */			0,
	/* Params */		(4,PARAM_NUMBER,_T("X1"),PARAM_NUMBER,_T("Y1"),PARAM_NUMBER,_T("X2"),PARAM_NUMBER,_T("Y2"))
) {
	TargetExists();
	int x1 = GetXPos();
	int y1 = GetYPos();
	int x2 = GetXPos();
	int y2 = GetYPos();
	//Delete
	if(!(x1-x2)||!(y1-y2))
	{
        TargetImg->Delete();
        rdPtr->targetValid = false;
        if Current(rdPtr->targetId)
        {
                rdPtr->rHo.hoImgWidth = 0;
                rdPtr->rHo.hoImgHeight = 0;
				rdPtr->rHo.hoImgXSpot = 0;
				rdPtr->rHo.hoImgYSpot = 0;
                ImageChanged();
        }
		
		return 0;
	}
	//Create temp
	cSurface tmp;
	tmp.Clone(*TargetImg);
	//Resize target
	cSurface* proto = 0;
	GetSurfacePrototype(&proto,rdPtr->depth,TargetImg->GetType(),TargetImg->GetDriver());
	COLORREF transColor = TargetImg->GetTransparentColor();
	TargetImg->Delete();
	TargetImg->Create(x2-x1,y2-y1,proto);
	TargetImg->Fill(transColor);
	TargetImg->SetTransparentColor(transColor);
	//Blit temp image
	tmp.Blit(*TargetImg,max(-x1,0),max(-y1,0),max(x1,0),max(y1,0),x2-x1,y2-y1,BMODE_OPAQUE,BOP_COPY,0,BLTF_COPYALPHA);

	if Current(rdPtr->targetId)
	{
		UpdateHotspot(rdPtr);
		ImageChanged();
	}
	
	return 0;
}

ACTION(
	/* ID */			23,
	/* Name */			_T("%o: Flip horizontally"),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();
	TargetImg->ReverseX();
	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}

ACTION(
	/* ID */			24,
	/* Name */			_T("%o: Flip vertically"),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();
	TargetImg->ReverseY();
	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}

ACTION(
	/* ID */			25,
	/* Name */			_T("%o: Clear with color %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_COLOUR,_T("Color"))
) {
	TargetExists();
	TargetImg->Fill((COLORREF)param1);
	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}

ACTION(
	/* ID */			26,
	/* Name */			_T("%o: Set pixel at (%0,%1) to %2"),
	/* Flags */			0,
	/* Params */		(3,PARAM_NUMBER,_T("X"),PARAM_NUMBER,_T("Y"),PARAM_COLOUR,_T("Color"))
) {
	TargetExists();
	int x = GetXPos();
	int y = GetYPos();
	COLORREF col = GetCol();
	if(rdPtr->buffer)
		TargetImg->SetPixelFast(x,y,col);
	else
		TargetImg->SetPixel(x,y,col);
	if Current(rdPtr->targetId)
		RectChanged(x,y,x+1,y+1);
	
	return 0;
}

ACTION(
	/* ID */			27,
	/* Name */			_T("%o: Flood fill at (%0,%1) with color %2 and tolerance %3"),
	/* Flags */			0,
	/* Params */		(4,PARAM_NUMBER,_T("X"),PARAM_NUMBER,_T("Y"),PARAM_COLOUR,_T("Color"),PARAM_NUMBER,_T("Tolerance (0-255)"))
) {
	TargetExists();
	int x = GetXPos();
	int y = GetYPos();
	COLORREF col = GetCol();
	int tol = GetInt();
	TargetImg->FloodFill(x,y,rdPtr->floodL,rdPtr->floodT,rdPtr->floodR,rdPtr->floodB,col,USE_AA,tol);
	if Current(rdPtr->targetId)
		RectChanged(rdPtr->floodL,rdPtr->floodT,rdPtr->floodR,rdPtr->floodB);
	
	return 0;
}


ACTION(
	/* ID */			28,
	/* Name */			_T("%o: Load bitmap from %0  Only alpha channel %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Source address"),PARAM_NUMBER,_T("0 - All channels, 1 - Only alpha channel"))
) {	
	DWORD src = GetInt();
	unsigned char onlyAlpha = GetInt();
	
	if ( *(SHORT*)src = 0x4D42 ) // BITMAP
	{
		// BMP header
		#pragma pack(push, 1)
		#pragma pack(1)
		struct BMPfheader // BMP file header structure
		{
			short magic; // BM
			unsigned int fileSize;
			unsigned short reserved1;
			unsigned short reserved2;
			unsigned int startingAddress;
		} BMPfheader;
		struct BMPiheader // BMP file info structure
		{
			unsigned int headerSize;
			signed int width;
			signed int height;
			unsigned short colorPlanes;
			unsigned short bitsPerPixel;
			unsigned int compression;
			unsigned int imageSize;
			signed int hImageRes;
			signed int vImageRes;
			unsigned int colourMapLength;
			unsigned int importantColors;
		} BMPiheader;
		#pragma pack(pop)
		// fill by data
		memcpy(&BMPfheader, (void*)src, sizeof(BMPfheader));
		memcpy(&BMPiheader, (void*)(src+sizeof(BMPfheader)), sizeof(BMPiheader));
	
		int bitMasks[4] = { 0, 0, 0, 0 }; // for 16 bits and 32 bits
		if (BMPiheader.compression == 3)
			memcpy((int*)&bitMasks, (int*)(src+sizeof(BMPfheader)+sizeof(BMPiheader)), sizeof(bitMasks));
	
		const unsigned char FullHeaderSize = sizeof(BMPfheader)+BMPiheader.headerSize;
	
		//ActionFunc21(rdPtr, width, height); //why it isn't working, below some workaround
		ImageInRange(rdPtr->targetId);
	
		if( abs(BMPiheader.width) && abs(BMPiheader.height) )
		{
			//Resize by cloning
			if(TargetImg->IsValid())
			{
				int ow = TargetImg->GetWidth();
				int oh = TargetImg->GetHeight();
				cSurface tmp;
				tmp.Clone(*TargetImg);
				//Create from prototype and Blit
				cSurface* proto = 0;
				GetSurfacePrototype(&proto,rdPtr->depth,TargetImg->GetType(),TargetImg->GetDriver());
				TargetImg->Delete();
				TargetImg->Create(abs(BMPiheader.width),abs(BMPiheader.height),proto);
				int flag = STRF_COPYALPHA|(rdPtr->rs.rsEffect&EFFECTFLAG_ANTIALIAS?STRF_RESAMPLE:0);
				tmp.Stretch(*TargetImg,0,0,abs(BMPiheader.width),abs(BMPiheader.height),0,0,ow,oh,BMODE_OPAQUE,BOP_COPY,0,flag);
			}
			//Invalid, create new from prototype
			else
			{
				cSurface* proto = 0;
				GetSurfacePrototype(&proto,rdPtr->depth,ST_MEMORYWITHDC,SD_DIB);
				TargetImg->Create(abs(BMPiheader.width),abs(BMPiheader.height),proto);
				TargetImg->Fill(BLACK);
				rdPtr->targetValid = true;
			}
			//Update display image
			if Current(rdPtr->targetId)
			{
				CurrentImg = TargetImg;
				UpdateHotspot(rdPtr);
				ImageChanged();
			}	
	    }
	    else if(TargetImg->IsValid())
	    {
			TargetImg->Delete();
			rdPtr->targetValid = false;
			if Current(rdPtr->targetId)
			{
				rdPtr->rHo.hoImgWidth = 0;
				rdPtr->rHo.hoImgHeight = 0;
				rdPtr->rHo.hoImgXSpot = 0;
				rdPtr->rHo.hoImgYSpot = 0;
				ImageChanged();
			}	
	    }
	
		ActionFunc0(rdPtr, 1, 0); //Create alpha channel
		cSurface* alpha = TargetImg->GetAlphaSurface();
		COLORREF col;
			
		const int RowSize = ((BMPiheader.bitsPerPixel*BMPiheader.width+31)/32)*4;
	
		if (onlyAlpha)
			TargetImg->Fill(WHITE);
		else
			alpha->Fill(255);
		switch (BMPiheader.bitsPerPixel)
		{
			case 1: // OMG, 2 colors
				for ( int y=0; y<BMPiheader.height; y++ )
					for ( int x=0; x<RowSize*8; x++ )
					{
						const unsigned int vflip = (BMPiheader.height+~y);
						const unsigned int Cursor = vflip*RowSize*8+x;
						unsigned char pxlArrayIndex = *(BYTE*)(src+FullHeaderSize + 2*4 + Cursor/8); // Eight pixels in one pack
						pxlArrayIndex = pxlArrayIndex >> (7-Cursor%8) & 1; // Extract pixels from the pack
							
						if (onlyAlpha)
						{
							CONST BYTE pixel = *(BYTE*)(src+FullHeaderSize + pxlArrayIndex*4+3);
							col = pixel | (pixel<<8) | (pixel<<16);	
						}
						else
						{
							col = _byteswap_ulong(*(DWORD*)(src+FullHeaderSize + pxlArrayIndex*4)) >> 8;
							alpha->SetPixel( x, y, *(BYTE*)(src+FullHeaderSize + pxlArrayIndex*4+3) );
						}
	
						if ( x<BMPiheader.width )
							TargetImg->SetPixelFast( x, y, col );
					}
			break;
	
			case 4: // 16 colors bitmap support impossible in MMF?
				if (BMPiheader.compression) break;
	
				for ( int y=0; y<abs(BMPiheader.height); y++ )
					for ( int x=0; x<RowSize*2; x++ )
					{
						const unsigned int vflip = (BMPiheader.height+~y);
						const unsigned int Cursor = vflip*RowSize*2+x;
						unsigned char pxlArrayIndex = *(BYTE*)(src+FullHeaderSize + 16*4 + Cursor/2 ); //two pixels in one pack
	
						if ( !(Cursor%2) ) // Extract pixels from the pack
							pxlArrayIndex /= 0x10;
						else
							pxlArrayIndex &= 0xF;
	
						if (onlyAlpha)
						{
							CONST BYTE pixel = *(BYTE*)(src+FullHeaderSize + pxlArrayIndex*4+3);
							col = pixel | (pixel<<8) | (pixel<<16);	
						}
						else
						{
							col = _byteswap_ulong(*(DWORD*)(src+FullHeaderSize + pxlArrayIndex*4)) >> 8;
							alpha->SetPixel( x, y, *(BYTE*)(src+FullHeaderSize + pxlArrayIndex*4+3) );
						}
	
						if ( x<BMPiheader.width )
							TargetImg->SetPixelFast( x, y, col );
					}
			break;
	
			case 8: // 256 colors
				if (BMPiheader.compression) break;
	
				for ( int y=0; y<BMPiheader.height; y++ )
					for ( int x=0; x<RowSize; x++ )
					{
						const unsigned int vflip = (BMPiheader.height+~y);
						const unsigned int Cursor = vflip*RowSize+x;
						const unsigned char pxlArrayIndex = *(BYTE*)(src+FullHeaderSize + 256*4 + Cursor);
		
						if (onlyAlpha)
						{
							CONST BYTE pixel = *(BYTE*)(src+FullHeaderSize + pxlArrayIndex*4+3);
							col = pixel | (pixel<<8) | (pixel<<16);	
						}
						else
						{
							col = _byteswap_ulong(*(DWORD*)(src+FullHeaderSize + pxlArrayIndex*4)) >> 8;
							alpha->SetPixel( x, y, *(BYTE*)(src+FullHeaderSize + pxlArrayIndex*4+3) );
						}
	
						if ( x<BMPiheader.width )
							TargetImg->SetPixelFast( x, y, col );
					}
			break;
	
			case 16: // 16 bpp advanced stuff
				for ( int y=0; y<BMPiheader.height; y++ )
					for ( int x=0; x<BMPiheader.width; x++ )
					{
						const unsigned int vflip = (BMPiheader.height+~y);
						const unsigned int Cursor = vflip*BMPiheader.width+x;
						CONST SHORT pixel = *(SHORT*)(src+FullHeaderSize + Cursor*2);
						SHORT r, g, b, a;
	
						if (!*bitMasks || bitMasks[1] == 0x03E0) // B5G5R5X1 or B5G5R5A1
						{
							if (!onlyAlpha)
							{
								r = (pixel & 0x7C00) >> 10;
								r = (r << 3) | (r >> 2);
								g = (pixel & 0x3E0) >> 5;
								g = (g << 3) | (g >> 2);	
								b = pixel & 0x1F;
								b = (b << 3) | (b >> 2);
							}
							if (bitMasks[3]) // A1
							{
								a = pixel & 0x8000;
								a = a >> 15;
								if (!onlyAlpha)
									alpha->SetPixel( x, y, a );
								else
									col = a | (a<<8) | (a<<16);
							}
						}
						else
							if (bitMasks[1] == 0x07E0) // B5G6R5
							{								
								r = (pixel & 0xF800) >> 11;
								r = (r << 3) | (r >> 2);
								g = (pixel & 0x07E0) >> 5;
								g = (g << 2) | (g >> 3);
								b = pixel & 0x001F;	
								b = (b << 3) | (b >> 2);
							}
	
						if (!onlyAlpha)
							col = r | (g<<8) | (b<<16);
	
						if (!onlyAlpha || bitMasks[3])
							if ( x<BMPiheader.width )
								TargetImg->SetPixelFast( x, y, col );
					}
			break;
	
			case 24: // 24 bpp
				for ( int y=0; y<BMPiheader.height; y++ )
					for ( int x=0; x<BMPiheader.width; x++ )
					{
						const unsigned int vflip = (BMPiheader.height+~y);
						const unsigned int Cursor = vflip*BMPiheader.width+x;
						const unsigned int Padding = vflip*(BMPiheader.width - (BMPiheader.width/4)*4);
		
						if (!onlyAlpha)
						{
							col = _byteswap_ulong(*(DWORD*)(src+FullHeaderSize + Cursor*3 + Padding)) >> 8;
							if ( x<BMPiheader.width )
								TargetImg->SetPixelFast( x, y, col );
						}
					}
			break;
	
			case 32: // 32 bpp
				for ( int y=0; y<BMPiheader.height; y++ )
					for ( int x=0; x<BMPiheader.width; x++ )
					{
						const unsigned int vflip = (BMPiheader.height+~y);
						const unsigned int Cursor = vflip*BMPiheader.width+x;
						CONST BYTE pixel = *(BYTE*)(src+FullHeaderSize + Cursor*4+3);
	
						if (onlyAlpha)	
							col = pixel | (pixel<<8) | (pixel<<16);	
						else
						{
							col = _byteswap_ulong(*(DWORD*)(src+FullHeaderSize + Cursor*4-1));
							alpha->SetPixel( x, y, pixel );
						}
						if ( x<BMPiheader.width )
							TargetImg->SetPixelFast( x, y, col );
					}
		}
	
		TargetImg->ReleaseAlphaSurface(alpha);
		if Current(rdPtr->targetId)
			ImageChanged();
	}
	return 0;
}

ACTION(
	/* ID */			29,
	/* Name */			_T("Set angle to %0, quality = %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Angle (Degrees)"),PARAM_NUMBER,_T("Quality (0: Speed, 1: Quality)"))
) {
	long p1temp = CNC_GetFloatParameter(rdPtr);
	float p1 = *(float*) &p1temp;
	int p2 = Param(TYPE_INT) != 0;
	while(p1 < 0) p1 += 360;
#ifndef ANGLEFLOAT
	rdPtr->rc.rcAngle = (int)p1 % 360;
#else
	rdPtr->rc.rcAngle = fmod(p1, 360);
#endif
	rdPtr->rs.rsFlags = (rdPtr->rs.rsFlags & ~RSFLAG_ROTATE_ANTIA) | (p2 ? RSFLAG_ROTATE_ANTIA : 0);
	
	UpdateHotspot(rdPtr);
	ImageChanged();
	
	return 0;
}


// ============================================================================
//
// EXPRESSIONS
// 
// ============================================================================


EXPRESSION(
	/* ID */			0,
	/* Name */			_T("Width("),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();
	return TargetImg->GetWidth();
}

EXPRESSION(
	/* ID */			1,
	/* Name */			_T("Height("),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();	
	return TargetImg->GetHeight();
}


EXPRESSION(
	/* ID */			2,
	/* Name */			_T("XScale("),
	/* Flags */			EXPFLAG_DOUBLE,
	/* Params */		(0)
) {
	ReturnFloat(rdPtr->rc.rcScaleX);
}

EXPRESSION(
	/* ID */			3,
	/* Name */			_T("YScale("),
	/* Flags */			EXPFLAG_DOUBLE,
	/* Params */		(0)
) {
	ReturnFloat(rdPtr->rc.rcScaleY);
}

EXPRESSION(
	/* ID */			4,
	/* Name */			_T("Angle("),
	/* Flags */			0,
	/* Params */		(0)
) {
	ReturnFloat(rdPtr->rc.rcAngle);
}
