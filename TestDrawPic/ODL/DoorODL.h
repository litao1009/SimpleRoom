#ifndef DoorODL_h__
#define DoorODL_h__

#pragma once

#include "HoleODL.h"

class DoorODL : public HoleODL
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	DoorODL(const SRenderContextWPtr& rc);
	~DoorODL();

public://BaseODL

	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_DOOR; }

	virtual	void	Init();

public://HoleODL

	virtual	void	UpdateHole();

public:

	void	Set2DLineColor(const irr::video::SColor& clr);
};

typedef	std::shared_ptr<DoorODL>	DoorODLSPtr;
typedef	std::weak_ptr<DoorODL>		DoorODLWPtr;

#endif // DoorODL_h__
