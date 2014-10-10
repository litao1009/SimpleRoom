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
		ESNT_NONE = 0,
		ESNT_2D = 1,
		ESNT_3D = ESNT_2D << 1,
		ESNT_COMBINE = ESNT_2D | ESNT_3D
	};

public:

	static	ESceneNodeType	GetRenderMode() { return RenderMode_; }

	static	void			SetRenderMode(ESceneNodeType val) { RenderMode_ = val; }

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

	void				SaveID() { SaveID_ = getID(); }
	void				ResetID() { setID(SaveID_); }

protected:

	irr::video::SColor&	GetColor() { return DbgColor_; }

private:

	irr::s32				SaveID_;

	irr::video::SColor		DbgColor_;

	SRenderContextWPtr		RenderContextWPtr_;

	CBaseODLWPtr			BaseODLWPtr_;

private:

	static	ESceneNodeType	RenderMode_;
};

#endif // BaseSceneNode_h__
