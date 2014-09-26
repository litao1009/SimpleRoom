#ifndef DesignODL_h__
#define DesignODL_h__

#pragma once

#include "baseodl.h"
#include "irrEngine/SRenderContextFwd.h"

class CDesignODL :	public CBaseODL
{
	class	Imp;
	std::shared_ptr<Imp>	ImpSPtr_;

public:
	CDesignODL(HWND Hwnd);
	~CDesignODL(void);

public://CBaseODL

	virtual	ENUM_OBOJECT_TYPE	GetType() { return EOBT_Design; }

public:

	void	Init();

	SRenderContextSPtr GetRenderContext() const { return m_spRenderContext; }

private:
	
	SRenderContextSPtr	m_spRenderContext;

	HWND				Hwnd_;
};

typedef	std::shared_ptr<CDesignODL> CDesignODLSPtr;
typedef	std::weak_ptr<CDesignODL>	CDesignODLWPtr;

#endif // DesignODL_h__
