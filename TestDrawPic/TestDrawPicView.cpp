// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// TestDrawPicView.cpp : CTestDrawPicView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。

#ifndef SHARED_HANDLERS
#include "TestDrawPic.h"
#endif

#include "TestDrawPicDoc.h"
#include "TestDrawPicView.h"
#include "MainFrm.h"
#include "DlgRoomStyle.h"
#include "irrEngine/SRenderContext.h"

#include "ODL/CameraODL.h"
#include "ODL/DecorODL.h"
#include "ODL/WallFaceODL.h"
#include "ODL/WallFaceZoneODL.h"
#include "ODL/FloorODL.h"
#include "ODL/FloorZoneODL.h"

#include "DlgSizeInfo.h"
#include "DlgZoneInfo.h"

#include "BRepBuilderAPI_MakeEdge.hxx"

#include <boost/timer/timer.hpp>
#include "gce_MakeLin.hxx"
// #ifdef _DEBUG
// #define new DEBUG_NEW
// #endif

// CTestDrawPicView

IMPLEMENT_DYNCREATE(CTestDrawPicView, CCtrlFuncView)

BEGIN_MESSAGE_MAP(CTestDrawPicView, CCtrlFuncView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CCtrlFuncView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CCtrlFuncView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTestDrawPicView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_COMMAND(ID_BTN_CREATE_WALL_LINE, &CTestDrawPicView::OnBtnCreateWallLine)
	ON_UPDATE_COMMAND_UI(ID_BTN_CREATE_WALL_LINE, &CTestDrawPicView::OnUpdateBtnCreateWallLine)

	ON_COMMAND(ID_BTN_CREATE_WALL_RECT, &CTestDrawPicView::OnBtnCreateWallRect)
	ON_UPDATE_COMMAND_UI(ID_BTN_CREATE_WALL_RECT, &CTestDrawPicView::OnUpdateBtnCreateWallRect)

	ON_COMMAND(ID_BTN_CREATE_BEAM_RECT, &CTestDrawPicView::OnBtnCreateBeamRect)
	ON_UPDATE_COMMAND_UI(ID_BTN_CREATE_BEAM_RECT, &CTestDrawPicView::OnUpdateBtnCreateBeamRect)

	ON_COMMAND(ID_BTN_CREATE_PILLAR_RECT, &CTestDrawPicView::OnBtnCreatePillarRect)
	ON_UPDATE_COMMAND_UI(ID_BTN_CREATE_PILLAR_RECT, &CTestDrawPicView::OnUpdateBtnCreatePillarRect)

	ON_COMMAND(ID_BTN_CREATE_ROOM_AUTO, &CTestDrawPicView::OnBtnCreateRoomAuto)
	ON_UPDATE_COMMAND_UI(ID_BTN_CREATE_ROOM_AUTO, &CTestDrawPicView::OnUpdateBtnCreateRoomAuto)

	ON_COMMAND(ID_BTN_CREATE_ROOM_STYLE, &CTestDrawPicView::OnBtnCreateRoomStyle)
	ON_UPDATE_COMMAND_UI(ID_BTN_CREATE_ROOM_STYLE, &CTestDrawPicView::OnUpdateBtnCreateRoomStyle)

	//////////////////////////////////////////////////////////////////////////
	//编辑

	ON_COMMAND(ID_BTN_EDIT_WALL_SKIN, &CTestDrawPicView::OnBtnEditWallSkin)
	ON_UPDATE_COMMAND_UI(ID_BTN_EDIT_WALL_SKIN, &CTestDrawPicView::OnUpdateBtnEditWallSkin)

	ON_COMMAND(ID_BTN_EDIT_FLOOR_SKIN, &CTestDrawPicView::OnBtnEditFloorSkin)
	ON_UPDATE_COMMAND_UI(ID_BTN_EDIT_FLOOR_SKIN, &CTestDrawPicView::OnUpdateBtnEditFloorSkin)

	ON_COMMAND(ID_BTN_EDIT_WALL_ZONE, &CTestDrawPicView::OnBtnEditWallZone)
	ON_UPDATE_COMMAND_UI(ID_BTN_EDIT_WALL_ZONE, &CTestDrawPicView::OnUpdateBtnEditWallZone)

	ON_COMMAND(ID_BTN_EDIT_FLOOR_ZONE, &CTestDrawPicView::OnBtnEditFloorZone)
	ON_UPDATE_COMMAND_UI(ID_BTN_EDIT_FLOOR_ZONE, &CTestDrawPicView::OnUpdateBtnEditFloorZone)

	//////////////////////////////////////////////////////////////////////////
	//查看

	ON_COMMAND(ID_BTN_VIEW_SHOW, &CTestDrawPicView::OnBtnViewShow)
	ON_UPDATE_COMMAND_UI(ID_BTN_VIEW_SHOW, &CTestDrawPicView::OnUpdateBtnViewShow)

	ON_COMMAND(ID_BTN_VIEW_HIDE, &CTestDrawPicView::OnBtnViewHide)
	ON_UPDATE_COMMAND_UI(ID_BTN_VIEW_HIDE, &CTestDrawPicView::OnUpdateBtnViewHide)

	//////////////////////////////////////////////////////////////////////////
	//视觉视图
	ON_COMMAND(ID_BTN_VIEW_TOP, &CTestDrawPicView::OnBtnViewTop)
	ON_UPDATE_COMMAND_UI(ID_BTN_VIEW_TOP, &CTestDrawPicView::OnUpdateBtnViewTop)

	ON_COMMAND(ID_BTN_VIEW_RENDER, &CTestDrawPicView::OnBtnViewRender)
	ON_UPDATE_COMMAND_UI(ID_BTN_VIEW_RENDER, &CTestDrawPicView::OnUpdateBtnViewRender)

	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_SUB_MENU_PROPERTY, &CTestDrawPicView::OnSubMenuProperty)

END_MESSAGE_MAP()

// CTestDrawPicView 构造/析构

CTestDrawPicView::CTestDrawPicView()
{
	// TODO: 在此处添加构造代码
	m_clGrout = Gdiplus::Color::Bisque;
	m_pDrop=nullptr;
}

CTestDrawPicView::~CTestDrawPicView()
{
	if (m_pDrop)
	{
		delete m_pDrop;
		m_pDrop=nullptr;
	}
}

BOOL CTestDrawPicView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CCtrlFuncView::PreCreateWindow(cs);
}

// CTestDrawPicView 绘制

void CTestDrawPicView::OnDraw(CDC* pDC)
{
	CTestDrawPicDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CDrawEngine::GetInstance().SetDC(pDC->GetSafeHdc());
	CRect rt;
	GetClientRect(rt);
	
	if (m_eCurViewType==EVT_TOP)
	{
		CDrawEngine::GetInstance().OnDrawTop(m_pObjMgr, static_cast<Gdiplus::REAL>(rt.Width()), static_cast<Gdiplus::REAL>(rt.Height()));
	}
	if (( (m_eCurViewType==EVT_FACE) ) && (m_pCurEditObj!=nullptr))
	{
		CDrawEngine::GetInstance().OnDrawFace(m_pCurEditObj, static_cast<Gdiplus::REAL>(rt.Width()), static_cast<Gdiplus::REAL>(rt.Height()));
	}
}

