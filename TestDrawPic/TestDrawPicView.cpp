// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// TestDrawPicView.cpp : CTestDrawPicView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣

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
	// ��׼��ӡ����
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
	//�༭

	ON_COMMAND(ID_BTN_EDIT_WALL_SKIN, &CTestDrawPicView::OnBtnEditWallSkin)
	ON_UPDATE_COMMAND_UI(ID_BTN_EDIT_WALL_SKIN, &CTestDrawPicView::OnUpdateBtnEditWallSkin)

	ON_COMMAND(ID_BTN_EDIT_FLOOR_SKIN, &CTestDrawPicView::OnBtnEditFloorSkin)
	ON_UPDATE_COMMAND_UI(ID_BTN_EDIT_FLOOR_SKIN, &CTestDrawPicView::OnUpdateBtnEditFloorSkin)

	ON_COMMAND(ID_BTN_EDIT_WALL_ZONE, &CTestDrawPicView::OnBtnEditWallZone)
	ON_UPDATE_COMMAND_UI(ID_BTN_EDIT_WALL_ZONE, &CTestDrawPicView::OnUpdateBtnEditWallZone)

	ON_COMMAND(ID_BTN_EDIT_FLOOR_ZONE, &CTestDrawPicView::OnBtnEditFloorZone)
	ON_UPDATE_COMMAND_UI(ID_BTN_EDIT_FLOOR_ZONE, &CTestDrawPicView::OnUpdateBtnEditFloorZone)

	//////////////////////////////////////////////////////////////////////////
	//�鿴

	ON_COMMAND(ID_BTN_VIEW_SHOW, &CTestDrawPicView::OnBtnViewShow)
	ON_UPDATE_COMMAND_UI(ID_BTN_VIEW_SHOW, &CTestDrawPicView::OnUpdateBtnViewShow)

	ON_COMMAND(ID_BTN_VIEW_HIDE, &CTestDrawPicView::OnBtnViewHide)
	ON_UPDATE_COMMAND_UI(ID_BTN_VIEW_HIDE, &CTestDrawPicView::OnUpdateBtnViewHide)

	//////////////////////////////////////////////////////////////////////////
	//�Ӿ���ͼ
	ON_COMMAND(ID_BTN_VIEW_TOP, &CTestDrawPicView::OnBtnViewTop)
	ON_UPDATE_COMMAND_UI(ID_BTN_VIEW_TOP, &CTestDrawPicView::OnUpdateBtnViewTop)

	ON_COMMAND(ID_BTN_VIEW_RENDER, &CTestDrawPicView::OnBtnViewRender)
	ON_UPDATE_COMMAND_UI(ID_BTN_VIEW_RENDER, &CTestDrawPicView::OnUpdateBtnViewRender)

	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_SUB_MENU_PROPERTY, &CTestDrawPicView::OnSubMenuProperty)

END_MESSAGE_MAP()

// CTestDrawPicView ����/����

CTestDrawPicView::CTestDrawPicView()
{
	// TODO: �ڴ˴���ӹ������
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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CCtrlFuncView::PreCreateWindow(cs);
}

// CTestDrawPicView ����

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


// CTestDrawPicView ��ӡ


void CTestDrawPicView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTestDrawPicView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CTestDrawPicView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CTestDrawPicView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CTestDrawPicView ���

#ifdef _DEBUG
void CTestDrawPicView::AssertValid() const
{
	CCtrlFuncView::AssertValid();
}

void CTestDrawPicView::Dump(CDumpContext& dc) const
{
	CCtrlFuncView::Dump(dc);
}

CTestDrawPicDoc* CTestDrawPicView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestDrawPicDoc)));
	return (CTestDrawPicDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestDrawPicView ��Ϣ�������


BOOL CTestDrawPicView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���

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
	//�ж�����ڶ�����û�����������ƶ�����
	
	CCtrlFuncView::OnLButtonDown(nFlags, point);
}

