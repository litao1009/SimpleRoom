#pragma once
#include "afxwin.h"


// CDlgSizeInfo 对话框
#include "NumberTextCtrl.h"

class CDlgSizeInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSizeInfo)

public:
	CDlgSizeInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSizeInfo();

// 对话框数据
	enum { IDD = IDD_DLG_SIZE_INFO };
public:
	void SetSizeInfo(float fXLen, float fYLen, float fZLen, float fYPos)
	{
		m_fXLen = fXLen;
		m_fYLen = fYLen;
		m_fZLen = fZLen;
		m_fYPos = fYPos;
	};
	void GetSizeInfo(float& fXLen, float& fYLen, float& fZLen, float& fYPos)
	{
		fXLen = m_fXLen;
		fYLen = m_fYLen;
		fZLen = m_fZLen;
		fYPos = m_fYPos;
	};
private:
	float m_fXLen;
	float m_fYLen;
	float m_fZLen;
	float m_fYPos;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CNumberTextCtrl m_txtXLen;
	CNumberTextCtrl m_txtYLen;
	CNumberTextCtrl m_txtYPos;
	CNumberTextCtrl m_txtZLen;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