void CTestDrawPicView::OnInitialUpdate()
{
	CCtrlFuncView::OnInitialUpdate();

	
}


// CTestDrawPicView 打印


void CTestDrawPicView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTestDrawPicView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CTestDrawPicView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CTestDrawPicView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CTestDrawPicView 诊断

#ifdef _DEBUG
void CTestDrawPicView::AssertValid() const
{
	CCtrlFuncView::AssertValid();
}

void CTestDrawPicView::Dump(CDumpContext& dc) const
{
	CCtrlFuncView::Dump(dc);
}

CTestDrawPicDoc* CTestDrawPicView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestDrawPicDoc)));
	return (CTestDrawPicDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestDrawPicView 消息处理程序

void CTestDrawPicView::InitDrawEngine()
{
	m_fPixX =10;// dc.GetDeviceCaps(LOGPIXELSX);
	m_fPixY =10;// dc.GetDeviceCaps(LOGPIXELSY);
	m_fScrollWidth = 20000;
	m_fScrollHeight = 20000;
	CDrawEngine::GetInstance().Create(m_fScrollWidth, m_fScrollHeight);
	Gdiplus::REAL fScale = 1024.0f / 10000.0f;
	CDrawEngine::GetInstance().SetScale(fScale);
	CDrawEngine::GetInstance().SetTopOffset(0, 0);
	
	CDrawEngine::GetInstance().SetSideOffset(0, 0);
}

BOOL CTestDrawPicView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL bRet = CCtrlFuncView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	
	InitDrawEngine();
	m_vwSubViewPort.Create(_T(""), WS_VISIBLE|WS_CHILD|SS_NOTIFY|SS_OWNERDRAW|WS_CLIPSIBLINGS,CRect(0,600,200,200),this,IDR_VIEW_SUBVIEWER);
	m_pObjMgr = std::make_shared<CDesignODL>(m_vwSubViewPort.GetSafeHwnd());
	m_pObjMgr->Init();
	m_vwSubViewPort.SetRenderContext(m_pObjMgr->GetRenderContext());

	m_pDrop=new COleDropTarget();

	if (!this->m_pDropTarget)
	{
		m_pDrop->Register(this);
	}
	return bRet;
}


void CTestDrawPicView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//判断鼠标在对象内没，在则启动移动操作
	
	Gdiplus::PointF pt(static_cast<Gdiplus::REAL>(point.x), static_cast<Gdiplus::REAL>(point.y));
	
	//计算点偏移到原点的距离
	switch (m_eCurViewType)
	{
	case EVT_TOP:
		{
			CDrawEngine::GetInstance().TranslateTopOffsetPos(pt);
			m_pDrawStartPoint = pt;
			switch (m_eCurOpt)
			{
			case EOT_Create_Wall_Line:
				{
					CreateWallLineStart(m_pDrawStartPoint);
					GetMainFrame()->RefreshLayer(this, m_pObjMgr);
				}
				break;
			case EOT_Create_Wall_Rect:
				{
					//要创建墙
					CreateWallRect(pt);
					GetMainFrame()->RefreshLayer(this, m_pObjMgr);
				}
				break;
			case EOT_Create_Beam_Rect:
				{
					//梁
					CreateBeamRect(pt);
					GetMainFrame()->RefreshLayer(this, m_pObjMgr);
				}
				break;
			case EOT_Create_Pillar_Rect:
				{
					//柱
					CreatePillarRect(pt);
					GetMainFrame()->RefreshLayer(this, m_pObjMgr);
				}
				break;
			case EOT_Create_None:
				{
					//判断哪个东西被选中
					OnTopMoveStart(pt);
				}
			}

			Invalidate();
		}
		break;
	case EVT_FACE:
		{
			CDrawEngine::GetInstance().TranslateSideOffsetPos(pt);
			m_pDrawStartPoint = pt;
			switch (m_eCurOpt)
			{
			case EOT_Create_None:
			default:
				{
					//判断当前选中的是某一个Zone，还是Zone的一边
					//如果是Zone：下一步操作->移动Zone
					//如果是边：下一步操作->ReSize
					//移动Zone
					OnSideMoveStart(pt);
				}
			}
		}
		break;
	}
	m_bLButtonDown=true;
	CCtrlFuncView::OnLButtonDown(nFlags, point);
}

void CTestDrawPicView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bLButtonDown=false;
	Gdiplus::PointF pt(static_cast<Gdiplus::REAL>(point.x), static_cast<Gdiplus::REAL>(point.y));
	switch (m_eCurViewType)
	{
	case EVT_TOP:
		{
			CDrawEngine::GetInstance().TranslateTopOffsetPos(pt);

			int nCount =0;			//当前被选中移动的对象数量
			if (m_eCurOpt==EOT_Create_None)
			{
				nCount = OnTopMoveEnd(pt);
			}
			if (nCount==0)
			{
				if (m_eCurOpt==EOT_Create_None)
				{
					CDrawEngine::GetInstance().SetIsDrag(false);
					OnDrawRect(m_pMoveStartPoint, pt);
				}
			}
		}
		break;
	case EVT_FACE:
		{
			CDrawEngine::GetInstance().TranslateSideOffsetPos(pt);
			int nCount =0;			//当前被选中移动的对象数量
			//if (m_eCurOpt==EOT_Create_None)
			{
				nCount = OnSideMoveEnd(pt);
			}
			if (nCount==0)
			{
				//if (m_eCurOpt==EOT_Create_None)
				{
					CDrawEngine::GetInstance().SetIsDrag(false);
					OnDrawRect(m_pMoveStartPoint, pt);
				}
			}
		}
		break;
	}
	Invalidate();
	CCtrlFuncView::OnLButtonUp(nFlags, point);
}

