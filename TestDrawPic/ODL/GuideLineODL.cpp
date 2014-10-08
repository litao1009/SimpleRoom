#include "stdafx.h"
#include "GuideLineODL.h"
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
#include "Precision.hxx"

CGuideLineODL::CGuideLineODL(void)
{
	m_ObjType = EOBT_GuideLine;
	m_fLockArc=45;
	m_bLockAngel=true;
	SetHasLength(true);
}


CGuideLineODL::~CGuideLineODL(void)
{
}

void CGuideLineODL::OnDrawTop( Gdiplus::Graphics& gcDrawer , float fScale)
{
	CBaseODL::OnDrawTop(gcDrawer, fScale);
	InitTopColor();
	CArray<Gdiplus::Point> st;
	for (auto& curItem : m_arrTempPoint)
	{
		Gdiplus::Point pt;
		pt.X = static_cast<INT>(curItem.X());
		pt.Y = static_cast<INT>(curItem.Z());
		st.Add(pt);
	}
	
	if(st.GetSize()<=0)
	{
		return;
	}
	Gdiplus::Pen pen( m_clPenColor, 1.0f / fScale);
	pen.SetDashStyle( Gdiplus::DashStyleSolid);
	if (st.GetSize()>1)
	{
		gcDrawer.DrawLines(&pen, st.GetData(), st.GetSize());
	}
	if (IsTopCreating())
	{
		//画第一个点时，current还不存在
		Gdiplus::Point ptCur(static_cast<INT>(m_ptTopCurrent.X()), static_cast<INT>(m_ptTopCurrent.Z()));
		if (st.GetSize()>0)
		{
			INT nIndex = st.GetSize()-1;
			gcDrawer.DrawLine(&pen, st.GetAt(nIndex), ptCur);
		}
	}
	if (IsCurrentHotspot())
	{
		gcDrawer.DrawRectangle(&pen, static_cast<INT>( m_ptTopCurrent.X() - 3/fScale), static_cast<INT>(m_ptTopCurrent.Z() - 3/fScale), static_cast<INT>(6/fScale), static_cast<INT>(6/fScale));
	}
	if (GetHasLength())
	{
		uint32_t i=0;
		for (auto& curPs:m_arrTempPoint)
		{
			gp_Pnt ptStart=curPs;
			gp_Pnt ptStop;
			if (m_arrTempPoint.size()==1)
			{
				//只有一个点
				ptStop=m_ptTopCurrent;
			}
			else
			{
				if (i<m_arrTempPoint.size()-1)
				{
					ptStop=m_arrTempPoint[i+1];
				}
				else
				{
					ptStop=m_ptTopCurrent;
				}
			}

			if (ptStop.IsEqual(ptStart,Precision::Confusion()))
			{
				return;
			}
			CString strText;

			gp_Dir curDir(gp_Vec(ptStart, ptStop));
			auto curAngle = curDir.AngleWithRef(gp::DX(), gp::DY());
			if ( std::abs(curAngle) > M_PI_2 )
			{
				curAngle += M_PI;
			}

			gp_Pnt midPnt((ptStop.X()+ptStart.X())/2, (ptStop.Y()+ptStart.Y())/2, (ptStop.Z()+ptStart.Z())/2);

			auto tmpPnt = midPnt.Rotated(gp::OY(), curAngle);

			Standard_Real fLength= ptStart.Distance(ptStop);
			strText.Format(_T("%.1f"), fLength);

			//需要长度显示
			
			Gdiplus::Font myFont(m_pFontFamily, 9 / fScale , Gdiplus::FontStyleRegular, Gdiplus::UnitPoint); //第二个是字体大小
			
			Gdiplus::PointF ptCur(static_cast<Gdiplus::REAL>(tmpPnt.X()), static_cast<Gdiplus::REAL>(tmpPnt.Z()));

			m_FontFormat.SetAlignment(Gdiplus::StringAlignmentNear);    //文本排列方式，即在对应位置居中、靠左、靠右
			gcDrawer.RotateTransform(static_cast<Gdiplus::REAL>(curAngle * M_1_PI * 180));

			gcDrawer.DrawString(strText, strText.GetLength(), &myFont, ptCur, &m_FontFormat, m_pFontBrush );//把string绘制到图上
			gcDrawer.RotateTransform(static_cast<Gdiplus::REAL>(-curAngle * M_1_PI * 180));
			i++;
		}
	}
}

void CGuideLineODL::SetTopStartPos( gp_Pnt ptStart )
{
	SetIsTopCreating(true);
	CBaseODL::SetTopStartPos(ptStart);
	m_arrTempPoint.clear();
	m_arrTempPoint.push_back(ptStart);
}

