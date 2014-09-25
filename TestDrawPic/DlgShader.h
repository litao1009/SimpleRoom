#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "NumberTextCtrl.h"

// CDlgShader 对话框

class CDlgShader : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgShader)

public:
	CDlgShader(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgShader();

// 对话框数据
	enum { IDD = IDD_DLG_SHADER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:

	float	GetSSAOReach() const;

	float	GetMaxDepthDecline() const;

	float	GetDepthAliasingEpsilon() const;

	float	GetRandTextureTiles() const;

	float	GetSSAOOutputMultiplier() const;

public:

	void	SetSSAOReachRange(float min, float max);
	void	SetSSAOReach(float val);

	void	SetMaxDepthDeclineRange(float min, float max);
	void	SetMaxDepthDecline(float val);

	void	SetDepthAliasingEpsilonRange(float min, float max);
	void	SetDepthAliasingEpsilon(float val);

	void	SetRandTextureTilesRange(float min, float max);
	void	SetRandTextureTiles(float val);

	void	SetSSAOOutputMultiplierRange(float min, float max);
	void	SetSSAOOutputMultiplier(float val);

public:

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_SlideSSAOReach;
//	CSliderCtrl m_slide;
	CSliderCtrl m_SlideMaxDepthDecline;
	CSliderCtrl m_SlideDepthAliasingEpsilon;
	CSliderCtrl m_SlideRandTextureTiles;
	CSliderCtrl m_SlideSSAOOutputMultiplier;
	CNumberTextCtrl m_txtSSAOReachMax;
	CNumberTextCtrl m_txtMaxDepthDeclineMax;
	CNumberTextCtrl m_txtDepthAliasingEpsilonMax;
	CNumberTextCtrl m_txtRandTextureTilesMax;
	CNumberTextCtrl m_txtSSAOOutputMultiplierMax;
	CNumberTextCtrl m_txtSSAOReachMin;
	CNumberTextCtrl m_txtMaxDepthDeclineMin;
	CNumberTextCtrl m_txtDepthAliasingEpsilonMin;
	CNumberTextCtrl m_txtRandTextureTilesMin;
	CNumberTextCtrl m_txtSSAOOutputMultiplierMin;
	CNumberTextCtrl m_SSAOReachNr;
	CNumberTextCtrl m_MaxDepthDeclineNr;
	CNumberTextCtrl m_DepthAliasingEpsilonNr;
	CNumberTextCtrl m_RandTextureTilesNr;
	CNumberTextCtrl m_SSAOOutputMultiplierNr;
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
