#ifndef BaseODL_h__
#define BaseODL_h__

#pragma once

#include "PublicDefine.h"
#include "ODLTools.h"
#include "BaseODLFwd.h"

#include "BLL/BaseBLL.h"

#include "TSceneNode.h"

#include "TopoDS_Shape.hxx"
#include "BRepExtrema_DistShapeShape.hxx"

class CBaseODL : public TSceneNode<CBaseODL>
{
public:

	CBaseODL(void);
	
	virtual ~CBaseODL(void);

public:

	virtual	EObjectDisplayLayerType	GetType() const = 0;

	virtual	void				Init() {}

protected:

	virtual void	UpdateSelection() {}

	virtual	void	UpdateSweeping() {}

public:

	void				UpdateShape() { BaseSelector_.LoadS1(BaseShape_); }

	TopoDS_Shape&		GetBaseShape() { return BaseShape_; }

	const TopoDS_Shape&	GetBaseShape() const { return BaseShape_; }

	void				SetBaseShape(const TopoDS_Shape& shape) { BaseShape_ = shape; UpdateShape(); }

	BRepExtrema_DistShapeShape&	GetSelector() { return BaseSelector_; }

	const BRepExtrema_DistShapeShape&	GetSelector() const { return BaseSelector_; }

	void				SetSelected(bool val) { Selected_ = val; UpdateSelection(); }
	bool				IsSelected() const { return Selected_; }

	void				SetSwept(bool val) { Swept_ = val; UpdateSweeping(); }
	bool				IsSwept() const { return Swept_; }

private:

	bool						Selected_;
	bool						Swept_;
	TopoDS_Shape				BaseShape_;
	BRepExtrema_DistShapeShape	BaseSelector_;
};

#endif // BaseODL_h__
