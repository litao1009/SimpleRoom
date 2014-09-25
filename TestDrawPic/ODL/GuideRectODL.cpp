#include "stdafx.h"
#include "GuideRectODL.h"
#include "gp_Lin.hxx"
#include "gce_MakeLin.hxx"
#include "GC_MakeLine.hxx"
#include "Geom_Line.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "Brep_Tool.hxx"
#include "TopoDS.hxx"
#include "Geom2dAPI_InterCurveCurve.hxx"
#include "GeomAPI.hxx"
#include "gp_Pln.hxx"
#include "GeomAPI_ProjectPointOnCurve.hxx"

CGuideRectODL::CGuideRectODL(void)
{
	m_ObjType = EOBT_GuideRect;
}


CGuideRectODL::~CGuideRectODL(void)
{
}

void CGuideRectODL::OnDrawTop( Gdiplus::Graphics& gcDrawer , float fScale)
{
	CBaseODL::OnDrawTop(gcDrawer, fScale);
	
	CArray<Gdiplus::Point> st;
	if (m_rtArea.Width<1.0f)
	{
		return;
	}
	Gdiplus::Pen pen( m_clPenColor, 1.0f / fScale);
	pen.SetDashStyle( Gdiplus::DashStyleSolid);

	gcDrawer.DrawRectangle(&pen, m_rtArea);
}

void CGuideRectODL::SetTopStartPos( gp_Pnt ptStart )
{
	SetIsTopCreating(true);
	CBaseODL::SetTopStartPos(ptStart);
	m_rtArea.X= static_cast<Gdiplus::REAL>(ptStart.X());
	m_rtArea.Y = static_cast<Gdiplus::REAL>(ptStart.Z());
}

void CGuideRectODL::SetTopCurrentPos( gp_Pnt ptCurrent )
{
	CBaseODL::SetTopCurrentPos(ptCurrent);
	
	m_rtArea.Width = static_cast<Gdiplus::REAL>(abs(m_ptTopStart.X() - ptCurrent.X()));
	m_rtArea.Height = static_cast<Gdiplus::REAL>(abs(m_ptTopStart.Z() - ptCurrent.Z()));
	if (m_ptTopStart.X()>ptCurrent.X())
	{
		m_rtArea.X = static_cast<Gdiplus::REAL>(ptCurrent.X());
		
	}
	if ( ptCurrent.Z()<m_ptTopStart.Z())
	{
		m_rtArea.Y= static_cast<Gdiplus::REAL>(ptCurrent.Z());
	}
}

void CGuideRectODL::SetTopStopPos( gp_Pnt ptStop )
{
	SetIsTopCreating(false);
	CBaseODL::SetTopStopPos(ptStop);
	m_rtArea.Width = static_cast<Gdiplus::REAL>(abs(m_ptTopStart.X() - ptStop.X()));
	m_rtArea.Height = static_cast<Gdiplus::REAL>(abs(m_ptTopStart.Z() - ptStop.Z()));
}

bool CGuideRectODL::IsTopPointInFace(gp_Pnt ptPos)
{
	return false;
}

void CGuideRectODL::GetTopPointList( std::vector<gp_Pnt>& arrPoint )
{
	m_arrTopPoint.clear();
	//如果开始点大于结束点
	
	Gdiplus::REAL fX0 = m_rtArea.GetLeft();
	Gdiplus::REAL fY0 = m_rtArea.GetTop();
	Gdiplus::REAL fX1 = m_rtArea.GetRight();
	Gdiplus::REAL fY1 = m_rtArea.GetBottom();
	gp_Pnt pt0(fX0, 0, fY0);
	gp_Pnt pt1(fX1, 0, fY0);
	gp_Pnt pt2(fX1, 0, fY1);
	gp_Pnt pt3(fX0, 0, fY1);
	m_arrTopPoint.push_back(pt0);
	m_arrTopPoint.push_back(pt1);
	m_arrTopPoint.push_back(pt2);
	m_arrTopPoint.push_back(pt3);
	m_arrTopPoint.push_back(pt0);
	arrPoint = m_arrTopPoint;
}

