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
#include "RenderController/UserEvent.h"
#include "StatusMgr.h"

#include "Dialog/DlgRoomLayoutPictureSize.h"
#include "Dialog/DlgRoomLayoutDrawlinInfo.h"
#include "resource.h"

#include <boost/filesystem/path.hpp>
// #ifdef _DEBUG
// #define new DEBUG_NEW
// #endif

// CTestDrawPicView

IMPLEMENT_DYNCREATE(CTestDrawPicView, CCtrlFuncView)

BEGIN_MESSAGE_MAP(CTestDrawPicView, CCtrlFuncView)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_SUB_MENU_PROPERTY, &CTestDrawPicView::OnSubMenuProperty)

	ON_COMMAND(ID_BTN_ROOMLAYOUT_CREATE_DRAWWALL, &CTestDrawPicView::OnBtnRoomLayoutCreateDrawWall)
	ON_COMMAND(ID_BTN_ROOMLAYOUT_CREATE_DRAWROOM, &CTestDrawPicView::OnBtnRoomLayoutCreateDrawRoom)
	ON_COMMAND(ID_BTN_ROOMLAYOUT_PICTURE_PICTURE, &CTestDrawPicView::OnBtnRoomLayoutPicturePicture)
	ON_COMMAND(ID_CHK_ROOMLAYOUT_PICTURE_SHOW, &CTestDrawPicView::OnChkRoomLayoutPictureShow)
	ON_UPDATE_COMMAND_UI(ID_CHK_ROOMLAYOUT_PICTURE_SHOW, &CTestDrawPicView::OnChkUpdateRoomLayoutPictureShow)
	ON_COMMAND(ID_SLIDE_ROOMLAYOUT_PICTURE_ALPHA, &CTestDrawPicView::OnSlideRoomLayoutPictureAlpha)
	ON_UPDATE_COMMAND_UI(ID_SLIDE_ROOMLAYOUT_PICTURE_ALPHA, &CTestDrawPicView::OnSlideUpdateRoomLayoutPictureAlpha)
	ON_COMMAND(ID_BTN_ROOMLAYOUT_PICTURE_SETSCALE, &CTestDrawPicView::OnBtnRoomLayoutPictureSetScale)
	ON_COMMAND(ID_BTN_ROOMLAYOUT_PICTURE_SETPOSITION, &CTestDrawPicView::OnBtnRoomLayoutPictureSetPosition)
	ON_COMMAND(ID_BTN_ROOMLAYOUT_TEST_DOOR, &CTestDrawPicView::OnBtnRoomLayoutTestDoor)
	ON_COMMAND(ID_BTN_ROOMLAYOUT_TEST_WINDOW, &CTestDrawPicView::OnBtnRoomlayoutTestWindow)
END_MESSAGE_MAP()


class	CTestDrawPicView::Imp
{
public:

	std::shared_ptr<DlgRoomLayoutDrawlinInfo>	DlgRoomLayoutDrawlinInfo_;
};

