#include "stdafx.h"

#include "DrawingRectWallCtrller.h"
#include "IrrEngine/SRenderContext.h"

#include "ODL/ODLTools.h"

#include "SMeshBuffer.h"

#include "BRepOffsetAPI_MakePipeShell.hxx"
#include "BRepBuilderAPI_MakePolygon.hxx"
#include "gp_Pnt.hxx"
#include "TopoDS_Wire.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "BRepBuilderAPI_MakeWire.hxx"
#include "TopExp_Explorer.hxx"
#include "TopExp.hxx"
#include "TopoDS.hxx"
#include "BRep_Tool.hxx"

#include "StatusMgr.h"

static	irr::core::vector3df s_PntNormal(0,1,0);
static	irr::core::vector2df s_PntCoord(0,0);
static	irr::core::plane3df	 s_PntPlane(0,0,0,0,1,0);
static	irr::video::SColor	 s_PntColor(~0);
static	irr::video::SColor	 s_LineColor(0xFF000000);
static	irr::video::SColor	 s_PathColor(0xFF0000FF);
static	irr::video::SColor	 s_RedColor(0xFFFF0000);

using namespace irr;
using namespace scene;
using namespace core;
using namespace video;

DrawingRectWallCtrller::DrawingRectWallCtrller()
{
	MeshBuf_ = new SMeshBuffer;
	LineMeshBuf_ = new SMeshBuffer;
	PathMeshBuf_ = new SMeshBuffer;
	/*
		4				5
			0		1

			3		2
		7				6
	*/

	irr::core::vector3df defaultPos;

	{
		auto smeshBuf = static_cast<SMeshBuffer*>(MeshBuf_);

		smeshBuf->Vertices.reallocate(8);
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(defaultPos, s_PntNormal, s_PntColor, irr::core::vector2df(0,0)));
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(defaultPos, s_PntNormal, s_PntColor, irr::core::vector2df(1,0)));
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(defaultPos, s_PntNormal, s_PntColor, irr::core::vector2df(0,0)));
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(defaultPos, s_PntNormal, s_PntColor, irr::core::vector2df(1,0)));
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(defaultPos, s_PntNormal, s_PntColor, irr::core::vector2df(0,1)));
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(defaultPos, s_PntNormal, s_PntColor, irr::core::vector2df(1,1)));
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(defaultPos, s_PntNormal, s_PntColor, irr::core::vector2df(0,1)));
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(defaultPos, s_PntNormal, s_PntColor, irr::core::vector2df(1,1)));

		smeshBuf->Indices.reallocate(24);
		smeshBuf->Indices.push_back(0);
		smeshBuf->Indices.push_back(1);
		smeshBuf->Indices.push_back(4);
		smeshBuf->Indices.push_back(4);
		smeshBuf->Indices.push_back(1);
		smeshBuf->Indices.push_back(5);
		smeshBuf->Indices.push_back(5);
		smeshBuf->Indices.push_back(1);
		smeshBuf->Indices.push_back(2);
		smeshBuf->Indices.push_back(5);
		smeshBuf->Indices.push_back(2);
		smeshBuf->Indices.push_back(6);
		smeshBuf->Indices.push_back(2);
		smeshBuf->Indices.push_back(3);
		smeshBuf->Indices.push_back(6);
		smeshBuf->Indices.push_back(6);
		smeshBuf->Indices.push_back(3);
		smeshBuf->Indices.push_back(7);
		smeshBuf->Indices.push_back(7);
		smeshBuf->Indices.push_back(3);
		smeshBuf->Indices.push_back(0);
		smeshBuf->Indices.push_back(7);
		smeshBuf->Indices.push_back(0);
		smeshBuf->Indices.push_back(4);
	}

	{
		auto smeshBuf = static_cast<SMeshBuffer*>(LineMeshBuf_);

		smeshBuf->Vertices.reallocate(8);
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(defaultPos, s_PntNormal, s_LineColor, irr::core::vector2df(0,0)));
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(defaultPos, s_PntNormal, s_LineColor, irr::core::vector2df(1,0)));
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(defaultPos, s_PntNormal, s_LineColor, irr::core::vector2df(0,0)));
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(defaultPos, s_PntNormal, s_LineColor, irr::core::vector2df(1,0)));
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(defaultPos, s_PntNormal, s_LineColor, irr::core::vector2df(0,1)));
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(defaultPos, s_PntNormal, s_LineColor, irr::core::vector2df(1,1)));
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(defaultPos, s_PntNormal, s_LineColor, irr::core::vector2df(0,1)));
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(defaultPos, s_PntNormal, s_LineColor, irr::core::vector2df(1,1)));

		smeshBuf->Indices.reallocate(24);
		smeshBuf->Indices.push_back(0);
		smeshBuf->Indices.push_back(1);
		smeshBuf->Indices.push_back(1);
		smeshBuf->Indices.push_back(2);
		smeshBuf->Indices.push_back(2);
		smeshBuf->Indices.push_back(3);
		smeshBuf->Indices.push_back(3);
		smeshBuf->Indices.push_back(0);
		smeshBuf->Indices.push_back(4);
		smeshBuf->Indices.push_back(5);
		smeshBuf->Indices.push_back(5);
		smeshBuf->Indices.push_back(6);
		smeshBuf->Indices.push_back(6);
		smeshBuf->Indices.push_back(7);
		smeshBuf->Indices.push_back(7);
		smeshBuf->Indices.push_back(4);
		smeshBuf->Indices.push_back(0);
		smeshBuf->Indices.push_back(4);
		smeshBuf->Indices.push_back(1);
		smeshBuf->Indices.push_back(5);
		smeshBuf->Indices.push_back(2);
		smeshBuf->Indices.push_back(6);
		smeshBuf->Indices.push_back(3);
		smeshBuf->Indices.push_back(7);
	}

	{
		auto smeshBuf = static_cast<SMeshBuffer*>(PathMeshBuf_);

		smeshBuf->Vertices.reallocate(4);
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(defaultPos, s_PntNormal, s_PathColor, irr::core::vector2df(0,0)));
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(defaultPos, s_PntNormal, s_PathColor, irr::core::vector2df(1,0)));
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(defaultPos, s_PntNormal, s_PathColor, irr::core::vector2df(0,0)));
		smeshBuf->Vertices.push_back(irr::video::S3DVertex(defaultPos, s_PntNormal, s_PathColor, irr::core::vector2df(1,0)));

		smeshBuf->Indices.reallocate(4);
		smeshBuf->Indices.push_back(0);
		smeshBuf->Indices.push_back(1);
		smeshBuf->Indices.push_back(2);
		smeshBuf->Indices.push_back(3);
	}
	
	Material_.Lighting = false;
	Material_.ZWriteEnable = false;
	Material_.BackfaceCulling = false;
	Checker_ = true;
	Material_.Thickness = 3;

	WallThickness_ = 200;

	State_ = EDWRS_BEGIN;

	SetName("DrawingRectWallCtrller");
}

