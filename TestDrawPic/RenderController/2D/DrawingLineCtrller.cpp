#include "stdafx.h"
 
#include "DrawingLineCtrller.h"
#include "IrrEngine/SRenderContext.h"

#include "SMeshBuffer.h"

static	irr::core::vector3df s_PntNormal(0,1,0);
static	irr::core::vector2df s_PntCoord(0,0);
static	irr::core::plane3df	 s_PntPlane(0,0,0,0,1,0);

DrawingLineCtrller::DrawingLineCtrller()
{
	Pnts_ = new irr::scene::SMeshBuffer;

	Pnts_->getMaterial().Lighting = false;
	Pnts_->getMaterial().Thickness = 4;
	Pnts_->getMaterial().ZWriteEnable = false;

	PntColor_.set(0xFF,0xF0, 0xF0, 0xF0);

	State_ = EDLS_BEGIN;

	SetName("DrawingLineCtrller");
}

DrawingLineCtrller::~DrawingLineCtrller()
{
	Pnts_->drop();
}

void DrawingLineCtrller::Init()
{

}


bool DrawingLineCtrller::OnPostEvent( const irr::SEvent& event )
{
	auto smeshBuf = static_cast<irr::scene::SMeshBuffer*>(Pnts_);

	if ( event.EventType == irr::EET_MOUSE_INPUT_EVENT )
	{
		if ( event.MouseInput.Event == irr::EMIE_MOUSE_MOVED )
		{
			CurrentPos_.X = event.MouseInput.X;
			CurrentPos_.Y = event.MouseInput.Y;

			return false;
		}
	}

	switch (State_)
	{
	case DrawingLineCtrller::EDLS_BEGIN:
		{
			if ( event.EventType == irr::EET_MOUSE_INPUT_EVENT )
			{
				if ( event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN )
				{
					State_ = EDLS_DRAWING;

					auto smgr = GetRenderContextSPtr()->Smgr_.get();

					auto line = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(CurrentPos_);
					irr::core::vector3df position;
					s_PntPlane.getIntersectionWithLine(line.start, line.getVector(), position);

					smeshBuf->Vertices.push_back(irr::video::S3DVertex(position, s_PntNormal, PntColor_, s_PntCoord));
					smeshBuf->Indices.push_back(Pnts_->getIndexCount());
				}
			}
		}
		break;
	case DrawingLineCtrller::EDLS_DRAWING:
		{
			if ( event.EventType == irr::EET_MOUSE_INPUT_EVENT )
			{
				if ( event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN )
				{
					auto smgr = GetRenderContextSPtr()->Smgr_.get();

					auto line = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(CurrentPos_);

					irr::core::vector3df position;
					s_PntPlane.getIntersectionWithLine(line.start, line.getVector(), position);

					smeshBuf->Vertices.push_back(irr::video::S3DVertex(position, s_PntNormal, PntColor_, s_PntCoord));
					smeshBuf->Indices.push_back(Pnts_->getIndexCount());
				}
			}

			if ( event.EventType == irr::EET_MOUSE_INPUT_EVENT )
			{
				if ( event.MouseInput.Event == irr::EMIE_RMOUSE_PRESSED_DOWN )
				{
					State_ = EDLS_FINISH;
				}
			}

			if ( event.EventType == irr::EET_KEY_INPUT_EVENT )
			{
				if ( event.KeyInput.Key == irr::KEY_ESCAPE )
				{
					assert(Pnts_->getVertexCount() == Pnts_->getIndexCount());

					if ( !smeshBuf->Vertices.empty() )
					{
						smeshBuf->Vertices.erase(Pnts_->getVertexCount()-1);
						smeshBuf->Indices.erase(Pnts_->getIndexCount()-1);
					}

					if ( smeshBuf->Vertices.empty() )
					{
						State_ = EDLS_BEGIN;
					}
				}
			}
		}
		break;
	default:
		break;
	}

	return false;
}

void DrawingLineCtrller::OnResize()
{

}

bool DrawingLineCtrller::PreRender3D()
{
	return false;
}

void DrawingLineCtrller::PostRender3D()
{
	switch (State_)
	{
	case DrawingLineCtrller::EDLS_BEGIN:
		{

		}
		break;
	case DrawingLineCtrller::EDLS_DRAWING:
		{
			auto driver = GetRenderContextSPtr()->Smgr_->getVideoDriver();

			driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
			GetRenderContextSPtr()->Smgr_->getVideoDriver()->setMaterial(Pnts_->getMaterial());

			if ( Pnts_->getVertexCount() > 1 )
			{
				GetRenderContextSPtr()->Smgr_->getVideoDriver()->drawVertexPrimitiveList(Pnts_->getVertices(), Pnts_->getVertexCount(), Pnts_->getIndices(), Pnts_->getIndexCount()-1, irr::video::EVT_STANDARD, irr::scene::EPT_LINE_STRIP);
			}

			{
				auto line = GetRenderContextSPtr()->Smgr_->getSceneCollisionManager()->getRayFromScreenCoordinates(CurrentPos_);
				irr::core::vector3df position;
				s_PntPlane.getIntersectionWithLine(line.start, line.getVector(), position);
				driver->draw3DLine(Pnts_->getPosition(Pnts_->getVertexCount()-1), position, PntColor_);
			}
		}
		break;
	case DrawingLineCtrller::EDLS_FINISH:
		{

		}
		break;
	case DrawingLineCtrller::EDLS_COUNT:
		break;
	default:
		break;
	}
}

bool DrawingLineCtrller::PreRender2D()
{
	return true;
}

void DrawingLineCtrller::PostRender2D()
{

}

void DrawingLineCtrller::Reset()
{
	Pnts_->drop();
	Pnts_ = new irr::scene::SMeshBuffer;
}

irr::scene::IMeshBuffer* DrawingLineCtrller::GetMeshBuffer() const
{
	return Pnts_;
}
