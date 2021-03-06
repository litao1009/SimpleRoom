// G:\rem\branches\RoomDecor3D\TestDrawPic\Dialog\DlgRoomLayoutDrawlinInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "DlgRoomLayoutDrawlinInfo.h"
#include "afxdialogex.h"
#include "RenderController/UserEvent.h"
#include "IEventReceiver.h"
#include "irrEngine/SRenderContext.h"

#include <string>

// DlgRoomLayoutDrawlinInfo 对话框

IMPLEMENT_DYNAMIC(DlgRoomLayoutDrawlinInfo, CDialogEx)

DlgRoomLayoutDrawlinInfo::DlgRoomLayoutDrawlinInfo(const SRenderContextWPtr& rc, CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgRoomLayoutDrawlinInfo::IDD, pParent)
{
	RC_ = rc;
	Parent_ = pParent;
	Inputting_ = false;
	OK_ = false;
	Result_ = 0;
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
	ON_EN_CHANGE(IDC_TXT_NUM, &DlgRoomLayoutDrawlinInfo::OnEnChangeTxtNum)
END_MESSAGE_MAP()


// DlgRoomLayoutDrawlinInfo 消息处理程序


BOOL DlgRoomLayoutDrawlinInfo::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	return CDialogEx::PreTranslateMessage(pMsg);
}


LRESULT DlgRoomLayoutDrawlinInfo::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( message==WM_CHAR )
	{
		Inputting_ = true;

		switch (wParam)
		{
		case VK_RETURN:
			{
				if ( !OK_ )
				{
					break;
				}

				irr::SEvent evt;
				evt.EventType = irr::EET_USER_EVENT;
				evt.UserEvent.UserData1 = EUT_ROOMLAYOUT_LINELENGTH_SET;
				evt.UserEvent.UserData2 = Result_;

				RC_.lock()->PostEvent(evt);

				EditNum_.SetWindowText(_T(""));
				Inputting_ = false;
			}
			break;
		case VK_ESCAPE:
			{
				Inputting_ = false;
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

	// TODO:  在此添加额外的初始化

	return FALSE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void DlgRoomLayoutDrawlinInfo::SetNum( int num )
{
	if ( Inputting_ )
	{
		return;
	}

	EditNum_.SetWindowText(std::to_wstring(num).c_str());
	EditNum_.SetSel(0, -1);
}


void DlgRoomLayoutDrawlinInfo::OnEnChangeTxtNum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString num;
	EditNum_.GetWindowText(num);

	if ( num.GetLength() < 3 || num.GetLength() > 5 )
	{
		OK_ = false;
	}
	else
	{
		OK_ = true;
		std::wstring str = num.GetBuffer();
		num.ReleaseBuffer();
		Result_= std::stoi(str);
	}
}
