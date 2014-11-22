#pragma once
#include "afxwin.h"

#include "irrEngine/SRenderContextFwd.h"

#include <memory>

// DlgRoomLayoutWindowProperty �Ի���

class DlgRoomLayoutWindowProperty : public CDialogEx
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

	DECLARE_DYNAMIC(DlgRoomLayoutWindowProperty)

public:
	DlgRoomLayoutWindowProperty(const SRenderContextSPtr& rc, int data, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgRoomLayoutWindowProperty();

// �Ի�������
	enum { IDD = IDD_DLG_ROOMLAYOUT_WINDOWPROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CButton BtnCancel_;
	CButton BtnOK_;
	CButton BtnDelete_;
	CButton BtnMove_;
	CEdit EditDepth_;
	CEdit EditHeight_;
	CEdit EditOffsetHeight_;
	CEdit EditWidth_;
	
	afx_msg void OnEnChangeEditWidth();
	afx_msg void OnEnChangeEditHeight();
	afx_msg void OnEnChangeEditDepth();
	afx_msg void OnEnChangeEditOffsetheight();
	afx_msg void OnBnClickedBtnMove();
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnCancel();
};
