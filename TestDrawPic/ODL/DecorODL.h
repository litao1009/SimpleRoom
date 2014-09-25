#ifndef DecorODL_h__
#define DecorODL_h__

#pragma once

#include "HoleODL.h"

#include <array>

class CDecorODL : public IHoleODL
{
public:
	CDecorODL(void);
	~CDecorODL(void);
public:
	//////////////////////////////////////////////////////////////////////////

	//创建
	void Create(float fXLen, float fYLen, float fZLen, gp_Pnt& ptPos,CString strModelPath);
	void SetSize(float fXLen, float fYLen, float fZLen);
	void SetPos(const gp_Pnt& ptPos);
	void UpdateModelPath(CString strModelPath);

public:
	//////////////////////////////////////////////////////////////////////////
	//画
	virtual void OnDrawTop(Gdiplus::Graphics& gcDrawer, float fScale);

	virtual void OnDrawFace(Gdiplus::Graphics& gcDrawer, float fScale);

	virtual	bool	UpdateMesh();
public:
	//////////////////////////////////////////////////////////////////////////
	//移动
	virtual void	OnTopMoveStart(const gp_Pnt& stInfo);
	virtual void	OnTopMoving(const gp_Pnt& stInfo);	//移动完成
	virtual void	OnTopMovingFinish(const gp_Pnt& stInfo, bool bCommitChange);	//取消移动
	virtual void	CancelTopMoving();

	virtual bool	HasTopCollisionWith(const TopoDS_Shape& theShape);
	const gp_XYZ& GetRawSize() const { return m_szRawSize; }
	gp_XYZ& GetRawSize() { return m_szRawSize; }
	void SetRawSize(const gp_XYZ& val) { m_szRawSize = val; }

	//////////////////////////////////////////////////////////////////////////
	//在墙面上移动时，在地板上移动时

	virtual void	OnSideMoveStart(const Gdiplus::PointF& ptStart);
	virtual void	OnSideMoving(const Gdiplus::PointF& ptOffset);

	virtual void	OnSideMovingFinish(const Gdiplus::PointF& ptOffset, bool bCommitChange);
	//强制停止移动，并回到原来的位置
	virtual void	CancelSideMoving();

	virtual void	SetSideSize(const Gdiplus::RectF& rtTarget){m_rtArea = rtTarget; m_rtMovingArea = m_rtArea;};

private:
	gp_XYZ m_szRawSize;		//原始数据大小
	
	float m_fYPos;
	CString m_strModelPath;
};

typedef	std::shared_ptr<CDecorODL>	CDecorODLSPtr;
typedef	std::weak_ptr<CDecorODL>	CDecorODLLWPtr;

#endif // DecorODL_h__
