#include "stdafx.h"

#include "WallODL.h"
#include "WallFaceODL.h"

#include "BRepPrimAPI_MakePrism.hxx"
#include "BRepBuilderAPI_MakePolygon.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "BRepBuilderAPI_MakeWire.hxx"
#include "BRepOffsetAPI_MakePipeShell.hxx"
#include "TopExp_Explorer.hxx"
#include "TopoDS.hxx"
#include "BRepBndLib.hxx"


#include "IVideoDriver.h"
#include "ISceneManager.h"

#include "IrrEngine/IrrEngine.h"

CWallODL::CWallODL()
{
	
}


CWallODL::~CWallODL(void)
{
}

CWallODL::ChildrenList CWallODL::CreateWallByBottomFace( SRenderContextWPtr renderContext, const TopoDS_Shape& bottomFace, const PointList& pntList, float wallHeight, const gp_Pnt& center )
{
	static irr::core::vector3df s_YDir(0,1,0);

	TopoDS_Shape transformedFace;
	{
		gp_Trsf t;
		t.SetTranslationPart(center.XYZ().Reversed());
		transformedFace = bottomFace.Moved(t);
	}

	ChildrenList ret;

	BRepPrimAPI_MakePrism mp(transformedFace, gp_Vec(0,wallHeight, 0));
	assert(mp.IsDone());

	auto wallCompSolids = mp.Shape();

	auto wallIndex = 0;
	for ( TopExp_Explorer exp(wallCompSolids, TopAbs_SOLID); exp.More(); exp.Next(),++wallIndex )
	{
		auto newWall = CWallODL::Create<CWallODL>(renderContext);
		auto curSolid = exp.Current();
		Bnd_Box wallBox;
		BRepBndLib::Add(curSolid, wallBox);
		gp_Pnt wallCenter;
		{
			Standard_Real xMin,yMin,zMin,xMax,yMax,zMax;
			wallBox.Get(xMin, yMin, zMin, xMax, yMax, zMax);
			wallCenter.SetXYZ(gp_XYZ((xMin+xMax)/2, (yMin+yMax)/2, (zMin+zMax)/2));
		}

		auto& pnt0 = pntList[wallIndex];
		auto& pnt1 = pntList[wallIndex+1];
		
		auto drawDir = (pnt1-pnt0).normalize();
		auto wallDir = drawDir.crossProduct(s_YDir);

		auto mesh = ODLTools::CreateMesh(curSolid, gp_Dir(wallDir.X, wallDir.Y, wallDir.Z), true);
		assert(mesh.RawMesh_);
		assert(!mesh.Faces_.empty());

		gp_Quaternion rotation;
		rotation.SetEulerAngles(gp_Extrinsic_XYZ, irr::core::degToRad(mesh.RawMesh_->Rotation_.X), irr::core::degToRad(mesh.RawMesh_->Rotation_.Y), irr::core::degToRad(mesh.RawMesh_->Rotation_.Z));
		gp_XYZ translation(mesh.RawMesh_->Translation_.X, mesh.RawMesh_->Translation_.Y, mesh.RawMesh_->Translation_.Z);
		{
			gp_Trsf t,r;
			t.SetTranslationPart(translation);
			r.SetRotation(rotation);
			curSolid.Move((t*r).Inverted());
		}
		newWall->SetBaseShape(curSolid);

		newWall->GetDataSceneNode()->setMesh(mesh.RawMesh_->Mesh_);
		newWall->GetDataSceneNode()->setPosition(mesh.RawMesh_->Translation_);
		newWall->SetTranslation(translation);
		newWall->GetDataSceneNode()->setRotation(mesh.RawMesh_->Rotation_);
		newWall->SetRoration(rotation);

		newWall->GetDataSceneNode()->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		newWall->GetDataSceneNode()->AddToDepthPass();
		newWall->SetDefaultTexture();

		for ( auto& curFace : mesh.Faces_ )
		{
			assert(curFace);
			auto& cufFaceValue = *curFace;

			auto newFace = newWall->CreateChild<CWallFaceODL>(renderContext);

			newFace->SetBaseShape(cufFaceValue.Shape_);

			newFace->GetDataSceneNode()->setMesh(cufFaceValue.Mesh_);
			newFace->GetDataSceneNode()->setPosition(cufFaceValue.Translation_);
			newFace->GetDataSceneNode()->setRotation(cufFaceValue.Rotation_);

			newFace->GetDataSceneNode()->setMaterialFlag(irr::video::EMF_LIGHTING, false);
			newFace->GetDataSceneNode()->setVisible(false);

			cufFaceValue.Mesh_->drop();
		}

		mesh.RawMesh_->Mesh_->drop();

		ret.push_back(newWall);
	}

	return ret;
}


