#include "stdafx.h"
#include "SkyODL.h"


CSkyODL::CSkyODL(const TopoDS_Face& theFace)
{
	m_ObjType = EOBT_Sky;
	SetFaceShape(theFace);
}


CSkyODL::~CSkyODL(void)
{
}

void CSkyODL::CancelTopMoving()
{
	CBaseODL::CancelTopMoving();
	m_arrMovingTopPoint = m_arrTopPoint;
}

void CSkyODL::OnTopMoving(const gp_Pnt& stInfo)
{
	CBaseODL::OnTopMoving(stInfo);
	m_arrMovingTopPoint.clear();
	//修改所有点的最终位置
	for (auto& curPnt : m_arrTopPoint)
	{
		gp_Pnt pt(curPnt.X() + stInfo.X(), 
			curPnt.Y() + stInfo.Y(),
			curPnt.Z() + stInfo.Z()
			);
		m_arrMovingTopPoint.push_back(pt);
	}
}

void CSkyODL::OnTopMovingFinish(const gp_Pnt& stInfo, bool bCommitChange )
{
	if (bCommitChange)
	{
		OnTopMoving(stInfo);
		SetIsTopMoving(false);

		m_arrTopPoint = m_arrMovingTopPoint;

		CBaseODL::OnTopMovingFinish(stInfo, bCommitChange);
	}
	else
	{
		m_arrMovingTopPoint = m_arrTopPoint;
	}
}

void CSkyODL::OnDrawTop(Gdiplus::Graphics& gcDrawer, float fScale)
{
	CBaseODL::OnDrawTop(gcDrawer, fScale);
	
	Gdiplus::Pen Dot( m_clDotColor, static_cast<Gdiplus::REAL>(1.0 / fScale));
	CArray<Gdiplus::Point> st;
	CArray<Gdiplus::Point> stMoving;
	CArray<BYTE> stBytes;

	for (auto& curItem : m_arrTopPoint)
	{
		Gdiplus::Point pt;
		pt.X = static_cast<INT>(curItem.X());
		pt.Y = static_cast<INT>(curItem.Z());
		st.Add(pt);
	}
	if (IsTopMoving())
	{
		for ( auto& curMoving : m_arrMovingTopPoint )
		{
			Gdiplus::Point pt;
			pt.X = static_cast<INT>(curMoving.X());
			pt.Y = static_cast<INT>(curMoving.Z());
			stMoving.Add(pt);
		}
	}
	if(st.GetSize()<=0)
	{
		return;
	}
	for (int i=0; i<st.GetSize(); ++i)
	{
		st[i].X = static_cast<INT>(st[i].X);
		st[i].Y = static_cast<INT>(st[i].Y);
	}
	for (int i=0; i<stMoving.GetSize(); ++i)
	{
		stMoving[i].X = static_cast<INT>(stMoving[i].X);
		stMoving[i].Y = static_cast<INT>(stMoving[i].Y);
	}
	Gdiplus::Pen pen( m_clPenColor, static_cast<Gdiplus::REAL>(1.0 / fScale));
	pen.SetDashStyle(Gdiplus::DashStyleSolid);

	if (!IsTopCreating())
	{
		Gdiplus::HatchBrush brush( Gdiplus::HatchStyle(Gdiplus::HatchStyle::HatchStyleCross ), m_clPenColor, Gdiplus::Color(0,255,255,255) );
		//画皮肤
		gcDrawer.FillPolygon(&brush, st.GetData(), st.GetSize(), Gdiplus::FillMode::FillModeAlternate);
	}

	gcDrawer.DrawLines(&pen, st.GetData(), st.GetSize());

	if (IsTopMoving())
	{
		Gdiplus::Pen penMoving( m_clDotColor, static_cast<Gdiplus::REAL>(1.0 / fScale));
		penMoving.SetDashStyle( Gdiplus::DashStyleSolid);
		gcDrawer.DrawLines(&penMoving, stMoving.GetData(), stMoving.GetSize());
	}
	if (IsTopSelected())
	{
		//每个转角画一个点
		for (int x=0; x<st.GetSize(); x++)
		{
			gcDrawer.DrawRectangle(&Dot, (st[x].X) - 3.0f/fScale, (st[x].Y ) - 3.0f/fScale, 6.0f/fScale, 6.0f/fScale);
		}
	}
}

void CSkyODL::OnDrawFace( Gdiplus::Graphics& gcDrawer, float fScale )
{
	CBaseODL::OnDrawFace(gcDrawer, fScale);
	
	Gdiplus::Pen Dot( m_clDotColor, static_cast<Gdiplus::REAL>(1.0 / fScale));
	CArray<Gdiplus::Point> st;
	CArray<BYTE> stBytes;

	for (auto& curItem : m_arrTopPoint)
	{
		Gdiplus::Point pt;
		pt.X = static_cast<INT>(curItem.X());
		pt.Y = static_cast<INT>(-curItem.Z());
		st.Add(pt);
	}

	if(st.GetSize()<=0)
	{
		return;
	}
	Gdiplus::Pen pen( m_clPenColor, static_cast<Gdiplus::REAL>(1.0 / fScale));
	pen.SetDashStyle(Gdiplus::DashStyleSolid);

	Gdiplus::HatchBrush brush( Gdiplus::HatchStyle(Gdiplus::HatchStyle::HatchStyleCross ), m_clPenColor, Gdiplus::Color(0,255,255,255) );
	//画皮肤
	gcDrawer.FillPolygon(&brush, st.GetData(), st.GetSize(), Gdiplus::FillMode::FillModeAlternate);

	gcDrawer.DrawLines(&pen, st.GetData(), st.GetSize());

}
