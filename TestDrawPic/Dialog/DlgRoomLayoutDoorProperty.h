#pragma once
#include "afxwin.h"

#include "irrEngine/SRenderContextFwd.h"

#include <memory>

// DlgRoomLayoutDoorProperty 对话框

class DlgRoomLayoutDoorProperty : public CDialogEx
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

	DECLARE_DYNAMIC(DlgRoomLayoutDoorProperty)

public:
	DlgRoomLayoutDoorProperty(const SRenderContextSPtr& rc, int data, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgRoomLayoutDoorProperty();

// 对话框数据
	enum { IDD = IDD_DLG_ROOMLAYOUT_DOORPROPERTY };

protected:
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	
	CButton BtnCancel_;
	CButton BtnOK_;
	CButton BtnDelete_;
	CButton BtnMove_;
	CEdit EditHeight_;
	CEdit EditWidth_;
	CEdit EditDepth_;

	afx_msg void OnEnChangeEditWidth();
	afx_msg void OnEnChangeEditHeight();
	afx_msg void OnEnChangeEditDepth();
	afx_msg void OnBnClickedBtnMove();
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnCancel();	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton BtnRotate_;
	afx_msg void OnBnClickedBtnRotation();
	afx_msg void OnBnClickedRadioLeftopen();
	afx_msg void OnBnClickedRadioRightopen();
};
