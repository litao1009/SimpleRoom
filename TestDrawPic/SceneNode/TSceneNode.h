#ifndef TSceneNode_h__
#define TSceneNode_h__

#pragma once

#include "gp_Trsf.hxx"
#include "gp_GTrsf.hxx"

#include "SceneNode/CDataSceneNode.h"

#include <memory>
#include <vector>

template<typename T>
class	TSceneNode : public std::enable_shared_from_this<T>
{
public:

	TSceneNode():m_Scale(1,1,1)
	{
		m_NeedUpdateTransformation = false;
	}

	~TSceneNode()
	{
		if ( m_spSceneNode )
		{
			m_spSceneNode->drop();
		}
	}

	typedef	typename T							Reference;
	typedef	typename std::shared_ptr<Reference>	SceneNodeSPtr;
	typedef	typename std::weak_ptr<Reference>	SceneNodeWPtr;
	typedef	typename std::vector<SceneNodeSPtr>	ChildrenList;

public:

	void	InitByParent(SceneNodeSPtr parent, std::string dbgName="")
	{
		assert(parent);
		
		auto pntDN = parent->GetSceneNode();
		if ( pntDN )
		{
			auto rcWPtr = pntDN->GetRenderContext();

			auto newNode = new CDataSceneNode(rcWPtr, shared_from_this(), dbgName);
			auto spNewNode = CDataSceneNode::TranslateNodeToSPtr(newNode);

			auto cur = GetSceneNode();
			assert( nullptr == cur );
			if ( cur )
			{
				cur->remove();
			}

			SetSceneNode(spNewNode);
		}

		parent->AddChild(shared_from_this());
	}

	//��ȡ���ڵ�
	SceneNodeWPtr GetParent() const
	{
		return m_wpParent;
	}

	//��ȡ�ӽڵ�
	ChildrenList& GetChildrenList()
	{
		return m_ChildrenList;
	}

	const ChildrenList& GetChildrenList() const
	{
		return m_ChildrenList;
	}

	//�Ƴ��ӽڵ�
	void	RemoveChild(SceneNodeSPtr child)
	{
		if ( !child )
		{
			return;
		}

		auto itor = std::remove(m_ChildrenList.begin(), m_ChildrenList.end(), child);

		if ( itor == m_ChildrenList.end() )
		{
			return;
		}

		m_ChildrenList.erase(itor, m_ChildrenList.end());
		child->m_wpParent.reset();

		if ( m_spSceneNode && child->m_spSceneNode )
		{
			auto rawParent = m_spSceneNode.get();
			auto rawChild = child->m_spSceneNode.get();

			rawParent->removeChild(rawChild);
		}
	}

	//�ڸ��ڵ㵱���Ƴ��Լ�
	void	RemoveFromParent()
	{
		auto spParent = m_wpParent.lock();
		if ( !spParent )
		{
			return;
		}

		spParent->RemoveChild(shared_from_this());

		if ( m_spSceneNode && spParent->m_spSceneNode )
		{
			auto rawParent = spParent->m_spSceneNode.get();
			auto rawChild = m_spSceneNode.get();
			rawParent->removeChild(rawChild);
		}
	}

	//�����ӽڵ�
	void	AddChild(SceneNodeSPtr child)
	{
		auto itor = std::find(m_ChildrenList.begin(), m_ChildrenList.end(), child );
		if ( itor != m_ChildrenList.end() )
		{
			return;
		}

		child->RemoveFromParent();
		child->m_wpParent = SceneNodeWPtr(shared_from_this());
		m_ChildrenList.push_back(child);

		if ( m_spSceneNode && child->m_spSceneNode )
		{
			m_spSceneNode->addChild(child->m_spSceneNode.get());
		}
	}

	//���ø��ڵ�
	void	SetParent(SceneNodeWPtr val)
	{
		if ( m_wpParent == val )
		{
			return;
		}

		{
			auto spParent = m_wpParent.lock();
			if ( spParent )
			{
				spParent->RemoveChild(shared_from_this());
			}
		}

		m_wpParent = val;
		
		{
			auto spParent = m_wpParent.lock();
			if ( spParent )
			{
				spParent->AddChild(shared_from_this());
			}
		}
	}

