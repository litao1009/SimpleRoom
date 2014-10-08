#include "stdafx.h"

#include "CameraODL.h"
#include "BRep_Tool.hxx"
#include "BRepBuilderAPI_MakeVertex.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "BRepBuilderAPI_MakePolygon.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "GC_MakeLine.hxx"
#include "Geom_Line.hxx"

#include "ICameraSceneNode.h"

CCameraODL::CCameraODL(irr::scene::ISceneManager* smgr)
{
	m_ObjType = EOBT_Camera;
	m_SelectType = EST_NONE;

	m_Smgr = smgr;

	m_CameraFov = 0;
	m_CameraPos = BRepBuilderAPI_MakeVertex(gp_Pnt(0,0,0));
	m_TargetPos = BRepBuilderAPI_MakeVertex(gp_Pnt(0,0,100));
}

CCameraODL::~CCameraODL()
{
	
}

void CCameraODL::OnDrawTop( Gdiplus::Graphics& gcDrawer, float fScale )
{
	if ( IsHide() )
	{
		return;
	}

	if ( m_CameraPos.IsNull() || m_TargetPos.IsNull() || m_CameraFacePnt.empty() )
	{
		return;
	}

	m_clPenColor = Gdiplus::Color::Black;
	m_clDotColor = Gdiplus::Color::Black;
	
	Gdiplus::Pen pen( m_clPenColor, static_cast<Gdiplus::REAL>(1.0 / fScale));
	pen.SetDashStyle(Gdiplus::DashStyleSolid);

	auto pntPos = BRep_Tool::Pnt(m_CameraPos);
	auto pntTar = BRep_Tool::Pnt(m_TargetPos);

	Gdiplus::SolidBrush brsh1(Gdiplus::Color::Aqua);
	Gdiplus::SolidBrush brsh2(Gdiplus::Color::CornflowerBlue);
	
	gcDrawer.DrawPie(&pen, static_cast<Gdiplus::REAL>(pntPos.X()-7/fScale), static_cast<Gdiplus::REAL>(pntPos.Z()-7/fScale), 14/fScale, 14/fScale, 0, 360);
	gcDrawer.FillPie(&brsh1, static_cast<Gdiplus::REAL>(pntPos.X()-7/fScale), static_cast<Gdiplus::REAL>(pntPos.Z()-7/fScale), 14/fScale, 14/fScale, 0, 360);
	gcDrawer.DrawPie(&pen, static_cast<Gdiplus::REAL>(pntTar.X()-7/fScale), static_cast<Gdiplus::REAL>(pntTar.Z()-7/fScale), 14/fScale, 14/fScale, 0, 360);
	gcDrawer.FillPie(&brsh2, static_cast<Gdiplus::REAL>(pntTar.X()-7/fScale), static_cast<Gdiplus::REAL>(pntTar.Z()-7/fScale), 14/fScale, 14/fScale, 0, 360);
	{
		std::adjacent_find(m_CameraFacePnt.begin(), m_CameraFacePnt.end(), [&gcDrawer,&pen](const gp_Pnt& p1, const gp_Pnt& p2)
		{
			Gdiplus::PointF gp1,gp2;
			gp1.X = static_cast<Gdiplus::REAL>(p1.X());
			gp1.Y = static_cast<Gdiplus::REAL>(p1.Z());
			gp2.X = static_cast<Gdiplus::REAL>(p2.X());
			gp2.Y = static_cast<Gdiplus::REAL>(p2.Z());
			gcDrawer.DrawLine(&pen, gp1, gp2);

			return false;
		});

		Gdiplus::PointF gp1,gp2;
		gp1.X = static_cast<Gdiplus::REAL>(m_CameraFacePnt.back().X());
		gp1.Y = static_cast<Gdiplus::REAL>(m_CameraFacePnt.back().Z());
		gp2.X = static_cast<Gdiplus::REAL>(m_CameraFacePnt.front().X());
		gp2.Y = static_cast<Gdiplus::REAL>(m_CameraFacePnt.front().Z());
		gcDrawer.DrawLine(&pen, gp1, gp2);
	}
}

bool CCameraODL::IsFaceInShape( BRepExtrema_DistShapeShape& dss )
{
	return false;
}

bool CCameraODL::HasTopCollisionWith( const TopoDS_Shape& theShape )
{
	m_DSS.LoadS2(theShape);

	m_DSS.LoadS1(m_CameraPos);
	m_DSS.Perform();
	if ( 0 != m_DSS.NbSolution() && m_DSS.Value() < 50 )
	{
		m_SelectType = EST_POSITION;
		m_TempCameraPos = BRep_Tool::Pnt(m_CameraPos);
		return true;
	}

	m_DSS.LoadS1(m_TargetPos);
	m_DSS.Perform();
	if ( 0 != m_DSS.NbSolution() && m_DSS.Value() < 50 )
	{
		m_SelectType = EST_TARGET;
		m_TempTargetPos = BRep_Tool::Pnt(m_TargetPos);
		return true;
	}

	m_DSS.LoadS1(m_CameraFace);
	m_DSS.Perform();
	if ( 0 != m_DSS.NbSolution() && m_DSS.Value() < Precision::Confusion() )
	{
		m_SelectType = EST_FACE;
		m_TempTargetPos = BRep_Tool::Pnt(m_TargetPos);
		m_TempCameraPos = BRep_Tool::Pnt(m_CameraPos);
		return true;
	}

	return false;
}

