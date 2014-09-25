#ifndef ODLTools_h__
#define ODLTools_h__

#pragma once

#include "IMesh.h"
#include "rect.h"

#include "TopoDS_Face.hxx"
#include "gp_Dir.hxx"

#include <boost/optional.hpp>

class	ODLTools
{
public:

	class	SFaceMesh
	{
	public:

		irr::scene::IMesh*		Mesh_;
		irr::core::vector3df	Translation_;
		irr::core::vector3df	Rotation_;
		irr::core::aabbox3df	Box_;
	};
	typedef	boost::optional<SFaceMesh>	SFaceMeshOpt;

public:

	static	SFaceMeshOpt		CreateFaceMesh(TopoDS_Face& faceShape, const gp_Dir& faceDir);

	static	SFaceMeshOpt		CalculateRelation(const TopoDS_Shape& faceShape, const gp_Dir& faceDir);

	static	irr::scene::IMesh*	CreateMesh(TopoDS_Shape& shape);

	static	irr::scene::IMesh*	CreateSimpleRectMesh(const irr::core::rectf& rect);

	static	bool				UpdateSimpleRectMesh(irr::scene::IMesh* mesh, const irr::core::rectf& rect);

	static	double	Mix(double vmin, double vmax, double vmix)
	{
		auto range = vmax - vmin;
		auto adjMix = vmix - vmin;

		adjMix /= range;

		return adjMix;
	}

};
#endif // ODLTools_h__
