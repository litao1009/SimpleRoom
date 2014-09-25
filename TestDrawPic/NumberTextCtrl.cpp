// NumberTextCtrl.cpp : 实现文件
//
//

#include "stdafx.h"
#include "NumberTextCtrl.h"
#include <math.h>
#include <locale.h>

// CFloatSpinCtrl
#define EPS 1.0e-5

IMPLEMENT_DYNAMIC(CFloatSpinCtrl, CSpinButtonCtrl)

CFloatSpinCtrl::CFloatSpinCtrl()
{
	m_Delta = 1;
	m_MinVal = -65534;
	m_MaxVal = 65535;
	m_IntRange = 65535;

	lconv* pLconv = localeconv ();
	m_DecSymbol = *pLconv->decimal_point;
	m_bTrimTrailingZeros = TRUE;
	SetDecimalPlaces (-1); // simple formatting through "%g"
	SetFormatString (NULL);
	m_bDeltaForce=false;
	m_dInitNumber = m_MinVal;
}

CFloatSpinCtrl::~CFloatSpinCtrl()
{
}


BEGIN_MESSAGE_MAP(CFloatSpinCtrl, CSpinButtonCtrl)
	ON_NOTIFY_REFLECT_EX(UDN_DELTAPOS, &CFloatSpinCtrl::OnDeltapos)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CFloatSpinCtrl 消息处理程序

void CFloatSpinCtrl::SetValueForBuddy (double val)
{
	CWnd* pEdit = GetBuddy();
	if (pEdit)
	{
		CString str;
		if (m_strFormat.IsEmpty ())
			str = FormatValue (val);
		else
			str.Format (m_strFormat, val);

		pEdit->SetWindowText (str);
		if (pEdit->GetParent())
		{
			//::PostMessage(pEdit->GetParent()->GetSafeHwnd(), WM_COMMAND, WM_SETTEXT, (LPARAM)this);
		}
	}
}

void CFloatSpinCtrl::SetPos(double val)
{
	SetValueForBuddy (val);
	SetIntPos (val);
}

double CFloatSpinCtrl::GetPos()
{
	CWnd* pEdit = GetBuddy();
	if (pEdit)
	{
		CString str;
		pEdit->GetWindowText (str);
		double val = _ttof (str);
		return val;
	}
	else
	{
		//ASSERT (FALSE); // you didn't set buddy
		return 0.0;
	}
}

void CFloatSpinCtrl::SetRangeAndDelta(double lower, double upper, double delta)
{
	m_MinVal = lower;
	m_MaxVal = upper;
	m_Delta = delta;

	//// avoid division by zero
	if (m_Delta == 0.0)
		return;

	/// Figure out the integer range to use, so that acceleration can work properly
	double range = fabs ((m_MaxVal - m_MinVal) / m_Delta);
	if (range > (double)UD_MAXVAL)
		m_IntRange = UD_MAXVAL;
	else
		m_IntRange = (int) range;
	if ((int)range>0)
	{
		CSpinButtonCtrl::SetRange32 (0, m_IntRange);
	}
	else
	{
		CSpinButtonCtrl::SetRange32 ((INT)m_MinVal, (INT)m_MaxVal);
	}

	/// Set integer position
	SetIntPos (GetPos());
	//如果当前最大=最小
	CWnd* pEdit = GetBuddy();
	if (pEdit)
	{
		pEdit->EnableWindow(fabs(m_MaxVal - m_MinVal) > 0.0001f);
	}
}

void CFloatSpinCtrl::SetIntPos (double pos)
{
	//// avoid division by zero
	if (m_MaxVal == m_MinVal)
		return;

	int int_pos;

	if (pos < m_MinVal)
		int_pos = 0;
	else if (pos > m_MaxVal)
		int_pos = m_IntRange;
	else
	{
		if (m_bDeltaForce)
		{
			double dTmp = pos-m_dInitNumber;
			dTmp /=m_Delta;
			pos= m_dInitNumber+m_Delta * (INT)dTmp;
		}
		// where in the range is current position?
		double pos_in_range = (pos - m_MinVal) / (m_MaxVal - m_MinVal);
		int_pos = (int)(m_IntRange * pos_in_range + 0.5);
		
	}
	CSpinButtonCtrl::SetPos (int_pos);
}

void CFloatSpinCtrl::GetRangeAndDelta(double& lower, double& upper, double& delta)
{
	lower = m_MinVal;
	upper = m_MaxVal;
	delta = m_Delta;
}