void CCameraODL::OnTopMoving( const gp_Pnt& stInfo )
{
	if ( EST_NONE == m_SelectType )
	{
		return;
	}

	SetIsTopFocused(true);

	switch (m_SelectType)
	{
	case CCameraODL::EST_POSITION:
		{
			m_CameraPos = BRepBuilderAPI_MakeVertex(m_TempCameraPos.XYZ()+stInfo.XYZ()).Vertex();
		}
		break;
	case CCameraODL::EST_TARGET:
		{
			m_TargetPos = BRepBuilderAPI_MakeVertex(m_TempTargetPos.XYZ()+stInfo.XYZ()).Vertex();
		}
		break;
	case CCameraODL::EST_FACE:
		{
			m_TargetPos = BRepBuilderAPI_MakeVertex(m_TempTargetPos.XYZ()+stInfo.XYZ()).Vertex();
			m_CameraPos = BRepBuilderAPI_MakeVertex(m_TempCameraPos.XYZ()+stInfo.XYZ()).Vertex();
		}
		break;
	default:
		break;
	}

	BuildCamera();

	auto curPos = BRep_Tool::Pnt(m_CameraPos);
	auto curTarget = BRep_Tool::Pnt(m_TargetPos);
	
	auto rawPos = m_Smgr->getActiveCamera()->getPosition();
	auto rawTar = m_Smgr->getActiveCamera()->getTarget();

	m_Smgr->getActiveCamera()->setPosition(irr::core::vector3df(static_cast<float>(curPos.X()), static_cast<float>(rawPos.Y), static_cast<float>(curPos.Z())));
	m_Smgr->getActiveCamera()->setTarget(irr::core::vector3df(static_cast<float>(curTarget.X()), static_cast<float>(rawTar.Y), static_cast<float>(curTarget.Z())));
	m_Smgr->getActiveCamera()->updateAbsolutePosition();
}

void CCameraODL::OnTopMovingFinish( const gp_Pnt& stInfo, bool bCommitChange )
{
	CancelTopMoving();
}

void CCameraODL::CancelTopMoving()
{
	m_SelectType = EST_NONE;
	SetIsTopFocused(false);
}

bool CCameraODL::SetCameraInfo( bool visible, const gp_Pnt& position, const gp_Pnt& target, float fov )
{
	return false;

	auto curPos = BRep_Tool::Pnt(m_CameraPos);
	auto curTarget = BRep_Tool::Pnt(m_TargetPos);

	auto needUpdate = false;

	if ( IsHide() != !visible )
	{
		SetIsHide(!visible);
		needUpdate = true;
	}

	auto pos = position;
	auto tar = target;
	pos.SetY(0);
	tar.SetY(0);

	if ( curPos.Distance(pos) > Precision::Confusion() )
	{
		m_CameraPos = BRepBuilderAPI_MakeVertex(pos).Vertex();
		needUpdate = true;
	}
	
	if ( curTarget.Distance(tar) > Precision::Confusion() )
	{
		m_TargetPos = BRepBuilderAPI_MakeVertex(tar).Vertex();
		needUpdate = true;
	}

	if ( std::abs(m_CameraFov-fov) > Precision::Confusion() )
	{
		m_CameraFov = fov;
		needUpdate = true;
	}

	if ( needUpdate )
	{
		BuildCamera();
	}

	return needUpdate;
}

void CCameraODL::BuildCamera()
{
	auto pntPos = BRep_Tool::Pnt(m_CameraPos);
	auto pntTar = BRep_Tool::Pnt(m_TargetPos);

	auto disPosTarget = pntPos.Distance(pntTar);
	gp_Dir dirPosToTar(gp_Vec(pntPos, pntTar));
	auto newDir = dirPosToTar.Crossed(gp::DY());
	auto line = GC_MakeLine(pntTar, newDir).Value();

	auto dis = disPosTarget*std::tan(m_CameraFov/2);
	gp_Pnt p1,p2;
	line->D0(dis, p1);
	line->D0(-dis, p2);

	BRepBuilderAPI_MakePolygon ply;
	ply.Add(pntPos);
	ply.Add(p1);
	ply.Add(p2);

	m_CameraFace = BRepBuilderAPI_MakeFace(ply.Wire());

	m_CameraFacePnt.clear();
	m_CameraFacePnt.push_back(pntPos);
	m_CameraFacePnt.push_back(p1);
	m_CameraFacePnt.push_back(p2);
}
