#include "stdafx.h"

#include "HoleODL.h"

#include "BRepPrimAPI_MakeBox.hxx"

HoleODL::HoleODL(const SRenderContextWPtr& rc):
	BaseODL(rc),HoleOffsetSize_(0,0,50),HoleSize_(0,0,0)
{
	m_NeedUpdate = true;
}

void HoleODL::UpdateHole()
{
	if ( !m_NeedUpdate )
	{
		return;
	}

	auto holeSize = HoleSize_ + HoleOffsetSize_ * 2;
	auto center = holeSize / 2; 
	auto box = BRepPrimAPI_MakeBox(holeSize.X(), holeSize.Y(), holeSize.Z()).Shape();

	gp_Trsf tfs;
	tfs.SetTranslation(center, gp::Origin());
	box.Move(tfs);

	SetBaseShape(box);

	m_NeedUpdate = false;
}