#include "stdafx.h"
#include "FloorODL.h"

#include "BRepAlgoAPI_Common.hxx"
#include "Bnd_B2d.hxx"

#include "IMeshBuffer.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"

#include <vector>

CFloorODL::CFloorODL(const TopoDS_Face& theFace)
{
	m_ObjType = EOBT_Floor;
	SetFaceShape(theFace);
}


CFloorODL::~CFloorODL(void)
{
}

void CFloorODL::CancelTopMoving()
{
	CBaseODL::CancelTopMoving();
	m_arrMovingTopPoint = m_arrTopPoint;
}

void CFloorODL::OnTopMoving(const gp_Pnt& stInfo)
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

void CFloorODL::OnTopMovingFinish( const gp_Pnt& stInfo, bool bCommitChange )
{
	if (bCommitChange)
	{
		OnTopMoving(stInfo);
		SetIsTopMoving(false);

		m_arrTopPoint = m_arrMovingTopPoint;

		CBaseODL::OnTopMovingFinish(stInfo, bCommitChange);
		//UpdateMesh();
		Gdiplus::PointF ptSet(m_ptSideGlobalOffset.X, m_ptSideGlobalOffset.Y);
		//修改下级Zone的LimitArea
		for (auto& curItem: GetChildrenList())
		{
			curItem->UpdateSideGlobalOffset(ptSet);
			curItem->SetSideLimitArea(m_rtArea);
		}

 		auto savePos = GetSceneNode()->GetSavePosition();
 		savePos += irr::core::vector3df(static_cast<irr::f32>(stInfo.X()), 0, static_cast<irr::f32>(stInfo.Z()));
 		GetSceneNode()->SetSavePosition(savePos);
 		GetSceneNode()->setPosition(savePos);
	}
	else
	{
		GetSceneNode()->setPosition(GetSceneNode()->GetSavePosition());

		m_arrMovingTopPoint = m_arrTopPoint;
	}
}

void CFloorODL::OnDrawTop(Gdiplus::Graphics& gcDrawer, float fScale)
{
	InitTopColor();
	Gdiplus::Pen Dot( m_clDotColor, static_cast<Gdiplus::REAL>(1.0 / fScale));
	CArray<Gdiplus::Point> st;
	CArray<Gdiplus::Point> stMoving;
	CArray<BYTE> stBytes;
	Gdiplus::PointF ptCenter(0.0f,0.0f);
	for (auto& curItem : m_arrTopPoint)
	{
		Gdiplus::Point pt;
		pt.X = static_cast<INT>(curItem.X());
		pt.Y = static_cast<INT>(curItem.Z());
		ptCenter.X += static_cast<Gdiplus::REAL>(curItem.X());
		ptCenter.Y += static_cast<Gdiplus::REAL>(curItem.Z());
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
	ptCenter.X /= st.GetSize();
	ptCenter.Y /= st.GetSize();
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
		//Gdiplus::HatchBrush brush( Gdiplus::HatchStyle(Gdiplus::HatchStyle::HatchStyleCross ), m_clPenColor, Gdiplus::Color(0,255,255,255) );
		Gdiplus::SolidBrush brush(Gdiplus::Color::Azure);
		if (IsTopFocused())
		{
			brush.SetColor(Gdiplus::Color::AliceBlue);
		}
		//画皮肤
		gcDrawer.FillPolygon(&brush, st.GetData(), st.GetSize(), Gdiplus::FillMode::FillModeAlternate);
	}

	//gcDrawer.DrawLines(&pen, st.GetData(), st.GetSize());

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
	Gdiplus::Font myFont(m_pFontFamily, 9 / fScale , Gdiplus::FontStyleRegular, Gdiplus::UnitPoint); //第二个是字体大小
	
	Gdiplus::StringFormat format;
	
	CString strIndex = _T("地板");
	format.SetAlignment(Gdiplus::StringAlignmentCenter);    //文本排列方式，即在对应位置居中、靠左、靠右
	gcDrawer.DrawString(strIndex, strIndex.GetLength(), &myFont, ptCenter, &format, m_pFontBrush );//把string绘制到图上


	CBaseODL::OnDrawTop(gcDrawer, fScale);

}

