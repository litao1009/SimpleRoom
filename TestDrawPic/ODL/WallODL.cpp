#include "stdafx.h"

#include "WallODL.h"
#include "CornerODL.h"
#include "GraphODL.h"

#include "IVideoDriver.h"
#include "ISceneManager.h"

#include "IrrEngine/IrrEngine.h"

#include "MeshSceneNode/WallMeshNode2D.h"

#include "gp_Lin.hxx"
#include "gp_Pln.hxx"
#include "gp_Ax3.hxx"
#include "GeomAPI.hxx"
#include "Geom2dAPI_InterCurveCurve.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "BRepAdaptor_Curve.hxx"
#include "TopoDS_Shape.hxx"
#include "BRepBuilderAPI_MakePolygon.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "BRepPrimAPI_MakePrism.hxx"
#include "BRepBndLib.hxx"

class	WallODL::Imp
{
public:

	static	void	CalculateSideMesh(const WallODLSPtr& wall, const CornerODLSPtr& corner)
	{
		auto& meshPoints_ = wall->MeshPoints_;

		auto beginCorner = wall->GetFirstCorner().lock();
		auto endCorner = wall->GetSecondCorner().lock();

		if ( beginCorner != corner )
		{
			beginCorner.swap(endCorner);
		}

		assert(beginCorner == corner);

		auto curBeginPos = beginCorner->GetPosition();
		auto curEndPos = endCorner->GetPosition();

		gp_Dir curDir = gp_Vec(curBeginPos, curEndPos);
		auto curThickDir = curDir.Crossed(gp::DY().Reversed());
		gp_Vec curThickVec = gp_Vec(curThickDir) * wall->GetThickness()/2;

		auto wallsAtCorner = wall->GraphODL_.lock()->GetWallsOnCorner(corner);
		assert(!wallsAtCorner.empty());

		{
			auto p1 = curThickVec.XYZ() + curBeginPos.XYZ();
			auto p2 = curBeginPos.XYZ();
			auto p3 = curThickVec.Reversed().XYZ() + curBeginPos.XYZ();

			if ( corner == wall->GetFirstCorner().lock() )
			{
				meshPoints_[0] = p1;
				meshPoints_[5] = p2;
				meshPoints_[4] = p3;
			}
			else
			{
				meshPoints_[3] = p1;
				meshPoints_[2] = p2;
				meshPoints_[1] = p3;
			}
		}

		if ( wallsAtCorner.size() == 1 )
		{
			return;
		}

		WallODLSPtr otherTopWall,otherBottomWall;

		if ( wallsAtCorner.size() == 2 )
		{
			auto otherWall = wallsAtCorner[0] == wall ? wallsAtCorner[1] : wallsAtCorner[0];
			otherTopWall = otherWall;
			otherBottomWall = otherWall;
		}
		else
		{
			wall->ImpUPtr_->RefCorner_ = beginCorner;
			std::sort(wallsAtCorner.begin(), wallsAtCorner.end(), *(wall->ImpUPtr_));

			auto curItor = std::find(wallsAtCorner.begin(), wallsAtCorner.end(), wall);
			if ( curItor == wallsAtCorner.begin() )
			{
				otherTopWall = *(curItor + 1);
				otherBottomWall = wallsAtCorner.back();
			}
			else if ( curItor + 1 == wallsAtCorner.end() )
			{
				otherTopWall = wallsAtCorner.front();
				otherBottomWall = *(curItor - 1);
			}
			else
			{
				otherTopWall = *(curItor + 1);
				otherBottomWall = *(curItor - 1);
			}
		}

		gp_Pnt p1,p3,p0,p4;
		p1 = meshPoints_[1];
		p3 = meshPoints_[3];
		p0 = meshPoints_[0];
		p4 = meshPoints_[4];

		{//Top
			auto otherBeginCorner = otherTopWall->GetFirstCorner().lock();
			auto otherEndCorner = otherTopWall->GetSecondCorner().lock();

			if ( otherBeginCorner != corner )
			{
				otherBeginCorner.swap(otherEndCorner);
			}

			auto otherBeginPos = otherBeginCorner->GetPosition();
			auto otherEndPos = otherEndCorner->GetPosition();
			gp_Dir otherDir = gp_Vec(otherBeginPos, otherEndPos);
			auto otherThickDir = otherDir.Crossed(gp::DY().Reversed());
			gp_Vec otherThickVec = gp_Vec(otherThickDir) * otherTopWall->GetThickness()/2;

			auto curEdge = BRepBuilderAPI_MakeEdge(curBeginPos.XYZ()+curThickVec.XYZ(), curEndPos.XYZ()+curThickVec.XYZ()).Edge();
			auto otherEdge = BRepBuilderAPI_MakeEdge(otherBeginPos.XYZ()+otherThickVec.Reversed().XYZ(), otherEndPos.XYZ()+otherThickVec.Reversed().XYZ()).Edge();

			BRepAdaptor_Curve curBC(curEdge);
			BRepAdaptor_Curve otherBC(otherEdge);

			if ( Standard_False == curBC.Line().Direction().IsParallel(otherBC.Line().Direction(), Precision::Confusion()) )
			{
				gp_Pln pln(gp_Ax3(gp::Origin(), gp::DY().Reversed(), gp::DX()));

				auto firstline2D = GeomAPI::To2d(curBC.Curve().Curve(), pln);
				auto secondline2D = GeomAPI::To2d(otherBC.Curve().Curve(), pln);

				Geom2dAPI_InterCurveCurve icc(firstline2D, secondline2D);
				assert(icc.NbPoints() == 1);

				auto pnt2D = icc.Point(1);
				gp_Pnt pnt(pnt2D.X(), 0, pnt2D.Y());

				if ( corner == wall->GetFirstCorner().lock() )
				{
					p0 = pnt;
				}
				else
				{
					p3 = pnt;
				}
			}
		}

		{//Bottom
			auto otherBeginCorner = otherBottomWall->GetFirstCorner().lock();
			auto otherEndCorner = otherBottomWall->GetSecondCorner().lock();

			if ( otherBeginCorner != corner )
			{
				otherBeginCorner.swap(otherEndCorner);
			}

			auto otherBeginPos = otherBeginCorner->GetPosition();
			auto otherEndPos = otherEndCorner->GetPosition();
			gp_Dir otherDir = gp_Vec(otherBeginPos, otherEndPos);
			auto otherThickDir = otherDir.Crossed(gp::DY().Reversed());
			gp_Vec otherThickVec = gp_Vec(otherThickDir) * otherTopWall->GetThickness()/2;

			auto curEdge = BRepBuilderAPI_MakeEdge(curBeginPos.XYZ()+curThickVec.Reversed().XYZ(), curEndPos.XYZ()+curThickVec.Reversed().XYZ()).Edge();
			auto otherEdge = BRepBuilderAPI_MakeEdge(otherBeginPos.XYZ()+otherThickVec.XYZ(), otherEndPos.XYZ()+otherThickVec.XYZ()).Edge();

			BRepAdaptor_Curve curBC(curEdge);
			BRepAdaptor_Curve otherBC(otherEdge);

			if ( Standard_False == curBC.Line().Direction().IsParallel(otherBC.Line().Direction(), Precision::Confusion()) )
			{
				gp_Pln pln(gp_Ax3(gp::Origin(), gp::DY().Reversed(), gp::DX()));

				auto firstline2D = GeomAPI::To2d(curBC.Curve().Curve(), pln);
				auto secondline2D = GeomAPI::To2d(otherBC.Curve().Curve(), pln);

				Geom2dAPI_InterCurveCurve icc(firstline2D, secondline2D);
				assert(icc.NbPoints() == 1);

				auto pnt2D = icc.Point(1);
				gp_Pnt pnt(pnt2D.X(), 0, pnt2D.Y());

				if ( corner == wall->GetFirstCorner().lock() )
				{
					p4 = pnt;
				}
				else
				{
					p1 = pnt;
				}
			}
		}

		meshPoints_[0] = p0;
		meshPoints_[1] = p1;
		meshPoints_[3] = p3;
		meshPoints_[4] = p4;
	}

public:

