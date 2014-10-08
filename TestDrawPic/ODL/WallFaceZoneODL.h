#ifndef WallFaceZoneODL_h__
#define WallFaceZoneODL_h__

#pragma once
//////////////////////////////////////////////////////////////////////////
//这个保存墙面的分区信息，只有2维数据
//包括位置、大小、纹理、平铺方式
//区域的点信息保存在：X/y中，Z=0
//////////////////////////////////////////////////////////////////////////

#include "WithHoleODL.h"

class CWallFaceZoneODL : public IWithHoleODL
{
public:
	CWallFaceZoneODL(void);
	~CWallFaceZoneODL(void);
public:
	
	void SetZoneSize(Gdiplus::REAL fWidth, Gdiplus::REAL fHeight);

	virtual int		GetSidePointCount(){return 4;};

public:
	virtual bool	UpdateMesh();

	virtual	bool		UpdateHoleShape();
	
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

public:
	virtual void	OnDrawFace(Gdiplus::Graphics& gcDrawer, float fScale);
	
protected:
	Gdiplus::PointF m_pPrevPos;
};

typedef	std::shared_ptr<CWallFaceZoneODL>	CWallFaceZoneODLSPtr;
typedef	std::weak_ptr<CWallFaceZoneODL>	CWallFaceZoneODLWPtr;

#endif // WallFaceZoneODL_h__
