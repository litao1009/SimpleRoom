// DlgRoomLayoutWallProperty.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "DlgRoomLayoutWallProperty.h"
#include "afxdialogex.h"

#include <string>

// DlgRoomLayoutWallProperty �Ի���

IMPLEMENT_DYNAMIC(DlgRoomLayoutWallProperty, CDialogEx)

DlgRoomLayoutWallProperty::DlgRoomLayoutWallProperty(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgRoomLayoutWallProperty::IDD, pParent)
{
	Result_ = 0;
}

DlgRoomLayoutWallProperty::~DlgRoomLayoutWallProperty()
{
}

void DlgRoomLayoutWallProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TXT_NUM, Num_);
	DDX_Control(pDX, IDOK, OKBtn_);
}


BEGIN_MESSAGE_MAP(DlgRoomLayoutWallProperty, CDialogEx)
	ON_EN_CHANGE(IDC_TXT_NUM, &DlgRoomLayoutWallProperty::OnEnChangeTxtNum)
END_MESSAGE_MAP()


// DlgRoomLayoutWallProperty ��Ϣ�������


void DlgRoomLayoutWallProperty::OnEnChangeTxtNum()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString num;
	Num_.GetWindowText(num);

	if ( num.IsEmpty() )
	{
		OKBtn_.EnableWindow(FALSE);
	}
	else
	{
		if ( num.GetLength() > 3 )
		{
			num.Truncate(3);
			Num_.SetWindowText(num);
		}

		std::wstring str = num.GetBuffer();
		num.ReleaseBuffer();
		Result_= std::stoi(str);

		OKBtn_.EnableWindow(TRUE);
	}
}
