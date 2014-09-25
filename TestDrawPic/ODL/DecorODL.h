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

	//����
	void Create(float fXLen, float fYLen, float fZLen, gp_Pnt& ptPos,CString strModelPath);
	void SetSize(float fXLen, float fYLen, float fZLen);
	void SetPos(const gp_Pnt& ptPos);
	void UpdateModelPath(CString strModelPath);

public:
	//////////////////////////////////////////////////////////////////////////
	//��
	virtual void OnDrawTop(Gdiplus::Graphics& gcDrawer, float fScale);

	virtual void OnDrawFace(Gdiplus::Graphics& gcDrawer, float fScale);

	virtual	bool	UpdateMesh();
public:
	//////////////////////////////////////////////////////////////////////////
	//�ƶ�
	virtual void	OnTopMoveStart(const gp_Pnt& stInfo);
	virtual void	OnTopMoving(const gp_Pnt& stInfo);	//�ƶ����
	virtual void	OnTopMovingFinish(const gp_Pnt& stInfo, bool bCommitChange);	//ȡ���ƶ�
	virtual void	CancelTopMoving();

	virtual bool	HasTopCollisionWith(const TopoDS_Shape& theShape);
	const gp_XYZ& GetRawSize() const { return m_szRawSize; }
	gp_XYZ& GetRawSize() { return m_szRawSize; }
	void SetRawSize(const gp_XYZ& val) { m_szRawSize = val; }

	//////////////////////////////////////////////////////////////////////////
	//��ǽ�����ƶ�ʱ���ڵذ����ƶ�ʱ

	virtual void	OnSideMoveStart(const Gdiplus::PointF& ptStart);
	virtual void	OnSideMoving(const Gdiplus::PointF& ptOffset);

	virtual void	OnSideMovingFinish(const Gdiplus::PointF& ptOffset, bool bCommitChange);
	//ǿ��ֹͣ�ƶ������ص�ԭ����λ��
	virtual void	CancelSideMoving();

	virtual void	SetSideSize(const Gdiplus::RectF& rtTarget){m_rtArea = rtTarget; m_rtMovingArea = m_rtArea;};

private:
	gp_XYZ m_szRawSize;		//ԭʼ���ݴ�С
	
	float m_fYPos;
	CString m_strModelPath;
};

typedef	std::shared_ptr<CDecorODL>	CDecorODLSPtr;
typedef	std::weak_ptr<CDecorODL>	CDecorODLLWPtr;

#endif // DecorODL_h__
