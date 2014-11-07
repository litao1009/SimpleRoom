#pragma once
#include "afxwin.h"


// DlgPicRefSize �Ի���

class DlgPicRefSize : public CDialogEx
{
	DECLARE_DYNAMIC(DlgPicRefSize)

public:
	DlgPicRefSize(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgPicRefSize();

// �Ի�������
	enum { IDD = IDD_DLG_PIC_REF_SIZE };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
