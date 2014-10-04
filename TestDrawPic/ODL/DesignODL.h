#ifndef DesignODL_h__
#define DesignODL_h__

#pragma once

#include "BaseODL.h"
#include "irrEngine/SRenderContextFwd.h"

class CDesignODL :	public CBaseODL
{
	class	Imp;
	std::shared_ptr<Imp>	ImpSPtr_;

public:
	CDesignODL(HWND Hwnd);
	~CDesignODL(void);

public://CBaseODL

	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_DESIGN; }

public:

	virtual	void	Init();

	SRenderContextSPtr GetRenderContext() const { return RenderContext_; }

private:

	HWND				Hwnd_;
	SRenderContextSPtr	RenderContext_;
};

typedef	std::shared_ptr<CDesignODL> CDesignODLSPtr;
typedef	std::weak_ptr<CDesignODL>	CDesignODLWPtr;

#endif // DesignODL_h__