void CFloorODL::OnDrawFace( Gdiplus::Graphics& gcDrawer, float fScale )
{
	InitSideColor();
	Gdiplus::Pen Dot( m_clDotColor, static_cast<Gdiplus::REAL>(1.0 / fScale));
	CArray<Gdiplus::PointF> st;
	CArray<BYTE> stBytes;

	Gdiplus::GraphicsPath gcPath;
	int nInc = 0;
	for (auto& curItem : m_arrTopPoint)
	{
		Gdiplus::PointF pt;
		pt.X = static_cast<Gdiplus::REAL>(curItem.X());
		pt.Y = static_cast<Gdiplus::REAL>(curItem.Z());
		st.Add(pt);
		if (nInc>0)
		{
			gcPath.AddLine(st[nInc-1], pt);
		}
		nInc++;
	}
	
	if(st.GetSize()<=0)
	{
		return;
	}
	Gdiplus::Pen pen( m_clPenColor, static_cast<Gdiplus::REAL>(1.0 / fScale));
	pen.SetDashStyle(Gdiplus::DashStyleSolid);

	//画皮肤
	Gdiplus::SolidBrush brush(Gdiplus::Color::Azure);
	//画皮肤
	gcDrawer.FillPolygon(&brush, st.GetData(), st.GetSize(), Gdiplus::FillMode::FillModeAlternate);

	gcDrawer.DrawLines(&pen, st.GetData(), st.GetSize());
	if (m_spSkin)
	{
		m_spSkin->DrawImage(gcDrawer, gcPath, Gdiplus::PointF(m_rtArea.X, m_rtArea.Y), fScale);
		//先把每个图片的坐标保存下来
	}
	for (auto& curItem: GetChildrenList())
	{
		curItem->OnDrawFace(gcDrawer, fScale);
	}

	gcDrawer.DrawRectangle(&pen, m_rtArea);
}

bool CFloorODL::UpdateMesh()
{
	Bnd_B2d bbox;

	for ( auto& curPnt : m_arrTopPoint )
	{
		bbox.Add(gp_Pnt2d(curPnt.X(), curPnt.Z()));
	}
	
	m_rtArea.X = static_cast<Gdiplus::REAL>(bbox.CornerMin().X());
	m_rtArea.Y = static_cast<Gdiplus::REAL>(bbox.CornerMin().Y());

	m_rtArea.Width = static_cast<Gdiplus::REAL>(bbox.CornerMax().X() - m_rtArea.X);
	m_rtArea.Height = static_cast<Gdiplus::REAL>(bbox.CornerMax().Y() - m_rtArea.Y);

	m_rtMovingArea = m_rtArea;
	m_ptSideGlobalOffset.X = m_rtArea.X;
	m_ptSideGlobalOffset.Y = m_rtArea.Y;

	auto meshInfo = ODLTools::CreateFaceMesh(GetFaceShape().GetFace(), GetFaceShape().GetFaceDir());
	if ( !meshInfo )
	{
		return false;
	}

	auto& meshRef = *meshInfo;

	GetSceneNode()->setPosition(meshRef.Translation_);
	GetSceneNode()->setRotation(meshRef.Rotation_);
	GetSceneNode()->SetSavePosition(meshRef.Translation_);
	GetSceneNode()->AddToDepthPass();
	GetSceneNode()->setMesh(meshRef.Mesh_);
	meshRef.Mesh_->drop();
	GetSceneNode()->setMaterialFlag(irr::video::EMF_LIGHTING, false);

	//设置纹理
	if ( m_spSkin && m_spSkin->HasPicData() )
	{
		auto tex = m_spSkin->GetTextureData();
		GetSceneNode()->setMaterialTexture(0, tex);

		auto uLen = m_spSkin->GetSkinWidth()+m_spSkin->GetGroutX();
		auto vLen = m_spSkin->GetSkinHeight()+m_spSkin->GetGroutY();
		auto uFactor = m_rtArea.Width / uLen;
		auto vFactor = m_rtArea.Height / vLen;

		irr::core::matrix4 texMat;
		texMat.setScale(irr::core::vector3df(uFactor, vFactor, 1));
		auto meshBufferPtr = GetSceneNode()->getMesh()->getMeshBuffer(0);
		meshBufferPtr->getMaterial().setTextureMatrix(0, texMat);
	}
	else
	{
		auto tex = GetSceneNode()->getSceneManager()->getVideoDriver()->getTexture("../Data/Resource/3D/floor.jpg");
		assert(tex);

		auto uLen = tex->getSize().Width;
		auto vLen = tex->getSize().Height;
		auto uFactor = static_cast<float>(GetFaceShape().GetURange()) / uLen;
		auto vFactor = static_cast<float>(GetFaceShape().GetVRange()) / vLen;

		GetSceneNode()->setMaterialTexture(0, tex);
		irr::core::matrix4 texMat;
		texMat.setScale(irr::core::vector3df(uFactor, vFactor, 1));
		auto meshBufferPtr = GetSceneNode()->getMesh()->getMeshBuffer(0);
		meshBufferPtr->getMaterial().setTextureMatrix(0, texMat);
	}

	return true;
}
