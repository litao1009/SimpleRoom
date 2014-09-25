#include "stdafx.h"

#include "WallODL.h"
#include "WallFaceODL.h"

#include "TopoDS.hxx"
#include "BRep_Tool.hxx"
#include "BRepTools_WireExplorer.hxx"
#include "TopExp_Explorer.hxx"
#include "TopExp.hxx"
#include "TopTools_IndexedMapOfShape.hxx"
#include "BRepTools.hxx"
#include "GeomLProp_SLProps.hxx"
#include "Precision.hxx"
#include "BRepBuilderAPI_Copy.hxx"
#include "Precision.hxx"
#include "BRepAlgoAPI_Cut.hxx"

#include "ISceneManager.h"
#include "IVideoDriver.h"

CWallODL::CWallODL()
{
	m_dirWall = -gp::DY();
	m_ObjType = EOBT_Wall;

	SetHasLength(true);
}


CWallODL::~CWallODL(void)
{
}

void CWallODL::SetIsTopSelected(bool isSelected)
{
	CBaseODL::SetIsTopSelected(isSelected);
	for ( auto& curChild : GetChildrenList() )
	{
		curChild->SetIsTopSelected(isSelected);
	}
}

void CWallODL::CreatePoint(const TopoDS_Face& theFace)
{
	m_arrTopPoint.clear();

	TopExp_Explorer faceExp(theFace, TopAbs_WIRE);

	for ( BRepTools_WireExplorer wireExp(TopoDS::Wire(faceExp.Current())); wireExp.More(); wireExp.Next() )
	{
		auto pnt = BRep_Tool::Pnt(wireExp.CurrentVertex());
		m_arrTopPoint.push_back(pnt);
	}

	if ( !m_arrTopPoint.empty() )
	{
		if (m_arrTopPoint.size()>1)
		{
			m_arrTopPoint.push_back(m_arrTopPoint.at(0));
			m_ptTopStop = m_arrTopPoint.at(0);
			m_ptTopStart = m_arrTopPoint.at(1);
		}

	}
	m_arrMovingTopPoint = m_arrTopPoint;
	SetIsTopCreating(false);
}

void CWallODL::CancelTopMoving()
{
	m_arrMovingTopPoint = m_arrTopPoint;
	CBaseODL::CancelTopMoving();
}

void CWallODL::OnTopMoving(const gp_Pnt& stInfo)
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

	auto sn = GetSceneNode();
	if ( sn )
	{
		sn->setPosition(sn->GetSavePosition()+irr::core::vector3df(static_cast<irr::f32>(stInfo.X()), 0, static_cast<irr::f32>(stInfo.Z())));
	}
}

void CWallODL::OnTopMovingFinish(const gp_Pnt& stInfo, bool bCommitChange )
{
	if (bCommitChange)
	{
		OnTopMoving(stInfo);
		SetIsTopMoving(false);

		m_arrTopPoint = m_arrMovingTopPoint;
		if (m_arrTopPoint.size()>1)
		{
			m_ptTopStop = m_arrTopPoint.at(0);
			m_ptTopStart = m_arrTopPoint.at(1);
		}

		gp_Trsf newPos;
		newPos.SetTranslation(gp_Pnt(0,0,0), stInfo);
		GetBaseShape().Move(newPos);

		auto savePos = GetSceneNode()->GetSavePosition();
		savePos += irr::core::vector3df(static_cast<irr::f32>(stInfo.X()), 0, static_cast<irr::f32>(stInfo.Z()));
		GetSceneNode()->SetSavePosition(savePos);
		GetSceneNode()->setPosition(savePos);

		CBaseODL::OnTopMovingFinish(stInfo, bCommitChange);
	}
	else
	{
		GetSceneNode()->setPosition(GetSceneNode()->GetSavePosition());
		m_arrMovingTopPoint = m_arrTopPoint;
	}
}

