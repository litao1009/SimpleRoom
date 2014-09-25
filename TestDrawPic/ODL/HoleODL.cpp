#include "stdafx.h"

#include "HoleODL.h"

#include "BRepBuilderAPI_MakePolygon.hxx"
#include "BRepPrimAPI_MakeBox.hxx"

IHoleODL::IHoleODL():
	CBaseODL(),m_HoleOffsetSize(0,0,50),m_HoleSize(0,0,0),m_HoleRotation(0,0,0)
{
	m_NeedUpdate = true;

	m_HolePos.SetXYZ(gp_XYZ(0,0,0));
}


TopoDS_Shape IHoleODL::GetHoleShape()
{
	Update();

	return m_BBox;
}

TopoDS_Shape IHoleODL::GetOffsetHoleShape()
{
	Update();

	return m_OffsetBox;
}

void IHoleODL::Update()
{
	if ( !m_NeedUpdate )
	{
		return;
	}

	gp_Trsf trans;
	gp_Ax1 ax;
	ax.SetDirection(gp::DY());
	ax.SetLocation(m_HolePos);
	trans.SetRotation(ax, -m_HoleRotation.Y());

	{
		gp_Ax2 ax2(m_HolePos, gp::DZ().Rotated(gp::OY(), -m_HoleRotation.Y()));

		BRepPrimAPI_MakeBox mb(ax2, m_HoleSize.X(), m_HoleSize.Y(), m_HoleSize.Z());
		mb.Build();
		assert(mb.IsDone());
		auto solid = mb.Shape();
		m_BBox = solid;
	}

	{
		gp_Pnt p;
		p.SetX(m_HolePos.X()-m_HoleOffsetSize.X());
		p.SetY(m_HolePos.Y()-m_HoleOffsetSize.Y());
		p.SetZ(m_HolePos.Z()-m_HoleOffsetSize.Z());

		BRepPrimAPI_MakeBox mb(p, m_HoleSize.X()+2*m_HoleOffsetSize.X(), m_HoleSize.Y()+2*m_HoleOffsetSize.Y(), m_HoleSize.Z()+2*m_HoleOffsetSize.Z());
		auto solid = mb.Shape();
		solid.Move(trans);

		m_OffsetBox = solid;
	}

	m_NeedUpdate = false;
}