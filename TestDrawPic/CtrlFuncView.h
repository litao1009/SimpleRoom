#pragma once


// CCtrlFuncView 视图
//////////////////////////////////////////////////////////////////////////
//这个类拿来保存操作函数
//创建墙、修改墙、移动鼠标、等等
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
	CCtrlFuncView();           // 动态创建所使用的受保护的构造函数
	virtual ~CCtrlFuncView();
public:
	//获取指定类型的对象
	bool GetFocused(CBaseODLSPtr pObj, CBaseODLSPtr& pFocused, std::vector<ENUM_OBOJECT_TYPE>& arrType);
	bool SetPointInObjFocused(CBaseODLSPtr pObj, gp_Pnt pt, CBaseODLSPtr pSelected);
	bool SetPointInObjHighLight(CBaseODLSPtr pObj, gp_Pnt pt);

	//获取当前直线穿过的梁、柱、墙、地板、照相机(从下往上)
	CBaseODLSPtr GetTopOnMouseObj(const TopoDS_Shape& inShape, CBaseODLSPtr pParent, CBaseODLSPtr spExcept);
	
	//获取当前直线穿过的梁、柱、墙、地板、照相机
	void GetTopOnMouseObj(const TopoDS_Shape& inShape, CBaseODLSPtr pParent, CBaseODLSPtr spExcept, std::vector<CBaseODLSPtr>& arrObj);

	int SetSideSelectedObj(Gdiplus::PointF& ptPos, CBaseODLSPtr spObj);

	int SetTopSelectedObj(Gdiplus::PointF& ptPos, CBaseODLSPtr spObj);
	//获取当前点所在最底层的对象
	CBaseODLSPtr GetSidePointInLeafObj(const Gdiplus::PointF& ptPos, CBaseODLSPtr spObj);
	CBaseODLSPtr GetSidePointAtLeafObj(const Gdiplus::PointF& ptPos, CBaseODLSPtr spObj);

	int GetSidePointAtLeafObj(const Gdiplus::PointF& ptPos, CBaseODLSPtr spObj, std::vector<CBaseODLSPtr>& arrResizeObj);

protected:
	bool m_bIsSubViewerHide;

	CDesignODLSPtr m_pObjMgr;
	//子窗口显示
	CSubViewer m_vwSubViewPort;

	CGuideLineODLSPtr m_pGuideLine;
	CGuideRectODLSPtr m_pGuideRect;

	CDecorODLSPtr m_pDecor;
	
	//当前被编辑的对象
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
	//开始建墙Line
	void CreateWallLineStart(Gdiplus::PointF& ptPos);
	//建墙中Line
	void CreateWallLineOnMove(Gdiplus::PointF& ptPos);
	//建墙结束Line
	void CreateWallLineFinish(Gdiplus::PointF& ptPos);
	//建墙Rect
	void CreateWallRect(Gdiplus::PointF& ptPos);
	void CreateWallRectOnMove(Gdiplus::PointF& ptPos);
	//画梁Rect
	void CreateBeamRect(Gdiplus::PointF& ptPos);
	//画柱Rect
	void CreatePillarRect(Gdiplus::PointF& ptPos);

	//////////////////////////////////////////////////////////////////////////
	//左键按下默认事件
	void OnTopMoveStart( Gdiplus::PointF& ptPos );
	//移动中
	int OnTopMoveGoing(Gdiplus::PointF& ptPos);
	//移动对象结束，返回有几个对象在移动
	int OnTopMoveEnd( Gdiplus::PointF& ptPos );
	//移动画布
	void OnTopMoveScene(Gdiplus::PointF& ptPos);

	//////////////////////////////////////////////////////////////////////////
	//左键按下默认事件
	void OnSideMoveStart( Gdiplus::PointF& ptPos );
	//移动中
	int OnSideMoveGoing(Gdiplus::PointF& ptPos);
	//移动对象结束，返回有几个对象在移动
	int OnSideMoveEnd( Gdiplus::PointF& ptPos );
	//移动画布
	void OnSideMoveScene(Gdiplus::PointF& ptPos);

	//////////////////////////////////////////////////////////////////////////
	void OnDrawRect(Gdiplus::PointF ptBegin, Gdiplus::PointF ptEnd);


	//////////////////////////////////////////////////////////////////////////
	
public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
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


