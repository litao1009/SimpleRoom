#include "stdafx.h"

#include "WallFaceODL.h"
#include "WallODL.h"

#include "TopExp_Explorer.hxx"
#include "BRepTools_WireExplorer.hxx"
#include "TopoDS.hxx"
#include "BRep_Tool.hxx"
#include "Precision.hxx"
#include "BRepAlgoAPI_Section.hxx"
#include "TopoDS.hxx"
#include "TopExp.hxx"

#include "IMeshBuffer.h"
#include "ISceneManager.h"
#include "IVideoDriver.h"

CWallFaceODL::CWallFaceODL(const TopoDS_Face& curFace)
{
	m_ObjType = EOBT_Wall_Face;
	SetFaceShape(curFace);

	TopExp_Explorer faceExp(curFace, TopAbs_WIRE);

	for ( BRepTools_WireExplorer wireExp(TopoDS::Wire(faceExp.Current())); wireExp.More(); wireExp.Next() )
	{
		auto pnt = BRep_Tool::Pnt(wireExp.CurrentVertex());
		//if ( std::abs(pnt.Y()) < Precision::Confusion() )
		{
			m_arrTopPoint.push_back(pnt);
		}
	}
	m_arrMovingTopPoint = m_arrTopPoint;
	//计算所有的点的位置，在上面的，是顶部，在下面的，是底部，在旁边的，是墙面
	int nPTXCount = 0;
	int nPTYCount = 0;
	int nPTZCount = 0;
	int nPTCount = m_arrTopPoint.size();
	gp_Pnt ptPrev;
	gp_Pnt ptNext;
	int i=0;
	for (auto& pt: m_arrTopPoint)
	{
		ptPrev = pt;
		if (i<nPTCount-1)
		{
			ptNext = m_arrTopPoint[i+1];
			if (IsEqual(ptPrev.X(), ptNext.X() ))
			{
				//左右2面
				nPTXCount++;
			}
			if (IsEqual(ptPrev.Y(), ptNext.Y() ))
			{
				//上下2面
				nPTYCount++;
			}
			if (IsEqual(ptPrev.Z(), ptNext.Z() ))
			{
				//前后2面
				nPTZCount++;
			}
		}
	}
	gp_Dir d1(gp_Vec(m_arrTopPoint[0], m_arrTopPoint[1]));
	gp_Dir d2(gp_Vec(m_arrTopPoint[1], m_arrTopPoint[2]));

	//计算第一条边是横着的还是竖着的
	auto d2Angle = d2.Angle(gp::DY());
	d2Angle = std::fmod(d2Angle, M_PI);

	auto d1Angle = d1.Angle(gp::DY());
	d1Angle = std::fmod(d1Angle, M_PI);
	m_rtArea.X = 0.0f;
	m_rtArea.Y =0.0f;
	m_rtArea.Width = 10.0f;
	m_rtArea.Height = 10.0f;
	if ( d2Angle < 0.01 )
	{
		//横着的
		m_rtArea.Width = static_cast<Gdiplus::REAL>(m_arrTopPoint[0].Distance(m_arrTopPoint[1]));
		m_rtArea.Height = static_cast<Gdiplus::REAL>(m_arrTopPoint[1].Distance(m_arrTopPoint[2]));
	}
	else if (d1Angle < 0.01)
	{
		//竖着的
		m_rtArea.Width = static_cast<Gdiplus::REAL>(m_arrTopPoint[1].Distance(m_arrTopPoint[2]));
		m_rtArea.Height = static_cast<Gdiplus::REAL>(m_arrTopPoint[0].Distance(m_arrTopPoint[1]));
	}
	else if (std::fabs(d2Angle - M_PI_2)<0.01)
	{
		m_rtArea.Width = static_cast<Gdiplus::REAL>(m_arrTopPoint[0].Distance(m_arrTopPoint[1]));
		m_rtArea.Height = static_cast<Gdiplus::REAL>(m_arrTopPoint[1].Distance(m_arrTopPoint[2]));
	}

	m_ptSideGlobalOffset.X = 0.0f;
	m_ptSideGlobalOffset.Y = 0.0f;
	
	if (nPTXCount==nPTCount)
	{
		//左右2面(x,y)
		
	}
	if (nPTYCount==nPTCount)
	{
		//上下2面
	}
	if (nPTZCount == nPTCount)
	{
		//前后2面
		
	}
	SetHasLength(true);
}