BOOL CFloatSpinCtrl::OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pUD = (NM_UPDOWN*)pNMHDR;

	double val = GetPos () + m_Delta * pUD->iDelta;

	const bool can_wrap = (UDS_WRAP & GetStyle());

	double dUpperValue = m_MaxVal;
	double dLowerValue = m_MinVal;
	if (m_bDeltaForce)
	{
		//使用强制初始值步长模式
		INT nStepCount = 0;
		dUpperValue = dUpperValue- m_dInitNumber;
		while (dUpperValue>=m_Delta)
		{
			dUpperValue -= m_Delta;
			nStepCount++;
		}
		dUpperValue = m_dInitNumber+ nStepCount * m_Delta;

		nStepCount = 0;
		dLowerValue = m_dInitNumber - dLowerValue;
		while (dLowerValue>=m_Delta)
		{
			dLowerValue -= m_Delta;
			nStepCount++;
		}
		dLowerValue = m_dInitNumber - nStepCount * m_Delta;
		//然后重置当前数据
		//当前数据必须满足  initnumber+ delta*x的模式
		double dTmp = val-m_dInitNumber;
		dTmp /=m_Delta;
		val= m_dInitNumber+m_Delta* (INT)dTmp;
	}
	if (pUD->iDelta < 0) // spin down
	{
		double abs_eps = fabs(EPS * max (val, dLowerValue));
		if (abs_eps < EPS) abs_eps = EPS;

		if (dLowerValue - val > abs_eps){  //if (val < m_MinVal){		
			if(can_wrap){
				val = dUpperValue;
			}
			else{
				val = dLowerValue;
			}
		}
	}
	else  // spin up
	{
		double abs_eps = fabs(EPS * max (val, dUpperValue));
		if (abs_eps < EPS) abs_eps = EPS;

		if (val - dUpperValue > abs_eps){   //if (val > m_MaxVal){		
			if(can_wrap){
				val = dLowerValue;
			}
			else{
				val = dUpperValue;
			}
		}
	}

	SetValueForBuddy (val);

	*pResult = 0;

	return FALSE; // let parent process this notification too.
}

int CFloatSpinCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSpinButtonCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitSpinCtrl();

	return 0;
}

void CFloatSpinCtrl::PreSubclassWindow() 
{
	CSpinButtonCtrl::PreSubclassWindow();
	InitSpinCtrl();
}

void CFloatSpinCtrl::InitSpinCtrl()
{
	ASSERT ((GetStyle () & UDS_SETBUDDYINT) != UDS_SETBUDDYINT); // Otherwise control won't work properly!
	//ModifyStyle (UDS_SETBUDDYINT, UDS_ARROWKEYS);
	SetRangeAndDelta (m_MinVal, m_MaxVal, m_Delta); // set default values
}

CString CFloatSpinCtrl::FormatValue (double val)
{
	CString str;

	if (m_NumDecPlaces == -1) // no precision specified
	{
		str.Format (_T("%g"), val);
		return str;
	}

	CString fstr;

	/// Check which format to use ('e' or 'f')
	bool bExponential;
	fstr.Format (_T("%g"), val);
	if (fstr.Find (_T('e')) != -1)
	{
		fstr.Format (_T("%%.%de"), m_NumDecPlaces);
		bExponential = true;
	}
	else
	{
		fstr.Format (_T("%%.%df"), m_NumDecPlaces);
		bExponential = false;
	}

	/// Output the number with given format
	str.Format (fstr, val);

	/// Check for trailing zeros and remove them
	if (m_bTrimTrailingZeros)
	{
		if (bExponential)
		{
			// separate base and exponent
			CString strBase, strExponent;
			int e_pos = str.Find (_T('e'));
			strBase = str.Left (e_pos);
			strExponent = str.Right (str.GetLength () - e_pos);

			if (str.Find (m_DecSymbol) != -1){
				strBase.TrimRight (_T('0'));
				strBase.TrimRight (m_DecSymbol); // if dot is all that's left
			}

			str = strBase + strExponent; //reconstruct
		}
		else
		{
			if (str.Find (m_DecSymbol) != -1){
				str.TrimRight (_T('0'));
				str.TrimRight (m_DecSymbol); // if dot is all that's left
			}
		}
	}

	return str;
}

void CFloatSpinCtrl::SetDecimalPlaces(int number)
{
	m_NumDecPlaces = number;
}

int CFloatSpinCtrl::GetDecimalPlaces()
{
	return m_NumDecPlaces;
}

void CFloatSpinCtrl::SetFormatString (LPCTSTR lpszFormatString /*= NULL*/)
{
	if (lpszFormatString == NULL)
		m_strFormat.Empty ();
	else
		m_strFormat = lpszFormatString;
}

// CNumberTextCtrl


IMPLEMENT_DYNAMIC(CNumberTextCtrl, CEdit)

	CNumberTextCtrl::CNumberTextCtrl()
{
	//std::numeric_limits<float>::min()
	m_fLower = -UD_MAXVAL;
	m_fUpper = UD_MAXVAL;
	m_fStep = 1.0f;
	m_nPos=1;
	m_fNumber = 0;
	m_bIsFloat=FALSE;
	m_strToolTip=_T("");
	m_strToolTitle=_T("提示");
}

