#ifndef DoubleShapeODL_h__
#define DoubleShapeODL_h__

#pragma once

#include "BaseODL.h"
#include "HoleODL.h"

#include "TopoDS_Shape.hxx"

#include <vector>

typedef	std::vector<IHoleODLWPtr>				HolesList;
typedef	std::tuple<IHoleODLWPtr,gp_Pnt2d,gp_XY>	RelationRect;
typedef	std::vector<RelationRect>				RelationRectList;

class	IWithHoleODL : public CBaseODL
{
public:

	IWithHoleODL();

	const HolesList&	GetHoleList() const
	{
		return m_HoleList;
	}

	HolesList&			GetHoleList()
	{
		return m_HoleList;
	}

	void				SetHoleList(const HolesList& holeList)
	{
		m_HoleList = holeList;
	}

	TopoDS_Shape&		GetBaseShape()
	{
		return m_BaseShape;
	}

	TopoDS_Shape&		GetBuildShape()
	{
		return m_BuildShape;
	}

	virtual	bool		UpdateHoleShape();

	bool				NeedUpdateHoleMesh() const
	{
		return m_UpdateHoleMesh && !m_HoleList.empty();
	}

	void				SetNeedUpdateHoleMesh(bool val)
	{
		m_UpdateHoleMesh = val;
	}

	RelationRectList&	GetRectList()
	{
		return m_RectList;
	}

	virtual void		UpdateHoleRect(RelationRect rect) {}

private:

	HolesList			m_HoleList;
	RelationRectList	m_RectList;
	TopoDS_Shape		m_BaseShape;
	TopoDS_Shape		m_BuildShape;
	bool				m_UpdateHoleMesh;
};

typedef	std::shared_ptr<IWithHoleODL>	IWithHoleODLSPtr;
typedef	std::weak_ptr<IWithHoleODL>		IWithHoleODLWPtr;

#endif // DoubleShapeODL_h__
