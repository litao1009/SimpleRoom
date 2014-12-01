#include "stdafx.h"

#include "WallMeshNode2D.h"

#include "irrlicht.h"

#include "TopExp.hxx"
#include "TopExp_Explorer.hxx"
#include "TopoDS.hxx"
#include "TopoDS_Vertex.hxx"
#include "TopoDS_Edge.hxx"
#include "BRep_Tool.hxx"

#include "ODL/ODLTools.h"
#include "ODL/WallODL.h"
#include "irrEngine/irrEngine.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

WallMeshNode2D::WallMeshNode2D(irr::scene::ISceneNode* parent, irr::s32 id):IMeshSceneNode(parent, parent->getSceneManager(), id)
{
	FaceBuffer_ = new SMeshBuffer;

	FaceBuffer_->Vertices.reallocate(6);
	FaceBuffer_->Vertices.push_back(S3DVertex(vector3df(0), vector3df(0,1,0), SColor(~0), vector2df(0,0)));
	FaceBuffer_->Vertices.push_back(S3DVertex(vector3df(0), vector3df(0,1,0), SColor(~0), vector2df(0,0)));
	FaceBuffer_->Vertices.push_back(S3DVertex(vector3df(0), vector3df(0,1,0), SColor(~0), vector2df(0,0)));
	FaceBuffer_->Vertices.push_back(S3DVertex(vector3df(0), vector3df(0,1,0), SColor(~0), vector2df(0,0)));
	FaceBuffer_->Vertices.push_back(S3DVertex(vector3df(0), vector3df(0,1,0), SColor(~0), vector2df(0,0)));
	FaceBuffer_->Vertices.push_back(S3DVertex(vector3df(0), vector3df(0,1,0), SColor(~0), vector2df(0,0)));

	FaceBuffer_->Indices.reallocate(6);
	FaceBuffer_->Indices.push_back(0);
	FaceBuffer_->Indices.push_back(1);
	FaceBuffer_->Indices.push_back(2);
	FaceBuffer_->Indices.push_back(3);
	FaceBuffer_->Indices.push_back(4);
	FaceBuffer_->Indices.push_back(5);

	FaceBuffer_->getMaterial().Lighting = false;
	FaceBuffer_->getMaterial().BackfaceCulling = false;
	FaceBuffer_->getMaterial().setTexture(0, SceneManager->getVideoDriver()->getTexture("../Data/Resource/3D/wallLine.png"));
	{
		matrix4 scale,rotate;
		scale.setTextureScale(1/300.f, 1/300.f);
		rotate.setTextureRotationCenter(static_cast<float>(M_PI_4));
		FaceBuffer_->getMaterial().setTextureMatrix(0, rotate*scale);
	}

	LineMaterial_.Lighting = false;
	LineMaterial_.BackfaceCulling = false;
	LineMaterial_.Thickness = 2;
	LineMaterial_.Wireframe = true;
	LineMaterial_.MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_LINE);
	LineMaterial_.DiffuseColor = 0xFF8F8F8F;
	//LineMaterial_.AntiAliasing = irr::video::EAAM_LINE_SMOOTH;
}

WallMeshNode2D::~WallMeshNode2D()
{
	FaceBuffer_->drop();
}

void WallMeshNode2D::OnRegisterSceneNode()
{
	if ( isVisible() )
	{
		SceneManager->registerNodeForRendering(this, irr::scene::ESNRP_SOLID);
	}

	ISceneNode::OnRegisterSceneNode();
}

void WallMeshNode2D::render()
{
	assert(FaceBuffer_);

	auto driver = SceneManager->getVideoDriver();

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

	driver->setMaterial(FaceBuffer_->getMaterial());
	driver->drawVertexPrimitiveList(FaceBuffer_->getVertices(), FaceBuffer_->getVertexCount(), FaceBuffer_->getIndices(), FaceBuffer_->getIndexCount()-2, EVT_STANDARD, EPT_TRIANGLE_FAN);

	driver->setMaterial(LineMaterial_);
	driver->drawVertexPrimitiveList(FaceBuffer_->getVertices(), FaceBuffer_->getVertexCount(), FaceBuffer_->getIndices(), FaceBuffer_->getIndexCount(), EVT_STANDARD, EPT_LINE_LOOP);
}

irr::video::SMaterial& WallMeshNode2D::getMaterial( irr::u32 i )
{
	return FaceBuffer_->getMaterial();
}

irr::u32 WallMeshNode2D::getMaterialCount() const
{
	return 1;
}

const irr::core::aabbox3df& WallMeshNode2D::getBoundingBox() const
{
	return FaceBuffer_->getBoundingBox();
}

void WallMeshNode2D::UpdateMesh( const WallODLSPtr& wall )
{
	wall->GetDataSceneNode()->updateAbsolutePosition();
	auto wallMat = wall->GetDataSceneNode()->getAbsoluteTransformation();
	wallMat.makeInverse();

	auto index = 0;
	for ( auto& curPnt : wall->GetMeshPoints() )
	{
		vector3df pos(static_cast<float>(curPnt.X()), static_cast<float>(curPnt.Y())+200.f, static_cast<float>(curPnt.Z()));
		wallMat.transformVect(pos);
		FaceBuffer_->Vertices[index].Pos = pos;
		FaceBuffer_->Vertices[index].TCoords = vector2df(pos.X, pos.Z);
		++index;
	}
}

void WallMeshNode2D::SetSweeping( bool val )
{
	if ( val )
	{
		FaceBuffer_->getMaterial().MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_PICKING);
		FaceBuffer_->getMaterial().DiffuseColor = 0xFFFFFF00;
	}
	else
	{
		FaceBuffer_->getMaterial().MaterialType = EMT_SOLID;
	}
}

void WallMeshNode2D::SetPicking( bool val )
{
	if ( val )
	{
		FaceBuffer_->getMaterial().MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_PICKING);
		FaceBuffer_->getMaterial().DiffuseColor = 0xFF00FFFF;
	}
	else
	{
		FaceBuffer_->getMaterial().MaterialType = EMT_SOLID;
	}
}