CWallODL::ChildrenList CWallODL::CreateWallByRectRange( SRenderContextWPtr renderContext, const irr::core::vector3df& first, const irr::core::vector3df& last, const irr::core::vector3df& thickVec, float wallHeight )
{
	auto vec = last - first;

	auto p1 = first;
	auto p2 = first + irr::core::vector3df(vec.X, 0, 0);
	auto p3 = last;
	auto p4 = first + irr::core::vector3df(0, 0, vec.Z);

	BRepBuilderAPI_MakePolygon mp;
	mp.Add(gp_Pnt(p1.X, p1.Y, p1.Z));
	mp.Add(gp_Pnt(p2.X, p2.Y, p2.Z));
	mp.Add(gp_Pnt(p3.X, p3.Y, p3.Z));
	mp.Add(gp_Pnt(p4.X, p4.Y, p4.Z));
	mp.Close();
	assert(mp.IsDone());

	auto thickEdge = BRepBuilderAPI_MakeEdge(gp_Pnt(first.X,first.Y, first.Z), gp_Pnt(first.X+thickVec.X,first.Y+thickVec.Y, first.Z+thickVec.Z) ).Edge();
	BRepBuilderAPI_MakeWire dirWireMaker(thickEdge);
	TopoDS_Wire dirWire = dirWireMaker.Wire();

	BRepOffsetAPI_MakePipeShell pipeMaker(mp.Wire());
	pipeMaker.SetTransitionMode(BRepBuilderAPI_RightCorner);	//延切线方向缝合
	pipeMaker.Add(dirWire);
	pipeMaker.Build();

	assert(pipeMaker.IsDone());

	auto bottomFace = pipeMaker.Shape();

	PointList pntList;
	pntList.push_back(p1);
	pntList.push_back(p2);
	pntList.push_back(p3);
	pntList.push_back(p4);
	pntList.push_back(p1);

	auto p5 = (first+last)/2;
	p5.Y = wallHeight/2;

	return CreateWallByBottomFace(renderContext, bottomFace, pntList, wallHeight, gp_Pnt(p5.X, p5.Y, p5.Z));
}


void CWallODL::SetDefaultTexture()
{
	auto tex = GetDataSceneNode()->getSceneManager()->getVideoDriver()->getTexture("../Data/Resource/3D/wall.jpg");
	assert(tex);

	auto uLen = 4000.f;//tex->getSize().Width;
	auto vLen = 3000.f;//tex->getSize().Height;

	GetDataSceneNode()->setMaterialTexture(0, tex);
	irr::core::matrix4 texMat;
	texMat.setScale(irr::core::vector3df(1/uLen, 1/vLen, 1));
	auto meshBufferPtr = GetDataSceneNode()->getMesh()->getMeshBuffer(0);
	meshBufferPtr->getMaterial().setTextureMatrix(0, texMat);
}

void CWallODL::UpdateSweeping()
{
	if ( IsSwept() )
	{
		GetDataSceneNode()->GetSceneNode3D()->SaveMaterialType();
		GetDataSceneNode()->GetSceneNode3D()->SetMaterialType(IrrEngine::GetInstance()->GetShaderType(EST_SELECTION));
	}
	else
	{
		GetDataSceneNode()->GetSceneNode3D()->ResetMaterialType();
	}
}
