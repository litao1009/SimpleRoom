#include "stdafx.h"

#include "PillarMeshNode2D.h"

#include "irrlicht.h"

#include "TopExp.hxx"
#include "TopExp_Explorer.hxx"
#include "TopoDS.hxx"
#include "TopoDS_Vertex.hxx"
#include "TopoDS_Edge.hxx"
#include "BRep_Tool.hxx"

#include "IrrEngine/IrrEngine.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

PillarMeshNode2D::PillarMeshNode2D(irr::scene::ISceneNode* parent, irr::s32 id):IMeshSceneNode(parent, parent->getSceneManager(), id)
{
	MeshBuffer_ = new irr::scene::SMeshBuffer;

	irr::core::vector3df dn(0,1,0);
	irr::video::SColor dc(0xFFF0F0F0);
	irr::video::SColor dbc(0xFF8F8F8F);
	irr::core::vector2df dt(0,0);
	irr::core::vector3df dv(0);

	MeshBuffer_->Vertices.reallocate(4);
	MeshBuffer_->Vertices.push_back(irr::video::S3DVertex(dv, dn, dc, irr::core::vector2df(0,0)));
	MeshBuffer_->Vertices.push_back(irr::video::S3DVertex(dv, dn, dc, irr::core::vector2df(1,0)));
	MeshBuffer_->Vertices.push_back(irr::video::S3DVertex(dv, dn, dc, irr::core::vector2df(1,1)));
	MeshBuffer_->Vertices.push_back(irr::video::S3DVertex(dv, dn, dc, irr::core::vector2df(0,1)));

	MeshBuffer_->Indices.reallocate(4);
	MeshBuffer_->Indices.push_back(0);
	MeshBuffer_->Indices.push_back(1);
	MeshBuffer_->Indices.push_back(2);
	MeshBuffer_->Indices.push_back(3);

	MeshBuffer_->recalculateBoundingBox();

	LineMaterial_.Lighting = false;
	LineMaterial_.MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_LINE);
	LineMaterial_.DiffuseColor = dbc;
	LineMaterial_.Thickness = 2;
	LineMaterial_.PolygonOffsetDirection = irr::video::EPO_FRONT;
	LineMaterial_.PolygonOffsetFactor = 7;
	LineMaterial_.Wireframe = true;

	MeshBuffer_->getMaterial().Lighting = false;
	MeshBuffer_->getMaterial().BackfaceCulling = false;
	MeshBuffer_->getMaterial().PolygonOffsetDirection = irr::video::EPO_FRONT;
	MeshBuffer_->getMaterial().PolygonOffsetFactor = 7;
	MeshBuffer_->getMaterial().setTexture(0, SceneManager->getVideoDriver()->getTexture("../Data/Resource/3D/wallLine.png"));
	matrix4 scale,rotate;
	scale.setTextureScale(1/300.f, 1/300.f);
	rotate.setTextureRotationCenter(static_cast<float>(M_PI_4));
	MeshBuffer_->getMaterial().setTextureMatrix(0, rotate*scale);
}

PillarMeshNode2D::~PillarMeshNode2D()
{
	if ( MeshBuffer_ )
	{
		MeshBuffer_->drop();
	}
}

void PillarMeshNode2D::OnRegisterSceneNode()
{
	if ( isVisible() )
	{
		SceneManager->registerNodeForRendering(this, irr::scene::ESNRP_SOLID);
	}

	ISceneNode::OnRegisterSceneNode();
}

void PillarMeshNode2D::render()
{

	auto driver = SceneManager->getVideoDriver();

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

	driver->setMaterial(MeshBuffer_->getMaterial());
	driver->drawMeshBuffer(MeshBuffer_);
	driver->drawVertexPrimitiveList(MeshBuffer_->getVertices(), MeshBuffer_->getVertexCount(), MeshBuffer_->getIndices(), MeshBuffer_->getIndexCount()/2, EVT_STANDARD, irr::scene::EPT_TRIANGLE_FAN);

	matrix4 mat;
	mat.setTranslation(vector3df(0, 10, 0));
	driver->setTransform(video::ETS_WORLD, mat * AbsoluteTransformation);
	driver->setMaterial(LineMaterial_);
	driver->drawVertexPrimitiveList(MeshBuffer_->getVertices(), MeshBuffer_->getVertexCount(), MeshBuffer_->getIndices(), MeshBuffer_->getIndexCount(), EVT_STANDARD, EPT_LINE_LOOP);
}

irr::video::SMaterial& PillarMeshNode2D::getMaterial( irr::u32 i )
{
	return MeshBuffer_->getMaterial();
}

irr::u32 PillarMeshNode2D::getMaterialCount() const
{
	return 1;
}

const irr::core::aabbox3df& PillarMeshNode2D::getBoundingBox() const
{
	return MeshBuffer_->getBoundingBox();
}

void PillarMeshNode2D::UpdateMesh( float xLength, float zLength )
{
	auto vertics = static_cast<S3DVertex*>(MeshBuffer_->getVertices());
	vertics[0].Pos = vector3df(-xLength/2, 0, zLength/2);
	vertics[0].TCoords = vector2df(-xLength/2, zLength/2);
	vertics[1].Pos = vector3df(xLength/2, 0, zLength/2);
	vertics[1].TCoords = vector2df(xLength/2, zLength/2);
	vertics[2].Pos = vector3df(xLength/2, 0, -zLength/2);
	vertics[2].TCoords = vector2df(xLength/2, -zLength/2);
	vertics[3].Pos = vector3df(-xLength/2, 0, -zLength/2);
	vertics[3].TCoords = vector2df(-xLength/2, -zLength/2);

	MeshBuffer_->recalculateBoundingBox();
}

void PillarMeshNode2D::SetLineColor( const irr::video::SColor& lineColor )
{
	LineMaterial_.DiffuseColor = lineColor;
}
