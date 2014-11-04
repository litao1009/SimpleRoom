#include "stdafx.h"

#include "CornerMeshNode2D.h"

#include "irrlicht.h"

#include "IrrEngine/IrrEngine.h"

using namespace irr;
using namespace video;
using namespace core;

class	CornerMeshNode2D::Imp
{
public:

	static	float	Radius_;
};

float	CornerMeshNode2D::Imp::Radius_ = 50.f;

CornerMeshNode2D::CornerMeshNode2D(irr::scene::ISceneNode* parent, irr::s32 id):IMeshSceneNode(parent, parent->getSceneManager(), id)
{
	MeshBuffer_ = new irr::scene::SMeshBuffer;

	irr::core::vector3df dn(0,1,0);
	irr::video::SColor dc(0xFFF0F0F0);
	irr::video::SColor dbc(0xFF8F8F8F);
	irr::core::vector2df dt(0,0);
	irr::core::vector3df dv(0);

	MeshBuffer_->Vertices.reallocate(4);
	MeshBuffer_->Indices.reallocate(4);

	MeshBuffer_->Vertices.push_back(S3DVertex(vector3df(-1, 0, -1), vector3df(0,1,0), SColor(~0), vector2df(0,0)));
	MeshBuffer_->Vertices.push_back(S3DVertex(vector3df(1, 0, -1), vector3df(0,1,0), SColor(~0), vector2df(1,0)));
	MeshBuffer_->Vertices.push_back(S3DVertex(vector3df(1, 0, 1), vector3df(0,1,0), SColor(~0), vector2df(1,1)));
	MeshBuffer_->Vertices.push_back(S3DVertex(vector3df(-1, 0, 1), vector3df(0,1,0), SColor(~0), vector2df(0,1)));

	MeshBuffer_->Indices.push_back(0);
	MeshBuffer_->Indices.push_back(1);
	MeshBuffer_->Indices.push_back(2);
	MeshBuffer_->Indices.push_back(3);

	setScale(vector3df(Imp::Radius_, 1, Imp::Radius_));
	MeshBuffer_->getMaterial().setTexture(0, SceneManager->getVideoDriver()->getTexture("../Data/Resource/3D/dot.png"));

	MeshBuffer_->getMaterial().Lighting = false;
	MeshBuffer_->getMaterial().BackfaceCulling = false;
	MeshBuffer_->getMaterial().ZWriteEnable = false;
	MeshBuffer_->getMaterial().MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
}

CornerMeshNode2D::~CornerMeshNode2D()
{
	if ( MeshBuffer_ )
	{
		MeshBuffer_->drop();
	}
}

void CornerMeshNode2D::OnRegisterSceneNode()
{
	if ( isVisible() )
	{
		SceneManager->registerNodeForRendering(this, irr::scene::ESNRP_TRANSPARENT);
	}

	ISceneNode::OnRegisterSceneNode();
}

void CornerMeshNode2D::render()
{
	auto driver = SceneManager->getVideoDriver();

	setScale(vector3df(Imp::Radius_, 1, Imp::Radius_));
	
	AbsoluteTransformation = getAbsoluteTransformation();

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

	driver->setMaterial(MeshBuffer_->getMaterial());
	driver->drawMeshBuffer(MeshBuffer_);
	driver->drawVertexPrimitiveList(MeshBuffer_->getVertices(), MeshBuffer_->getVertexCount(), MeshBuffer_->getIndices(), MeshBuffer_->getIndexCount()/2, EVT_STANDARD, irr::scene::EPT_TRIANGLE_FAN);
}

irr::video::SMaterial& CornerMeshNode2D::getMaterial( irr::u32 i )
{
	return MeshBuffer_->getMaterial();
}

irr::u32 CornerMeshNode2D::getMaterialCount() const
{
	return 1;
}

const irr::core::aabbox3df& CornerMeshNode2D::getBoundingBox() const
{
	return MeshBuffer_->getBoundingBox();
}

void CornerMeshNode2D::SetRadius( float scale )
{
	Imp::Radius_ = scale;
}
