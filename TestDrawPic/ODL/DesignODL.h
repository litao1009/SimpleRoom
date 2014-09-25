#ifndef DesignODL_h__
#define DesignODL_h__

#pragma once
#include "baseodl.h"
#include "irrEngine/SRenderContextFwd.h"
#include "CameraODLFwd.h"
class CDesignODL :
	public CBaseODL
{
public:
	CDesignODL(HWND Hwnd);
	~CDesignODL(void);
public:
	virtual void	OnDrawTop(Gdiplus::Graphics& gcDrawer, float fScale);
	void	Init();
	SRenderContextSPtr GetRenderContext() const { return m_spRenderContext; }
	
	CCameraODLSPtr		GetCamera() const { return m_spCamera; }

private:
	
	SRenderContextSPtr	m_spRenderContext;
	CCameraODLSPtr		m_spCamera;
};

typedef	std::shared_ptr<CDesignODL> CDesignODLSPtr;
typedef	std::weak_ptr<CDesignODL> CDesignODLWPtr;

#endif // DesignODL_h__
