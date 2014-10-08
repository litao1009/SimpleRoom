#ifndef RectODL_h__
#define RectODL_h__

#pragma once
#include "baseodl.h"
class CGuideRectODL :
	public CBaseODL
{
public:
	CGuideRectODL(void);
	~CGuideRectODL(void);
public:
	virtual void OnDrawTop(Gdiplus::Graphics& gcDrawer, float fScale);

	virtual void SetTopStartPos(gp_Pnt ptStart);

	virtual void SetTopStopPos(gp_Pnt ptStop);

	virtual void SetTopCurrentPos(gp_Pnt ptCurrent);

	//…Ë÷√µ„Œª÷√
	void		SetTopContinuePos(gp_Pnt ptContinue);

	virtual bool IsTopPointInFace(gp_Pnt ptPos);
	virtual void GetTopPointList(std::vector<gp_Pnt>& arrPoint);

private:
	
};

typedef std::shared_ptr<CGuideRectODL> CGuideRectODLSPtr;
typedef std::weak_ptr<CGuideRectODL> CGuideRectODLWPtr;

#endif // RectODL_h__