void CTestDrawPicView::OnMouseMove(UINT nFlags, CPoint point)
{
	//找看看有米有被选中的，有，且当前左键按下，则移动该对象
	TRACKMOUSEEVENT tEventTrack = {0};
	tEventTrack.cbSize = sizeof(tEventTrack);
	tEventTrack.dwFlags = TME_LEAVE;
	tEventTrack.dwHoverTime = 1;
	tEventTrack.hwndTrack = GetSafeHwnd();
	BOOL bRet = _TrackMouseEvent(&tEventTrack);

// 	static uint32_t count = 0;
// 	boost::timer::cpu_timer ct;
	//static int nMove = 0;
	//nMove++;
	Gdiplus::PointF pt(static_cast<Gdiplus::REAL>(point.x), static_cast<Gdiplus::REAL>(point.y));
	switch (m_eCurViewType)
	{
	case EVT_TOP:
		{
			CDrawEngine::GetInstance().TranslateTopOffsetPos(pt);
			//TRACE(_T("POSX,Y: %f,%f\n"), pt.X, pt.Y);
			if (m_bLButtonDown)
			{
				if (m_eCurOpt==EOT_Create_None)
				{
					if (pt.Equals(m_pMoveStartPoint))
					{
						return;
					}
					//如果有个装饰品在被拖动
					if (m_pCurMoveObj)
					{
						
					}
					if (OnTopMoveGoing(pt)==0)
					{
						CDrawEngine::GetInstance().SetDragPos(m_pMoveStartPoint, pt, true);
					}
						Invalidate();
				}
			}
			else
			{
				//鼠标没有按下
				if (m_eCurOpt==EOT_Create_None)
				{
					int nCount = 0;
					//SetPointInObjHighLight(m_pObjMgr, stInfo);
					//左键下去,判断有没有东西被选中，没有则显示 Drag区域
				}
				//TRACE("SetPointInObjHighLight,%s", boost::timer::format(ct.elapsed()).c_str());
			}
			if (m_bIsDrawing)
			{
				m_pDrawStopPoint = pt;
				switch (m_eCurOpt)
				{
				case EOT_Create_Wall_Line:
					{
						CreateWallLineOnMove(m_pDrawStopPoint);
							Invalidate();
					}
					break;
				case EOT_Create_Wall_Rect:
				case EOT_Create_Pillar_Rect:
				case EOT_Create_Beam_Rect:
					{
						CreateWallRectOnMove(m_pDrawStopPoint);
							Invalidate();
					}
					break;
				}
			}
			
			if (m_bMButtonDown)
			{
				//中键在按下中
				//移动窗口
				OnTopMoveScene(pt);
					Invalidate();
			}
		}
		break;
	case EVT_FACE:
		{
			CDrawEngine::GetInstance().TranslateSideOffsetPos(pt);
			if (m_bLButtonDown)
			{
				//if (m_eCurOpt==EOT_Create_None)
				{
					if (pt.Equals(m_pMoveStartPoint))
					{
						return;
					}

					if (OnSideMoveGoing(pt)==0)
					{
						CDrawEngine::GetInstance().SetDragPos(m_pMoveStartPoint, pt, true);
					}
						Invalidate();
				}
			}
			else
			{
				//鼠标没有按下
				if (m_eCurOpt==EOT_Create_None)
				{
					int nCount = 0;
					gp_Pnt stInfo;
					stInfo.SetX(pt.X);
					stInfo.SetY(0);
					stInfo.SetZ(pt.Y);
					//SetPointInObjHighLight(m_pObjMgr, stInfo);
					//左键下去,判断有没有东西被选中，没有则显示 Drag区域
				}
				OnSideMoveGoing(pt);
				//TRACE("SetPointInObjHighLight,%s", boost::timer::format(ct.elapsed()).c_str());
			}
			if (m_bMButtonDown)
			{
				//中键在按下中
				//移动窗口
				OnSideMoveScene(pt);
					Invalidate();
			}

		}
		break;
	}
	
	m_vwSubViewPort.Invalidate();

	CCtrlFuncView::OnMouseMove(nFlags, point);

	//TRACE("Count:%d,%s", count++, boost::timer::format(ct.elapsed()).c_str());
}

void CTestDrawPicView::OnRButtonDown(UINT nFlags, CPoint point)
{
	Gdiplus::PointF pt(static_cast<Gdiplus::REAL>(point.x), static_cast<Gdiplus::REAL>(point.y));
	switch (m_eCurViewType)
	{
	case EVT_TOP:
		{
			CDrawEngine::GetInstance().TranslateTopOffsetPos(pt);
			switch (m_eCurOpt)
			{
			case EOT_Create_Wall_Line:
				{
					//正在画墙
					if (m_bIsDrawing)
					{
						//结束画墙
						m_bIsDrawing=FALSE;
						CreateWallLineFinish(pt);
						GetMainFrame()->RefreshLayer(this, m_pObjMgr);
					}
				}
				break;

			case EOT_Create_Wall_Face:
				{
					//创建墙纸点右键
					if (m_pCurEditObj)
					{
						if (m_pCurEditObj->GetType()==EOBT_Wall_Face)
						{
							m_eCurViewType = EVT_FACE;
						}
					}
				}
				break;

			}
		}
		break;
	case EVT_FACE:
		{
			CDrawEngine::GetInstance().TranslateSideOffsetPos(pt);

		}
		break;
	}
	Invalidate();
	CCtrlFuncView::OnRButtonDown(nFlags, point);
}

void CTestDrawPicView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	if (m_eCurOpt != EOT_Create_None)
	{
		m_eCurOpt = EOT_Create_None;
	}
	else
	{
		if ((m_pCurMoveObj) || (m_pCurEditObj))
		{
			OnContextMenu(this, point);
		}
	}
}

void CTestDrawPicView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{

	CMenu menu;
	menu.LoadMenu(IDR_MENU_POPUP);
	CMenu* pPopup = menu.GetSubMenu(0);

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,GetParent());

}


void CTestDrawPicView::OnSize(UINT nType, int cx, int cy)
{
	CCtrlFuncView::OnSize(nType, cx, cy);

	if (this->GetSafeHwnd())
	{
		static auto firstUpdate = true;
		
		CRect rtMain;
		GetWindowRect(rtMain);

		if (m_vwSubViewPort.GetSafeHwnd())
		{
			if ( firstUpdate )
			{
				m_vwSubViewPort.MoveWindow(0,rtMain.Height()-200, 200,200);
				firstUpdate = false;
			}
			else
			{
				CRect rtSub;
				m_vwSubViewPort.GetWindowRect(rtSub);
				m_vwSubViewPort.MoveWindow(0, rtMain.Height()-rtSub.Height(), rtSub.Width(), rtSub.Height());
			}
		}
	}
}


void CTestDrawPicView::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//发现鼠标出去了
	if (m_eCurOpt==EOT_Create_None)
	{
		//ClearState();

	}
	CCtrlFuncView::OnMouseLeave();
}

void CTestDrawPicView::ClearState()
{
	//清除所有正在创建的对象
	//清除所有未创建完成的对象
	m_pCurMoveObj = nullptr;
	m_pCurEditObj = nullptr;
	m_pCurResizeObj = nullptr;
	//清除操作状态
	if (m_eCurOpt!=EOT_Create_None)
	{
		/*if (m_pGuideLine)
		{
		RemoveObj((CBaseObjectBLL*)m_pGuideLine);
		m_pGuideLine = nullptr;
		}*/
		m_eCurOpt = EOT_Create_None;
	}
	//如果当前有选中的对象正在移动的，取消移动状态
	if (m_eCurViewType = EVT_TOP)
	{
		for (auto& curItem : m_pObjMgr->GetChildrenList() )
		{
			curItem->CancelTopMoving();
		}
	}
	if (m_eCurViewType==EVT_FACE)
	{

		for (auto& curItem : m_pObjMgr->GetChildrenList() )
		{
			curItem->CancelSideMoving();
		}
	}
	
	m_bLButtonDown =false;
	m_bMButtonDown=false;
	Invalidate();
}

