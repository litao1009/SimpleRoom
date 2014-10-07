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

	CBaseODL(void);
	
	virtual ~CBaseODL(void);

protected:

	virtual void	UpdateSelection() {}

	virtual	void	UpdateSweeping() {}

public:

	virtual	EObjectDisplayLayerType	GetType() const = 0;

	virtual	void				Init() {}

public:

	void				UpdateShapeAndDataSceneNode();

	void				UpdateAbsoluteTransform();
	const gp_Trsf&		GetAbsoluteTransform() const { return AbsoluteTransform_; }

	void				SetSelected(bool val) { Selected_ = val; UpdateSelection(); }
	bool				IsSelected() const { return Selected_; }

	void				SetSwept(bool val) { Swept_ = val; UpdateSweeping(); }
	bool				IsSwept() const { return Swept_; }

private:

	gp_Trsf				AbsoluteTransform_;
	bool				Selected_;
	bool				Swept_;
};

#endif // BaseODL_h__