	bool	operator() (const WallODLSPtr& wall1, const WallODLSPtr& wall2)
	{
		auto wall1FirstCorner = wall1->GetFirstCorner().lock();
		auto wall1SecondCorner = wall1->GetSecondCorner().lock();
		auto wall2FirstCorner = wall2->GetFirstCorner().lock();
		auto wall2SecondCorner = wall2->GetSecondCorner().lock();

		if ( wall1FirstCorner != RefCorner_ )
		{
			wall1FirstCorner.swap(wall1SecondCorner);
			assert(wall1FirstCorner == RefCorner_);
		}

		if ( wall2FirstCorner != RefCorner_ )
		{
			wall2FirstCorner.swap(wall2SecondCorner);
			assert(wall2FirstCorner == RefCorner_);
		}

		gp_Dir d1 = gp_Vec(wall1FirstCorner->GetPosition(), wall1SecondCorner->GetPosition());
		gp_Dir d2 = gp_Vec(wall2FirstCorner->GetPosition(), wall2SecondCorner->GetPosition());

		auto angle1 = d1.AngleWithRef(gp::DX(), gp::DY().Reversed());
		auto angle2 = d2.AngleWithRef(gp::DX(), gp::DY().Reversed());

		angle1 = angle1 < 0 ? M_PI * 2 + angle1 : angle1;
		angle2 = angle2 < 0 ? M_PI * 2 + angle2 : angle2;

		return angle1 < angle2;
	}

