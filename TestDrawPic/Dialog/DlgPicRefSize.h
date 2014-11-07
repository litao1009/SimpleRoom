#pragma once
#include "afxwin.h"


// DlgPicRefSize 对话框

class DlgPicRefSize : public CDialogEx
{
	DECLARE_DYNAMIC(DlgPicRefSize)

public:
	DlgPicRefSize(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgPicRefSize();

// 对话框数据
	enum { IDD = IDD_DLG_PIC_REF_SIZE };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

public:

	unsigned	GetNum() const;
	
public:

	afx_msg void OnEnChangeTxtPicRefSizeNum();
	
private:

	CEdit	NumText_;
	CButton OKBtn_;
	int		Result_;
public:
	virtual BOOL OnInitDialog();
};
