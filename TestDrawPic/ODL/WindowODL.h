#ifndef WindowODL_h__
#define WindowODL_h__

#pragma once

#include "BaseODL.h"

class CWindowODL : public CBaseODL
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	CWindowODL();
	~CWindowODL();

public:

	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_WINDOW; }

	virtual	void	Init();

public:

	void	Update2DZone(float xLength, float zLength);

	void	Set2DLineColor(const irr::video::SColor& clr);

	void	Draw2DMesh();
};

typedef	std::shared_ptr<CWindowODL>	CWindowODLSPtr;
typedef	std::weak_ptr<CWindowODL>	CWindowODLWPtr;

#endif // WindowODL_h__
