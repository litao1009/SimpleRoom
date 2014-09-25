#ifndef FloorZoneODL_h__
#define FloorZoneODL_h__

#pragma once
#include "baseodl.h"

class CFloorZoneODL : public CBaseODL
{
public:
	CFloorZoneODL(void);
	~CFloorZoneODL(void);
public:
	void SetZoneSize(Gdiplus::REAL fWidth, Gdiplus::REAL fHeight);

	virtual int		GetSidePointCount(){return 4;};

public:
	virtual bool	UpdateMesh();

public:
	virtual void	OnSideMoveStart(const Gdiplus::PointF& ptStart);
	virtual void	OnSideMoving(const Gdiplus::PointF& ptOffset);
	virtual void	OnSideMovingFinish(const Gdiplus::PointF& ptStop, bool bCommitChange);
	//强制停止移动，并回到原来的位置
	virtual void	CancelSideMoving();
	//////////////////////////////////////////////////////////////////////////
	//修改尺寸
	//nPos:1、左上；2、上；3、右上；4、右；5、右下；6、下；7、左下；8、左
	virtual void	OnSideReSizeStart(const Gdiplus::PointF& ptStart);
	virtual void OnSideReSizing( const Gdiplus::PointF& ptCurrent);
	void	OnSideReSizing(ENUM_RECT_SELECTED_POS ePos, const Gdiplus::PointF& ptCurrent);
	virtual void	OnSideReSizeEnd(const Gdiplus::PointF& ptStop);
	//////////////////////////////////////////////////////////////////////////
	//只能在此区域移动

	virtual void SetIsTopSelected( bool isSelected);

	virtual void	SetSideSize(const Gdiplus::RectF& rtTarget);

	virtual void SetSideLimitArea(const Gdiplus::RectF& rtLimit);

	void RefreshRealArea();

public:
	virtual void	OnDrawFace(Gdiplus::Graphics& gcDrawer, float fScale);

protected:
	Gdiplus::PointF m_pPrevPos;
};

typedef	std::shared_ptr<CFloorZoneODL>	CFloorZoneODLSPtr;
typedef	std::weak_ptr<CFloorZoneODL>	CFloorZoneODLWPtr;

#endif // FloorZoneODL_h__
