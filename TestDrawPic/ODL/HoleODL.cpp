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

	{
		auto center = HoleSize_ / 2; 
		auto box = BRepPrimAPI_MakeBox(HoleSize_.X(), HoleSize_.Y(), HoleSize_.Z()).Shape();

		gp_Trsf tfs;
		tfs.SetTranslation(center, gp::Origin());
		box.Move(tfs);

		SetBaseShape(box);
	}

	{
		auto offsetSize = HoleSize_ + HoleOffsetSize_;
		auto center = offsetSize / 2; 
		OffsetSizeShape_ = BRepPrimAPI_MakeBox(offsetSize.X(), offsetSize.Y(), offsetSize.Z()).Shape();

		gp_Trsf tfs;
		tfs.SetTranslation(center, gp::Origin());
		OffsetSizeShape_.Move(tfs);
	}

	m_NeedUpdate = false;
}