void CTestDrawPicView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// 此功能要求 Windows Vista 或更高版本。
	// _WIN32_WINNT 符号必须 >= 0x0600。
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CCtrlFuncView::OnMouseHWheel(nFlags, zDelta, pt);
}


BOOL CTestDrawPicView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rtMain;
	GetWindowRect(rtMain);
	if (rtMain.Width()<=0)
	{
		return TRUE;
	}
	if (-zDelta>rtMain.Width())
	{
		return TRUE;
	}
	if (-zDelta>rtMain.Height())
	{
		return TRUE;
	}
	CDrawEngine::GetInstance().SetScaleOffset(static_cast<Gdiplus::REAL>(-zDelta)/m_fScrollWidth);
	Invalidate();
	return TRUE;
	return CCtrlFuncView::OnMouseWheel(nFlags, zDelta, pt);
}

void CTestDrawPicView::OnMButtonDown(UINT nFlags, CPoint point)
{
	//中键按下
	if (!m_bLButtonDown)
	{
		//左键未按下
		Gdiplus::PointF pt(static_cast<Gdiplus::REAL>(point.x), static_cast<Gdiplus::REAL>(point.y));

		//计算点偏移到原点的距离
		switch (m_eCurViewType)
		{
		case EVT_TOP:
			{
				CDrawEngine::GetInstance().TranslateTopOffsetPos(pt);

			}
			break;
		case EVT_FACE:
			{
				CDrawEngine::GetInstance().TranslateSideOffsetPos(pt);

			}
			break;
		}
		m_pMoveStartPoint = pt;
		m_bMButtonDown=true;
		HCURSOR hCur  =  LoadCursor( NULL  , IDC_HAND ) ;
		::SetCursor(hCur);
	}
	CCtrlFuncView::OnMButtonDown(nFlags, point);
}


void CTestDrawPicView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bMButtonDown=false;
	CCtrlFuncView::OnMButtonUp(nFlags, point);
}

BOOL CTestDrawPicView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;//CCtrlFuncView::OnEraseBkgnd(pDC);
}

void CTestDrawPicView::OnBtnCreateWallLine()
{
	ClearState();
	m_eCurViewType = EVT_TOP;
	m_eCurOpt = EOT_Create_Wall_Line;
	CDrawEngine::GetInstance().SetViewType(m_eCurViewType);
	
}

void CTestDrawPicView::OnUpdateBtnCreateWallLine(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}

void CTestDrawPicView::OnBtnCreateWallRect()
{
	ClearState();
	m_eCurViewType = EVT_TOP;
	m_eCurOpt = EOT_Create_Wall_Rect;
	CDrawEngine::GetInstance().SetViewType(m_eCurViewType);
}

void CTestDrawPicView::OnUpdateBtnCreateWallRect(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}

void CTestDrawPicView::OnBtnCreateBeamRect()
{
	//矩形画梁
	ClearState();
	m_eCurViewType = EVT_TOP;
	m_eCurOpt = EOT_Create_Beam_Rect;
	CDrawEngine::GetInstance().SetViewType(m_eCurViewType);
}


