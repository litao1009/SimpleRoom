#ifndef HoleODL_h__
#define HoleODL_h__

#pragma once

#include "HoleODLFwd.h"
#include "BaseODL.h"

#include "TopoDS_Shape.hxx"

class	HoleODL : public BaseODL
{
public:

	HoleODL(const SRenderContextWPtr& rc);

public:

	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_HOLE; }

public:

	const gp_XYZ&	GetHoleSize() const
	{
		return HoleSize_;
	}

	gp_XYZ&			GetHoleSize()
	{
		return HoleSize_;
	}

	void			SetHoleSize(Standard_Real x, Standard_Real y, Standard_Real z)
	{
		HoleSize_.SetX(x);
		HoleSize_.SetY(y);
		HoleSize_.SetZ(z);
		m_NeedUpdate = true;
	}

	void			SetOffsetSize(Standard_Real x, Standard_Real y, Standard_Real z)
	{
		HoleOffsetSize_.SetX(x);
		HoleOffsetSize_.SetY(y);
		HoleOffsetSize_.SetZ(z);
		m_NeedUpdate = true;
	}

	virtual	void	UpdateHole();

private:

	bool	m_NeedUpdate;
	gp_XYZ	HoleSize_;
	gp_XYZ	HoleOffsetSize_;
	TopoDS_Shape	OffsetSizeShape_;
};

#endif // HoleODL_h__
