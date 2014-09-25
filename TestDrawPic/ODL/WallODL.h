#ifndef WallODL_h__
#define WallODL_h__

#pragma once

#include "baseodl.h"
#include "TopoDS_Face.hxx"
#include "TopoDS_Solid.hxx"
#include "DecorODL.h"
#include "WithHoleODL.h"

class CWallODL : public IWithHoleODL
{
public:
	CWallODL();
	~CWallODL(void);
public:
	//////////////////////////////////////////////////////////////////////////
	//显示
	//////////////////////////////////////////////////////////////////////////
	virtual void	OnDrawTop(Gdiplus::Graphics& gcDrawer, float fScale);

	//移动过程
	virtual void	OnTopMoving(const gp_Pnt& stInfo);	//移动完成
	virtual void	OnTopMovingFinish(const gp_Pnt& stInfo, bool bCommitChange);	//取消移动
	virtual void	CancelTopMoving();

	virtual	bool	UpdateMesh();

	void GetSize(float& fXLen, float& fYLen, float& fZLen)
	{fXLen = m_fXLen; fYLen = m_fYLen; fZLen = m_fZLen;}
	gp_XYZ GetSize()
	{
		gp_XYZ szSize(m_fXLen, m_fYLen, m_fZLen);
		return szSize;
	};
	//重设尺寸，并且重建大小
	void SetSize(float fXLen, float fYLen, float fZLen)
	{m_fXLen = fXLen; m_fYLen = fYLen; m_fZLen = fZLen;};
	float GetYPos(){return m_fYPos;};
	//重设位置，并且重建
	void SetYPos(float fYPos){m_fYPos = fYPos;};

	void	SetBaseSolid(const TopoDS_Solid& theWall);

	HolesList GetHoleObj() const;

	void	CreateWallFace();

public:

	//Rect包含Object
	virtual bool	IsFaceInShape(BRepExtrema_DistShapeShape& dss);

	//Rect相交Object
	virtual bool	HasTopCollisionWith(const TopoDS_Shape& theShape);

	virtual void	SetIsTopSelected( bool isSelected);
public:
	//////////////////////////////////////////////////////////////////////////
	//数据
	//////////////////////////////////////////////////////////////////////////
	virtual void CreatePoint(const TopoDS_Face& theFace);

private:

	gp_Dir m_dirWall;
	float m_fXLen;
	float m_fYLen;
	float m_fZLen;
	float m_fYPos;
};

typedef	std::shared_ptr<CWallODL>	CWallODLSPtr;
typedef	std::weak_ptr<CWallODL>		CWallODLWPtr;

#endif // WallODL_h__