// CTestDrawPicView 构造/析构
CTestDrawPicView::CTestDrawPicView():ImpUPtr_(new Imp)
{
	// TODO: 在此处添加构造代码
	m_clGrout = Gdiplus::Color::Bisque;
	m_pDrop=nullptr;
	m_Render3D = true;

	HasRoomPicture_ = false;
	ShowPic_ = false;
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
void CTestDrawPicView::OnTimer(UINT_PTR nIDEvent)
{
	if ( m_Render3D )
	{
		Render();
	}

	CCtrlFuncView::OnTimer(nIDEvent);
}

void CTestDrawPicView::OnInitialUpdate()
{
	CCtrlFuncView::OnInitialUpdate();

	{
		CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arr;
		GetMainFrame()->GetRibbonBar()->GetElementsByID(ID_SLIDE_ROOMLAYOUT_PICTURE_ALPHA, arr);
		auto slide = static_cast<CMFCRibbonSlider*>(arr[0]);
		slide->SetPos(50);
	}
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

	auto rootODL = std::make_shared<DesignODL>(this->GetSafeHwnd());
	SetRoot(rootODL);

	rootODL->Init();

	m_spRenderContext = (rootODL->GetRenderContext());

	m_pDrop=new COleDropTarget();

	if (!this->m_pDropTarget)
	{
		m_pDrop->Register(this);
	}
	SetTimer(1000, 20/*ms*/,NULL);

	ImpUPtr_->DlgRoomLayoutDrawlinInfo_ = std::make_shared<DlgRoomLayoutDrawlinInfo>(m_spRenderContext, this);
	ImpUPtr_->DlgRoomLayoutDrawlinInfo_->Create(DlgRoomLayoutDrawlinInfo::IDD, this);
	ImpUPtr_->DlgRoomLayoutDrawlinInfo_->SetVisible(false);

	return bRet;
}

void CTestDrawPicView::OnSize(UINT nType, int cx, int cy)
{
	CCtrlFuncView::OnSize(nType, cx, cy);

	if ( ImpUPtr_->DlgRoomLayoutDrawlinInfo_ )
	{
		ImpUPtr_->DlgRoomLayoutDrawlinInfo_->Resize();
	}

	if ( m_spRenderContext )
	{
		m_spRenderContext->SetOnResize();
	}
}


LRESULT CTestDrawPicView::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	//return CStatic::DefWindowProc(message, wParam, lParam);
	CRect rtMain;
	GetWindowRect(rtMain);
	static irr::s32 ClickCount=0;
	if (::GetCapture() != GetSafeHwnd() && ClickCount > 0)
		ClickCount = 0;

	if ( WM_IRR_DLG_MSG == message )
	{
		if ( WM_USER_ROOMLAYOUT_PICTUREREFLINE_SIZE == wParam )
		{
			DlgRoomLayoutPictureSize dlg;
			
			dlg.DoModal();

			auto num = dlg.GetNum();

			irr::SEvent evt;
			evt.EventType = irr::EET_USER_EVENT;
			evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_PICTUREREFLINE_SIZE;
			evt.UserEvent.UserData2 = num;
			m_spRenderContext->PostEvent(evt);
		}

		if ( WM_USER_ROOMLAYOUT_DLG_LINELENGTH_SHOW == wParam )
		{
			ImpUPtr_->DlgRoomLayoutDrawlinInfo_->SetVisible(true);
		}

		if ( WM_USER_ROOMLAYOUT_DLG_LINELENGTH_HIDE == wParam )
		{
			ImpUPtr_->DlgRoomLayoutDrawlinInfo_->SetVisible(false);
		}

		if ( WM_USER_ROOMLAYOUT_LINELENGTH_SET == wParam )
		{
			ImpUPtr_->DlgRoomLayoutDrawlinInfo_->SetNum(lParam);
		}

		return TRUE;
	}

	struct messageMap
	{
		irr::s32 group;
		UINT winMessage;
		irr::s32 irrMessage;
	};

	static messageMap mouseMap[] =
	{
		{0, WM_LBUTTONDOWN, irr::EMIE_LMOUSE_PRESSED_DOWN},
		{0, WM_LBUTTONDBLCLK, irr::EMIE_LMOUSE_DOUBLE_CLICK},
		{1, WM_LBUTTONUP,   irr::EMIE_LMOUSE_LEFT_UP},
		{0, WM_RBUTTONDOWN, irr::EMIE_RMOUSE_PRESSED_DOWN},
		{1, WM_RBUTTONUP,   irr::EMIE_RMOUSE_LEFT_UP},
		{0, WM_MBUTTONDOWN, irr::EMIE_MMOUSE_PRESSED_DOWN},
		{1, WM_MBUTTONUP,   irr::EMIE_MMOUSE_LEFT_UP},
		{2, WM_MOUSEMOVE,   irr::EMIE_MOUSE_MOVED},
		{3, WM_MOUSEWHEEL,  irr::EMIE_MOUSE_WHEEL},
		{-1, 0, 0}
	};

	messageMap * m = mouseMap;
	while ( m->group >=0 && m->winMessage != message )
		m += 1;

	if ( m->group >= 0 )
	{
		irr::SEvent event;

		if ( m->group == 0 )	// down
		{
			ClickCount++;
			::SetCapture(GetSafeHwnd());
		}
		else if ( m->group == 1 )	// up
		{
			ClickCount--;
			if (ClickCount<1)
			{
				ClickCount=0;
				::ReleaseCapture();
			}
		}

		event.EventType = irr::EET_MOUSE_INPUT_EVENT;
		event.MouseInput.Event = (irr::EMOUSE_INPUT_EVENT) m->irrMessage;
		event.MouseInput.X = (short)LOWORD(lParam);
		event.MouseInput.Y = (short)HIWORD(lParam);
		event.MouseInput.Shift = ((LOWORD(wParam) & MK_SHIFT) != 0);
		event.MouseInput.Control = ((LOWORD(wParam) & MK_CONTROL) != 0);
		// left and right mouse buttons
		event.MouseInput.ButtonStates = wParam & ( MK_LBUTTON | MK_RBUTTON);
		// middle and extra buttons
		if (wParam & MK_MBUTTON)
			event.MouseInput.ButtonStates |= irr::EMBSM_MIDDLE;
		if (wParam & MK_XBUTTON1)
			event.MouseInput.ButtonStates |= irr::EMBSM_EXTRA1;
		if (wParam & MK_XBUTTON2)
			event.MouseInput.ButtonStates |= irr::EMBSM_EXTRA2;
		event.MouseInput.Wheel = 0.f;

		// wheel
		if ( m->group == 3 )
		{
			POINT p; // fixed by jox
			p.x = 0; p.y = 0;
			::ClientToScreen(GetSafeHwnd(), &p);
			event.MouseInput.X -= p.x;
			event.MouseInput.Y -= p.y;
			event.MouseInput.Wheel = ((irr::f32)((short)HIWORD(wParam))) / (irr::f32)WHEEL_DELTA;
		}

		m_spRenderContext->PostEvent(event);
	}

	if ( WM_CHAR == message && ImpUPtr_->DlgRoomLayoutDrawlinInfo_ && ImpUPtr_->DlgRoomLayoutDrawlinInfo_->IsVisible() )
	{
		::SendMessage(ImpUPtr_->DlgRoomLayoutDrawlinInfo_->GetSafeHwnd(), message, wParam, lParam);
		::SendMessage(ImpUPtr_->DlgRoomLayoutDrawlinInfo_->GetTextHwnd(), message, wParam, lParam);
	}
	
	return CCtrlFuncView::DefWindowProc(message, wParam, lParam);
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
		::ClientToScreen(GetSafeHwnd(), &p);

		irr::SEvent evt;
		evt.EventType = irr::EET_MOUSE_INPUT_EVENT;
		evt.MouseInput.Event = irr::EMIE_MOUSE_WHEEL;
		evt.MouseInput.X = (short)LOWORD(pMsg->lParam) - p.x;
		evt.MouseInput.Y = (short)HIWORD(pMsg->lParam) - p.y;
		evt.MouseInput.Wheel = ((irr::f32)((short)HIWORD(pMsg->wParam))) / (irr::f32)WHEEL_DELTA;
		rc->PostEvent(evt);
	}

	return CView::PreTranslateMessage(pMsg);
}

