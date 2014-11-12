#ifndef DesignODL_h__
#define DesignODL_h__

#pragma once

#include "BaseODL.h"
#include "irrEngine/SRenderContextFwd.h"

class DesignODL :	public BaseODL
{
	class	Imp;
	std::shared_ptr<Imp>	ImpSPtr_;

public:
	DesignODL(HWND Hwnd);
	~DesignODL(void);

public://CBaseODL

	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_DESIGN; }

	virtual	void	Init();

public:

	SRenderContextSPtr GetRenderContext() const { return RenderContext_; }

private:

	HWND				Hwnd_;
	SRenderContextSPtr	RenderContext_;
};

typedef	std::shared_ptr<DesignODL>	DesignODLSPtr;
typedef	std::weak_ptr<DesignODL>	DesignODLWPtr;

#endif // DesignODL_h__