	CornerODLSPtr		RefCorner_;
	WallMeshNode2D*		Node2D_;
};

WallODL::WallODL( const GraphODLWPtr graphODL, const CornerODLSPtr& firstCorner, const CornerODLSPtr& secondCorner, float wallThickness /*= 200.f*/, float height )
	:ImpUPtr_(new Imp),CBaseODL(graphODL.lock()->GetRenderContextWPtr())
{
	GraphODL_ = graphODL;
	Thickness_ = wallThickness;
	Height_ = height;
	FirstCorner_ = firstCorner;
	SecondCorner_ = secondCorner;
	ImpUPtr_->Node2D_ = nullptr;
}

WallODL::~WallODL( void )
{

}

void WallODL::Init()
{
	auto wall2DNode = new WallMeshNode2D(GetDataSceneNode()->GetSceneNode2D());
	wall2DNode->drop();
	ImpUPtr_->Node2D_ = wall2DNode;
}

bool WallODL::IsBezierCurve() const
{
	return false;
}

TopoDS_Edge WallODL::GetEdge(const CornerODLSPtr& fromCorner) const
{
	auto beginCorner = fromCorner == FirstCorner_.lock() ? FirstCorner_.lock() : SecondCorner_.lock();
	auto endCorner = beginCorner == FirstCorner_.lock() ? SecondCorner_.lock() : FirstCorner_.lock();

	return BRepBuilderAPI_MakeEdge(beginCorner->GetPosition(), endCorner->GetPosition()).Edge();
}