void CTestDrawPicView::OnUpdateBtnCreateBeamRect(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CTestDrawPicView::OnBtnCreatePillarRect()
{
	//矩形画柱
	ClearState();
	m_eCurViewType = EVT_TOP;
	m_eCurOpt = EOT_Create_Pillar_Rect;
	CDrawEngine::GetInstance().SetViewType(m_eCurViewType);
}


void CTestDrawPicView::OnUpdateBtnCreatePillarRect(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}

void CTestDrawPicView::OnBtnCreateRoomAuto()
{
	// TODO: 在此添加命令处理程序代码
}


void CTestDrawPicView::OnUpdateBtnCreateRoomAuto(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CTestDrawPicView::OnBtnCreateRoomStyle()
{
	//根据户型生成房间
	m_eCurViewType = EVT_TOP;
	CDlgRoomStyle dlg(this);
	if (dlg.DoModal()==IDOK)
	{
		CDrawEngine::GetInstance().SetViewType(m_eCurViewType);
		//根据点信息生成房间
		//m_eCurOpt = EOT_Create_Wall_RoomStyle;
		std::vector<Gdiplus::PointF> arrPointF;
		dlg.GetPointList(arrPointF);
		if (arrPointF.size()>0)
		{
			std::vector<gp_Pnt> arrPoint;
			for (auto pt:arrPointF)
			{
				arrPoint.emplace_back(gp_Pnt(pt.X,0,pt.Y));
			}

			auto pGroup=std::make_shared<CObjGroupODL>();
			pGroup->InitByParent(m_pObjMgr);

			CShapeTool::CreateWall(arrPoint, -gp::DY(), m_fWallHeight, m_fWallThick, pGroup);
			//最后完成
		}
	}
}

void CTestDrawPicView::OnUpdateBtnCreateRoomStyle(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CTestDrawPicView::OnBtnEditWallSkin()
{
	// TODO: 在此添加命令处理程序代码
}


void CTestDrawPicView::OnUpdateBtnEditWallSkin(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}

void CTestDrawPicView::OnBtnEditFloorSkin()
{
	// TODO: 在此添加命令处理程序代码
}


void CTestDrawPicView::OnUpdateBtnEditFloorSkin(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}

void CTestDrawPicView::OnBtnEditWallZone()
{
	//找看看有没有选中的墙壁，有则显示立面图
	ClearState();
	std::vector<ENUM_OBOJECT_TYPE> arrType;
	arrType.push_back(EOBT_Wall_Face);
	CBaseODLSPtr pObj;
	if (!GetFocused(m_pObjMgr, pObj, arrType))
	{
		m_pCurEditObj=nullptr;
		return;
	}
	if (!pObj)
	{
		m_pCurEditObj=nullptr;
		return;
	}

	m_eCurOpt = EOT_Create_Wall_Face;
	m_eCurViewType = EVT_FACE;
	CDrawEngine::GetInstance().SetViewType(m_eCurViewType);

	//把这个选中的墙给我竖起来
	
	//
	m_pCurEditObj = pObj;
	std::vector<Gdiplus::PointF> arrPoint;
	m_pCurEditObj->GetSidePointList(arrPoint);
	float fOffsetX = 0.0f;
	float fOffsetY = 0.0f;
	if (arrPoint.size()==4)
	{
		CRect rt;
		GetClientRect(rt);
		//Offset尺寸
		fOffsetX = static_cast<float>((static_cast<Gdiplus::REAL>(rt.Width()) - (arrPoint[1].X - arrPoint[0].X)) /2);
		fOffsetY = static_cast<float>((static_cast<Gdiplus::REAL>(rt.Height()) - (arrPoint[2].Y - arrPoint[0].Y)) /2);
		CDrawEngine::GetInstance().SetSideOffset(fOffsetX, fOffsetY);
	}
	//如果是floor
	if (m_pCurEditObj->GetType()==EOBT_Wall_Face)
	{
		//m_vwSubViewPort.ShowWindow(SW_HIDE);
		//把墙上所有的hole全部更新
		CWallFaceODLSPtr spWallFace = std::static_pointer_cast<CWallFaceODL>(m_pCurEditObj);
		//更新hole的大小
		for (auto& wpHole:spWallFace->GetRectList())
		{
			auto& curHole = std::get<0>(wpHole);
			auto& pos = std::get<1>(wpHole);
			auto& szSize = std::get<2>(wpHole);
			auto csHole = curHole.lock();
			assert(std::dynamic_pointer_cast<CDecorODL>(csHole));
			auto spDecor = std::static_pointer_cast<CDecorODL>(csHole);
			Gdiplus::RectF rtTarget=Gdiplus::RectF( static_cast<Gdiplus::REAL>(pos.X()), static_cast<Gdiplus::REAL>(pos.Y()), static_cast<Gdiplus::REAL>(szSize.X()), static_cast<Gdiplus::REAL>(szSize.Y()));
			spDecor->SetSideSize(rtTarget);
			spDecor->SetSideGolbalOffset(Gdiplus::PointF(static_cast<float>(pos.X()), static_cast<float>(pos.Y())));
		}
	}
	else
	{
		AfxMessageBox(_T("请先选中需要编辑的边。"));
	}

}

void CTestDrawPicView::OnUpdateBtnEditWallZone(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}

void CTestDrawPicView::OnBtnEditFloorZone()
{
	ClearState();
	std::vector<ENUM_OBOJECT_TYPE> arrType;
	arrType.push_back(EOBT_Floor);
	CBaseODLSPtr pObj;
	if (!GetFocused(m_pObjMgr, pObj, arrType))
	{
		m_pCurEditObj=nullptr;
		OnBtnViewTop();
		return;
	}
	if (!pObj)
	{
		m_pCurEditObj=nullptr;
		OnBtnViewTop();
		return;
	}
	m_eCurOpt = EOT_Create_Floor_Zone;
	m_eCurViewType = EVT_FACE;
	CDrawEngine::GetInstance().SetViewType(m_eCurViewType);

	//把这个选中的墙给我竖起来
	
	//
	m_pCurEditObj = pObj;
	std::vector<Gdiplus::PointF> arrPoint;
	m_pCurEditObj->GetSidePointList(arrPoint);
	float fOffsetX = 0.0f;
	float fOffsetY = 0.0f;
	if (arrPoint.size()>=3)
	{
		int nCount = arrPoint.size();
		float fMinX = 0.0f;
		float fMaxX = 0.0f;
		
		float fMinY = 0.0f;
		float fMaxY = 0.0f;
		bool bInit=false;
		for (auto& ptX:arrPoint)
		{
			if (!bInit)
			{
				if (std::fabs(fMinX)<0.0001f)
				{
					fMinX = ptX.X;
				}
				if (std::fabs(fMaxX)<0.0001f)
				{
					fMaxX = ptX.X;
				}
				if (std::fabs(fMinY)<0.0001f)
				{
					fMinY = ptX.Y;
				}
				if (std::fabs(fMaxY)<0.0001f)
				{
					fMaxY = ptX.Y;
				}
				bInit=true;
			}
			if (fMinX>ptX.X)
			{
				fMinX = ptX.X;
			}
			if (fMaxX<ptX.X)
			{
				fMaxX = ptX.X;
			}
			if (fMinY>ptX.Y)
			{
				fMinY = ptX.Y;
			}
			if (fMaxY<ptX.Y)
			{
				fMaxY = ptX.Y;
			}
		}
		CRect rt;
		GetClientRect(rt);
		//Offset尺寸
		fOffsetX = static_cast<float>((static_cast<Gdiplus::REAL>(rt.Width()) - (fMaxX - fMinX)) / 2.0f);
		fOffsetY = static_cast<float>((static_cast<Gdiplus::REAL>(rt.Height()) - (fMaxY - fMinY))/ 2.0f);
		CDrawEngine::GetInstance().SetSideOffset(0, 0);
	}
	//如果是floor
	if (m_pCurEditObj->GetType()==EOBT_Floor)
	{
		
	}
	else
	{
		AfxMessageBox(_T("请先选中需要编辑的地板。"));
	}
}


void CTestDrawPicView::OnUpdateBtnEditFloorZone(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CTestDrawPicView::OnBtnViewHide()
{
	// TODO: 在此添加命令处理程序代码
}


void CTestDrawPicView::OnUpdateBtnViewHide(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CTestDrawPicView::OnBtnViewShow()
{
	//显示隐藏的对象
}


void CTestDrawPicView::OnUpdateBtnViewShow(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}

void CTestDrawPicView::OnBtnViewTop()
{
	//切换到俯视图，取消未完成的所有操作
	m_eCurViewType = EVT_TOP;
	m_eCurOpt = EOT_Create_None;
	m_bLButtonDown=false;
	m_bMButtonDown=false;
	ClearState();
	if (m_pGuideLine)
	{
		m_pObjMgr->RemoveChild(m_pGuideLine);
		m_pGuideLine=nullptr;
	}
	if (m_pGuideRect)
	{
		m_pObjMgr->RemoveChild(m_pGuideRect);
		m_pGuideRect=nullptr;
	}
	m_pCurEditObj=nullptr;
	if (!m_bIsSubViewerHide)
	{
		m_vwSubViewPort.ShowWindow(SW_SHOW);
	}
}

void CTestDrawPicView::OnUpdateBtnViewTop(CCmdUI *pCmdUI)
{
	//禁用、启用 切换到俯视图功能
	//编辑墙壁分区、地板分区时，禁止切换，直到编辑完成
}

void CTestDrawPicView::OnBtnViewRender()
{
	//
}

void CTestDrawPicView::OnUpdateBtnViewRender(CCmdUI *pCmdUI)
{
	//禁用、启用 光线渲染功能
}

BOOL CTestDrawPicView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	//数据放下了
	if (!pDataObject->IsDataAvailable(CF_TEXT))
	{
		return FALSE;
	}
	HGLOBAL hData=pDataObject->GetGlobalData(CF_TEXT);
	if (!hData) 
	{
		return FALSE;
	}
	Gdiplus::PointF pt(static_cast<Gdiplus::REAL>(point.x), static_cast<Gdiplus::REAL>(point.y));
	StObjInfo *ddsInfo=new StObjInfo();
	CopyMemory(ddsInfo,GlobalLock(hData),sizeof(StObjInfo));
	//当前对象存在且是面
	switch (m_eCurViewType)
	{
	case EVT_TOP:
		{
			if (ddsInfo->IsSkin)
			{
				GlobalUnlock(hData);
				hData=NULL;

				delete(ddsInfo);
				return FALSE;
			}
			CDrawEngine::GetInstance().TranslateTopOffsetPos(pt);

			if (_tcslen(ddsInfo->ModelPath)>5)
			{
				//判断当前的东西的类型
				//如果是墙纸，直接设置当前选中的对象
				//如果是物体，判断能不能放在地板上，停靠关系
				//数据逻辑上，他属于当前停靠的对象的上级ObjGroup。
				// 位于Y=0平面上的点
				auto ver = BRepBuilderAPI_MakeVertex(gp_Pnt(pt.X, 0, pt.Y)).Vertex();
				//判断这个点上存在的对象
				CBaseODLSPtr spRet = GetTopOnMouseObj(ver, m_pObjMgr, m_pDecor);
				if (spRet)
				{
					if (spRet->GetType()==EOBT_Floor)
					{
						//往地板上放东西
						m_pDecor=nullptr;
					}
					else if (spRet->GetType()==EOBT_Wall)
					{
						spRet->UpdateMesh();
						m_pDecor=nullptr;
					}
					else
					{
						if (m_pDecor)
						{
							m_pDecor->RemoveSelf();
							m_pDecor=nullptr;
						}
					}
					GetMainFrame()->RefreshLayer(this, m_pObjMgr);
				}
			}
			GlobalUnlock(hData);
			hData=NULL;
			delete(ddsInfo);
			return FALSE;
		}
		break;
	case EVT_FACE:
		{
			if ((!m_pCurEditObj) ||
				(ddsInfo->GTypeID==EGT_RoomDoor) ||
				(ddsInfo->GTypeID==EGT_RoomWindow)||
				(ddsInfo->GTypeID==EGT_BayWindow))
			{
				GlobalUnlock(hData);
				hData=NULL;
				delete(ddsInfo);
				return FALSE;
			}
			CDrawEngine::GetInstance().TranslateSideOffsetPos(pt);
			bool bSetted=false;
			Gdiplus::PointF ptTmp = pt;

			StObjInfo *ddsInfo=new StObjInfo();
			CopyMemory(ddsInfo,GlobalLock(hData),sizeof(StObjInfo));
			if (_tcslen(ddsInfo->SkinPath)>5)
			{
				Gdiplus::Image img(ddsInfo->SkinPath);
				INT nSkinWidth = static_cast<INT>(ddsInfo->XLen);
				INT nSkinHeight = static_cast<INT>(ddsInfo->YLen);
				m_nGroutX = 10;
				m_nGroutY = 20;
				//生成新图片
				
				CBaseODLSPtr spRet = GetSidePointInLeafObj(ptTmp, m_pCurEditObj);
				if (!spRet)
				{
					spRet = GetSidePointAtLeafObj(ptTmp, m_pCurEditObj);
					if (!spRet)
					{
						spRet = m_pCurEditObj;
					}
				}
				if (spRet)
				{
					StCoverInfo stCVInfo;
					stCVInfo.SkinWidth = nSkinWidth;
					stCVInfo.SkinHeight = nSkinHeight;
					stCVInfo.XGrout = m_nGroutX;
					stCVInfo.YGrout = m_nGroutY;
					stCVInfo.GroutColor = m_clGrout.GetValue();
					_tcscpy(stCVInfo.SkinPath, ddsInfo->SkinPath);
					if (ddsInfo->IsSkin)
					{
						spRet->GetSkin()->SetSkinPath(stCVInfo);
						spRet->UpdateMesh();
					}
					else
					{
						//创建新Zone
						CSkinUnitODLSPtr spSkin = std::make_shared<CSkinUnitODL>();
						spSkin->SetSkinPath(stCVInfo);
						//////////////////////////////////////////////////////////////////////////
						//创建的时候，选中的是一个对象，这个对象的点已经被局域坐标化
						spRet->CreateSub(pt, spSkin);
					}
				}
				bSetted=true;
			}
			GlobalUnlock(hData);
			hData=NULL;
			delete(ddsInfo);
			if (!bSetted)
			{
				return FALSE;
			}
			GetMainFrame()->RefreshLayer(this, m_pObjMgr);
		}
		break;
	}
	GlobalUnlock(hData);
	hData=NULL;
	delete(ddsInfo);
	Invalidate();
	return CCtrlFuncView::OnDrop(pDataObject, dropEffect, point);
}


DROPEFFECT CTestDrawPicView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	// TODO: 在此添加专用代码和/或调用基类
	//清除当前所有状态
	
	m_pCurMoveObj=nullptr;
	m_pCurResizeObj=nullptr;
	m_pDecor=nullptr;
	return DROPEFFECT_MOVE;
}

DROPEFFECT CTestDrawPicView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if (!pDataObject->IsDataAvailable(CF_TEXT))
	{
		return DROPEFFECT_NONE;
	}
	HGLOBAL hData=pDataObject->GetGlobalData(CF_TEXT);
	if (!hData) 
	{
		return DROPEFFECT_NONE;
	}
	Gdiplus::PointF pt(static_cast<Gdiplus::REAL>(point.x), static_cast<Gdiplus::REAL>(point.y));
	
	bool bHasSth = false;

	StObjInfo *ddsInfo=new StObjInfo();
	CopyMemory(ddsInfo,GlobalLock(hData),sizeof(StObjInfo));
	switch (m_eCurViewType)
	{
	case EVT_TOP:
		{
			if (ddsInfo->IsSkin)
			{
				//俯视图下不能更换皮肤
				GlobalUnlock(hData);
				hData=NULL;
				delete(ddsInfo);
				return DROPEFFECT_NONE;
			}
			//不许放除了DECOR以外的对象
			if ((ddsInfo->GTypeID!=EGT_RoomDoor) &&
				(ddsInfo->GTypeID!=EGT_RoomWindow) &&
				(ddsInfo->GTypeID!=EGT_BayWindow))
			{
				GlobalUnlock(hData);
				hData=NULL;
				delete(ddsInfo);
				return DROPEFFECT_NONE;
			}
			if (ddsInfo->ValueID==0)
			{
				GlobalUnlock(hData);
				hData=NULL;
				delete(ddsInfo);
				Invalidate();
				return DROPEFFECT_NONE;
			}
			//创建Decor过程中，OnTopMove不起作用
			CDrawEngine::GetInstance().TranslateTopOffsetPos(pt);
			gp_Pnt stInfo;
			stInfo.SetX(pt.X);
			stInfo.SetY(0);
			stInfo.SetZ(pt.Y);
			auto vertex = BRepBuilderAPI_MakeVertex(stInfo).Vertex();
			auto line = gce_MakeLin(stInfo, gp::DY()).Value();
			auto edge = BRepBuilderAPI_MakeEdge(line).Edge();
			//查找当前鼠标所在Y线上的对象,从上到下(这个东西容易把自己找到，所以：去掉自己)
			CBaseODLSPtr spRet = GetTopOnMouseObj(edge, m_pObjMgr, m_pDecor);
			if (!spRet)
			{
				GlobalUnlock(hData);
				hData=NULL;
				delete(ddsInfo);
				Invalidate();
				return DROPEFFECT_NONE;
			}
			//判断当前拖进来的这个对象是墙上的，还是地上的
			//创建矩形框
			if (spRet->GetType()==EOBT_Wall)
			{
				//帖到墙上
				if (!m_pDecor)
				{
					//创建对象，设置初始位置
					m_pDecor = std::make_shared<CDecorODL>();
					m_pDecor->InitByParent(m_pObjMgr, "Decor");
					m_pDecor->SetHoleRotation(0,0,0);
					m_pDecor->SetSize(ddsInfo->XLen, ddsInfo->YLen, ddsInfo->ZLen);
					m_pDecor->SetRawSize(gp_XYZ(ddsInfo->XLen, ddsInfo->YLen, ddsInfo->ZLen));
					m_pDecor->SetPos(stInfo);
				}
				else
				{
					auto& start = spRet->GetTopStart();
					auto& stop = spRet->GetTopStop();
					gp_Pnt ptPos (pt.X, 0.0, pt.Y);

					auto edge = BRepBuilderAPI_MakeEdge(BRepBuilderAPI_MakeVertex(start), BRepBuilderAPI_MakeVertex(stop)).Edge();
					BRepExtrema_DistShapeShape dss(edge, BRepBuilderAPI_MakeVertex(ptPos));

					assert(0 != dss.NbSolution());

					auto& adjustPnt = dss.PointOnShape1(1);
					if (adjustPnt.IsEqual(ptPos, Precision::Confusion()))
					{
						GlobalUnlock(hData);
						hData=NULL;
						delete(ddsInfo);
						Invalidate();
						return DROPEFFECT_NONE;
					}
					//修改对象，判断停靠
					spRet->AddChild(m_pDecor);
					gp_Dir dir(stop.XYZ()-start.XYZ());
					auto angle = dir.AngleWithRef(gp::DX(), gp::DY());
					//angle = std::fmod(angle, M_PI);
					m_pDecor->SetHoleRotation(0,angle,0);
					m_pDecor->SetSize(static_cast<Gdiplus::REAL>(ddsInfo->XLen), static_cast<Gdiplus::REAL>(ddsInfo->YLen), m_fWallThick);

					TRACE(_T("OnDragLover:angle:%f\r\n"), angle);


					gp_Dir wallDir(gp_Vec(adjustPnt,ptPos));
					auto wallVec = wallDir.XYZ().Multiplied(m_fWallThick);
					m_pDecor->SetPos(adjustPnt.XYZ()+wallVec);
				}
				TRACE(_T("pt: %f, %f\r\n"), pt.X, pt.Y);
				GlobalUnlock(hData);
				hData=NULL;
				delete(ddsInfo);
				Invalidate();
				return DROPEFFECT_MOVE;
			}
			else if (spRet->GetType()==EOBT_Floor)
			{
				//帖到地板上
				if (!m_pDecor)
				{
					m_pDecor = std::make_shared<CDecorODL>();
					m_pDecor->InitByParent(m_pObjMgr, "Decor");
					m_pDecor->SetHoleRotation(0,0,0);
					m_pDecor->SetSize(ddsInfo->XLen, ddsInfo->YLen, ddsInfo->ZLen);
					m_pDecor->SetRawSize(gp_XYZ(ddsInfo->XLen, ddsInfo->YLen, ddsInfo->ZLen));
					m_pDecor->SetPos(stInfo);
				}
				else
				{
					//这个矩形拿来装Decor
					spRet->AddChild(m_pDecor);
					m_pDecor->SetHoleRotation(0,0,0);
					m_pDecor->SetSize(ddsInfo->XLen, ddsInfo->YLen, ddsInfo->ZLen);
					m_pDecor->SetPos(stInfo);
				}
				TRACE(_T("pt: %f, %f\r\n"), pt.X, pt.Y);
				GlobalUnlock(hData);
				hData=NULL;
				delete(ddsInfo);
				Invalidate();
				return DROPEFFECT_MOVE;
			}
			else
			{
				//在不是墙，不是底板的对象内
				//不许创建
				GlobalUnlock(hData);
				hData=NULL;
				delete(ddsInfo);
				Invalidate();
				return DROPEFFECT_NONE;
			}
			GlobalUnlock(hData);
			hData=NULL;
			delete(ddsInfo);
			return DROPEFFECT_NONE;
		}
		break;
	case EVT_FACE:
		{
			if ((!m_pCurEditObj) || 
				(ddsInfo->GTypeID==EGT_RoomDoor) ||
				(ddsInfo->GTypeID==EGT_RoomWindow)||
				(ddsInfo->GTypeID==EGT_BayWindow))
			{
				GlobalUnlock(hData);
				hData=NULL;
				delete(ddsInfo);
				return DROPEFFECT_NONE;
			}
			CDrawEngine::GetInstance().TranslateSideOffsetPos(pt);
			for (auto& spItem:m_pCurEditObj->GetChildrenList())
			{
				switch (spItem->GetType())
				{
					case  EOBT_Wall_Face_Zone:
					{
						int nRet =spItem->IsSidePointInside(pt);
						if (nRet==0)
						{
							continue;
						}
						bHasSth=true;
					}
					break;
				}
			}
			GlobalUnlock(hData);
			hData=NULL;
			delete(ddsInfo);
			return DROPEFFECT_MOVE;
		}
		break;
	}
	GlobalUnlock(hData);
	hData=NULL;
	delete(ddsInfo);
	return DROPEFFECT_NONE;
}


void CTestDrawPicView::OnDragLeave()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_pDecor)
	{
		m_pDecor->RemoveSelf();
		m_pDecor=nullptr;
		GetMainFrame()->RefreshLayer(this, m_pObjMgr);
	}
	CCtrlFuncView::OnDragLeave();
}