CWallFaceODL::~CWallFaceODL(void)
{
}

void CWallFaceODL::CancelTopMoving()
{
	m_arrMovingTopPoint = m_arrTopPoint;
	CBaseODL::CancelTopMoving();
}

void CWallFaceODL::OnTopMoving(const gp_Pnt& stInfo)
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

void CWallFaceODL::OnTopMovingFinish(const gp_Pnt& stInfo, bool bCommitChange )
{
	auto sn = GetSceneNode();

	if (bCommitChange)
	{
		OnTopMoving(stInfo);

		m_arrTopPoint = m_arrMovingTopPoint;
		if (m_arrTopPoint.size()>1)
		{
			m_ptTopStart = m_arrTopPoint.front();
			m_ptTopStop = m_arrTopPoint.at(1);
		}

		CBaseODL::OnTopMovingFinish(stInfo, bCommitChange);
	}
	else
	{
		m_arrMovingTopPoint = m_arrTopPoint;
	}
}

void CWallFaceODL::OnDrawTop( Gdiplus::Graphics& gcDrawer, float fScale )
{
	//墙的顶面//底面不画
	if ((GetSide()==ESP_Down) || (GetSide()==ESP_Up))
	{
		return;
	}
	//画这个面在俯视图下级的线段
	CBaseODL::OnDrawTop(gcDrawer, fScale);
	//画本身的内容
	if (m_arrTopPoint.size()==2)
	{
		m_ptTopStart = m_arrTopPoint.front();
		m_ptTopStop = m_arrTopPoint.back();
	}
	
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
	ptCenter.X /=st.GetSize();
	ptCenter.Y /=st.GetSize();
	Gdiplus::Pen pen( m_clPenColor, static_cast<Gdiplus::REAL>(1.0 / fScale));
	pen.SetDashStyle(Gdiplus::DashStyleSolid);

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
	
	Gdiplus::Font myFont(m_pFontFamily, 9 / fScale , Gdiplus::FontStyleRegular, Gdiplus::UnitPoint); //第二个是字体大小
	
	m_FontFormat.SetAlignment(Gdiplus::StringAlignmentNear);    //文本排列方式，即在对应位置居中、靠左、靠右
	if (GetHasLength())
	{
		if (m_ptTopStart.IsEqual(m_ptTopStop, Precision::Confusion()))
		{
			return;
		}
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
		
		Gdiplus::PointF ptCur(static_cast<Gdiplus::REAL>(tmpPnt.X()), static_cast<Gdiplus::REAL>(tmpPnt.Z()));

		gcDrawer.RotateTransform(static_cast<Gdiplus::REAL>(curAngle * M_1_PI * 180));

		gcDrawer.DrawString(strText, strText.GetLength(), &myFont, ptCur, &m_FontFormat, m_pFontBrush );//把string绘制到图上
		gcDrawer.RotateTransform(static_cast<Gdiplus::REAL>(-curAngle * M_1_PI * 180));
	}
	CString strIndex = _T("");
	strIndex.Format(_T("%d"), GetIndex());
	m_FontFormat.SetAlignment(Gdiplus::StringAlignmentCenter);    //文本排列方式，即在对应位置居中、靠左、靠右
	gcDrawer.DrawString(strIndex, strIndex.GetLength(), &myFont, ptCenter, &m_FontFormat, m_pFontBrush );//把string绘制到图上
}