void WallODL::UpdateMesh()
{
	if ( IsBezierCurve() )
	{
		//TODO
	}
	else
	{
		auto thisSPtr = std::static_pointer_cast<WallODL>(shared_from_this());

		Imp::CalculateSideMesh(thisSPtr, FirstCorner_.lock());
		Imp::CalculateSideMesh(thisSPtr, SecondCorner_.lock());
	}

	BRepBuilderAPI_MakePolygon mp;
	for ( auto& curPnt : MeshPoints_ )
	{
		mp.Add(curPnt);
	}
	mp.Close();

	assert(mp.IsDone());
	
	auto bottomFace = BRepBuilderAPI_MakeFace(mp.Wire()).Face();
	auto solid = BRepPrimAPI_MakePrism(bottomFace, gp_Vec(gp::Origin(), gp_Pnt(0, Height_, 0))).Shape();

	gp_Trsf absoluteToRelation;
	{//设置位置
		gp_Trsf rotToDZ;
		gp_Trsf transToCenter;

		gp_Dir wallDir = gp_Vec(FirstCorner_.lock()->GetPosition(), SecondCorner_.lock()->GetPosition());
		rotToDZ.SetRotation(gp_Quaternion(wallDir, gp::DZ()));
		solid.Move(rotToDZ);

		Bnd_Box wallBox;
		gp_Pnt wallCenter;
		{
			BRepBndLib::Add(solid, wallBox);
			Standard_Real xMin,yMin,zMin,xMax,yMax,zMax;
			wallBox.Get(xMin, yMin, zMin, xMax, yMax, zMax);
			wallCenter.SetXYZ(gp_XYZ((xMin+xMax)/2, (yMin+yMax)/2, (zMin+zMax)/2));
		}

		transToCenter.SetTranslationPart(gp_Vec(gp::Origin(), wallCenter).Reversed());

		solid.Move(transToCenter);

		absoluteToRelation = transToCenter * rotToDZ;

		auto relationToAbsolute = absoluteToRelation.Inverted();
		auto translation = relationToAbsolute.TranslationPart();
		auto rotation = relationToAbsolute.GetRotation();

		SetBaseShape(solid);

		SetTranslation(translation);
		GetDataSceneNode()->setPosition(irr::core::vector3df(static_cast<float>(translation.X()), static_cast<float>(translation.Y()), static_cast<float>(translation.Z())));

		SetRoration(rotation);
		Standard_Real rX,rY,rZ;
		rotation.GetEulerAngles(gp_Extrinsic_XYZ, rX, rY, rZ);
		GetDataSceneNode()->setRotation(irr::core::vector3df(static_cast<float>(irr::core::radToDeg(rX)), static_cast<float>(irr::core::radToDeg(rY)), static_cast<float>(irr::core::radToDeg(rZ))));
	}

	{//3D模型
		auto meshBuf = ODLTools::NEW_CreateMeshBuffer(solid);
		assert(meshBuf);
		auto newMesh = new irr::scene::SMesh;
		newMesh->addMeshBuffer(meshBuf);
		newMesh->recalculateBoundingBox();

		GetDataSceneNode()->setMesh(newMesh);
		newMesh->drop();
		meshBuf->drop();
	}
	
	{//2D模型
		auto transformedFace = bottomFace.Moved(absoluteToRelation);
		gp_Trsf tfs;
		tfs.SetTranslationPart(gp_Vec(gp::Origin(), gp_Pnt(0, Height_/2, 0)));
		transformedFace.Move(tfs);
		ImpUPtr_->Node2D_->UpdateMesh(transformedFace);
	}
	
	{//设置效果
		GetDataSceneNode()->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		GetDataSceneNode()->AddToDepthPass();
		SetDefaultTexture();
	}
}

void WallODL::SetDefaultTexture()
{
	auto tex = GetDataSceneNode()->getSceneManager()->getVideoDriver()->getTexture("../Data/Resource/3D/wall.jpg");
	assert(tex);

	auto uLen = 4000.f;//tex->getSize().Width;
	auto vLen = 3000.f;//tex->getSize().Height;

	irr::core::matrix4 texMat;
	texMat.setScale(irr::core::vector3df(1/uLen, 1/vLen, 1));
	auto meshBufferPtr = GetDataSceneNode()->getMesh()->getMeshBuffer(0);

	meshBufferPtr->getMaterial().setTextureMatrix(0, texMat);
	GetDataSceneNode()->setMaterialTexture(0, tex);
}

void WallODL::UpdateSweeping()
{
	if ( IsSweeping() )
	{
		GetDataSceneNode()->getMaterial(0).MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_LUMINANCE);
	}
	else
	{
		GetDataSceneNode()->getMaterial(0).MaterialType = irr::video::EMT_SOLID;
	}

	ImpUPtr_->Node2D_->SetSweeping(IsSweeping());
}

void WallODL::UpdatePicking()
{
	ImpUPtr_->Node2D_->SetPicking(IsPicking());
}