void CGuideLineODL::SetTopCurrentPos( gp_Pnt ptCurrent )
{
	static auto resolution = 5.0;
	SetIsCurrentHotspot(false);
	
	m_ptTopCurrent = ptCurrent;

	assert(!m_arrTempPoint.empty());

	const auto& lastPnt = m_arrTempPoint.back();
	auto dis = lastPnt.Distance(ptCurrent);
	// if 有热点吸附
	{
		for ( const auto& savePnt : m_arrTempPoint )
		{
			if ( savePnt.Distance(ptCurrent) >= 100 )
			{
				continue;
			}

			m_ptTopCurrent = savePnt;
			SetIsCurrentHotspot(true);
			return;
		}
	}

	gp_Dir curDir(gp_Vec(lastPnt, ptCurrent));
	auto curEdge = BRepBuilderAPI_MakeEdge(lastPnt, ptCurrent).Edge();

	//极轴追踪
	bool lockDir = false;
	{
		//追踪角度
		auto testDeg = m_fLockArc;
		
		//与水平位置的夹角
		auto curDeg = curDir.AngleWithRef(gp::DX(), gp::DY()) * M_1_PI * 180;
		curDeg += 180;

		auto restDeg = std::fmod(curDeg,testDeg);

		bool needUpdate = false;
		double updateValue = 0;

		if ( restDeg < resolution )
		{
			updateValue = true;
			curDeg -= restDeg;
		}

		if ( testDeg - restDeg < resolution )
		{
			updateValue = true;
			curDeg += testDeg - restDeg;
		}

		if ( updateValue )
		{
			curDeg -= 180;
			auto curRad = curDeg * M_PI / 180;
			curDir = gp::DX().Rotated(gp::OY().Reversed(), curRad);
			
			auto curCurve = GC_MakeLine(lastPnt, curDir).Value();
			curCurve->D0(lastPnt.Distance(ptCurrent), m_ptTopCurrent);

			curEdge = BRepBuilderAPI_MakeEdge(lastPnt, m_ptTopCurrent).Edge();

			lockDir = true;
		}
	}

	//延长线追踪
	{
		gp_Pln xoz(gp::Origin(), gp::DY());

		Standard_Real curF,curL;
		auto curCurve = BRep_Tool::Curve(curEdge, curF, curL);
		auto cur2dCurve = GeomAPI::To2d(curCurve, xoz);
		
		for ( const auto& tmpEdge : m_arrTempEdge )
		{
			if ( tmpEdge == m_arrTempEdge.back() )
			{
				break;
			}

			Standard_Real tmpF,tmpL;
			auto tmpCurve = BRep_Tool::Curve(TopoDS::Edge(tmpEdge), tmpF, tmpL);

			if ( lockDir )
			{
				auto tmp2dCurve = GeomAPI::To2d(tmpCurve, xoz);
				Geom2dAPI_InterCurveCurve iss(cur2dCurve, tmp2dCurve);
				if ( 0 == iss.NbPoints() )
				{
					continue;
				}

				auto int2dPnt = iss.Point(1);
				gp_Pnt intPnt(int2dPnt.Y(), 0, int2dPnt.X());

				if ( intPnt.Distance(m_ptTopCurrent) < resolution )
				{
					m_ptTopCurrent = intPnt;
					SetIsCurrentHotspot(true);
					curEdge = BRepBuilderAPI_MakeEdge(lastPnt, m_ptTopCurrent).Edge();
					break;
				}
			}
			else
			{
				GeomAPI_ProjectPointOnCurve ppc(m_ptTopCurrent, tmpCurve);
				auto intPnt = ppc.NearestPoint();
				auto dis = intPnt.Distance(m_ptTopCurrent);

				if ( intPnt.Distance(m_ptTopCurrent) < resolution )
				{
					m_ptTopCurrent = intPnt;
					SetIsCurrentHotspot(true);
					curEdge = BRepBuilderAPI_MakeEdge(lastPnt, m_ptTopCurrent).Edge();
					break;
				}
			}
			
		}
	}

	//垂线追踪
	{

	}

}

void CGuideLineODL::SetTopContinuePos( gp_Pnt ptContinue )
{
	if (m_bLockAngel)
	{
		m_arrTempPoint.push_back(m_ptTopCurrent);
	}
	else
	{
		m_arrTempPoint.push_back(ptContinue);
	}

	auto pntSize = m_arrTempPoint.size();
	assert(pntSize >= 2);

	auto newEdge = BRepBuilderAPI_MakeEdge(m_arrTempPoint[pntSize-2], m_arrTempPoint[pntSize-1]).Edge();
	m_arrTempEdge.push_back(newEdge);
}

void CGuideLineODL::SetTopStopPos( gp_Pnt ptStop )
{
	SetIsTopCreating(false);
	m_arrTempPoint.push_back(ptStop);
}