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
	//��ʾ
	//////////////////////////////////////////////////////////////////////////
	virtual void	OnDrawTop(Gdiplus::Graphics& gcDrawer, float fScale);

	//�ƶ�����
	virtual void	OnTopMoving(const gp_Pnt& stInfo);	//�ƶ����
	virtual void	OnTopMovingFinish(const gp_Pnt& stInfo, bool bCommitChange);	//ȡ���ƶ�
	virtual void	CancelTopMoving();

	virtual	bool	UpdateMesh();

	void GetSize(float& fXLen, float& fYLen, float& fZLen)
	{fXLen = m_fXLen; fYLen = m_fYLen; fZLen = m_fZLen;}
	gp_XYZ GetSize()
	{
		gp_XYZ szSize(m_fXLen, m_fYLen, m_fZLen);
		return szSize;
	};
	//����ߴ磬�����ؽ���С
	void SetSize(float fXLen, float fYLen, float fZLen)
	{m_fXLen = fXLen; m_fYLen = fYLen; m_fZLen = fZLen;};
	float GetYPos(){return m_fYPos;};
	//����λ�ã������ؽ�
	void SetYPos(float fYPos){m_fYPos = fYPos;};

	void	SetBaseSolid(const TopoDS_Solid& theWall);

	HolesList GetHoleObj() const;

	void	CreateWallFace();

public:

	//Rect����Object
	virtual bool	IsFaceInShape(BRepExtrema_DistShapeShape& dss);

	//Rect�ཻObject
	virtual bool	HasTopCollisionWith(const TopoDS_Shape& theShape);

	virtual void	SetIsTopSelected( bool isSelected);
public:
	//////////////////////////////////////////////////////////////////////////
	//����
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