CNumberTextCtrl::~CNumberTextCtrl()
{
}


BEGIN_MESSAGE_MAP(CNumberTextCtrl, CEdit)
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, &CNumberTextCtrl::OnEnKillfocus)
END_MESSAGE_MAP()


void CNumberTextCtrl::SetRange(DOUBLE fLower, DOUBLE fUpper)
{
	//std::to_string(fLower).length()
	if ((fLower<65535.0f)&&(fLower>=-65535.0f))
	{
		m_fLower=fLower;
	}
	if ((fUpper<=65535.0f)&&(fUpper>=-65535.0f))
	{
		m_fUpper=fUpper;
	}
	if (m_fLower>m_fUpper)
	{
		DOUBLE fTmp=m_fUpper;
		m_fUpper=m_fLower;
		m_fLower=fTmp;
	}
	if (m_spinSize.GetSafeHwnd())
	{
		m_spinSize.SetRangeAndDelta(m_fLower, m_fUpper, m_fStep);
	}
}

void CNumberTextCtrl::SetLower(DOUBLE fLower)
{
	SetRange(fLower, m_fUpper);
}

void CNumberTextCtrl::SetUpper(DOUBLE fUpper)
{
	SetRange(m_fLower, fUpper);
}

void CNumberTextCtrl::GetRange(DOUBLE& fLower, DOUBLE& fUpper)
{
	fLower = m_fLower;
	fUpper = m_fUpper;
}

void CNumberTextCtrl::SetRoundPos(INT nPos)
{
	INT nPosT=nPos;
	if (nPos>10)
	{
		nPosT=10;
	}
	if (nPos<0)
	{
		nPosT=1;
	}
	m_nPos=nPosT;
}

DOUBLE CNumberTextCtrl::GetNumber(DOUBLE& fOutput) const
{
	fOutput = m_fNumber;
	return m_fNumber;
}

INT CNumberTextCtrl::GetNumber(INT& nOutput) const
{
	nOutput =(INT) m_fNumber;
	return (INT)m_fNumber;
}
BOOL CNumberTextCtrl::SetNumber(DOUBLE fInput)
{
	BOOL bIsChanged=FALSE;
	if (m_spinSize.GetSafeHwnd())
	{
		m_spinSize.SetPos(fInput);
		DOUBLE fRet = m_spinSize.GetPos();
		bIsChanged = fRet !=fInput;
	}
	m_fNumber = fInput;
	CString strNumber;
	RoundFloat(m_fNumber, strNumber);
	SetWindowText(strNumber);
	return bIsChanged;
}

void CNumberTextCtrl::RoundFloat(DOUBLE fNumber, CString& strNumber)
{
	strNumber.Format(_T("%f"), fNumber);
	strNumber.Delete(m_nPos,strNumber.GetLength() - m_nPos);
}

void CNumberTextCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	BOOL bConitue;
	
	bConitue = (nChar >= '0' && nChar <= '9')
		|| (nChar == '.'&& m_nPos > 0 )
		|| nChar == '-'
		|| nChar == VK_BACK
		|| nChar==VK_HOME
		|| nChar==VK_END
		|| nChar==VK_DELETE
		|| nChar == VK_RETURN;
	if(!bConitue) return;
	CEdit::OnChar(  nChar,  nRepCnt,  nFlags);	
	return;
	CString sBefore,sAfter;	
	GetWindowText(sBefore);// 先获取文本
	CEdit::OnChar(  nChar,  nRepCnt,  nFlags);	
	GetWindowText(sAfter);// 先获取文本	
	int nLength = sAfter.GetLength();//获取长度
	if((nLength==1)&&(nChar == 13))
	{
		SetWindowText(_T("0"));
		UpdateData(TRUE);
		return;	
	}
	int nPos = sAfter.Find(_T(".")); // 找小数点位置
	//如果有小数点，但大于设置的精度，则取消当前输入的字符
	if(nPos != CB_ERR && nLength - nPos -1 > m_nPos)
	{
		SetWindowText(sBefore);            // 还原数字
		UpdateData(TRUE);
		return;
	}
	if(nChar == '-')	
	{
		sBefore.Insert(0, _T("-"));
		if (sBefore!=sAfter)
		{
			//当前-号不是第一个字符
			SetWindowText(sBefore);            // 还原数字
			UpdateData(TRUE);
			return;
		}
	}
	//第一个字符是小数点
	if(nLength > 0 && sAfter.Left(1) == ".")
	{
		SetWindowText(sBefore);            // 还原数字
		UpdateData(TRUE);
		return;
	}	//只有一个小数点	
	if(sBefore.Find(_T(".")) != CB_ERR && nChar == VK_DELETE)
	{
		SetWindowText(sBefore);
		UpdateData(TRUE);
		return;
	}
	
}


