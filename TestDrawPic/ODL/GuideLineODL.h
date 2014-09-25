#ifndef GuideLineODL_h__
#define GuideLineODL_h__

#pragma once

#include "baseodl.h"

#include <gdiplus.h>

class CGuideLineODL :
	public CBaseODL
{
public:
	CGuideLineODL(void);
	~CGuideLineODL(void);

public:
	//划线
	virtual void OnDrawTop(Gdiplus::Graphics& gcDrawer, float fScale);

	virtual void SetTopStartPos(gp_Pnt ptStart);

	virtual void SetTopStopPos(gp_Pnt ptStop);

	virtual void SetTopCurrentPos(gp_Pnt ptCurrent);
	
	//设置点位置
	void		SetTopContinuePos(gp_Pnt ptContinue);

	virtual void GetPointList(std::vector<gp_Pnt>& arrPoint){arrPoint = m_arrTempPoint;}

	float GetLockAngel() const { return m_bLockAngel; }
	float& GetLockAngel() { return m_bLockAngel; }
	void SetLockAngel(const float& val) { m_bLockAngel = val; }

private:

	gp_Pnt m_ptContinue;
	std::vector<gp_Pnt> m_arrTempPoint;
	std::vector<TopoDS_Shape> m_arrTempEdge;
	float m_bLockAngel;
	float m_fLockArc; //45
};

typedef std::shared_ptr<CGuideLineODL> CGuideLineODLSPtr;
typedef std::weak_ptr<CGuideLineODL> CGuideLineODLWPtr;

#endif // GuideLineODL_h__
