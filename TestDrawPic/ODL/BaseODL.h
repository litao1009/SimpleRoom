#ifndef BaseODL_h__
#define BaseODL_h__

#pragma once
#include "..\\PublicDefine.h"
#include <vector>
#include <gdiplus.h>
#include <math.h>

#include "ODLTools.h"
#include "BaseODLFwd.h"
#include "BLL/BaseBLL.h"
#include "SceneNode/TSceneNode.h"
#include "TopoShape/CTopoShape.h"

#include "Geom_Curve.hxx"
#include "gp_pnt.hxx"

#include "TopoDS_Vertex.hxx"

#include <boost/optional.hpp>
#include "SkinUnitODL.h"

class CBaseODL : public TSceneNode<CBaseODL>
{
public:

	CBaseODL(void);
	virtual ~CBaseODL(void);

public:
	//////////////////////////////////////////////////////////////////////////
	//�������ݶ��󡢴�����
	//////////////////////////////////////////////////////////////////////////

	//���ݵ����д�������
	void	Create();
	bool			IsTopCreating(){return m_bIsTopCreating;};
	void			SetIsTopCreating(const bool& val) { m_bIsTopCreating = val; }

	virtual void RemoveSub(CBaseODLSPtr spSub);
	virtual void RemoveSelf();

	virtual ENUM_OBOJECT_TYPE GetType() {return m_ObjType; }
	void SetType(ENUM_OBOJECT_TYPE etType){m_ObjType = etType;}

	//Ƥ���������ⲿ����
	virtual CBaseODLSPtr	CreateSub(const Gdiplus::PointF& ptPos, CSkinUnitODLSPtr spSkin);

	virtual void SetSkin(CSkinUnitODLSPtr spSkin){m_spSkin=spSkin;};
	virtual CSkinUnitODLSPtr GetSkin()
	{
		if (!m_spSkin)
		{
			m_spSkin = std::make_shared<CSkinUnitODL>();
		}
		return m_spSkin;
	};

	bool& IsCanDelete() { return m_bIsCanDelete; }
	void SetIsCanDelete(const bool& val) { m_bIsCanDelete = val; }


	TopoDS_Shape	GetEdgeByPoint(gp_Pnt ptPos);

	//Rect����Object
	virtual bool	IsFaceInShape(BRepExtrema_DistShapeShape& dss);

	//�ཻ�ж�
	virtual bool	HasTopCollisionWith(const TopoDS_Shape& theShape);
	virtual	bool	UpdateMesh() { return false; }

	bool			IsHide() const { return m_bIsHide; }
	void			SetIsHide(const bool& val) { m_bIsHide = val; }

	bool			IsLocked() { return m_bIsLocked; }
	void			SetIsLocked(const bool& val) { m_bIsLocked = val; }

	bool			GetHasMark() const { return m_bHasMark; }
	void			SetHasMark(const bool& val) { m_bHasMark = val; }

	int&			GetIndex() { return m_nIndex; }
	void			SetIndex(const int& val) { m_nIndex = val; }

	ENUM_SIDE_POS GetSide() { return m_eSide; }
	void SetSide(const ENUM_SIDE_POS& val) { m_eSide = val; }

	const CTopoShape& GetFaceShape() const { return m_FaceShape; }
	CTopoShape&	GetFaceShape() { return m_FaceShape; }
	void			SetFaceShape(const CTopoShape& val) { m_FaceShape = val; }

	bool			GetHasLength() const { return m_bHasLength; }
	void			SetHasLength(const bool& val) { m_bHasLength = val; }
	//��ǰ����Ҫһ�����ط��ţ���ʾ�ȵ�
	virtual void SetIsCurrentHotspot(bool bIsCurrentHotspot){m_bIsCurrentHotspot=bIsCurrentHotspot;}
	virtual bool IsCurrentHotspot(){return m_bIsCurrentHotspot;}

protected:
	Gdiplus::FontFamily* m_pFontFamily;
	Gdiplus::SolidBrush* m_pFontBrush;
	Gdiplus::StringFormat m_FontFormat;

	CBaseBLLWPtr m_wpBaseBLL;

	CTopoShape	m_FaceShape;

	//�����Ƥ��
	CSkinUnitODLSPtr m_spSkin;

	//��ǰ���������
	ENUM_OBOJECT_TYPE m_ObjType;

	bool m_bIsHide;

	bool m_bIsLocked;

	ENUM_SIDE_POS m_eSide;		//1���ϣ�2���£�3������
	int m_nIndex;				//λ���ϼ������µ�Index

	bool m_bIsCanDelete;
	//���󴴽���
	bool m_bIsTopCreating;


	bool m_bHasMark;			//�б�ע
	bool m_bHasLength;			//�г�����ʾ
	bool m_bIsCurrentHotspot;
public:
	//////////////////////////////////////////////////////////////////////////
	//����ͼ���ݶ��󡢴�����
	//////////////////////////////////////////////////////////////////////////

