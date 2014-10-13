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
#include "irrEngine/SRenderContext.h"
#include "StatusMgr.h"

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


BOOL CTestDrawPicView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL bRet = CCtrlFuncView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	
	
	GetSubView().Create(_T(""), WS_VISIBLE|WS_CHILD|SS_NOTIFY|SS_OWNERDRAW|WS_CLIPSIBLINGS,CRect(0,600,200,200),this,IDR_VIEW_SUBVIEWER);
	auto rootODL = std::make_shared<CDesignODL>(GetSubView().GetSafeHwnd());
	SetRoot(rootODL);

	rootODL->Init();

	GetSubView().SetRenderContext(rootODL->GetRenderContext());

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
	
	CCtrlFuncView::OnLButtonDown(nFlags, point);
}

void CTestDrawPicView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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

	CCtrlFuncView::OnMouseMove(nFlags, point);
}

void CTestDrawPicView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CCtrlFuncView::OnRButtonDown(nFlags, point);
}

void CTestDrawPicView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	
}

void CTestDrawPicView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
}


void CTestDrawPicView::OnSize(UINT nType, int cx, int cy)
{
	CCtrlFuncView::OnSize(nType, cx, cy);

	if (GetSubView().GetSafeHwnd())
	{
		CRect rtMain;
		GetWindowRect(rtMain);

		GetSubView().MoveWindow(0, 0, rtMain.Width(), rtMain.Height());
	}
	

	/*
	if (this->GetSafeHwnd())
	{
		static auto firstUpdate = true;
		
		CRect rtMain;
		GetWindowRect(rtMain);

		if (GetSubView().GetSafeHwnd())
		{
			if ( firstUpdate )
			{
				GetSubView().MoveWindow(0,rtMain.Height()-500, 800,500);
				firstUpdate = false;
			}
			else
			{
				CRect rtSub;
				GetSubView().GetWindowRect(rtSub);
				GetSubView().MoveWindow(0, rtMain.Height()-rtSub.Height(), rtSub.Width(), rtSub.Height());
			}
		}
	}*/
}


void CTestDrawPicView::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//发现鼠标出去了
	CCtrlFuncView::OnMouseLeave();
}

void CTestDrawPicView::ClearState()
{
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
	return CCtrlFuncView::OnMouseWheel(nFlags, zDelta, pt);
}

void CTestDrawPicView::OnMButtonDown(UINT nFlags, CPoint point)
{
	CCtrlFuncView::OnMButtonDown(nFlags, point);
}


void CTestDrawPicView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CCtrlFuncView::OnMButtonUp(nFlags, point);
}

BOOL CTestDrawPicView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;//CCtrlFuncView::OnEraseBkgnd(pDC);
}

void CTestDrawPicView::OnBtnCreateWallLine()
{
	StatusMgr::GetInstance().DrawingState_ = StatusMgr::EDS_LINE_WALL;
}

void CTestDrawPicView::OnUpdateBtnCreateWallLine(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}

void CTestDrawPicView::OnBtnCreateWallRect()
{
	StatusMgr::GetInstance().DrawingState_ = StatusMgr::EDS_RECT_WALL;
}

void CTestDrawPicView::OnUpdateBtnCreateWallRect(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}

void CTestDrawPicView::OnBtnCreateBeamRect()
{
	
}


void CTestDrawPicView::OnUpdateBtnCreateBeamRect(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CTestDrawPicView::OnBtnCreatePillarRect()
{
	
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
	
}

void CTestDrawPicView::OnUpdateBtnEditWallZone(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}

void CTestDrawPicView::OnBtnEditFloorZone()
{
	
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
	return CCtrlFuncView::OnDrop(pDataObject, dropEffect, point);
}


DROPEFFECT CTestDrawPicView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	// TODO: 在此添加专用代码和/或调用基类
	//清除当前所有状态
	return DROPEFFECT_MOVE;
}

DROPEFFECT CTestDrawPicView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	
	return DROPEFFECT_NONE;
}


void CTestDrawPicView::OnDragLeave()
{
	// TODO: 在此添加专用代码和/或调用基类
	
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

	if ( !GetRootODL() || !GetRootODL()->GetRenderContext() )
	{
		return CView::PreTranslateMessage(pMsg);
	}

	auto rc = GetRootODL()->GetRenderContext();

	if ( pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP )
	{
		irr::SEvent event;

		BYTE allKeys[256];

		event.EventType = irr::EET_KEY_INPUT_EVENT;
		event.KeyInput.Key = (irr::EKEY_CODE)pMsg->wParam;
		event.KeyInput.PressedDown = (pMsg->message==WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN);

		const UINT MY_MAPVK_VSC_TO_VK_EX = 3;
		if ( event.KeyInput.Key == irr::KEY_SHIFT )
		{
			event.KeyInput.Key = (irr::EKEY_CODE)MapVirtualKey( ((pMsg->lParam>>16) & 255), MY_MAPVK_VSC_TO_VK_EX );
		}
		if ( event.KeyInput.Key == irr::KEY_CONTROL )
		{
			event.KeyInput.Key = (irr::EKEY_CODE)MapVirtualKey( ((pMsg->lParam>>16) & 255), MY_MAPVK_VSC_TO_VK_EX );
			if (pMsg->lParam & 0x1000000)
				event.KeyInput.Key = irr::KEY_RCONTROL;
		}
		if ( event.KeyInput.Key == irr::KEY_MENU )
		{
			event.KeyInput.Key = (irr::EKEY_CODE)MapVirtualKey( ((pMsg->lParam>>16) & 255), MY_MAPVK_VSC_TO_VK_EX );
			if (pMsg->lParam & 0x1000000)
				event.KeyInput.Key = irr::KEY_RMENU;
		}

		GetKeyboardState(allKeys);

		event.KeyInput.Shift = ((allKeys[VK_SHIFT] & 0x80)!=0);
		event.KeyInput.Control = ((allKeys[VK_CONTROL] & 0x80)!=0);

		if ((allKeys[VK_MENU] & 0x80) != 0)
			event.KeyInput.Control = 0;

		rc->PostEvent(event);
	}

	if ( pMsg->message == WM_MOUSEWHEEL )
	{
		POINT p; // fixed by jox
		p.x = 0; p.y = 0;
		::ClientToScreen(GetSubView().GetSafeHwnd(), &p);

		irr::SEvent evt;
		evt.EventType = irr::EET_MOUSE_INPUT_EVENT;
		evt.MouseInput.Event = irr::EMIE_MOUSE_WHEEL;
		evt.MouseInput.X = (short)LOWORD(pMsg->lParam) - p.x;
		evt.MouseInput.Y = (short)HIWORD(pMsg->lParam) - p.y;
		evt.MouseInput.Wheel = ((irr::f32)((short)HIWORD(pMsg->wParam))) / (irr::f32)WHEEL_DELTA;
		rc->PostEvent(evt);
		TRACE(L"Wheel:%f\n", evt.MouseInput.Wheel);
	}

	return CView::PreTranslateMessage(pMsg);
}


void CTestDrawPicView::OnSubMenuProperty()
{
	
}

CMainFrame* CTestDrawPicView::GetMainFrame() const
{
	CMainFrame* pMain = (CMainFrame*)theApp.GetMainWnd();
	return pMain;
}