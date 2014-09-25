#ifndef WallFaceZoneODL_h__
#define WallFaceZoneODL_h__

#pragma once
//////////////////////////////////////////////////////////////////////////
//�������ǽ��ķ�����Ϣ��ֻ��2ά����
//����λ�á���С������ƽ�̷�ʽ
//����ĵ���Ϣ�����ڣ�X/y�У�Z=0
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
	//ǿ��ֹͣ�ƶ������ص�ԭ����λ��
	virtual void	CancelSideMoving();
	//////////////////////////////////////////////////////////////////////////
	//�޸ĳߴ�
	//nPos:1�����ϣ�2���ϣ�3�����ϣ�4���ң�5�����£�6���£�7�����£�8����
	virtual void	OnSideReSizeStart(const Gdiplus::PointF& ptStart);
	virtual void OnSideReSizing( const Gdiplus::PointF& ptCurrent);
	void	OnSideReSizing(ENUM_RECT_SELECTED_POS ePos, const Gdiplus::PointF& ptCurrent);
	virtual void	OnSideReSizeEnd(const Gdiplus::PointF& ptStop);
	//////////////////////////////////////////////////////////////////////////
	//ֻ���ڴ������ƶ�
	
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