void CTestDrawPicView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CCtrlFuncView::OnLButtonUp(nFlags, point);
}

void CTestDrawPicView::OnMouseMove(UINT nFlags, CPoint point)
{
	//�ҿ��������б�ѡ�еģ��У��ҵ�ǰ������£����ƶ��ö���
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//��������ȥ��
	CCtrlFuncView::OnMouseLeave();
}

void CTestDrawPicView::ClearState()
{
	Invalidate();
}

void CTestDrawPicView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// �˹���Ҫ�� Windows Vista ����߰汾��
	// _WIN32_WINNT ���ű��� >= 0x0600��
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ������������û����洦��������
}

void CTestDrawPicView::OnBtnCreateWallRect()
{
	StatusMgr::GetInstance().DrawingState_ = StatusMgr::EDS_RECT_WALL;
}

void CTestDrawPicView::OnUpdateBtnCreateWallRect(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}

void CTestDrawPicView::OnBtnCreateBeamRect()
{
	
}


void CTestDrawPicView::OnUpdateBtnCreateBeamRect(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}


void CTestDrawPicView::OnBtnCreatePillarRect()
{
	
}


void CTestDrawPicView::OnUpdateBtnCreatePillarRect(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}

void CTestDrawPicView::OnBtnCreateRoomAuto()
{
	// TODO: �ڴ���������������
}


void CTestDrawPicView::OnUpdateBtnCreateRoomAuto(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}


void CTestDrawPicView::OnBtnCreateRoomStyle()
{
	
}

void CTestDrawPicView::OnUpdateBtnCreateRoomStyle(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}


void CTestDrawPicView::OnBtnEditWallSkin()
{
	// TODO: �ڴ���������������
}


void CTestDrawPicView::OnUpdateBtnEditWallSkin(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}

void CTestDrawPicView::OnBtnEditFloorSkin()
{
	// TODO: �ڴ���������������
}


void CTestDrawPicView::OnUpdateBtnEditFloorSkin(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}

void CTestDrawPicView::OnBtnEditWallZone()
{
	
}

void CTestDrawPicView::OnUpdateBtnEditWallZone(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}

void CTestDrawPicView::OnBtnEditFloorZone()
{
	
}


void CTestDrawPicView::OnUpdateBtnEditFloorZone(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}


void CTestDrawPicView::OnBtnViewHide()
{
	// TODO: �ڴ���������������
}


void CTestDrawPicView::OnUpdateBtnViewHide(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}


void CTestDrawPicView::OnBtnViewShow()
{
	//��ʾ���صĶ���
}


void CTestDrawPicView::OnUpdateBtnViewShow(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}

void CTestDrawPicView::OnBtnViewTop()
{
	//�л�������ͼ��ȡ��δ��ɵ����в���
	
}

void CTestDrawPicView::OnUpdateBtnViewTop(CCmdUI *pCmdUI)
{
	//���á����� �л�������ͼ����
	//�༭ǽ�ڷ������ذ����ʱ����ֹ�л���ֱ���༭���
}

void CTestDrawPicView::OnBtnViewRender()
{
	//
}

void CTestDrawPicView::OnUpdateBtnViewRender(CCmdUI *pCmdUI)
{
	//���á����� ������Ⱦ����
}

BOOL CTestDrawPicView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	return CCtrlFuncView::OnDrop(pDataObject, dropEffect, point);
}


DROPEFFECT CTestDrawPicView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	// TODO: �ڴ����ר�ô����/����û���
	//�����ǰ����״̬
	return DROPEFFECT_MOVE;
}

DROPEFFECT CTestDrawPicView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	
	return DROPEFFECT_NONE;
}


void CTestDrawPicView::OnDragLeave()
{
	// TODO: �ڴ����ר�ô����/����û���
	
	CCtrlFuncView::OnDragLeave();
}

void CTestDrawPicView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	CCtrlFuncView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CTestDrawPicView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	CCtrlFuncView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CTestDrawPicView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

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