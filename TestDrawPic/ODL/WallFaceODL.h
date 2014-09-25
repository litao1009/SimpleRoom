#pragma once
//////////////////////////////////////////////////////////////////////////
//这个东西是墙的墙面
//1段墙最少有2个面，正面背面，理论上6个面
//墙面包含有Shape，由墙CreatPoint时，同时生成
//////////////////////////////////////////////////////////////////////////

#include "baseodl.h"
#include "WallFaceZoneODL.h"
#include "WithHoleODL.h"

#include <vector>
#include <map>

class CWallFaceODL : public IWithHoleODL
{
public:
	CWallFaceODL(const TopoDS_Face& curFace);
	~CWallFaceODL(void);
public:

	bool		UpdateMesh();

	//////////////////////////////////////////////////////////////////////////
	//俯视图显示
	virtual void OnDrawTop(Gdiplus::Graphics& gcDrawer, float fScale);

	//移动过程
	virtual void OnTopMoving(const gp_Pnt& stInfo);
	//移动完成
	virtual void OnTopMovingFinish(const gp_Pnt& stInfo, bool bCommitChange);
	//取消移动
	virtual void CancelTopMoving();
	//////////////////////////////////////////////////////////////////////////
	//墙面图显示
	virtual void	OnDrawFace(Gdiplus::Graphics& gcDrawer, float fScale);

public:

	virtual	bool	UpdateHoleShape();

	virtual void	UpdateHoleRect(RelationRect rect);
};


typedef	std::shared_ptr<CWallFaceODL>	CWallFaceODLSPtr;
typedef	std::weak_ptr<CWallFaceODL>		CWallFaceODLWPtr;
