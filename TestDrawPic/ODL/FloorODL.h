#ifndef FloorODL_h__
#define FloorODL_h__

#pragma once

#include "baseodl.h"
#include "TopoDS_Face.hxx"
#include "FloorZoneODL.h"

class CFloorODL : public CBaseODL
{
public:
	CFloorODL(const TopoDS_Face& theFace);
	~CFloorODL(void);
	virtual void OnDrawTop(Gdiplus::Graphics& gcDrawer, float fScale);
	virtual void OnDrawFace( Gdiplus::Graphics& gcDrawer, float fScale );

	//ÒÆ¶¯¹ý³Ì
	virtual void OnTopMoving(const gp_Pnt& stInfo);
	virtual void OnTopMovingFinish(const gp_Pnt& stInfo, bool bCommitChange);
	virtual void CancelTopMoving();

	virtual bool UpdateMesh();

private:
	//Gdiplus::GraphicsPath m_gcPath;
};

typedef	std::shared_ptr<CFloorODL>		CFloorODLSPtr;
typedef	std::weak_ptr<CFloorODL>		CFloorODLWPtr;
#endif // FloorODL_h__