void CWallODL::OnDrawTop(Gdiplus::Graphics& gcDrawer, float fScale)
{

	CBaseODL::OnDrawTop(gcDrawer, fScale);
	InitTopColor();
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
	Gdiplus::Pen pen( m_clPenColor, static_cast<Gdiplus::REAL>(1.0 / fScale));
	pen.SetDashStyle(Gdiplus::DashStyleSolid);

	if (!IsTopCreating())
	{
		if (m_ObjType==EOBT_Floor)
		{
			Gdiplus::HatchBrush brush( Gdiplus::HatchStyle(Gdiplus::HatchStyle::HatchStyleCross ), m_clPenColor, Gdiplus::Color(0,255,255,255) );
			//画皮肤
			gcDrawer.FillPolygon(&brush, st.GetData(), st.GetSize(), Gdiplus::FillMode::FillModeAlternate);
		}
		else
		{
			Gdiplus::HatchBrush brush( Gdiplus::HatchStyle(Gdiplus::HatchStyleBackwardDiagonal ), m_clPenColor, Gdiplus::Color(0,255,255,255) );
			//画皮肤
			gcDrawer.FillPolygon(&brush, st.GetData(), st.GetSize(), Gdiplus::FillMode::FillModeAlternate);
		}
	}
	return;
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
		for (const auto& curPnt : m_arrTopPoint )
		{
			gcDrawer.DrawRectangle(&Dot, static_cast<INT>( curPnt.X() - 3/fScale), static_cast<INT>(curPnt.Z() - 3/fScale), static_cast<INT>(6/fScale), static_cast<INT>(6/fScale));
		}
	}
	if (GetHasLength())
	{
		CString strText;

		gp_Dir curDir(gp_Vec(m_ptTopStart, m_ptTopStop));
		auto curAngle = curDir.AngleWithRef(gp::DX(), gp::DY());
		if ( std::abs(curAngle) > M_PI_2 )
		{
			curAngle += M_PI;
		}

		gp_Pnt midPnt((m_ptTopStop.X()+m_ptTopStart.X())/2, (m_ptTopStop.Y()+m_ptTopStart.Y())/2, (m_ptTopStop.Z()+m_ptTopStart.Z())/2);
		
		auto tmpPnt = midPnt.Rotated(gp::OY(), curAngle);

		Standard_Real fLength= m_ptTopStart.Distance(m_ptTopStop);
		strText.Format(_T("%.1f"), fLength);
		
		//需要长度显示
		
		Gdiplus::Font myFont(m_pFontFamily, 9 / fScale , Gdiplus::FontStyleRegular, Gdiplus::UnitPoint); //第二个是字体大小

		Gdiplus::PointF ptCur(static_cast<Gdiplus::REAL>(tmpPnt.X()), static_cast<Gdiplus::REAL>(tmpPnt.Z()));

		m_FontFormat.SetAlignment(Gdiplus::StringAlignmentNear);    //文本排列方式，即在对应位置居中、靠左、靠右
		gcDrawer.RotateTransform(static_cast<Gdiplus::REAL>(curAngle * M_1_PI * 180));

		gcDrawer.DrawString(strText, strText.GetLength(), &myFont, ptCur, &m_FontFormat, m_pFontBrush );//把string绘制到图上
		gcDrawer.RotateTransform(static_cast<Gdiplus::REAL>(-curAngle * M_1_PI * 180));
	}
}

bool CWallODL::IsFaceInShape( BRepExtrema_DistShapeShape& dss )
{
	for (auto& curItem : GetChildrenList() )
	{
		if ( curItem->IsFaceInShape(dss) )
		{
			return true;
		}
	}
	return CBaseODL::IsFaceInShape(dss);
}

bool CWallODL::HasTopCollisionWith( const TopoDS_Shape& theShape )
{
	for (auto& curItem : GetChildrenList() )
	{
		if ( curItem->HasTopCollisionWith(theShape) )
		{
			return true;
		}
	}
	return CBaseODL::HasTopCollisionWith(theShape);
}

