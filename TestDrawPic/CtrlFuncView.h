#pragma once


// CCtrlFuncView ��ͼ
//////////////////////////////////////////////////////////////////////////
//��������������������
//����ǽ���޸�ǽ���ƶ���ꡢ�ȵ�
//////////////////////////////////////////////////////////////////////////
#include "ShowMgr/DrawEngine.h"
#include "ShowMgr/DrawImageEngine.h"
#include "ShapeTool.h"
#include "SubViewer.h"

#include "ODL/BaseODL.h"
#include "ODL/GuideLineODL.h"
#include "ODL/GuideRectODL.h"
#include "ODL/DesignODL.h"
#include "ODL/WallFaceODL.h"
#include "ODL/DecorODL.h"
#include "BRepBuilderAPI_MakeVertex.hxx"

class CCtrlFuncView : public CView
{
	DECLARE_DYNCREATE(CCtrlFuncView)

protected:
	CCtrlFuncView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CCtrlFuncView();
public:
	//��ȡָ�����͵Ķ���
	bool GetFocused(CBaseODLSPtr pObj, CBaseODLSPtr& pFocused, std::vector<ENUM_OBOJECT_TYPE>& arrType);
	bool SetPointInObjFocused(CBaseODLSPtr pObj, gp_Pnt pt, CBaseODLSPtr pSelected);
	bool SetPointInObjHighLight(CBaseODLSPtr pObj, gp_Pnt pt);

	//��ȡ��ǰֱ�ߴ�������������ǽ���ذ塢�����(��������)
	CBaseODLSPtr GetTopOnMouseObj(const TopoDS_Shape& inShape, CBaseODLSPtr pParent, CBaseODLSPtr spExcept);
	
	//��ȡ��ǰֱ�ߴ�������������ǽ���ذ塢�����
	void GetTopOnMouseObj(const TopoDS_Shape& inShape, CBaseODLSPtr pParent, CBaseODLSPtr spExcept, std::vector<CBaseODLSPtr>& arrObj);

	int SetSideSelectedObj(Gdiplus::PointF& ptPos, CBaseODLSPtr spObj);

	int SetTopSelectedObj(Gdiplus::PointF& ptPos, CBaseODLSPtr spObj);
	//��ȡ��ǰ��������ײ�Ķ���
	CBaseODLSPtr GetSidePointInLeafObj(const Gdiplus::PointF& ptPos, CBaseODLSPtr spObj);
	CBaseODLSPtr GetSidePointAtLeafObj(const Gdiplus::PointF& ptPos, CBaseODLSPtr spObj);

	int GetSidePointAtLeafObj(const Gdiplus::PointF& ptPos, CBaseODLSPtr spObj, std::vector<CBaseODLSPtr>& arrResizeObj);

protected:
	bool m_bIsSubViewerHide;

	CDesignODLSPtr m_pObjMgr;
	//�Ӵ�����ʾ
	CSubViewer m_vwSubViewPort;

	CGuideLineODLSPtr m_pGuideLine;
	CGuideRectODLSPtr m_pGuideRect;

	CDecorODLSPtr m_pDecor;
	
	//��ǰ���༭�Ķ���
	CBaseODLSPtr m_pCurEditObj;

	CBaseODLSPtr m_pCurResizeObj;

	CBaseODLSPtr m_pCurMoveObj;

	Gdiplus::REAL m_fScrollHeight;
	Gdiplus::REAL m_fScrollWidth;

	Gdiplus::REAL m_fPixX;
	Gdiplus::REAL m_fPixY;

	Gdiplus::REAL m_fWallHeight;
	Gdiplus::REAL m_fWallThick;
	Gdiplus::REAL m_fBeamHeight;
	Gdiplus::REAL m_fPillarHeight;

	Gdiplus::PointF m_pDrawStartPoint;
	Gdiplus::PointF m_pDrawStopPoint;

	Gdiplus::PointF m_pMoveStartPoint;
	Gdiplus::PointF m_pMoveStopPoint;

	BOOL m_bIsDrawing;
	bool m_bLButtonDown;

	bool m_bMButtonDown;

	ENUM_OPERAT_TYPE m_eCurOpt;
	ENUM_ViewType m_eCurViewType;

protected:
	//��ʼ��ǽLine
	void CreateWallLineStart(Gdiplus::PointF& ptPos);
	//��ǽ��Line
	void CreateWallLineOnMove(Gdiplus::PointF& ptPos);
	//��ǽ����Line
	void CreateWallLineFinish(Gdiplus::PointF& ptPos);
	//��ǽRect
	void CreateWallRect(Gdiplus::PointF& ptPos);
	void CreateWallRectOnMove(Gdiplus::PointF& ptPos);
	//����Rect
	void CreateBeamRect(Gdiplus::PointF& ptPos);
	//����Rect
	void CreatePillarRect(Gdiplus::PointF& ptPos);

	//////////////////////////////////////////////////////////////////////////
	//�������Ĭ���¼�
	void OnTopMoveStart( Gdiplus::PointF& ptPos );
	//�ƶ���
	int OnTopMoveGoing(Gdiplus::PointF& ptPos);
	//�ƶ���������������м����������ƶ�
	int OnTopMoveEnd( Gdiplus::PointF& ptPos );
	//�ƶ�����
	void OnTopMoveScene(Gdiplus::PointF& ptPos);

	//////////////////////////////////////////////////////////////////////////
	//�������Ĭ���¼�
	void OnSideMoveStart( Gdiplus::PointF& ptPos );
	//�ƶ���
	int OnSideMoveGoing(Gdiplus::PointF& ptPos);
	//�ƶ���������������м����������ƶ�
	int OnSideMoveEnd( Gdiplus::PointF& ptPos );
	//�ƶ�����
	void OnSideMoveScene(Gdiplus::PointF& ptPos);

	//////////////////////////////////////////////////////////////////////////
	void OnDrawRect(Gdiplus::PointF ptBegin, Gdiplus::PointF ptEnd);


	//////////////////////////////////////////////////////////////////////////
	
public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


