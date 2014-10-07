#ifndef ShapeSceneNode_h__
#define ShapeSceneNode_h__

#pragma once

#include "TopoDS_Shape.hxx"
#include "BRepExtrema_DistShapeShape.hxx"
#include "Bnd_Box.hxx"
#include "gp_Quaternion.hxx"
#include "Precision.hxx"

class	ShapeSceneNode
{
public:

	ShapeSceneNode();
	~ShapeSceneNode();

public:

	typedef	BRepExtrema_DistShapeShape	ShapeSelector;

public:

	void					UpdateShape();

	TopoDS_Shape&			GetBaseShape() { return BaseShape_; }
	const TopoDS_Shape&		GetBaseShape() const { return BaseShape_; }
	void					SetBaseShape(const TopoDS_Shape& shape);

	ShapeSelector&			GetBaseSelector() { return BaseSelector_; }
	const ShapeSelector&	GetBaseSelector() const { return BaseSelector_; }

	const gp_XYZ&			GetTranslation() const;
	void					SetTranslation(const gp_XYZ& val);

	const gp_Quaternion&	GetRotation() const;
	void					SetRoration(const gp_Quaternion& val);

	const gp_XYZ&			GetScale() const;
	void					SetScale(const gp_XYZ& val);

	const Bnd_Box&			GetBaseBndBox() const { return BaseBndBox_; }
	void					SetBaseBndBox(const Bnd_Box& bbox) { BaseBndBox_ = bbox; }

	const gp_Trsf&			GetTransform() const;

private:

	TopoDS_Shape	BaseShape_;

	Bnd_Box			BaseBndBox_;

	ShapeSelector	BaseSelector_;

	gp_XYZ			Translation_;
	gp_Quaternion	Rotation_;
	gp_XYZ			Scale_;

	mutable gp_Trsf	RelationTransform_;
	mutable bool	NeedUpdate_;
};
#endif // ShapeSceneNode_h__
