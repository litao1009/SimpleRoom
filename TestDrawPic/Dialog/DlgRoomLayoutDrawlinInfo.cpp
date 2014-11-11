// G:\rem\branches\RoomDecor3D\TestDrawPic\Dialog\DlgRoomLayoutDrawlinInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "DlgRoomLayoutDrawlinInfo.h"
#include "afxdialogex.h"
#include "RenderController/UserEvent.h"
#include "IEventReceiver.h"
#include "irrEngine/SRenderContext.h"

#include <string>

// DlgRoomLayoutDrawlinInfo �Ի���

IMPLEMENT_DYNAMIC(DlgRoomLayoutDrawlinInfo, CDialogEx)

DlgRoomLayoutDrawlinInfo::DlgRoomLayoutDrawlinInfo(const SRenderContextWPtr& rc, CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgRoomLayoutDrawlinInfo::IDD, pParent)
{
	RC_ = rc;
	Parent_ = pParent;
	Inputting_ = false;
}

DlgRoomLayoutDrawlinInfo::~DlgRoomLayoutDrawlinInfo()
{
}

void DlgRoomLayoutDrawlinInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TXT_NUM, EditNum_);
}


BEGIN_MESSAGE_MAP(DlgRoomLayoutDrawlinInfo, CDialogEx)
END_MESSAGE_MAP()


// DlgRoomLayoutDrawlinInfo ��Ϣ�������


BOOL DlgRoomLayoutDrawlinInfo::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	return CDialogEx::PreTranslateMessage(pMsg);
}


LRESULT DlgRoomLayoutDrawlinInfo::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	if ( message==WM_CHAR )
	{
		switch (wParam)
		{
		case VK_RETURN:
			{
				CString str;
				auto num = 0;
				EditNum_.GetWindowText(str);
				if ( !str.IsEmpty() )
				{
					num = std::stoi(str.GetBuffer());
					str.ReleaseBuffer();

					irr::SEvent evt;
					evt.EventType = irr::EET_USER_EVENT;
					evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_LINELENGTH_SET;
					evt.UserEvent.UserData2 = num;

					RC_.lock()->PostEvent(evt);

					EditNum_.SetWindowText(_T(""));
					Inputting_ = false;
				}
			}
			break;
		}
	}

	return CDialogEx::DefWindowProc(message, wParam, lParam);
}

void DlgRoomLayoutDrawlinInfo::Resize()
{
	assert(Parent_);

	CRect r;
	GetWindowRect(r);
	
	auto width = r.Width();
	auto height = r.Height();

	CRect parentR;
	Parent_->GetWindowRect(parentR);
	r.left = parentR.Width() - width;
	r.right = parentR.Width();
	r.top = 0;
	r.bottom = height;

	MoveWindow(r);
}


BOOL DlgRoomLayoutDrawlinInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return FALSE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void DlgRoomLayoutDrawlinInfo::SetNum( int num )
{
	if ( Inputting_ )
	{
		return;
	}


}