void CWallFaceODL::OnDrawFace( Gdiplus::Graphics& gcDrawer, float fScale )
{
	//画墙面矩形
	InitSideColor();
	//画本身的内容
	
	Gdiplus::PointF ptCenter(m_rtArea.Width/2.0f, m_rtArea.Height/2.0f);
	
	Gdiplus::Pen pen( m_clPenColor, static_cast<Gdiplus::REAL>(1.0 / fScale));
	pen.SetDashStyle(Gdiplus::DashStyleSolid);

	gcDrawer.DrawRectangle(&pen, m_rtArea);
	if (m_spSkin)
	{
		m_spSkin->DrawImage(gcDrawer, m_rtArea, fScale);
		//先把每个图片的坐标保存下来
	}
	
	Gdiplus::Font myFont(m_pFontFamily, 9 / fScale , Gdiplus::FontStyleRegular, Gdiplus::UnitPoint); //第二个是字体大小
	
	m_FontFormat.SetAlignment(Gdiplus::StringAlignmentNear);    //文本排列方式，即在对应位置居中、靠左、靠右

	if (GetHasLength())
	{
	//画宽
	//画高
		CString strText;

		std::vector<gp_Pnt> arrPt;
		arrPt.emplace_back(gp_Pnt(0.0f, 0.0f, 0.0f));
		arrPt.emplace_back(gp_Pnt(m_rtArea.Width, 0.0f, 0.0f));
		arrPt.emplace_back(gp_Pnt(m_rtArea.Width, m_rtArea.Height, 0.0f));
		for (unsigned i=1; i<arrPt.size(); ++i)
		{
			gp_Pnt& pt0 = arrPt[i-1];
			gp_Pnt& pt1 = arrPt[i];
			gp_Dir curDir(gp_Vec(pt0, pt1));
			auto curAngle = curDir.AngleWithRef(gp::DX(), gp::DY());
			if ( std::abs(curAngle) > M_PI_2 )
			{
				curAngle += M_PI;
			}

			gp_Pnt midPnt((pt1.X()+pt0.X())/2, (pt1.Y()+pt0.Y())/2, (pt1.Z()+pt0.Z())/2);

			auto tmpPnt = midPnt.Rotated(gp::OY(), curAngle);

			Standard_Real fLength= pt0.Distance(pt1);
			strText.Format(_T("%.1f"), fLength);

			//需要长度显示

			Gdiplus::PointF ptCur(static_cast<Gdiplus::REAL>(tmpPnt.X()), static_cast<Gdiplus::REAL>(tmpPnt.Z()));

			gcDrawer.RotateTransform(static_cast<Gdiplus::REAL>(curAngle * M_1_PI * 180));

			gcDrawer.DrawString(strText, strText.GetLength(), &myFont, ptCur, &m_FontFormat, m_pFontBrush );//把string绘制到图上
			gcDrawer.RotateTransform(static_cast<Gdiplus::REAL>(-curAngle * M_1_PI * 180));
		}
	}
	CString strIndex = _T("");
	strIndex.Format(_T("%d"), GetIndex());
	m_FontFormat.SetAlignment(Gdiplus::StringAlignmentCenter);    //文本排列方式，即在对应位置居中、靠左、靠右
	gcDrawer.DrawString(strIndex, strIndex.GetLength(), &myFont, ptCenter, &m_FontFormat, m_pFontBrush );//把string绘制到图

	//画这个面在俯视图下级的线段
	CBaseODL::OnDrawFace(gcDrawer, fScale );
	//如果上级有嵌入式Decor，把Decor位置画出来

	//if (sp)
	{
		TRACE(_T("乱写的装饰门位置信息开始"));
		for (auto& curObj : GetRectList())
		{
			auto& curHoleODL = std::get<0>(curObj);
			auto& offset = std::get<1>(curObj);
			auto& size = std::get<2>(curObj);
			
			auto spHoleODL = curHoleODL.lock();
			spHoleODL->SetSideGolbalOffset(Gdiplus::PointF(static_cast<Gdiplus::REAL>(offset.X()), static_cast<Gdiplus::REAL>(offset.Y())));
			
			spHoleODL->OnDrawFace(gcDrawer, fScale);
		}
		
		TRACE(_T("乱写的装饰门位置信息结束"));
	}
}

bool CWallFaceODL::UpdateMesh()
{
	return UpdateHoleShape();
}

