#ifndef DoorODL_h__
#define DoorODL_h__

#pragma once

#include "BaseODL.h"

class CDoorODL : public CBaseODL
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	CDoorODL();
	~CDoorODL();

public:

	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_DOOR; }

	virtual	void	Init();

public:

	void	Update2DZone(float xLength, float zLength);

	void	Set2DLineColor(const irr::video::SColor& clr);

	void	Draw2DMesh();
};

typedef	std::shared_ptr<CDoorODL>	CDoorODLSPtr;
typedef	std::weak_ptr<CDoorODL>		CDoorODLWPtr;

#endif // DoorODL_h__
