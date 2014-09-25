#pragma once
#include "afxwin.h"


// CDlgZoneInfo 对话框
#include "NumberTextCtrl.h"
#include "afxcolorbutton.h"

class CDlgZoneInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgZoneInfo)

public:
	CDlgZoneInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgZoneInfo();

// 对话框数据
	enum { IDD = IDD_DLG_ZONE_INFO };
public:
	void SetGroutXY(int nGroutX, int nGroutY){m_nGroutX = nGroutX; m_nGroutY = nGroutY;};
	void SetRotate(float fRotate){m_fRotate = fRotate;};
	void SetColor(COLORREF clColor){m_clColor = clColor;};
	void SetWrapMode(INT nMode){m_nWrapMode=nMode;};

	void GetGroutXY(int& nGroutX, int& nGroutY){nGroutX = m_nGroutX; nGroutY=m_nGroutY;};
	float GetRotate(){return m_fRotate;};
	COLORREF GetColor(){return m_clColor;};
	INT GetWrapMode(){return m_nWrapMode;};
private:
	int m_nGroutX;
	int m_nGroutY;
	float m_fRotate;
	INT m_nWrapMode;
	COLORREF m_clColor;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CNumberTextCtrl m_txtGroutX;
	CNumberTextCtrl m_txtGroutY;
	CNumberTextCtrl m_txtRotate;
	afx_msg void OnBnClickedOk();
	CMFCColorButton m_cbxColor;
	virtual BOOL OnInitDialog();
	CComboBox m_cbxWrapMode;
};