void CTestDrawPicView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CCtrlFuncView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CTestDrawPicView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CCtrlFuncView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CTestDrawPicView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message ==WM_KEYDOWN)
	{
		if ( m_pObjMgr )
		{
			auto src = m_pObjMgr->GetRenderContext();
			if ( src )
			{
				irr::SEvent evt;
				evt.EventType = irr::EET_KEY_INPUT_EVENT;
				evt.KeyInput.PressedDown = true;
				switch (pMsg->wParam)
				{
				case 'w':
				case 'W':
					evt.KeyInput.Key = irr::KEY_KEY_W;
					break;
				case 'a':
				case 'A':
					evt.KeyInput.Key = irr::KEY_KEY_A;
					break;
				case 's':
				case 'S':
					evt.KeyInput.Key = irr::KEY_KEY_S;
					break;
				case 'd':
				case 'D':
					evt.KeyInput.Key = irr::KEY_KEY_D;
					break;
				default:
					break;
				}
				src->PostEvent(evt);
				m_vwSubViewPort.Invalidate();
			}
		}
	}
	if (pMsg->message==WM_KEYUP)
	{
		switch (pMsg->wParam)
		{
		case VK_DELETE:
			{
				if (m_pCurMoveObj)
				{
					//删除，只能删除Wall,Face,Zone,Decor
					ENUM_OBOJECT_TYPE eType =m_pCurMoveObj->GetType();
					if ((eType==EOBT_Wall) || (eType==EOBT_Floor) || (eType==EOBT_Floor_Zone) || (eType==EOBT_Wall_Face_Zone))
					{
						m_pCurMoveObj->RemoveSelf();
						m_pCurMoveObj=nullptr;
						Invalidate();
					}
				}
			}
			break;
		default:
			{
				if ( m_pObjMgr )
				{
					auto src = m_pObjMgr->GetRenderContext();
					if ( src )
					{
						irr::SEvent evt;
						evt.EventType = irr::EET_KEY_INPUT_EVENT;
						evt.KeyInput.PressedDown = false;
						switch (pMsg->wParam)
						{
						case 'w':
						case 'W':
							evt.KeyInput.Key = irr::KEY_KEY_W;
							break;
						case 'a':
						case 'A':
							evt.KeyInput.Key = irr::KEY_KEY_A;
							break;
						case 's':
						case 'S':
							evt.KeyInput.Key = irr::KEY_KEY_S;
							break;
						case 'd':
						case 'D':
							evt.KeyInput.Key = irr::KEY_KEY_D;
							break;
						default:
							break;
						}

						src->PostEvent(evt);
						m_vwSubViewPort.Invalidate();
					}
					switch (pMsg->wParam)
					{
					case 'Q':
					case 'q':
						{
							//新建墙
							OnBtnCreateWallRect();
						}
						break;
					case 'F':
					case 'f':
						{
							//墙面编辑
							OnBtnEditWallZone();
						}
						break;
					}
				}
			}
			break;
		}
	}

	return CView::PreTranslateMessage(pMsg);
}