bool CWallFaceODL::UpdateHoleShape()
{
	auto hasTexture = m_spSkin && m_spSkin->HasPicData();

	{//Mesh

		ODLTools::SFaceMesh meshRef;

		if ( !hasTexture )
		{
			meshRef = *ODLTools::CalculateRelation(GetFaceShape().GetFace(), GetFaceShape().GetFaceDir());
		}
		else
		{
			TopoDS_Face face;

			if ( !GetHoleList().empty() )
			{
				GetBaseShape() = GetFaceShape().GetFace();
				IWithHoleODL::UpdateHoleShape();
				TopExp_Explorer exp(GetBuildShape(), TopAbs_FACE);
				face = TopoDS::Face(exp.Current());
			}
			else
			{
				face = GetFaceShape().GetFace();
			}
			
			meshRef = *ODLTools::CreateFaceMesh(face, GetFaceShape().GetFaceDir());
			GetSceneNode()->setMesh(meshRef.Mesh_);
			meshRef.Mesh_->drop();
			GetSceneNode()->CreateTriSelector();

			{//Texture
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
				meshBufferPtr->getMaterial().PolygonOffsetDirection = irr::video::EPO_BACK;
				meshBufferPtr->getMaterial().PolygonOffsetFactor = 3;
			}
		}

		GetSceneNode()->SetSavePosition(meshRef.Translation_);
		GetSceneNode()->setPosition(meshRef.Translation_);
		GetSceneNode()->setRotation(meshRef.Rotation_);
		GetSceneNode()->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	}
	
	{//Hole
		GetRectList().clear();

		auto translation3d = GetSceneNode()->getPosition();
		auto rotation3d = GetSceneNode()->getRotation();
		auto relation = GetSceneNode()->getRelativeTransformation();
		auto invertRelation = relation;
		invertRelation.makeInverse();

		for ( auto& curHole : GetHoleList() )
		{
			auto spHole = curHole.lock();

			BRepAlgoAPI_Section bs(GetFaceShape().GetFace(), spHole->GetOffsetHoleShape());
			if ( !bs.IsDone() )
			{
				continue;
			}

			auto section = bs.Shape();

			TopTools_IndexedMapOfShape m;
			TopExp::MapShapes(section, TopAbs_VERTEX, m);

			if ( m.IsEmpty() )
			{
				continue;
			}

			irr::core::aabbox3df testBB;
			for ( auto index=1; index<=m.Extent(); ++index )
			{
				auto pnt = BRep_Tool::Pnt(TopoDS::Vertex(m.FindKey(index)));
				if ( 1 == index )
				{
					testBB.reset(static_cast<float>(pnt.X()), static_cast<float>(pnt.Y()), static_cast<float>(pnt.Z()));
				}
				else
				{
					testBB.addInternalPoint(static_cast<float>(pnt.X()), static_cast<float>(pnt.Y()), static_cast<float>(pnt.Z()));
				}
			}

			invertRelation.transformBoxEx(testBB);
			auto extent = testBB.getExtent();
			GetRectList().emplace_back(curHole,gp_Pnt2d(testBB.MinEdge.X,-testBB.MaxEdge.Y), gp_XY(extent.X,extent.Y));
		}
	}

	{//dispatch
		for ( auto& curChild : GetChildrenList() )
		{
			if ( EOBT_Wall_Face_Zone == curChild->GetType() )
			{
				assert(std::dynamic_pointer_cast<IWithHoleODL>(curChild));

				auto withHole = std::static_pointer_cast<IWithHoleODL>(curChild);
				withHole->SetHoleList(GetHoleList());
				withHole->GetRectList() = GetRectList();
				withHole->UpdateHoleShape();
			}
		}
	}

	return true;
}

void CWallFaceODL::UpdateHoleRect( RelationRect rect )
{
	auto wpHole = std::get<0>(rect);
	auto newPos = std::get<1>(rect);
	auto size = std::get<2>(rect);

	auto sphole = wpHole.lock(); 
	if ( !sphole )
	{
		return;
	}

	auto itor = std::find_if(GetRectList().begin(), GetRectList().end(),
		[sphole](RelationRect& curRect)
	{
		return sphole == std::get<0>(curRect).lock();
	});

	assert(itor != GetRectList().end());

	auto rawPos = std::get<1>(*itor);

	irr::core::vector3df raw3d(static_cast<float>(rawPos.X()), static_cast<float>(-rawPos.Y()), 0);
	irr::core::vector3df new3d(static_cast<float>(newPos.X()), static_cast<float>(-newPos.Y()), 0);

	auto translation = GetSceneNode()->getRelativeTransformation();
	translation.transformVect(raw3d);
	translation.transformVect(new3d);
	auto vec = new3d-raw3d;

	auto pos = sphole->GetHolePostion();
	pos.SetXYZ(pos.XYZ()+gp_XYZ(vec.X, vec.Y, vec.Z));
	sphole->SetHolePosition(pos);
}