void CWallODL::CreateWallFace()
{
	assert(!GetBaseShape().IsNull());

	TopTools_IndexedMapOfShape m;
	TopExp::MapShapes(GetBaseShape(), TopAbs_FACE, m);

	Standard_Real uMin, uMax, vMin, vMax;
	
	for ( auto index=1; index<=m.Extent(); ++index )
	{
		auto& curFace = TopoDS::Face(m.FindKey(index));

		auto newWallFace = std::make_shared<CWallFaceODL>(curFace);
		newWallFace->SetIndex(index);
		newWallFace->InitByParent(shared_from_this(), "WallFaceODL");
		newWallFace->UpdateMesh();

		auto dirWallFace = newWallFace->GetFaceShape().GetFaceDir();
		auto angle = dirWallFace.Angle(gp::DY());
		if (angle < Precision::Confusion() )
		{
			//shangmian
			newWallFace->SetSide(ESP_Up);
			
		}
		else if ( std::abs(M_PI-angle) < Precision::Confusion() )
		{
			//xiamian
			newWallFace->SetSide(ESP_Down);
		}
		else
		{
			newWallFace->SetSide(ESP_Other);
		}
		BRepTools::UVBounds (curFace, uMin, uMax, vMin, vMax);
		Handle(Geom_Surface) surface = BRep_Tool::Surface(curFace);
		GeomLProp_SLProps props (surface, uMin, vMin, 1, 0.001);

		auto dir = props.Normal();
		if ( TopAbs_REVERSED == curFace.Orientation() )
		{
			dir.Reverse();
		}

		if ( std::abs(dir.Angle(gp::DY().Reversed())) < Precision::Angular() )
		{
			CreatePoint(curFace);
			SetFaceShape(curFace);
		}
	}
}

HolesList CWallODL::GetHoleObj() const
{
	HolesList ret;

	for (auto& curItem:GetChildrenList())
	{
		if (curItem->GetType()==EOBT_Decor)
		{
			ret.push_back(std::static_pointer_cast<IHoleODL>(curItem));
		}
	}

	return ret;
}

bool CWallODL::UpdateMesh()
{
	auto holes = GetHoleObj();

	SetHoleList(holes);

	if ( NeedUpdateHoleMesh() )
	{
		UpdateHoleShape();
		auto mesh = ODLTools::CreateMesh(GetBuildShape());
		GetSceneNode()->setMesh(mesh);
		mesh->drop();
	}
	else
	{
		auto mesh = ODLTools::CreateMesh(GetBaseShape());
		if ( nullptr == mesh )
		{
			return false;
		}

		GetSceneNode()->setMesh(mesh);
		mesh->drop();
	}
	
	GetSceneNode()->SetSavePosition(irr::core::vector3df(0));
	GetSceneNode()->setPosition(irr::core::vector3df(0));
	GetSceneNode()->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	GetSceneNode()->CreateTriSelector();

	auto tex = GetSceneNode()->getSceneManager()->getVideoDriver()->getTexture("../Data/Resource/3D/wall.jpg");
	assert(tex);

	auto uLen = 4000.f;//tex->getSize().Width;
	auto vLen = 3000.f;//tex->getSize().Height;

	GetSceneNode()->setMaterialTexture(0, tex);
	irr::core::matrix4 texMat;
	texMat.setScale(irr::core::vector3df(1/uLen, 1/vLen, 1));
	auto meshBufferPtr = GetSceneNode()->getMesh()->getMeshBuffer(0);
	meshBufferPtr->getMaterial().setTextureMatrix(0, texMat);

	for ( auto& curChild : GetChildrenList() )
	{
		if ( EOBT_Wall_Face == curChild->GetType() )
		{
			assert(std::dynamic_pointer_cast<IWithHoleODL>(curChild));

			auto withHole = std::static_pointer_cast<IWithHoleODL>(curChild);
			withHole->SetHoleList(holes);
			withHole->UpdateHoleShape();
		}
	}

	return true;
}

void CWallODL::SetBaseSolid( const TopoDS_Solid& theWall )
{
	GetBaseShape() = theWall;
}