BOOL CTestDrawPicView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;//CCtrlFuncView::OnEraseBkgnd(pDC);
}

void CTestDrawPicView::OnSubMenuProperty()
{
	
}

CMainFrame* CTestDrawPicView::GetMainFrame() const
{
	CMainFrame* pMain = (CMainFrame*)theApp.GetMainWnd();
	return pMain;
}

void CTestDrawPicView::OnBtnRoomLayoutCreateDrawWall()
{
	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_DRAWLINE;

	
	GetRootODL()->GetRenderContext()->PostEvent(evt);
}


void CTestDrawPicView::OnBtnRoomLayoutCreateDrawRoom()
{
	
}


void CTestDrawPicView::OnBtnRoomLayoutPicturePicture()
{
	static	boost::filesystem::path filePath;

	CFileDialog fd(TRUE, NULL, NULL, 0, _T("JPG(*.jpg)|*.jpg|ALL Files(*.*)|*.*||"));
	
	m_Render3D = false;

	if ( IDOK == fd.DoModal() )
	{
		auto pn = fd.GetPathName();
		filePath = pn.GetBuffer();
		pn.ReleaseBuffer();

		HasRoomPicture_ = true;
		ShowPic_ = true;

		irr::SEvent evt;
		evt.EventType = irr::EET_USER_EVENT;
		evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_PICTURE_SETTING;
		evt.UserEvent.UserData2 = reinterpret_cast<int>(static_cast<void*>(&filePath));

		GetRenderContext()->PostEvent(evt);
	}

	m_Render3D = true;
}