DrawingRectWallCtrller::~DrawingRectWallCtrller()
{
	if ( MeshBuf_ )
	{
		MeshBuf_->drop();
	}

	if ( LineMeshBuf_ )
	{
		LineMeshBuf_->drop();
	}

	if ( PathMeshBuf_ )
	{
		PathMeshBuf_->drop();
	}
}

bool DrawingRectWallCtrller::OnPostEvent( const irr::SEvent& event )
{
	if ( !IsEnable() )
	{
		return false;
	}

	if ( event.EventType == irr::EET_MOUSE_INPUT_EVENT )
	{
		if ( event.MouseInput.Event == irr::EMIE_MOUSE_MOVED )
		{
			CursolIPos_.X = event.MouseInput.X;
			CursolIPos_.Y = event.MouseInput.Y;

			return false;
		}
	}

	switch (State_)
	{
	case DrawingRectWallCtrller::EDWRS_BEGIN:
		{
			if ( event.EventType == irr::EET_MOUSE_INPUT_EVENT )
			{
				if ( event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN )
				{
					if ( Checker_ )
					{
						FirstPnt_ = CurrentPos_;
						State_ = EDWRS_DRAWING;
					}

					return true;
				}
			}
		}
		break;
	case DrawingRectWallCtrller::EDWRS_DRAWING:
		{
			if ( event.EventType == irr::EET_MOUSE_INPUT_EVENT )
			{
				if ( event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN )
				{
					if ( Checker_ )
					{
						LastPnt_ = CurrentPos_;
						State_ = EDWRS_FINISH;
					}

					return true;
				}
			}
		}
		break;
	default:
		break;
	}

	return false;
}