	//���ø��ڵ�
	void	SetParent(SceneNodeSPtr val)
	{
		auto wpParent = SceneNodeWPtr(val);
		SetParent(wpParent);
	}

public:

	//��ȡƫ��
	const gp_Vec& GetTranslation() const
	{
		return m_Translation;
	}

	//����ƫ��
	void SetTranslation( const gp_Vec& val)
	{
		if ( !m_Translation.IsEqual(val, Precision::Confusion()) )
		{
			m_NeedUpdateTransformation = true;
		}

		m_Translation = val;
	}

	//��ȡ��ת
	const gp_Vec& GetRotation() const
	{
		return m_Rotation;
	}


	//������ת
	void SetRotation( const gp_Vec& val)
	{
		if ( !m_Rotation.IsEqual(val, Precision::Confusion()) )
		{
			m_NeedUpdateTransformation = true;
		}

		m_Rotation = val;
	}


	//��ȡ����
	const gp_Vec& GetScale() const
	{
		return m_Scale;
	}

	//��������
	void SetScale( const gp_Vec& val)
	{
		if ( !m_Scale.IsEqual(val, Precision::Confusion()) )
		{
			m_NeedUpdateTransformation = true;
		}

		m_Scale = val;
	}

	//���õȱ�������
	void	SetScaleFactor(Standard_Real val)
	{
		gp_Vec vec(val,val,val);
		SetScale(vec);
	}

	//��ȡ��Ծ���
	const gp_Trsf&	GetRelateTransformation() const
	{
		if ( m_NeedUpdateTransformation )
		{
			gp_Trsf t,r,s;

			t.SetTranslation(m_Translation);
			
			gp_Trsf rX,rY,rZ;
			rX.SetRotation(gp::OX(), m_Rotation.X() * 180 * M_1_PI);
			rY.SetRotation(gp::OY(), m_Rotation.Y() * 180 * M_1_PI);
			rZ.SetRotation(gp::OZ(), m_Rotation.Z() * 180 * M_1_PI);
			r = rZ * rY * rX;

			auto max = std::max(m_Scale.X(), m_Scale.Y());
			max = std::max(max, m_Scale.Z());

			auto min = std::min(m_Scale.X(), m_Scale.Y());
			min = std::min(min, m_Scale.Z());

			if ( std::abs(max-min) < Precision::Confusion() )
			{
				s.SetScaleFactor(max);
			}
			else
			{
				gp_GTrsf gs;
				gs.SetValue(1,1, m_Scale.X());
				gs.SetValue(2,2, m_Scale.Y());
				gs.SetValue(3,3, m_Scale.Z());
				gs.SetForm();
				
				s = gs.Trsf();
				s.SetScaleFactor(1);
			}

			m_Translation = t * r * s;

			m_NeedUpdateTransformation = false;
		}

		return m_Translation;
	}

	//��ȡ���Ծ���
	gp_Trsf			GetAbsoluteTransformation()
	{
		auto spParent = m_wpParent.lock();
		if ( spParent )
		{
			return spParent->GetAbsoluteTransformation() * GetTranslation();
		}
		else
		{
			return GetTranslation();
		}
	}

public:

	CDataSceneNodeSPtr GetSceneNode() const { return m_spSceneNode; }
	void			SetSceneNode(CDataSceneNodeSPtr val) { m_spSceneNode = val; }

private:

	SceneNodeWPtr	m_wpParent;
	ChildrenList	m_ChildrenList;

	//��ά�ڵ�
	CDataSceneNodeSPtr	m_spSceneNode;

private:

	gp_Vec			m_Translation;
	gp_Vec			m_Rotation;
	gp_Vec			m_Scale;
	bool			m_NeedUpdateTransformation;
	gp_Trsf			m_Transformation;
};

#endif // TSceneNode_h__