	//����1��gdi����
	//����2����������
	//standby:������������
	//begin:���ÿ�ʼ��
	//moving:���ƿ�ʼ�㵽��ǰλ�õ�����
	//end:���ƽ������Ժ���ƵĶ�������������
	virtual void	OnDrawTop(Gdiplus::Graphics& gcDrawer, float fScale);

	void InitTopColor();
	virtual void	OnTopMoveStart(const gp_Pnt& stInfo);
	//�ƶ�����
	virtual void	OnTopMoving(const gp_Pnt& stInfo);

	virtual void	OnTopMovingFinish(const gp_Pnt& stInfo, bool bCommitChange);
	//ǿ��ֹͣ�ƶ������ص�ԭ����λ��
	virtual void	CancelTopMoving();

	//���ô�����ʼ�ĵ㣬������Ϊ������
	virtual void	SetTopStartPos(gp_Pnt ptStart)
	{
		m_ptTopStart = ptStart;
		m_ptTopStop = ptStart;
		m_ptTopCurrent = ptStart;
	}

	const gp_Pnt& GetTopStart() const { return m_ptTopStart; }

	const gp_Pnt& GetTopStop() const { return m_ptTopStop; }

	//���ô��������ĵ㣬������Ϊ�������
	virtual void	SetTopStopPos(gp_Pnt ptStop) {	m_ptTopStop = ptStop; }
	//���õ�ǰ������ڵĵ㣬
	virtual void	SetTopCurrentPos(gp_Pnt ptCurrent){ m_ptTopCurrent = ptCurrent;}

	bool			IsTopMoving(){return m_bIsTopMoving;};
	virtual void	SetIsTopMoving(bool bIsTopMoving){m_bIsTopMoving = bIsTopMoving;};

	bool			IsTopSelected(){return m_bIsTopSelected;};
	virtual void	SetIsTopSelected(bool isTopSelected);

	void	SetIsTopSelected(bool isTopSelected, bool bSetAll);

	bool			IsTopFocused() const { return m_bIsTopFocused; }
	void			SetIsTopFocused(const bool& val) { m_bIsTopFocused = val; }

	void			SetIsTopFocused(bool bIsFocused, bool bSetAll );

	bool			IsTopResizing(){return m_bIsTopResizing;};
	void			SetIsTopResizing(const bool& val) { m_bIsTopResizing = val; }

	virtual void	GetTopPointList(std::vector<gp_Pnt>& arrPoint){arrPoint = m_arrTopPoint;}
	virtual int		GetTopPointCount(){return m_arrTopPoint.size();};

	//ǿ���޸ĵ�����
	virtual void	SetTopPointList(std::vector<gp_Pnt> arrPoint){m_arrTopPoint = arrPoint; m_arrMovingTopPoint=m_arrTopPoint;};
protected:

	//��ʼ���ĵ��λ��
	gp_Pnt m_ptTopStart;
	//�����ʱ��ĵ��λ��
	gp_Pnt m_ptTopStop;
	//��ǰ���λ��
	gp_Pnt m_ptTopCurrent;

	//�����ƶ���
	bool m_bIsTopMoving;
	//����ѡ��
	bool m_bIsTopSelected;
	//�����ǽ���
	bool m_bIsTopFocused;
	//����ߴ�仯��
	bool m_bIsTopResizing;
	//����ͼ��
	std::vector<gp_Pnt> m_arrMovingTopPoint;
	std::vector<gp_Pnt> m_arrTopPoint;
public:
	//////////////////////////////////////////////////////////////////////////
	//������ͼ���ݶ��󡢴�����
	//////////////////////////////////////////////////////////////////////////
	void InitSideColor();
	//��ĳ�������������(ǽ�桢���桢�컨����)
	//�컨�塢�ذ���ʾ�����ݱ�ʾ���ӷ����ڹ۲�
	//ǽ�����ݣ���ǽ�ⲿ�۲�(Offset:λ���ϼ���ƫ��)
	virtual void	OnDrawFace(Gdiplus::Graphics& gcDrawer, float fScale);

	//�жϵ��Ƿ���������(Zoneר�ã�2ά��0�����ڣ�
	//1�����ϣ�2���ϣ�3�����ϣ�4���ң�5�����£�6���£�7�����£�8����10�����м�)
	virtual ENUM_RECT_SELECTED_POS IsSidePointInside(const Gdiplus::PointF& ptPos);

	virtual void	OnSideMoveStart(const Gdiplus::PointF& ptStart);
	virtual void	OnSideMoving(const Gdiplus::PointF& ptOffset);

	virtual void	OnSideMovingFinish(const Gdiplus::PointF& ptOffset, bool bCommitChange);
	//ǿ��ֹͣ�ƶ������ص�ԭ����λ��
	virtual void	CancelSideMoving(){};
	//
	virtual void	OnSideReSizeStart( const Gdiplus::PointF& ptStart){};
	//�޸ĳߴ���
	virtual void	OnSideReSizing( const Gdiplus::PointF& ptCurrent){};
	//�޸ĳߴ����
	virtual void	OnSideReSizeEnd( const Gdiplus::PointF& ptEnd){};

