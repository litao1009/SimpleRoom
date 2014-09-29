#ifndef BaseSceneNode_h__
#define BaseSceneNode_h__

#pragma once

#include "IMeshSceneNode.h"
#include "ODL/BaseODLFwd.h"
#include "irrEngine/SRenderContextFwd.h"

#include <string>

class	CBaseSceneNode : public irr::scene::IMeshSceneNode
{
public:

	enum ESceneNodeType
	{
		ESNT_2D,
		ESNT_3D,
		ESNT_COMBINE,
		ESNT_COUNT
	};

public:

	CBaseSceneNode(SRenderContextWPtr rcWPtr, CBaseODLWPtr odlWPtr);
	~CBaseSceneNode();

public:

	virtual	ESceneNodeType	GetSceneNodeType() const = 0;

public:

	CBaseODLWPtr		GetBaseODLWPtr() const { return BaseODLWPtr_; }
	CBaseODLSPtr		GetBaseODLSPtr() const { return BaseODLWPtr_.lock(); }

	SRenderContextWPtr	GetRenderContextWPtr() const { return RenderContextWPtr_; }
	SRenderContextSPtr	GetRenderContextSPtr() const { return RenderContextWPtr_.lock(); }

	void				SaveID() { m_SaveID = getID(); }
	void				ResetID() { setID(m_SaveID); }

protected:

	irr::video::SColor&	GetColor() { return DbgColor_; }

private:

	irr::s32				m_SaveID;

	irr::video::SColor		DbgColor_;

	SRenderContextWPtr		RenderContextWPtr_;

	CBaseODLWPtr			BaseODLWPtr_;
};

#endif // BaseSceneNode_h__
