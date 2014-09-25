#pragma once

//////////////////////////////////////////////////////////////////////////
//�����������ı��������+-��ť
//����ǰ�������ؼ�ʱ�����ݲ���ÿ�μӶ��ƣ�����������
//����ǰ�Ǹ������ؼ�ʱ�����ݲ���ÿ�μӶ��ƣ�����������(�����ⲿ���ÿ�����С����û��)

//////////////////////////////////////////////////////////////////////////

// CFloatSpinCtrl

class CFloatSpinCtrl : public CSpinButtonCtrl
{
	DECLARE_DYNAMIC(CFloatSpinCtrl)

public:
	CFloatSpinCtrl();
	virtual ~CFloatSpinCtrl();
public:
	void GetRangeAndDelta(double &lower, double& upper, double& delta);
	void SetRangeAndDelta(double lower, double upper, double delta);

	double GetPos();
	void   SetPos(double val);

	int  GetDecimalPlaces ();
	void SetDecimalPlaces (int num_places);
	void SetFormatString (LPCTSTR lpszFormatString = NULL);

	void SetTrimTrailingZeros (BOOL bTrim)		{ m_bTrimTrailingZeros = bTrim; }
	BOOL GetTrimTrailingZeros ()					{ return m_bTrimTrailingZeros; }
	void SetDeltaForce(bool bDeltaForce){m_bDeltaForce=bDeltaForce;}
	void SetInitNumber(DOUBLE dInitNumber){m_dInitNumber = dInitNumber;}
protected:
	void InitSpinCtrl();
	void SetIntPos (double pos);
	void SetValueForBuddy (double val);
	CString FormatValue (double val);
protected:
	virtual void PreSubclassWindow();
protected:
	double m_MinVal;
	double m_MaxVal;
	double m_Delta;
	UINT m_IntRange;
	int m_NumDecPlaces;
	CString m_strFormat;
	BOOL m_bTrimTrailingZeros;
	char m_DecSymbol;
	bool m_bDeltaForce;			//ǿ��ִ�г�ʼֵģʽ
	double m_dInitNumber;		//��ʼֵ
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnDeltapos(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


// CNumberTextCtrl

class CNumberTextCtrl : public CEdit
{
	DECLARE_DYNAMIC(CNumberTextCtrl)
	enum{ENTP_TEXT=1, ENTP_INT, ENTP_FLOAT};
public:
	CNumberTextCtrl();
	virtual ~CNumberTextCtrl();
public:
	void InitSpinButton(BOOL bShowSpin=TRUE);
	void GetRange(DOUBLE &fLower, DOUBLE& fUpper);
	void SetRange(DOUBLE fLower, DOUBLE fUpper);
	void SetLower(DOUBLE fLower);
	void SetUpper(DOUBLE fUpper);
	void SetToolTip(CString strToolTip){m_strToolTip = strToolTip;};
	void SetToolTip(CString strToolTitle, CString strToolTip)
	{m_strToolTitle = strToolTitle; m_strToolTip = strToolTip;};
	void SetInitNumber(DOUBLE fInput);		//�����ʼֵ������ֵ������ ��ʼֵ+����*x
	void SetInitNumber(INT nInput);
	BOOL SetNumber(DOUBLE fInput);
	DOUBLE GetNumber(DOUBLE& fOutput) const;
	INT GetNumber(INT& nOutput)const ;
	void RoundFloat(DOUBLE fNumber, CString& strNumber);

	float CNumberTextCtrl::GetNumber() const
	{
		return (float)m_fNumber;
	}

	void SetStep(DOUBLE fStep){m_fStep = fStep; SetRange(m_fLower, m_fUpper);};

	void SetRoundPos(INT nPos);/*С���㱣����λ*/
	void MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);
	BOOL EnableWindow(BOOL bEnable = TRUE);
	BOOL ShowWindow(int nCmdShow);
	BOOL SetReadOnly(BOOL bReadOnly = TRUE);
private:
	CFloatSpinCtrl m_spinSize;
	CMFCToolTipCtrl m_tipHelp;
	CString m_strToolTitle;
	CString m_strToolTip;
	DOUBLE m_fLower;
	DOUBLE m_fUpper;
	DOUBLE m_fStep;

	DOUBLE m_fNumber;
	DOUBLE m_fInitNumber;
	BOOL m_bIsShowSpin;
	BOOL m_bIsFloat;

	

	INT m_nPos;//����С��λ

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEnKillfocus();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


