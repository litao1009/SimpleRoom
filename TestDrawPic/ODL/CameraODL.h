#ifndef CameraODL_h__
#define CameraODL_h__

#pragma once

#include "CameraODLFwd.h"
#include "BaseODL.h"

#include "ISceneManager.h"

#include "TopoDS_Vertex.hxx"
#include "TopoDS_Edge.hxx"

class	CCameraODL : public CBaseODL
{
	enum ESelectType
	{
		EST_NONE,
		EST_POSITION,
		EST_TARGET,
		EST_FACE,
		EST_COUNT
	};
public:

	CCameraODL(irr::scene::ISceneManager* smgr);
	~CCameraODL();

public:

	virtual void	OnDrawTop(Gdiplus::Graphics& gcDrawer, float fScale);

	//Rect包含Object
	virtual bool	IsFaceInShape(BRepExtrema_DistShapeShape& dss);

	//相交判断
	virtual bool	HasTopCollisionWith(const TopoDS_Shape& theShape);

	//移动过程
	virtual void	OnTopMoving(const gp_Pnt& stInfo);

	virtual void	OnTopMovingFinish(const gp_Pnt& stInfo, bool bCommitChange);

	//强制停止移动，并回到原来的位置
	virtual void	CancelTopMoving();

	bool			SetCameraInfo(bool visible, const gp_Pnt& position, const gp_Pnt& target, float fov);

	void			BuildCamera();

private:

	irr::scene::ISceneManager*	m_Smgr;
	gp_Pnt			m_TempCameraPos;
	gp_Pnt			m_TempTargetPos;
	TopoDS_Vertex	m_CameraPos;
	TopoDS_Vertex	m_TargetPos;
	TopoDS_Face		m_CameraFace;
	std::vector<gp_Pnt>	m_CameraFacePnt;
	float			m_CameraFov;
	ESelectType		m_SelectType;
	BRepExtrema_DistShapeShape	m_DSS;
};

#endif // CameraODL_h__
