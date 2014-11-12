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

	auto center = HoleSize_ / 2; 
	auto box = BRepPrimAPI_MakeBox(HoleSize_.X(), HoleSize_.Y(), HoleSize_.Z()).Shape();

	gp_Trsf tfs;
	tfs.SetTranslation(center, gp::Origin());
	box.Move(tfs);

	SetBaseShape(box);

	m_NeedUpdate = false;
}