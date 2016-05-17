#include "common.h"


void RotatePoint(double angle, int* hotX, int* hotY, int sw, int sh, int* dx, int* dy)
{
	//Rotate hotspot
	float hx = (float)*hotX;
	float hy = (float)*hotY;

	float si = sin(angle);
	float co = cos(angle);

	int trX = sw*co;
	int trY = -sw*si;
	int blX = sh*si;
	int blY = sh*co;
	int brX = sw*co + sh*si;
	int brY = sh*co - sw*si;

	//Update hotspot
	//*hotX = round(hx*co + hy*si);
	//*hotY = round(hy*co - hx*si);

	//Update topleft coordinate
	*dx += min(0,min(trX,min(blX,brX)));
	*dy += min(0,min(trY,min(blY,brY)));
}

cSurface* GetImgAt(RUNDATA* rdPtr,int image)
{
	if(image<0||image>=rdPtr->surf->size())
		return 0;
	return Image(image);
}

int GetImgCount(RUNDATA* rdPtr)
{
	return rdPtr->surf->size();
}

WORD CreateImageFromSurface(LPMV pMV, LPSURFACE pSf, int dwWidth, int dwHeight, int hotSpotX, int hotSpotY,     int actionPointX, int actionPointY){
        // Create image
        WORD    wMode = (WORD)(pMV->mvAppMode & SM_MASK);
        if ( pSf->HasAlpha() != NULL ){
                wMode |= (IF_ALPHA << 8);
        }
        WORD newImg = (WORD)AddImage (pMV->mvIdAppli, (WORD)dwWidth, (WORD)dwHeight, hotSpotX, hotSpotY,
                                                                actionPointX, actionPointY, pSf->GetTransparentColor(), wMode, 
                                                                NULL, NULL);
        if ( newImg != 0 ) {
                // Lock new image surface
                cSurface sfNewImg;
                if ( LockImageSurface(pMV->mvIdAppli, newImg, sfNewImg) ) {
                        // Blit surface into image surface
                        pSf->Blit(sfNewImg);
                        // Unlock image surface
                        UnlockImageSurface(sfNewImg);
                } else
                        DelImage(pMV->mvIdAppli, newImg);
        }
        return newImg;
}



void UpdateHotspot(LPRDATA rdPtr)
{
	if(rdPtr->currentId < 0 || !CurrentImg)
		return;

	//Get scale (absolute since negative will mirror)
	float scaleX = abs(rdPtr->rc.rcScaleX);
	float scaleY = abs(rdPtr->rc.rcScaleY);

	//Get scaled size
	int width = CurrentImg->GetWidth()*scaleX;
	int height = CurrentImg->GetHeight()*scaleY;

	//Get scaled hotspot
	SImage* si = ImageS(rdPtr->currentId);
	int hotX = si->hotX*scaleX;
	int hotY = si->hotY*scaleY;

	int dx = 0, dy = 0;
	if(rdPtr->rc.rcAngle)
	{
		//Rotate hotspot
		double angle = rdPtr->rc.rcAngle/180.0*PI;
		RotatePoint(angle,&hotX,&hotY,width,height,&dx,&dy);

		//Calculate rotated size
#ifdef HWABETA
		//cSurface::GetSizeOfRotatedRect(&width, &height, *(float*)&rdPtr->rc.rcAngle); // What
#else
		//cSurface::GetSizeOfRotatedRect(&width, &height, rdPtr->rc.rcAngle);
#endif
	}

	//Update size and scale image
	rdPtr->rHo.hoImgWidth = width; // leave room for rounding errors
	rdPtr->rHo.hoImgHeight = height;

	//Apply hotspot
	rdPtr->rHo.hoImgXSpot = (short)hotX-dx;
	rdPtr->rHo.hoImgYSpot = (short)hotY-dy;
}


const TCHAR * GetExtension(const TCHAR* Filename)
{
	const TCHAR * Extension = (Filename + _tcslen(Filename)) - 1;
	for(; Extension != Filename && *Extension != '.'; --Extension);
	return ++Extension;
}

DWORD FindFilter(CImageFilterMgr* pImgMgr, TCHAR* file,bool isext)
{
	//Get filter by extension
	const TCHAR* ext = isext ? file : GetExtension(file);
	DWORD filter = 0;
	for(int i=0;i<pImgMgr->GetFilterCount();++i)
	{
		const TCHAR** exts = pImgMgr->GetFilterExts(i);
		if(exts == 0) break;
		for(int j=0;exts[j];j++)
		{
			if(!_tcsicmp(exts[j],ext))
			{
				filter = pImgMgr->GetFilterID(i);
				break;
			}
		}
	} 
	return filter;
}
