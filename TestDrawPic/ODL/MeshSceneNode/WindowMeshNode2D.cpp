#include "stdafx.h"

#include "WindowMeshNode2D.h"

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

WindowMeshNode2D::WindowMeshNode2D(irr::scene::ISceneNode* parent, irr::s32 id):IMeshSceneNode(parent, parent->getSceneManager(), id)
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
	MeshBuffer_->getMaterial().setTexture(0, SceneManager->getVideoDriver()->getTexture("../Data/Resource/3D/floorLine.png"));
}

WindowMeshNode2D::~WindowMeshNode2D()
{
	if ( MeshBuffer_ )
	{
		MeshBuffer_->drop();
	}
}

void WindowMeshNode2D::OnRegisterSceneNode()
{
	if ( isVisible() )
	{
		SceneManager->registerNodeForRendering(this, irr::scene::ESNRP_SOLID);
	}

	ISceneNode::OnRegisterSceneNode();
}

void WindowMeshNode2D::render()
{

	auto driver = SceneManager->getVideoDriver();

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

	driver->setMaterial(MeshBuffer_->getMaterial());
	driver->drawMeshBuffer(MeshBuffer_);
	driver->drawVertexPrimitiveList(MeshBuffer_->getVertices(), MeshBuffer_->getVertexCount(), MeshBuffer_->getIndices(), MeshBuffer_->getIndexCount()/2, EVT_STANDARD, irr::scene::EPT_TRIANGLE_FAN);

	driver->setMaterial(LineMaterial_);
	driver->drawVertexPrimitiveList(MeshBuffer_->getVertices(), MeshBuffer_->getVertexCount(), MeshBuffer_->getIndices(), MeshBuffer_->getIndexCount(), EVT_STANDARD, EPT_LINE_LOOP);
}

irr::video::SMaterial& WindowMeshNode2D::getMaterial( irr::u32 i )
{
	return MeshBuffer_->getMaterial();
}

irr::u32 WindowMeshNode2D::getMaterialCount() const
{
	return 1;
}

const irr::core::aabbox3df& WindowMeshNode2D::getBoundingBox() const
{
	return MeshBuffer_->getBoundingBox();
}

void WindowMeshNode2D::UpdateMesh( float xLength, float zLength )
{
	MeshBuffer_->getPosition(0) = irr::core::vector3df(-xLength/2, 0, zLength/2);
	MeshBuffer_->getPosition(1) = irr::core::vector3df(xLength/2, 0, zLength/2);
	MeshBuffer_->getPosition(2) = irr::core::vector3df(xLength/2, 0, -zLength/2);
	MeshBuffer_->getPosition(3) = irr::core::vector3df(-xLength/2, 0, -zLength/2);
	MeshBuffer_->recalculateBoundingBox();
}

void WindowMeshNode2D::SetLineColor( const irr::video::SColor& lineColor )
{
	LineMaterial_.DiffuseColor = lineColor;
}