void CTestDrawPicView::SetCameraInfo( bool visible, const gp_Pnt& position, const gp_Pnt& target, float fov )
{
	auto camera = m_pObjMgr->GetCamera();
	if ( !camera )
	{
		return;
	}

	if ( camera->SetCameraInfo( visible, position, target, fov) )
	{
		Invalidate();
	}
}


void CTestDrawPicView::OnSubMenuProperty()
{
	if (m_eCurViewType == EVT_TOP)
	{
		if (m_pCurMoveObj)
		{
			//编辑当前对象
			switch (m_pCurMoveObj->GetType())
			{
			case EOBT_Wall:
				{
					//俯视图下编辑墙
				}
				break;
			case EOBT_Beam:
			case EOBT_Pillar:
				{
					//俯视图下编辑梁柱
					CDlgSizeInfo dlg;
					CWallODLSPtr sp=std::static_pointer_cast<CWallODL>(m_pCurMoveObj);
					float fXLen = 0.0f;
					float fYLen = 0.0f;
					float fZLen = 0.0f;
					float fYPos = 0.0f;
					sp->GetSize(fXLen, fYLen, fZLen);
					fYPos =sp->GetYPos();
					dlg.SetSizeInfo(fXLen, fYLen, fZLen, fYPos);
					if (dlg.DoModal()==IDOK)
					{
						dlg.GetSizeInfo(fXLen, fYLen, fZLen, fYPos);
						sp->SetSize(fXLen, fYLen, fZLen);
						sp->SetYPos(fYPos);
						Invalidate();
					}
				}
				break;
			case EOBT_Floor:
				{

				}
				break;
			case EOBT_Decor:
				{

				}
				break;
			}
		}
	}
	if (m_eCurViewType ==EVT_FACE)
	{
		if ((!m_pCurMoveObj) && (!m_pCurEditObj))
		{
			return;
		}
		//先看有没有side被选中
		if (m_pCurMoveObj)
		{
			switch (m_pCurMoveObj->GetType())
			{
			case EOBT_Floor_Zone:
				{
					EditZone(m_pCurMoveObj);
				}
				break;
			case EOBT_Wall_Face_Zone:
				{
					EditZone(m_pCurMoveObj);
				}
				break;
			}
		}
		else if (m_pCurEditObj)
		{
			//没side，然后看是不是WallFace或者Floor，是则处理
			switch (m_pCurEditObj->GetType())
			{
			case EOBT_Wall_Face:
				{
					//编辑Face的皮肤
					EditZone(m_pCurEditObj);
				}
				break;
			case EOBT_Floor:
				{
					EditZone(m_pCurEditObj);
				}
				break;
			}
		}
	}
}

