#include "stdafx.h"

#include "DrawingController.h"

#include "irrEngine/SRenderContext.h"

DrawingController::DrawingController()
{

}

DrawingController::~DrawingController()
{

}

void DrawingController::PreInit( SRenderContextSPtr sprc )
{

}

void DrawingController::PostInit( SRenderContextSPtr sprc )
{

}

bool DrawingController::OnEvent( const irr::SEvent& event )
{
	return false;
}

void DrawingController::OnResize( const SRenderContext& rc )
{

}

bool DrawingController::PreRender3D( const SRenderContext& rc )
{
	return true;
}

void DrawingController::PostRender3D( const SRenderContext& rc )
{

}

bool DrawingController::PreRender2D( const SRenderContext& rc )
{
	return true;
}

void DrawingController::PostRender2D( const SRenderContext& rc )
{

}
