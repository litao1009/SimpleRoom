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
#include "irrEngine/irrEngine.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

WallMeshNode2D::WallMeshNode2D(irr::scene::ISceneNode* parent, irr::s32 id):IMeshSceneNode(parent, parent->getSceneManager(), id)
{
	FaceBuffer_ = nullptr;
	LineBuffer_ = nullptr;
}

WallMeshNode2D::~WallMeshNode2D()
{
	if ( FaceBuffer_ )
	{
		FaceBuffer_->drop();

		assert(LineBuffer_);
		LineBuffer_->drop();
		LineBuffer_ = nullptr;
	}

	assert(!LineBuffer_);
}

void WallMeshNode2D::OnRegisterSceneNode()
{
	if ( isVisible() )
	{
		assert(FaceBuffer_ && LineBuffer_);

		SceneManager->registerNodeForRendering(this, irr::scene::ESNRP_SOLID);
	}

	ISceneNode::OnRegisterSceneNode();
}

void WallMeshNode2D::render()
{
	assert(FaceBuffer_ && LineBuffer_);

	auto driver = SceneManager->getVideoDriver();

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

	driver->setMaterial(FaceBuffer_->getMaterial());
	driver->drawMeshBuffer(FaceBuffer_);

	driver->setMaterial(LineBuffer_->getMaterial());
	driver->drawVertexPrimitiveList(LineBuffer_->getVertices(), LineBuffer_->getVertexCount(), LineBuffer_->getIndices(), LineBuffer_->getIndexCount()/2, EVT_STANDARD, EPT_LINES);
}

irr::video::SMaterial& WallMeshNode2D::getMaterial( irr::u32 i )
{
	if ( FaceBuffer_ )
	{
		return FaceBuffer_->getMaterial();
	}

	return ISceneNode::getMaterial(i);
}

irr::u32 WallMeshNode2D::getMaterialCount() const
{
	if ( FaceBuffer_ )
	{
		return 1;
	}
	else
	{
		return ISceneNode::getMaterialCount();
	}
}

const irr::core::aabbox3df& WallMeshNode2D::getBoundingBox() const
{
	if ( FaceBuffer_ )
	{
		return FaceBuffer_->getBoundingBox();
	}
	
	static	aabbox3df box;
	return box;
}

void WallMeshNode2D::UpdateMesh( const TopoDS_Shape& wallBottomFace )
{
	if ( FaceBuffer_ )
	{
		FaceBuffer_->drop();
	}
	FaceBuffer_ = ODLTools::NEW_CreateMeshBuffer(wallBottomFace);

	if ( LineBuffer_ )
	{
		LineBuffer_->drop();
	}
	LineBuffer_ = new irr::scene::SMeshBuffer;

	for ( TopExp_Explorer expEdge(wallBottomFace, TopAbs_EDGE); expEdge.More(); expEdge.Next() )
	{
		auto& curEdge = TopoDS::Edge(expEdge.Current());
		auto& firstVertex = TopExp::FirstVertex(curEdge);
		auto& secondVertex = TopExp::LastVertex(curEdge);
		auto firstPnt = BRep_Tool::Pnt(firstVertex);
		auto secondPnt = BRep_Tool::Pnt(secondVertex);

		static vector3df lineNormal(0,1,0);
		static SColor lineColor(0xFF8F8F8F);
		static vector2df lineCoord(0,0);

		S3DVertex sv1(vector3df(static_cast<float>(firstPnt.X()), static_cast<float>(firstPnt.Y()), static_cast<float>(firstPnt.Z())), lineNormal, lineColor, lineCoord);
		S3DVertex sv2(vector3df(static_cast<float>(secondPnt.X()), static_cast<float>(secondPnt.Y()), static_cast<float>(secondPnt.Z())), lineNormal, lineColor, lineCoord);

		auto curIndex = LineBuffer_->Indices.size();
		LineBuffer_->Vertices.push_back(sv1);
		LineBuffer_->Vertices.push_back(sv2);
		LineBuffer_->Indices.push_back(curIndex++);
		LineBuffer_->Indices.push_back(curIndex++);
	}

	FaceBuffer_->getMaterial().Lighting = false;
	FaceBuffer_->getMaterial().BackfaceCulling = false;
	FaceBuffer_->getMaterial().setTexture(0, SceneManager->getVideoDriver()->getTexture("../Data/Resource/3D/wallLine.png"));
	{
		matrix4 scale,rotate;
		scale.setTextureScale(1/300.f, 1/300.f);
		rotate.setTextureRotationCenter(static_cast<float>(M_PI_4));
		FaceBuffer_->getMaterial().setTextureMatrix(0, rotate*scale);
	}

	LineBuffer_->getMaterial().Lighting = false;
	LineBuffer_->getMaterial().BackfaceCulling = false;
	LineBuffer_->getMaterial().Thickness = 2;
	LineBuffer_->getMaterial().Wireframe = true;
	//LineBuffer_->getMaterial().AntiAliasing = irr::video::EAAM_LINE_SMOOTH;
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
