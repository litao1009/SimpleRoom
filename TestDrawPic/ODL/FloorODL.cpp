#include "stdafx.h"

#include "FloorODL.h"

#include "BRepBuilderAPI_MakePolygon.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "BRepAlgoAPI_Cut.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "BRepBuilderAPI_MakeWire.hxx"
#include "BRepOffsetAPI_MakePipeShell.hxx"
#include "TopoDS_Wire.hxx"

#include "IVideoDriver.h"
#include "ISceneManager.h"

#include "irrEngine/irrEngine.h"

CFloorODL::CFloorODL()
{

}

CFloorODL::~CFloorODL( void )
{

}

CFloorODLSPtr CFloorODL::CreateFloorByPath( SRenderContextWPtr renderContext, const TopoDS_Shape& bottomFace, const PointList& pntList, const gp_Pnt& center )
{
	gp_Trsf t;
	t.SetTranslationPart(center.XYZ().Reversed());

	BRepBuilderAPI_MakePolygon mp;
	for ( auto& curPnt : pntList )
	{
		mp.Add(gp_Pnt(curPnt.X, curPnt.Y, curPnt.Z));
	}
	mp.Close();
	assert(mp.IsDone());

	auto floorFace = BRepBuilderAPI_MakeFace(mp.Wire()).Shape();

 	BRepAlgoAPI_Cut bc(floorFace, bottomFace);
 	floorFace = bc.Shape();
	floorFace.Move(t);

	auto mesh = ODLTools::CreateMesh(floorFace, boost::none);
	assert(mesh.RawMesh_);

	auto newFloor = CFloorODL::Create<CFloorODL>(renderContext);

	gp_Quaternion rotation;
	gp_XYZ translation(mesh.RawMesh_->Translation_.X, mesh.RawMesh_->Translation_.Y, mesh.RawMesh_->Translation_.Z);
	{
		rotation.SetEulerAngles(gp_Extrinsic_XYZ, irr::core::degToRad(mesh.RawMesh_->Rotation_.X), irr::core::degToRad(mesh.RawMesh_->Rotation_.Y), irr::core::degToRad(mesh.RawMesh_->Rotation_.Z));
		
		gp_Trsf t,r;
		t.SetTranslationPart(translation);
		r.SetRotation(rotation);
		floorFace.Move((t*r).Inverted());
	}

	newFloor->SetBaseShape(floorFace);

	newFloor->GetDataSceneNode()->setMesh(mesh.RawMesh_->Mesh_);
	newFloor->GetDataSceneNode()->setPosition(mesh.RawMesh_->Translation_);
	newFloor->SetTranslation(gp_XYZ(mesh.RawMesh_->Translation_.X, mesh.RawMesh_->Translation_.Y, mesh.RawMesh_->Translation_.Z));
	newFloor->GetDataSceneNode()->setRotation(mesh.RawMesh_->Rotation_);
	newFloor->SetRoration(rotation);

	newFloor->GetDataSceneNode()->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	newFloor->GetDataSceneNode()->getMaterial(0).PolygonOffsetDirection = irr::video::EPO_FRONT;
	newFloor->GetDataSceneNode()->getMaterial(0).PolygonOffsetFactor = 1;
	newFloor->GetDataSceneNode()->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
	newFloor->GetDataSceneNode()->AddToDepthPass();
	newFloor->SetDefaultTexture();

	mesh.RawMesh_->Mesh_->drop();

	return newFloor;
}

void CFloorODL::SetDefaultTexture()
{
	auto tex = GetDataSceneNode()->getSceneManager()->getVideoDriver()->getTexture("../Data/Resource/3D/floor.jpg");
	assert(tex);

	auto uLen = tex->getSize().Width;
	auto vLen = tex->getSize().Height;

	GetDataSceneNode()->setMaterialTexture(0, tex);
	irr::core::matrix4 texMat;
	texMat.setScale(irr::core::vector3df(1/static_cast<float>(uLen), 1/static_cast<float>(vLen), 1));
	auto meshBufferPtr = GetDataSceneNode()->getMesh()->getMeshBuffer(0);
	meshBufferPtr->getMaterial().setTextureMatrix(0, texMat);
}

void CFloorODL::UpdateSweeping()
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

CFloorODLSPtr CFloorODL::CreateFloorByRect( SRenderContextWPtr renderContext, const irr::core::vector3df& first, const irr::core::vector3df& last, const irr::core::vector3df& thickVec, const gp_Pnt& center )
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

	return CreateFloorByPath(renderContext, bottomFace, pntList, center);
}
