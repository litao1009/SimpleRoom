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

	
	irr::SEvent evt;
	evt.EventType = irr::EET_MOUSE_INPUT_EVENT;
	evt.MouseInput.X = (short)LOWORD(lParam);
	evt.MouseInput.Y = (short)HIWORD(lParam);
	bool bNeedRedraw=false;
	switch (message)
	{
	case WM_PAINT:
		{
			bNeedRedraw=true;
		}
		break;
	case WM_MOUSEMOVE:
		{
			bNeedRedraw=true;
			evt.MouseInput.Event = irr::EMIE_MOUSE_MOVED;
			m_spRenderContext->PostEvent(evt);
		}
		break;
	case WM_MOUSEWHEEL:
		{
			return TRUE;
		}
		break;
	case WM_MBUTTONDOWN:
		{
			bNeedRedraw=true;
			m_bMButtonDown = true;
			ClipCursor(rtMain);
			evt.MouseInput.Event = irr::EMIE_MMOUSE_PRESSED_DOWN;
			m_spRenderContext->PostEvent(evt);
		}
		break;
	case WM_MBUTTONUP:
		{
			bNeedRedraw=true;
			m_bMButtonDown = false;
			ClipCursor(NULL);
			evt.MouseInput.Event = irr::EMIE_MMOUSE_LEFT_UP;
			m_spRenderContext->PostEvent(evt);
			this->GetParent()->Invalidate();
		}
		break;
	case WM_LBUTTONDOWN:
		{
			bNeedRedraw=true;
			m_bLButtonDown = true;
			evt.MouseInput.Event = irr::EMIE_LMOUSE_PRESSED_DOWN;
			SetCapture();
			GetCapture();
			m_spRenderContext->PostEvent(evt);
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			bNeedRedraw=true;

			evt.MouseInput.Event = irr::EMIE_LMOUSE_DOUBLE_CLICK;
			m_spRenderContext->PostEvent(evt);
		}
		break;
	case WM_LBUTTONUP:
		{
			bNeedRedraw=true;
			m_bLButtonDown = false;
			evt.MouseInput.Event = irr::EMIE_LMOUSE_LEFT_UP;
			ReleaseCapture();
			m_spRenderContext->PostEvent(evt);
			this->GetParent()->Invalidate();
		}
		break;
	case WM_RBUTTONDOWN:
		{
			bNeedRedraw=true;
			m_bRButtonDown = true;
			evt.MouseInput.Event = irr::EMIE_RMOUSE_PRESSED_DOWN;
			m_spRenderContext->PostEvent(evt);
		}
		break;
	case WM_RBUTTONUP:
		{
			bNeedRedraw=true;
			m_bRButtonDown = false;
			evt.MouseInput.Event = irr::EMIE_RMOUSE_LEFT_UP;
			m_spRenderContext->PostEvent(evt);
			this->GetParent()->Invalidate();
		}
		break;
	default:
		break;
	}
	if (bNeedRedraw)
	{
		Invalidate();
	}
	return CStatic::DefWindowProc(message, wParam, lParam);
}


BOOL CSubViewer::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (pMsg->message==WM_KEYDOWN)
	{

	}

	if (pMsg->message==WM_KEYUP)
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

		m_spRenderContext->PostEvent(evt);
	}
	return CStatic::PreTranslateMessage(pMsg);
}


void CSubViewer::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
	// TODO:  添加您的代码以绘制指定项
	static unsigned count = 0;
	if ( m_spRenderContext )
	{
		m_spRenderContext->Render();

		//auto str = std::to_wstring(fps);
		//str = std::to_wstring(count++) + L" FPS:" + str + L"\n";
		//OutputDebugString(str.c_str());

		auto pos = m_spRenderContext->Smgr_->getActiveCamera()->getPosition();
		auto target = m_spRenderContext->Smgr_->getActiveCamera()->getTarget();
		auto pPnt = static_cast<CTestDrawPicView*>(GetParent());

		assert( nullptr != pPnt );

		//pPnt->SetCameraInfo(m_spRenderContext->IsCameraVisible(), gp_Pnt(pos.X, pos.Y, pos.Z), gp_Pnt(target.X, target.Y, target.Z), m_spRenderContext->Smgr_->getActiveCamera()->getFOV());
	}
}

void CSubViewer::OnTimer(UINT_PTR nIDEvent)
{
	//渲染
	if (m_spRenderContext)
	{
		m_spRenderContext->Render();
	}
	CStatic::OnTimer(nIDEvent);
}