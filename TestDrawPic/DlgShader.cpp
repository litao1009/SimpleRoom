// DlgShader.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "DlgShader.h"
#include "afxdialogex.h"


// CDlgShader 对话框

IMPLEMENT_DYNAMIC(CDlgShader, CDialogEx)

CDlgShader::CDlgShader(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgShader::IDD, pParent)
{

}

CDlgShader::~CDlgShader()
{
}

void CDlgShader::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_SlideSSAOReach);
	//  DDX_Control(pDX, IDC_SLIDER2, m_slide);
	DDX_Control(pDX, IDC_SLIDER2, m_SlideMaxDepthDecline);
	DDX_Control(pDX, IDC_SLIDER3, m_SlideDepthAliasingEpsilon);
	DDX_Control(pDX, IDC_SLIDER4, m_SlideRandTextureTiles);
	DDX_Control(pDX, IDC_SLIDER5, m_SlideSSAOOutputMultiplier);
	DDX_Control(pDX, IDC_TXT_MAX_1, m_txtSSAOReachMax);
	DDX_Control(pDX, IDC_TXT_MAX_2, m_txtMaxDepthDeclineMax);
	DDX_Control(pDX, IDC_TXT_MAX_3, m_txtDepthAliasingEpsilonMax);
	DDX_Control(pDX, IDC_TXT_MAX_4, m_txtRandTextureTilesMax);
	DDX_Control(pDX, IDC_TXT_MAX_5, m_txtSSAOOutputMultiplierMax);
	DDX_Control(pDX, IDC_TXT_MIN_1, m_txtSSAOReachMin);
	DDX_Control(pDX, IDC_TXT_MIN_2, m_txtMaxDepthDeclineMin);
	DDX_Control(pDX, IDC_TXT_MIN_3, m_txtDepthAliasingEpsilonMin);
	DDX_Control(pDX, IDC_TXT_MIN_4, m_txtRandTextureTilesMin);
	DDX_Control(pDX, IDC_TXT_MIN_5, m_txtSSAOOutputMultiplierMin);
	DDX_Control(pDX, IDC_TXT_VALUE_1, m_SSAOReachNr);
	DDX_Control(pDX, IDC_TXT_VALUE_2, m_MaxDepthDeclineNr);
	DDX_Control(pDX, IDC_TXT_VALUE_3, m_DepthAliasingEpsilonNr);
	DDX_Control(pDX, IDC_TXT_VALUE_4, m_RandTextureTilesNr);
	DDX_Control(pDX, IDC_TXT_VALUE_5, m_SSAOOutputMultiplierNr);
}


