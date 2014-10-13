// SubViewer.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "SubViewer.h"

#include "irrEngine/SRenderContext.h"
#include "RenderController/EventUserType.h"
#include "TestDrawPicView.h"

#include <string>
// CSubViewer

IMPLEMENT_DYNAMIC(CSubViewer, CStatic)

CSubViewer::CSubViewer()
{
	m_bLButtonDown = false;
	m_bMButtonDown = false;
	m_bRButtonDown = false;
}

CSubViewer::~CSubViewer()
{
	
}


BEGIN_MESSAGE_MAP(CSubViewer, CStatic)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CSubViewer 消息处理程序




int CSubViewer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_pDC = this->GetDC();
	m_hDC = m_pDC->GetSafeHdc();
	// TODO:  在此添加您专用的创建代码
	m_ptStart.SetPoint(0,0);
	SetTimer(1000, 20/*ms*/,NULL);
	return 0;
}


void CSubViewer::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);
	TRACE("OnSize:%d,%d\n", cx, cy);
	//当发生窗口尺寸变化时
	
	if ( m_spRenderContext )
	{
		m_spRenderContext->SetOnResize();
	}
}


LRESULT CSubViewer::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	//return CStatic::DefWindowProc(message, wParam, lParam);
	CRect rtMain;
	GetWindowRect(rtMain);
	if (message==WM_IRR_DLG_MSG)
	{
		switch (wParam)
		{
		case WM_RESIZE_CHANGE_BEGIN:
			{
				//开始缩放
				GetWindowRect(m_rtMain);
				this->GetParent()->ScreenToClient(m_rtMain);
				GetCursorPos(&m_ptStart);
				this->GetParent()->ScreenToClient(&m_ptStart);
			}
			break;
		case WM_RESIZE_CHANGE_DURING:
			{
				//缩放中
				CRect rtParent;
				this->GetParent()->GetClientRect(rtParent);
				CPoint ptCurrent;
				GetCursorPos(&ptCurrent);
				this->GetParent()->ScreenToClient(&ptCurrent);
				
				auto curWidth = m_rtMain.Width();
				auto curHeight = m_rtMain.Height();

				curWidth += ptCurrent.x - m_ptStart.x;
				curHeight += m_ptStart.y - ptCurrent.y;

				curWidth = max(curWidth, 200);
				curWidth = min(curWidth, rtParent.Width());
				curHeight = max(curHeight, 200);
				curHeight = min(curHeight, rtParent.Height());

				if ( curWidth != m_rtMain.Width() || curHeight != m_rtMain.Height() )
				{
					m_rtMain.left = 0;
					m_rtMain.right = curWidth;
					m_rtMain.top = rtParent.Height() - curHeight;
					m_rtMain.bottom = rtParent.Height();

					MoveWindow(m_rtMain);
				}

				m_ptStart = ptCurrent;
			}
			break;
		case WM_RESIZE_CHANGE_END:
			{
				//缩放结束
				ClipCursor(NULL);
			}
			break;
		case WM_RESIZE_MINIMUM:
			{
				CRect rtParent;
				this->GetParent()->GetClientRect(rtParent);
				GetClientRect(m_rtMain);

				CRect rtMin;
				rtMin.left = 0;
				rtMin.right = 30;
				rtMin.top = rtParent.Height() - 30;
				rtMin.bottom = rtParent.Height();

				MoveWindow(rtMin);
			}
			break;
		case  WM_RESIZE_RESTORE:
			{
				MoveWindow(m_rtMain);
			}
			break;
		}
		return CStatic::DefWindowProc(message, wParam, lParam);
	}

	static irr::s32 ClickCount=0;
	if (::GetCapture() != GetSafeHwnd() && ClickCount > 0)
		ClickCount = 0;

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
	
	return CStatic::DefWindowProc(message, wParam, lParam);
}


BOOL CSubViewer::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CStatic::PreTranslateMessage(pMsg);
}


void CSubViewer::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
	// TODO:  添加您的代码以绘制指定项
	//渲染
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

void CSubViewer::OnTimer(UINT_PTR nIDEvent)
{
	Invalidate();

	CStatic::OnTimer(nIDEvent);
}

BOOL CSubViewer::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	KillTimer(1000);
	return CStatic::DestroyWindow();
}
