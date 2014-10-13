#include "stdafx.h"

#include "DoorMeshNode2D.h"
#include "irrlicht.h"

#include "Geom2d_Circle.hxx"
#include "Geom2dAdaptor_Curve.hxx"
#include "GCPnts_UniformAbscissa.hxx"

#include "irrEngine/irrEngine.h"

DoorMeshNode2D::DoorMeshNode2D( irr::scene::ISceneNode* parent, irr::s32 id /*= -1*/ ):IMeshSceneNode(parent, parent->getSceneManager(), id)
{
	irr::core::vector3df dn(0,1,0);
	irr::video::SColor dc(0xFFF0F0F0);
	irr::video::SColor dbc(0xFF8F8F8F);
	irr::core::vector2df dt(0,0);

	{
		RectBuffer_ = new irr::scene::SMeshBuffer;
		RectBuffer_->Vertices.reallocate(4);
		RectBuffer_->Indices.reallocate(4);

		irr::core::vector3df dv(0);

		RectBuffer_->Vertices.push_back(irr::video::S3DVertex(dv, dn, dc, irr::core::vector2df(0,0)));
		RectBuffer_->Vertices.push_back(irr::video::S3DVertex(dv, dn, dc, irr::core::vector2df(1,0)));
		RectBuffer_->Vertices.push_back(irr::video::S3DVertex(dv, dn, dc, irr::core::vector2df(1,1)));
		RectBuffer_->Vertices.push_back(irr::video::S3DVertex(dv, dn, dc, irr::core::vector2df(0,1)));
		RectBuffer_->Indices.push_back(0);
		RectBuffer_->Indices.push_back(1);
		RectBuffer_->Indices.push_back(2);
		RectBuffer_->Indices.push_back(3);
		RectBuffer_->recalculateBoundingBox();
		RectBuffer_->getMaterial().Lighting = false;
		RectBuffer_->getMaterial().BackfaceCulling = false;
		//RectBuffer_->getMaterial().ZBuffer = irr::video::ECFN_ALWAYS;
		RectBuffer_->getMaterial().PolygonOffsetDirection = irr::video::EPO_FRONT;
		RectBuffer_->getMaterial().PolygonOffsetFactor = 5;
	}

	{
		CircleBuffer_ = new irr::scene::SMeshBuffer;
		CircleBuffer_->Vertices.reallocate(91);
		CircleBuffer_->Indices.reallocate(91);

		Handle(Geom2d_Circle) C = new Geom2d_Circle(gp::OX2d(),1);
		Geom2dAdaptor_Curve GAC (C);
		GCPnts_UniformAbscissa UA (GAC,361);
		assert(UA.IsDone());

		gp_Pnt2d P;
		irr::video::SColor	 clr(0xFF8F8F8F);
		for( auto index=1, count=91; index<=count; ++index )
		{
			C->D0(UA.Parameter(index),P);

			irr::core::vector3df pos(static_cast<float>(P.X()), 0, static_cast<float>(-P.Y()));
			CircleBuffer_->Vertices.push_back(irr::video::S3DVertex(pos, dn, clr, dt));
			CircleBuffer_->Indices.push_back(index-1);
		}

		CircleBuffer_->Vertices.push_back(irr::video::S3DVertex(irr::core::vector3df(0), dn, clr, dt));
		CircleBuffer_->Indices.push_back(91);
		CircleBuffer_->recalculateBoundingBox();
		CircleBuffer_->getMaterial().ZBuffer = irr::video::ECFN_ALWAYS;
		CircleBuffer_->getMaterial().Lighting = false;
		CircleBuffer_->getMaterial().MaterialType = IrrEngine::GetInstance()->GetShaderType(EST_LINE);
		CircleBuffer_->getMaterial().DiffuseColor = dbc;
		//CircleBuffer_->getMaterial().AntiAliasing = irr::video::EAAM_LINE_SMOOTH;
		CircleBuffer_->getMaterial().Thickness = 2;
		CircleBuffer_->getMaterial().PolygonOffsetDirection = irr::video::EPO_FRONT;
		CircleBuffer_->getMaterial().PolygonOffsetFactor = 7;
		//CircleBuffer_->getMaterial().Wireframe = true;
		//CircleBuffer_->getMaterial().ZBuffer = irr::video::ECFN_ALWAYS;
	}

	DrawTriangle_ = true;
}

DoorMeshNode2D::~DoorMeshNode2D()
{
	RectBuffer_->drop();
	CircleBuffer_->drop();
}

void DoorMeshNode2D::OnRegisterSceneNode()
{
	if ( isVisible() )
	{
		SceneManager->registerNodeForRendering(this, irr::scene::ESNRP_SOLID);
	}

	ISceneNode::OnRegisterSceneNode();
}

void DoorMeshNode2D::render()
{
	auto driver = SceneManager->getVideoDriver();

	if ( DrawTriangle_ )
	{
		driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
		driver->setMaterial(RectBuffer_->getMaterial());
		driver->drawVertexPrimitiveList(RectBuffer_->getVertices(), RectBuffer_->getVertexCount(), RectBuffer_->getIndices(), RectBuffer_->getIndexCount()/2, irr::video::EVT_STANDARD, irr::scene::EPT_TRIANGLE_FAN);
	}
	
	driver->setMaterial(CircleBuffer_->getMaterial());
	driver->drawVertexPrimitiveList(RectBuffer_->getVertices(), RectBuffer_->getVertexCount(), RectBuffer_->getIndices(), RectBuffer_->getIndexCount(), irr::video::EVT_STANDARD, irr::scene::EPT_LINE_LOOP);

	driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation * CircleTransform_);
	driver->drawVertexPrimitiveList(CircleBuffer_->getVertices(), CircleBuffer_->getVertexCount(), CircleBuffer_->getIndices(), CircleBuffer_->getIndexCount()-1, irr::video::EVT_STANDARD, irr::scene::EPT_LINE_STRIP);
}

irr::video::SMaterial& DoorMeshNode2D::getMaterial( irr::u32 i )
{
	return RectBuffer_->getMaterial();
}

irr::u32 DoorMeshNode2D::getMaterialCount() const
{
	return 1;
}

const irr::core::aabbox3df& DoorMeshNode2D::getBoundingBox() const
{
	return RectBuffer_->getBoundingBox();
}

void DoorMeshNode2D::UpdateMesh( float xLength, float zLength )
{
	RectBuffer_->getPosition(0) = irr::core::vector3df(-xLength/2, 0, zLength/2);
	RectBuffer_->getPosition(1) = irr::core::vector3df(xLength/2, 0, zLength/2);
	RectBuffer_->getPosition(2) = irr::core::vector3df(xLength/2, 0, -zLength/2);
	RectBuffer_->getPosition(3) = irr::core::vector3df(-xLength/2, 0, -zLength/2);
	RectBuffer_->recalculateBoundingBox();

	irr::core::matrix4 t,s;
	t.setTranslation(irr::core::vector3df(-xLength/2, 0, zLength/2));
	s.setScale(irr::core::vector3df(xLength, 1, xLength));
	CircleTransform_ = t * s;
}

void DoorMeshNode2D::SetLineColor( const irr::video::SColor& lineColor )
{
	CircleBuffer_->getMaterial().DiffuseColor = lineColor;
}