void CNumberTextCtrl::InitSpinButton(BOOL bShowSpin)
{
	UINT nTextID = this->GetDlgCtrlID();
	if (!m_spinSize.GetSafeHwnd())
	{
		//计算当前窗口大小

		DWORD dwStyle=WS_CHILD|WS_VISIBLE|UDS_ALIGNRIGHT|UDS_ARROWKEYS;
		if (!bShowSpin)
		{
			dwStyle=WS_CHILD|UDS_ALIGNRIGHT|UDS_ARROWKEYS;
		}
		//获取按钮位置
		m_spinSize.Create(dwStyle,CRect(0,0,10,20),this->GetParent(), nTextID+10000);
		m_spinSize.SetBuddy(this);
	}
	if (!m_tipHelp.GetSafeHwnd())
	{
		m_tipHelp.Create(this);
		m_tipHelp.AddTool(this, m_strToolTitle);
		m_tipHelp.SetMaxTipWidth(123);
		m_tipHelp.Activate(TRUE);
	}
}

void CNumberTextCtrl::MoveWindow(LPCRECT lpRect, BOOL bRepaint /* = TRUE */)
{
	CEdit::MoveWindow(lpRect,bRepaint);
	//然后移动spin
	if (m_spinSize.GetSafeHwnd())
	{
		CRect spn(lpRect->right, lpRect->top, lpRect->right+14, lpRect->bottom);
		m_spinSize.MoveWindow(spn,bRepaint);
	}
}

BOOL CNumberTextCtrl::EnableWindow( BOOL bEnable /*= TRUE*/ )
{
	BOOL bRet = CEdit::EnableWindow(bEnable);
	if (m_spinSize.GetSafeHwnd())
	{
		m_spinSize.EnableWindow(bEnable);
	}
	return bRet;
}

BOOL CNumberTextCtrl::ShowWindow( int nCmdShow )
{
	BOOL bRet = CEdit::ShowWindow(nCmdShow);
	if (m_spinSize.GetSafeHwnd())
	{
		m_spinSize.ShowWindow(nCmdShow);
	}
	return bRet;
}

BOOL CNumberTextCtrl::SetReadOnly( BOOL bReadOnly /*= TRUE*/ )
{
	BOOL bRet = CEdit::SetReadOnly(bReadOnly);
	if (m_spinSize.GetSafeHwnd())
	{
		m_spinSize.EnableWindow(!bReadOnly);
	}
	return bRet;
}


void CNumberTextCtrl::OnEnKillfocus()
{
	//当数字不属于范围时，不许移出焦点
	DOUBLE fValue=0;
	CString strTmp=_T("0");
	GetWindowText(strTmp);
	fValue = _ttof(strTmp);
	DOUBLE fLower=0;
	DOUBLE fUpper=0;
	GetRange(fLower, fUpper);
	if ((fValue>fUpper)||(fValue<fLower))
	{
		if (fValue>fUpper)
		{
			fValue=fUpper;
		}
		if (fValue<fLower)
		{
			fValue=fLower;
		}
		SetNumber(fValue);
	}
	m_fNumber = fValue;
	if (this->GetParent())
	{
		//::PostMessage(this->GetParent()->GetSafeHwnd(), WM_COMMAND, WM_KILLFOCUS, (LPARAM)this);
	}
}

void CNumberTextCtrl::SetInitNumber( DOUBLE fInput )
{
	m_fInitNumber=fInput;
	m_fNumber = m_fInitNumber;
	if (m_spinSize.GetSafeHwnd())
	{
		m_spinSize.SetDeltaForce(true);
		m_spinSize.SetInitNumber(fInput);
		m_spinSize.SetPos(fInput);
		DOUBLE fRet = m_spinSize.GetPos();
	}
	CString strNumber;
	RoundFloat(m_fNumber, strNumber);
	SetWindowText(strNumber);
}

void CNumberTextCtrl::SetInitNumber( INT nInput )
{
	SetInitNumber((DOUBLE)nInput);
}


BOOL CNumberTextCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_MOUSEMOVE || pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONUP)
	{
		if (m_tipHelp.GetSafeHwnd())
		{
			if (!m_strToolTip.IsEmpty())
			{
				//m_tipHelp.SetTitle(0, m_strToolTitle);
				m_tipHelp.SetDescription(m_strToolTip);
				m_tipHelp.RelayEvent(pMsg);
			}
		}
	}
	return CEdit::PreTranslateMessage(pMsg);
}
