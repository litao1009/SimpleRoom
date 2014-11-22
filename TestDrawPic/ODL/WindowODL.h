#ifndef WindowODL_h__
#define WindowODL_h__

#pragma once

#include "HoleODL.h"

class WindowODL : public HoleODL
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	WindowODL(const SRenderContextWPtr& rc);
	~WindowODL();

public:

	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_WINDOW; }

	virtual	void	Init();

protected:

	virtual	void	UpdateSweeping();

	virtual void	UpdatePicking();

public://HoleODL

	virtual	void	UpdateHole();

public:

	void	Set2DLineColor(const irr::video::SColor& clr);
};

typedef	std::shared_ptr<WindowODL>	WindowODLSPtr;
typedef	std::weak_ptr<WindowODL>	WindowODLWPtr;

#endif // WindowODL_h__
