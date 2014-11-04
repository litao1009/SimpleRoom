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
#include "SMesh.h"

#include "irrEngine/irrEngine.h"

FloorODL::FloorODL(const SRenderContextWPtr& rc):CBaseODL(rc)
{

}

FloorODL::~FloorODL( void )
{

}

void FloorODL::SetDefaultTexture()
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

FloorODLSPtr FloorODL::CreateByFace( SRenderContextWPtr renderContext, const TopoDS_Shape& floorFace )
{
	auto newFloor = FloorODL::Create<FloorODL>(renderContext);

	newFloor->SetBaseShape(floorFace);

	{//3D
		auto meshBuf = ODLTools::NEW_CreateMeshBuffer(floorFace);
		assert(meshBuf);

		auto mesh = new irr::scene::SMesh;
		mesh->addMeshBuffer(meshBuf);
		newFloor->GetDataSceneNode()->setMesh(mesh);
		mesh->recalculateBoundingBox();
		meshBuf->drop();
		mesh->drop();

		newFloor->GetDataSceneNode()->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
		newFloor->GetDataSceneNode()->getMaterial(0).PolygonOffsetDirection = irr::video::EPO_FRONT;
		newFloor->GetDataSceneNode()->getMaterial(0).PolygonOffsetFactor = 1;
		newFloor->GetDataSceneNode()->AddToDepthPass();
		newFloor->SetDefaultTexture();
	}

	{//2D
		auto meshBuf = ODLTools::NEW_CreateMeshBuffer(floorFace);
		assert(meshBuf);

		auto smgr = newFloor->GetDataSceneNode()->getSceneManager();

		meshBuf->getMaterial().setTexture(0, smgr->getVideoDriver()->getTexture("../Data/Resource/3D/floorLine.png"));
		meshBuf->getMaterial().BackfaceCulling = false;
		irr::core::matrix4 r,s;
		s.setTextureScale(1/200.f, 1/200.f);
		r.setTextureRotationCenter(static_cast<float>(M_PI_2));
		meshBuf->getMaterial().setTextureMatrix(0, r*s);

		auto mesh = new irr::scene::SMesh;
		mesh->addMeshBuffer(meshBuf);
		mesh->recalculateBoundingBox();
		
		auto cur2DNode = newFloor->GetDataSceneNode()->GetSceneNode2D();
		cur2DNode->getSceneManager()->addMeshSceneNode(mesh, cur2DNode);

		meshBuf->drop();
		mesh->drop();
	}

	return newFloor;
}
