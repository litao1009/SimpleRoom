#ifndef BaseODL_h__
#define BaseODL_h__

#pragma once

#include "PublicDefine.h"
#include "ODLTools.h"
#include "BaseODLFwd.h"

#include "BLL/BaseBLL.h"

#include "TSceneNode.h"
#include "ShapeSceneNode.h"

#include "TopoDS_Shape.hxx"
#include "BRepExtrema_DistShapeShape.hxx"

class CBaseODL : public TSceneNode<CBaseODL>, public ShapeSceneNode
{
public:

	CBaseODL(const SRenderContextWPtr& rc);
	
	virtual ~CBaseODL(void);

public:

	virtual	EObjectDisplayLayerType	GetType() const = 0;

	virtual	void				Init() {}

public:

	void				UpdateShapeAndDataSceneNode();

	void				UpdateAbsoluteTransform();
	const gp_Trsf&		GetAbsoluteTransform() const { return AbsoluteTransform_; }

	bool				IsDirty() const { return Dirty_; }
	void				SetDirty(bool val) { Dirty_ = val; }

	bool				IsSweeping() const { return Sweeping_; }
	void				SetSweeping(bool val) { Sweeping_ = val; UpdateSweeping(); }

	bool				IsPicking() const { return Picking_; }
	void				SetPicking(bool val) { Picking_ = val; UpdatePicking(); }

protected:

	virtual void		UpdateSweeping() {}

	virtual	void		UpdatePicking() {}

private:

	gp_Trsf				AbsoluteTransform_;
	bool				Dirty_;
	bool				Sweeping_;
	bool				Picking_;
};

#endif // BaseODL_h__