	//��ȡ��ǰ��������λ�úͳߴ�
	virtual void	GetSideRect(Gdiplus::RectF& rtSelf){rtSelf = m_rtArea;};
	//��ȡ���˿��Ա����С�ߴ�
	virtual bool	GetSideMinRect(Gdiplus::RectF& rtLimit,int& nCount);
	//��ȡ����������õ�����,����false�����ܷ���
	virtual bool	GetSideFitRect(CBaseODLSPtr spSelect, bool bUpdateSelf, bool bIsMax, Gdiplus::RectF& rtSelf);
	//��ȡ��С���õ�X����
	virtual bool	GetSideMinLimitXPos(CBaseODLSPtr spSelect, Gdiplus::REAL& fXPos);
	//��ȡ�����õ�X����
	virtual bool	GetSideMaxLimitXPos(CBaseODLSPtr spSelect, Gdiplus::REAL& fXPos);
	//��ȡ��С���õ�Y����
	virtual bool	GetSideMinLimitYPos(CBaseODLSPtr spSelect, Gdiplus::REAL& fXPos);
	//��ȡ�����õ�Y����
	virtual bool	GetSideMaxLimitYPos(CBaseODLSPtr spSelect, Gdiplus::REAL& fXPos);

	virtual ENUM_RECT_SELECTED_POS		GetSideSelectedPos(){return m_eSelectedPos;};

	virtual void	GetSidePointList(std::vector<Gdiplus::PointF>& arrPoint);
	virtual int		GetSidePointCount(){return 0;};

	virtual void	SetSideGolbalOffset(const Gdiplus::PointF& ptOffset){m_ptSideGlobalOffset = ptOffset;};

	virtual void	UpdateSideGlobalOffset(const Gdiplus::PointF& ptOffset);

	virtual void	SetSideStartPos(const Gdiplus::PointF& ptStart){m_ptSideStart = ptStart;};
	virtual void	SetSideStopPos(const Gdiplus::PointF& ptStop){m_ptSideStop = ptStop;};


	//��鴫��������Ѿ����ڵĶ����Ƿ��в��У���ȥ����Ķ���(���ж���)
	virtual bool	CheckSideIntersected(CBaseODLSPtr spExcept, Gdiplus::RectF& rtCheck);
	virtual void	SetSideSize(const Gdiplus::RectF& rtTarget){};
	virtual void	SetSideLimitArea(const Gdiplus::RectF& rtLimit){};

	bool			IsSideMoving(){return m_bIsSideMoving;};
	virtual void	SetIsSideMoving(bool bIsSideMoving){m_bIsSideMoving = bIsSideMoving;};

	bool			IsSideSelected(){return m_bIsSideSelected;};
	virtual void	SetIsSideSelected(bool isSideSelected);

	bool			IsSideFocused() const { return m_bIsSideFocused; }
	void			SetIsSideFocused(const bool& val) { m_bIsSideFocused = val; }

	bool			IsSideResizing(){return m_bIsSideResizing;};
	void			SetIsSideResizing(const bool& val) { m_bIsSideResizing = val; }
	//�������״̬
	void SetClearSideState(bool bAllSet);
protected:
	Gdiplus::Color m_clPenColor;
	Gdiplus::Color m_clDotColor;
	Gdiplus::Color m_clResizeColor;
	//����ͼ�ϵ���Ϣ
	Gdiplus::PointF m_ptSideStart;
	Gdiplus::PointF m_ptSideStop;

	//��ǰ��������ڶ��������ƫ��(X0,Y0�㣬��������������������λ���ϼ���ƫ��)
	Gdiplus::PointF m_ptSideGlobalOffset;

	ENUM_RECT_SELECTED_POS m_eSelectedPos;
	//�����ƶ���
	bool m_bIsSideMoving;
	//����ѡ��
	bool m_bIsSideSelected;
	//�����ǽ���
	bool m_bIsSideFocused;
	//����ߴ�仯��
	bool m_bIsSideResizing;
	//��������Ĵ�С��λ��ʼ��Ϊ��ǰ�������ϼ������ƫ��
	Gdiplus::RectF m_rtTopArea;
	Gdiplus::RectF m_rtMovingTopArea;
	Gdiplus::RectF m_rtLimitTopArea;		//����ֻ���ڴ˷�Χ���ƶ�(ǽ��)

	Gdiplus::RectF m_rtArea;
	Gdiplus::RectF m_rtMovingArea;
	Gdiplus::RectF m_rtLimitArea;		//����ֻ���ڴ˷�Χ���ƶ�(ǽ��)
	//���ϵ�����ĵ�
	std::vector<Gdiplus::PointF>	m_arrSidePoint;
protected:
	
	//���ű���
	//float m_fScale;
	
	
};

#endif // BaseODL_h__
