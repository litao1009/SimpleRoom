#ifndef TSceneNode_h__
#define TSceneNode_h__

#pragma once

#include "irrEngine/IrrExtension/CombineSceneNode.h"

#include <memory>
#include <vector>

template<typename T>
class	TSceneNode : public std::enable_shared_from_this<T>
{
public:

	typedef	typename T							Reference;
	typedef	typename std::shared_ptr<Reference>	SceneNodeSPtr;
	typedef	typename std::weak_ptr<Reference>	SceneNodeWPtr;
	typedef	typename std::vector<SceneNodeSPtr>	ChildrenList;
	

public:

	TSceneNode()
	{}

	virtual ~TSceneNode()
	{
		if ( DataSceneNode_ )
		{
			DataSceneNode_->drop();
		}
	}

public:

	//��ȡ���ڵ�
	SceneNodeWPtr GetParent() const { return ParentWPtr_; }

	//��ȡ�ӽڵ�
	ChildrenList& GetChildrenList() { return ChildrenList_; }

	const ChildrenList& GetChildrenList() const { return ChildrenList_; }

	//�Ƴ��ӽڵ�
	void	RemoveChild(SceneNodeSPtr child)
	{
		if ( !child )
		{
			return;
		}

		auto itor = std::remove(ChildrenList_.begin(), ChildrenList_.end(), child);

		if ( itor == ChildrenList_.end() )
		{
			return;
		}

		ChildrenList_.erase(itor, ChildrenList_.end());
		child->ParentWPtr_.reset();

		assert( DataSceneNode_ && child->DataSceneNode_ );

		DataSceneNode_->removeChild(child->DataSceneNode_.get());

	}

	//�ڸ��ڵ㵱���Ƴ��Լ�
	void	RemoveFromParent()
	{
		auto spParent = ParentWPtr_.lock();
		if ( !spParent )
		{
			return;
		}

		spParent->RemoveChild(shared_from_this());

		assert( DataSceneNode_ && spParent->DataSceneNode_ );

		spParent->DataSceneNode_->removeChild(DataSceneNode_.get());
	}

	//�����ӽڵ�
	void	AddChild(SceneNodeSPtr child)
	{
		auto itor = std::find(ChildrenList_.begin(), ChildrenList_.end(), child );
		if ( itor != ChildrenList_.end() )
		{
			return;
		}

		child->RemoveFromParent();
		child->ParentWPtr_ = shared_from_this();
		ChildrenList_.push_back(child);

		assert( DataSceneNode_ && child->DataSceneNode_ );

		DataSceneNode_->addChild(child->DataSceneNode_.get());
	}

	//���ø��ڵ�
	void	SetParent(SceneNodeWPtr val)
	{
		if ( ParentWPtr_ == val )
		{
			return;
		}

		{
			auto spParent = ParentWPtr_.lock();
			if ( spParent )
			{
				spParent->RemoveChild(shared_from_this());
			}
		}

		ParentWPtr_ = val;
		
		{
			auto spParent = ParentWPtr_.lock();
			if ( spParent )
			{
				spParent->AddChild(shared_from_this());
			}
		}
	}

public:

	CombineSceneNodeSPtr	GetDataSceneNode() const { return DataSceneNode_; }

	void					SetDataSceneNode(CombineSceneNodeSPtr val)
	{
		if ( DataSceneNode_ )
		{
			DataSceneNode_->drop();
		}

		DataSceneNode_ = val;
		if ( DataSceneNode_ )
		{
			DataSceneNode_->grab();
		}
	}

private:

	SceneNodeWPtr	ParentWPtr_;
	ChildrenList	ChildrenList_;

	//��ά�ڵ�
	CombineSceneNodeSPtr	DataSceneNode_;
};

#endif // TSceneNode_h__