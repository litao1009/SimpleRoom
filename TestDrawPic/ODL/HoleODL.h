#ifndef HoleODL_h__
#define HoleODL_h__

#pragma once

#include "BaseODL.h"

#include "TopoDS_Shape.hxx"

class	IHoleODL : public CBaseODL
{
public:

	IHoleODL();

public:

	const gp_Pnt&	GetHolePostion() const
	{
		return m_HolePos;
	}

	gp_Pnt&			GetHolePostion()
	{
		return m_HolePos;
	}

	void			SetHolePosition(const gp_Pnt& pos)
	{
		m_HolePos = pos;
		m_NeedUpdate = true;
	}

	void			SetHolePosition(Standard_Real x, Standard_Real y, Standard_Real z)
	{
		m_HolePos.SetX(x);
		m_HolePos.SetY(y);
		m_HolePos.SetZ(z);
		m_NeedUpdate = true;
	}

	const gp_XYZ&	GetHoleSize() const
	{
		return m_HoleSize;
	}

	gp_XYZ&			GetHoleSize()
	{
		return m_HoleSize;
	}

	void			SetHoleSize(Standard_Real x, Standard_Real y, Standard_Real z)
	{
		m_HoleSize.SetX(x);
		m_HoleSize.SetY(y);
		m_HoleSize.SetZ(z);
		m_NeedUpdate = true;
	}

	const gp_XYZ&	GetHoleRotation() const
	{
		return m_HoleRotation;
	}

	gp_XYZ&			GetHoleRotation()
	{
		return m_HoleRotation;
	}

	void			SetHoleRotation(Standard_Real x, Standard_Real y, Standard_Real z)
	{
		m_HoleRotation.SetX(x);
		m_HoleRotation.SetY(y);
		m_HoleRotation.SetZ(z);
		m_NeedUpdate = true;
	}

	const gp_XYZ&	GetOffsetSize() const
	{
		return m_HoleOffsetSize;
	}

	gp_XYZ&			GetOffsetSize()
	{
		return m_HoleOffsetSize;
	}

	void			SetOffsetSize(Standard_Real x, Standard_Real y, Standard_Real z)
	{
		m_HoleOffsetSize.SetX(x);
		m_HoleOffsetSize.SetY(y);
		m_HoleOffsetSize.SetZ(z);
		m_NeedUpdate = true;
	}

	TopoDS_Shape	GetHoleShape();

	TopoDS_Shape	GetOffsetHoleShape();

private:

	void			Update();

private:

	bool	m_NeedUpdate;
	gp_Pnt	m_HolePos;
	gp_XYZ	m_HoleSize;
	gp_XYZ	m_HoleRotation;
	gp_XYZ	m_HoleOffsetSize;

	TopoDS_Shape	m_BBox;
	TopoDS_Shape	m_OffsetBox;
};

typedef	std::shared_ptr<IHoleODL>	IHoleODLSPtr;
typedef	std::weak_ptr<IHoleODL>		IHoleODLWPtr;

#endif // HoleODL_h__