bool DrawingRectWallCtrller::PreRender3D()
{
	if ( !IsEnable() )
	{
		Reset();
		return false;
	}

	auto smgr = GetRenderContextSPtr()->Smgr_.get();

	Checker_ = true;

	switch (State_)
	{
	case DrawingRectWallCtrller::EDWRS_BEGIN:
		{
			if ( StatusMgr::GetInstance().GridAlign_ )
			{
				CurrentPos_ = *StatusMgr::GetInstance().GridAlign_;
			}
			else
			{
				auto line = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(CursolIPos_);
				s_PntPlane.getIntersectionWithLine(line.start, line.getVector(), CurrentPos_);
			}
		}
		break;
	case DrawingRectWallCtrller::EDWRS_DRAWING:
		{
			if ( StatusMgr::GetInstance().GridAlign_ )
			{
				CurrentPos_ = *StatusMgr::GetInstance().GridAlign_;
			}
			else
			{
				auto line = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(CursolIPos_);
				s_PntPlane.getIntersectionWithLine(line.start, line.getVector(), CurrentPos_);
			}

			auto vec = CurrentPos_ - FirstPnt_;

			Checker_ = vec.getLengthSQ() >= WallThickness_ * WallThickness_;
			Checker_ &= vec.X * vec.X >= WallThickness_ * WallThickness_;
			Checker_ &= vec.Z * vec.Z >= WallThickness_ * WallThickness_;

			MeshBuf_->getPosition(0)		= FirstPnt_;
			LineMeshBuf_->getPosition(0)	= FirstPnt_;
			PathMeshBuf_->getPosition(0)	= FirstPnt_;
			MeshBuf_->getPosition(1)		= FirstPnt_ + vector3df(vec.X, 0, 0);
			LineMeshBuf_->getPosition(1)	= FirstPnt_ + vector3df(vec.X, 0, 0);
			PathMeshBuf_->getPosition(1)	= FirstPnt_ + vector3df(vec.X, 0, 0);
			MeshBuf_->getPosition(2)		= CurrentPos_;
			LineMeshBuf_->getPosition(2)	= CurrentPos_;
			PathMeshBuf_->getPosition(2)	= CurrentPos_;
			MeshBuf_->getPosition(3)		= FirstPnt_ + vector3df(0, 0, vec.Z);
			LineMeshBuf_->getPosition(3)	= FirstPnt_ + vector3df(0, 0, vec.Z);
			PathMeshBuf_->getPosition(3)	= FirstPnt_ + vector3df(0, 0, vec.Z);
			MeshBuf_->getPosition(4)		= MeshBuf_->getPosition(0) + vector3df(-WallThickness_, 0, -WallThickness_);
			LineMeshBuf_->getPosition(4)	= LineMeshBuf_->getPosition(0) + vector3df(-WallThickness_, 0, -WallThickness_);
			MeshBuf_->getPosition(5)		= MeshBuf_->getPosition(1) + vector3df(WallThickness_, 0, -WallThickness_);
			LineMeshBuf_->getPosition(5)	= LineMeshBuf_->getPosition(1) + vector3df(WallThickness_, 0, -WallThickness_);
			MeshBuf_->getPosition(6)		= MeshBuf_->getPosition(2) + vector3df(WallThickness_, 0, WallThickness_);
			LineMeshBuf_->getPosition(6)	= LineMeshBuf_->getPosition(2) + vector3df(WallThickness_, 0, WallThickness_);
			MeshBuf_->getPosition(7)		= MeshBuf_->getPosition(3) + vector3df(-WallThickness_, 0, WallThickness_);
			LineMeshBuf_->getPosition(7)	= LineMeshBuf_->getPosition(3) + vector3df(-WallThickness_, 0, WallThickness_);
		}
		break;
	case DrawingRectWallCtrller::EDWRS_FINISH:
		break;
	case DrawingRectWallCtrller::EDWRS_COUNT:
		break;
	default:
		break;
	}

	return false;
}

void DrawingRectWallCtrller::PostRender3D()
{
	if ( !IsEnable() )
	{
		return;
	}

	switch (State_)
	{
	case DrawingRectWallCtrller::EDWRS_BEGIN:
		{

		}
		break;
	case DrawingRectWallCtrller::EDWRS_DRAWING:
		{
			auto driver = GetRenderContextSPtr()->Smgr_->getVideoDriver();

			driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
			driver->setMaterial(Material_);

			if ( MeshBuf_ )
			{
				driver->drawMeshBuffer(MeshBuf_);
			}

			if ( LineMeshBuf_ )
			{
				driver->drawVertexPrimitiveList(LineMeshBuf_->getVertices(), LineMeshBuf_->getVertexCount(), LineMeshBuf_->getIndices(), LineMeshBuf_->getIndexCount()/2, EVT_STANDARD, EPT_LINES);
			}

			if ( PathMeshBuf_ )
			{
				auto smeshBuf = static_cast<SMeshBuffer*>(PathMeshBuf_);
				for ( unsigned index=0; index<smeshBuf->getVertexCount(); ++index )
				{
					smeshBuf->Vertices[index].Color = Checker_ ? s_PathColor : s_RedColor;
				}
				driver->drawVertexPrimitiveList(PathMeshBuf_->getVertices(), PathMeshBuf_->getVertexCount(), PathMeshBuf_->getIndices(), PathMeshBuf_->getIndexCount(), EVT_STANDARD, EPT_LINE_LOOP);
			}
		}
		break;
	case DrawingRectWallCtrller::EDWRS_FINISH:
		{

		}
		break;
	case DrawingRectWallCtrller::EDWRS_COUNT:
		break;
	default:
		break;
	}
}

void DrawingRectWallCtrller::Reset()
{
	State_ = EDWRS_BEGIN;
}