CMainFrame* CTestDrawPicView::GetMainFrame() const
{
	CMainFrame* pMain = (CMainFrame*)theApp.GetMainWnd();
	return pMain;
}

void CTestDrawPicView::SwitchToEdit( CBaseODLSPtr spObj )
{
	//进入Side编辑模式
	switch (spObj->GetType())
	{
	case EOBT_Floor:
		{
			//切换到side编辑
			OnBtnEditFloorZone();
		}
		break;
	case EOBT_Floor_Zone:
		{
			//弹出修改窗口
		}
		break;
	case EOBT_Wall_Face:
		{
			//切换到side编辑
			OnBtnEditWallZone();
		}
		break;
	case EOBT_Wall_Face_Zone:
		{
			//弹出修改窗口
		}
		break;

	}
}

void CTestDrawPicView::EditZone(CBaseODLSPtr spZone)
{
	if (!spZone->GetSkin()->HasPicData())
	{
		AfxMessageBox(_T("没有设置图片。"));
		return;
	}
	CDlgZoneInfo dlg;
	float fRotate = 0.0f;
	int nGroutX = 0;
	int nGroutY = 0;
	INT nMode = (INT)Gdiplus::WrapModeTile;
	Gdiplus::Color clGrout;
	spZone->GetSkin()->GetSetting(nGroutX, nGroutY, nMode, fRotate, clGrout);
	dlg.SetColor(RGB(clGrout.GetR(),clGrout.GetG(),clGrout.GetB()));
	dlg.SetRotate(fRotate);
	dlg.SetGroutXY(nGroutX, nGroutY);
	dlg.SetWrapMode(nMode);
	if (dlg.DoModal()==IDOK)
	{
		fRotate = dlg.GetRotate();
		dlg.GetGroutXY(nGroutX,nGroutY);
		clGrout.SetFromCOLORREF(dlg.GetColor());
		nMode = dlg.GetWrapMode();
		spZone->GetSkin()->SetSetting(nGroutX, nGroutY, nMode, fRotate, clGrout);
		spZone->UpdateMesh();
		Invalidate();
	}
}