void CTestDrawPicView::Render()
{
	if (m_spRenderContext)
	{
		MSG msg;

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{	
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		m_spRenderContext->Render();
	}
}


void CTestDrawPicView::OnChkRoomLayoutPictureShow()
{
	// TODO: 在此添加命令处理程序代码

	ShowPic_ = !ShowPic_;
	
	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_PICTURE_VISIBLE;
	evt.UserEvent.UserData2 = ShowPic_ ? 1 : 0;

	m_spRenderContext->PostEvent(evt);
}


void CTestDrawPicView::OnChkUpdateRoomLayoutPictureShow(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->Enable(HasRoomPicture_ ? 1 : 0);

	if ( HasRoomPicture_ )
	{
		pCmdUI->SetCheck(ShowPic_ ? 1 : 0);
	}
}


void CTestDrawPicView::OnSlideRoomLayoutPictureAlpha()
{
	// TODO: 在此添加命令处理程序代码
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arr;

	GetMainFrame()->GetRibbonBar()->GetElementsByID(ID_SLIDE_ROOMLAYOUT_PICTURE_ALPHA, arr);

	auto slide = static_cast<CMFCRibbonSlider*>(arr[0]);

	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_PICTURE_ALPHA;
	evt.UserEvent.UserData2 = slide->GetPos();
	
	m_spRenderContext->PostEvent(evt);
}


void CTestDrawPicView::OnSlideUpdateRoomLayoutPictureAlpha(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码

	pCmdUI->Enable(HasRoomPicture_ ? 1 : 0);
}


void CTestDrawPicView::OnBtnRoomLayoutPictureSetScale()
{
	// TODO: 在此添加命令处理程序代码

	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_PICTUREREFLINE_DRAW;
	m_spRenderContext->PostEvent(evt);
}


void CTestDrawPicView::OnBtnRoomLayoutPictureSetPosition()
{
	// TODO: 在此添加命令处理程序代码

	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_PICTURE_POSITION;
	m_spRenderContext->PostEvent(evt);
}


void CTestDrawPicView::OnBtnRoomLayoutTestDoor()
{
	// TODO: 在此添加命令处理程序代码
	static SEventDoorInfo doorInfo;
	doorInfo.XLength_ = 900;
	doorInfo.YLength_ = 2000;
	doorInfo.ZLength_ = 200;

	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_TEST_DOOR;
	evt.UserEvent.UserData2 = reinterpret_cast<int>(static_cast<void*>(&doorInfo));
	m_spRenderContext->PostEvent(evt);
}


void CTestDrawPicView::OnBtnRoomlayoutTestWindow()
{
	// TODO: 在此添加命令处理程序代码
	static SEventWindowInfo windowInfo;
	windowInfo.XLength_ = 1800;
	windowInfo.YLength_ = 1400;
	windowInfo.ZLength_ = 200;
	windowInfo.SetupHeight_ = 1000;

	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_TEST_WINDOW;
	evt.UserEvent.UserData2 = reinterpret_cast<int>(static_cast<void*>(&windowInfo));
	m_spRenderContext->PostEvent(evt);
}