BEGIN_MESSAGE_MAP(CDlgShader, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CDlgShader 消息处理程序


BOOL CDlgShader::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_SSAOReachNr.InitSpinButton();
	m_SSAOReachNr.SetNumber(1.f);
	m_txtSSAOReachMin.SetNumber(0);
	m_txtSSAOReachMax.SetNumber(10.f);
	m_SlideSSAOReach.SetRange(0, 100);
	m_SlideSSAOReach.SetPos(10);

	m_MaxDepthDeclineNr.InitSpinButton();
	m_MaxDepthDeclineNr.SetNumber(0.2);
	m_txtMaxDepthDeclineMin.SetNumber(0);
	m_txtMaxDepthDeclineMax.SetNumber(1.f);
	m_SlideMaxDepthDecline.SetRange(0, 100);
	m_SlideMaxDepthDecline.SetPos(20);

	m_DepthAliasingEpsilonNr.InitSpinButton();
	m_DepthAliasingEpsilonNr.SetRoundPos(4);
	m_DepthAliasingEpsilonNr.SetRange(-10,10);
	m_DepthAliasingEpsilonNr.SetNumber(0.02);
	m_txtDepthAliasingEpsilonMin.SetRoundPos(4);
	m_txtDepthAliasingEpsilonMin.SetRange(-10, 0);
	m_txtDepthAliasingEpsilonMin.SetNumber(-0.02);
	m_txtDepthAliasingEpsilonMax.SetNumber(0.02);
	m_SlideDepthAliasingEpsilon.SetRange(0, 400);
	m_SlideDepthAliasingEpsilon.SetPos(400);

	m_RandTextureTilesNr.InitSpinButton();
	m_RandTextureTilesNr.SetNumber(3);
	m_txtRandTextureTilesMin.SetNumber(0);
	m_txtRandTextureTilesMax.SetNumber(10);
	m_SlideRandTextureTiles.SetRange(0, 100);
	m_SlideRandTextureTiles.SetPos(30);

	m_SSAOOutputMultiplierNr.InitSpinButton();
	m_SSAOOutputMultiplierNr.SetNumber(1.5);
	m_txtSSAOOutputMultiplierMin.SetNumber(0);
	m_txtSSAOOutputMultiplierMax.SetNumber(10);
	m_SlideSSAOOutputMultiplier.SetRange(0, 100);
	m_SlideSSAOOutputMultiplier.SetPos(15);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgShader::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if ( pScrollBar->GetSafeHwnd() == m_SlideSSAOReach.GetSafeHwnd() )
	{
		auto pos = m_SlideSSAOReach.GetPos();
		INT minPos,maxPos;
		m_SlideSSAOReach.GetRange(minPos, maxPos);
		auto range = maxPos - minPos;
		auto ratio = float(pos) / float(range);

		float minVar = m_txtSSAOReachMin.GetNumber();
		float maxVar = m_txtSSAOReachMax.GetNumber();

		m_SSAOReachNr.SetNumber(minVar + (maxVar-minVar)*ratio);
	}
	if ( pScrollBar->GetSafeHwnd() == m_SlideMaxDepthDecline.GetSafeHwnd() )
	{
		auto pos = m_SlideMaxDepthDecline.GetPos();
		INT minPos,maxPos;
		m_SlideMaxDepthDecline.GetRange(minPos, maxPos);
		auto range = maxPos - minPos;
		auto ratio = float(pos) / float(range);

		float minVar = m_txtMaxDepthDeclineMin.GetNumber();
		float maxVar = m_txtMaxDepthDeclineMax.GetNumber();

		m_MaxDepthDeclineNr.SetNumber(minVar + (maxVar-minVar)*ratio);
	}
	if ( pScrollBar->GetSafeHwnd() == m_SlideDepthAliasingEpsilon.GetSafeHwnd() )
	{
		auto pos = m_SlideDepthAliasingEpsilon.GetPos();
		INT minPos,maxPos;
		m_SlideDepthAliasingEpsilon.GetRange(minPos, maxPos);
		auto range = maxPos - minPos;
		auto ratio = float(pos) / float(range);

		float minVar = m_txtDepthAliasingEpsilonMin.GetNumber();
		float maxVar = m_txtDepthAliasingEpsilonMax.GetNumber();

		m_DepthAliasingEpsilonNr.SetNumber(minVar + (maxVar-minVar)*ratio);
	}
	if ( pScrollBar->GetSafeHwnd() == m_SlideRandTextureTiles.GetSafeHwnd() )
	{
		auto pos = m_SlideRandTextureTiles.GetPos();
		INT minPos,maxPos;
		m_SlideRandTextureTiles.GetRange(minPos, maxPos);
		auto range = maxPos - minPos;
		auto ratio = float(pos) / float(range);

		float minVar = m_txtRandTextureTilesMin.GetNumber();
		float maxVar = m_txtRandTextureTilesMax.GetNumber();

		m_RandTextureTilesNr.SetNumber(minVar + (maxVar-minVar)*ratio);
	}
	if ( pScrollBar->GetSafeHwnd() == m_SlideSSAOOutputMultiplier.GetSafeHwnd() )
	{
		auto pos = m_SlideSSAOOutputMultiplier.GetPos();
		INT minPos,maxPos;
		m_SlideSSAOOutputMultiplier.GetRange(minPos, maxPos);
		auto range = maxPos - minPos;
		auto ratio = float(pos) / float(range);

		float minVar = m_txtSSAOOutputMultiplierMin.GetNumber();
		float maxVar = m_txtSSAOOutputMultiplierMax.GetNumber();

		m_SSAOOutputMultiplierNr.SetNumber(minVar + (maxVar-minVar)*ratio);
	}

	
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

float CDlgShader::GetSSAOReach() const
{
	return m_SSAOReachNr.GetNumber();
}

float CDlgShader::GetMaxDepthDecline() const
{
	return m_MaxDepthDeclineNr.GetNumber();
}

float CDlgShader::GetDepthAliasingEpsilon() const
{
	return m_DepthAliasingEpsilonNr.GetNumber();
}

float CDlgShader::GetRandTextureTiles() const
{
	return m_RandTextureTilesNr.GetNumber();
}

float CDlgShader::GetSSAOOutputMultiplier() const
{
	return m_SSAOOutputMultiplierNr.GetNumber();
}

void CDlgShader::SetSSAOReachRange( float min, float max )
{

}

void CDlgShader::SetSSAOReach( float val )
{

}

void CDlgShader::SetMaxDepthDeclineRange( float min, float max )
{

}

void CDlgShader::SetMaxDepthDecline( float val )
{

}

void CDlgShader::SetDepthAliasingEpsilonRange( float min, float max )
{

}

void CDlgShader::SetDepthAliasingEpsilon( float val )
{

}

void CDlgShader::SetRandTextureTilesRange( float min, float max )
{

}

void CDlgShader::SetRandTextureTiles( float val )
{

}

void CDlgShader::SetSSAOOutputMultiplierRange( float min, float max )
{

}

void CDlgShader::SetSSAOOutputMultiplier( float val )
{

}
