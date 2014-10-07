#include "stdafx.h"

#include "ShapeSceneNode.h"
#include "BRepBndLib.hxx"

ShapeSceneNode::ShapeSceneNode()
{
	NeedUpdate_ = false;
	Scale_.SetX(1);
	Scale_.SetY(1);
	Scale_.SetZ(1);
}

ShapeSceneNode::~ShapeSceneNode()
{

}


void ShapeSceneNode::UpdateShape()
{
	if ( !BaseShape_.IsNull() )
	{
		BaseBndBox_.SetVoid();
		BRepBndLib::Add(BaseShape_, BaseBndBox_);
		BaseSelector_.LoadS1(BaseShape_);
	}
}

const gp_XYZ& ShapeSceneNode::GetTranslation() const
{
	return Translation_;
}

void ShapeSceneNode::SetTranslation( const gp_XYZ& val )
{
	if ( !Translation_.IsEqual(val, Precision::Confusion()) )
	{
		Translation_ = val;
		NeedUpdate_ = true;
	}
}

const gp_Quaternion& ShapeSceneNode::GetRotation() const
{
	return Rotation_;
}

void ShapeSceneNode::SetRoration( const gp_Quaternion& val )
{
	if ( !Rotation_.IsEqual(val) )
	{
		Rotation_ = val;
		NeedUpdate_ = true;
	}
}

const gp_XYZ& ShapeSceneNode::GetScale() const
{
	return Scale_;
}

void ShapeSceneNode::SetScale( const gp_XYZ& val )
{
	if ( !Scale_.IsEqual(val, Precision::Confusion()) )
	{
		Scale_ = val;
		NeedUpdate_ = true;
	}
}

const gp_Trsf& ShapeSceneNode::GetTransform() const
{
	if ( NeedUpdate_ )
	{
		gp_Trsf t,r,s;
		s.SetScaleFactor(Scale_.X());
		r.SetRotation(Rotation_);
		t.SetTranslationPart(Translation_);

		RelationTransform_ = t * r * s;

		NeedUpdate_ = false;
	}
	return RelationTransform_;
}

void ShapeSceneNode::SetBaseShape( const TopoDS_Shape& shape )
{
	BaseShape_ = shape;

	UpdateShape();
}