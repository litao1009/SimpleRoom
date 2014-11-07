// G:\rem\branches\RoomDecor3D\TestDrawPic\Dialog\DlgPicRefSize.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "G:\rem\branches\RoomDecor3D\TestDrawPic\Dialog\DlgPicRefSize.h"
#include "afxdialogex.h"

#include <string>

// DlgPicRefSize �Ի���

IMPLEMENT_DYNAMIC(DlgPicRefSize, CDialogEx)

DlgPicRefSize::DlgPicRefSize(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgPicRefSize::IDD, pParent)
{
	Result_ = 0;
}

DlgPicRefSize::~DlgPicRefSize()
{
}

void DlgPicRefSize::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TXT_PIC_REF_SIZE_NUM, NumText_);
	DDX_Control(pDX, IDOK, OKBtn_);
}

BOOL DlgPicRefSize::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if ( pMsg->message==WM_KEYDOWN || pMsg->message == WM_KEYUP )
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:return TRUE;
		case VK_ESCAPE:return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BEGIN_MESSAGE_MAP(DlgPicRefSize, CDialogEx)
	ON_EN_CHANGE(IDC_TXT_PIC_REF_SIZE_NUM, &DlgPicRefSize::OnEnChangeTxtPicRefSizeNum)
END_MESSAGE_MAP()

unsigned DlgPicRefSize::GetNum() const
{
	return Result_;
}

// DlgPicRefSize ��Ϣ�������


void DlgPicRefSize::OnEnChangeTxtPicRefSizeNum()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString num;
	NumText_.GetWindowText(num);

	if ( num.IsEmpty() )
	{
		OKBtn_.EnableWindow(FALSE);
	}
	else
	{
		std::wstring str = num.GetBuffer();
		num.ReleaseBuffer();
		Result_= std::stoi(str);

		OKBtn_.EnableWindow(TRUE);
	}
}


BOOL DlgPicRefSize::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	OKBtn_.EnableWindow(FALSE);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
