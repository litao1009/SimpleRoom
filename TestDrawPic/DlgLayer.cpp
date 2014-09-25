// DlgLayer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "DlgLayer.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "TestDrawPicView.h"
// CDlgLayer �Ի���

IMPLEMENT_DYNAMIC(CDlgLayer, CDialogEx)

CDlgLayer::CDlgLayer(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgLayer::IDD, pParent)
{
	m_spRoot = nullptr;
	m_pActiveView= nullptr;
}

CDlgLayer::~CDlgLayer()
{
}

void CDlgLayer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_OBJ, m_treeObj);
}


BEGIN_MESSAGE_MAP(CDlgLayer, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_OBJ, &CDlgLayer::OnTvnSelchangedTreeObj)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_OBJ, &CDlgLayer::OnNMDblclkTreeObj)
END_MESSAGE_MAP()


// CDlgLayer ��Ϣ�������


BOOL CDlgLayer::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_treeObj.ModifyStyle(NULL,TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT); 
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgLayer::Refresh(CView* pActiveView, CBaseODLSPtr spObj)
{
	m_pActiveView = pActiveView;
	m_treeObj.DeleteAllItems();
	//HTREEITEM hItem = m_treeObj.InsertItem(_T("����"));
	if (!spObj)
	{
		return;
	}
	m_spRoot = spObj;
	UpdateTree(TVI_ROOT, spObj);
}

void CDlgLayer::UpdateTree( HTREEITEM hItem, CBaseODLSPtr spSub )
{
	ENUM_OBOJECT_TYPE eot = spSub->GetType();
	CString strName=_T("");
	switch (eot)
	{
	case EOBT_Camera:
		{
			strName=_T("���");
		}
		break;
	case EOBT_OBJ_Group:
		{
			strName=_T("����");
		}
		break;
	case EOBT_Wall:
		{
			int nIndex = spSub->GetIndex();
			strName.Format(_T("ǽ%d"),  nIndex);
		}
		break;
	case EOBT_Wall_Face:
		{
			//�ж��ϼ���������
			CBaseODLSPtr spParent = spSub->GetParent().lock();
			switch (spParent->GetType())
			{
			case EOBT_Wall:
				{
					int nIndex = spSub->GetIndex();
					if ((nIndex!=1) && (nIndex!=3))
					{
						//��һ��3��ǽ���Ա༭���������༭
						return;
					}
					strName.Format(_T("ǽ��%d"), nIndex);
				}
				break;
			case EOBT_Beam:
				{
					if (spSub->GetSide()==ESP_Up)
					{
						//���ϲ��ܱ༭
						return;
					}
					int nIndex = spSub->GetIndex();
					strName.Format(_T("����%d"), nIndex);
				}
				break;
			case EOBT_Pillar:
				{
					ENUM_SIDE_POS esp = spSub->GetSide();
					if ((esp==ESP_Up) ||(esp==ESP_Down))
					{
						return;
					}
					int nIndex = spSub->GetIndex();
					strName.Format(_T("����%d"), nIndex);
				}
				break;
			default:
				{
					return;
				}
				break;
			}
			
		}
		break;
	case EOBT_Wall_Face_Zone:
		{
			strName=_T("ǽ������");
		}
		break;
	case EOBT_Beam:
		{
			strName=_T("��");
		}
		break;
	case EOBT_Pillar:
		{
			strName=_T("��");
		}
		break;
	case EOBT_Floor:
		{
			strName=_T("�ذ�");
		}
		break;
	case EOBT_Floor_Zone:
		{
			strName=_T("�ذ�����");
		}
		break;
	case EOBT_Sky:
		{
			strName=_T("�컨��");
		}
		break;
	case EOBT_Sky_Zone:
		{
			strName=_T("�컨������");
		}
		break;
	case EOBT_Decor:
		{
			strName=_T("װ��Ʒ");
		}
		break;
	case EOBT_Design:
		{
			strName=_T("��Ʒ���");
		}
		break;
	default:
		{
			strName=_T("����");
		}
		break;
	}
	HTREEITEM hSubItem = m_treeObj.InsertItem(strName,-1,-1,hItem); 
	m_treeObj.SetItemData(hSubItem, (DWORD_PTR)spSub.get());
	for (auto& curItem:spSub->GetChildrenList())
	{
		UpdateTree(hSubItem, curItem);
	}
	m_treeObj.Expand(hSubItem,TVE_EXPAND|TVE_TOGGLE);
	
}


void CDlgLayer::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (!m_treeObj.GetSafeHwnd())
	{
		return;
	}
	CRect rtMain;
	GetClientRect(rtMain);
	CRect rtTree;
	rtTree.left = 1;
	rtTree.right = rtMain.Width() -1;
	rtTree.top = 1;
	rtTree.bottom = rtMain.Height() - 1;
	m_treeObj.MoveWindow(rtTree);
}


void CDlgLayer::OnTvnSelchangedTreeObj(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	HTREEITEM itmX = m_treeObj.GetSelectedItem();
	if (itmX)
	{
		DWORD_PTR ptrRet = m_treeObj.GetItemData(itmX);
		if (ptrRet)
		{
			CBaseODL* spLock=(CBaseODL*)ptrRet;
			if (spLock)
			{
				CBaseODLSPtr spRet= spLock->shared_from_this();
				//�����ģ�ȫ��ȡ������״̬
				m_spRoot->SetIsTopFocused(false, true);
				spLock->SetIsTopFocused(true);
				if (m_pActiveView)
				{
					m_pActiveView->Invalidate();
				}
			}
		}
	}
	*pResult = 0;
}


void CDlgLayer::OnNMDblclkTreeObj(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM itmX = m_treeObj.GetSelectedItem();
	if (itmX)
	{
		DWORD_PTR ptrRet = m_treeObj.GetItemData(itmX);
		if (ptrRet)
		{
			CBaseODL* spLock=(CBaseODL*)ptrRet;
			if (spLock)
			{
				CBaseODLSPtr spRet= spLock->shared_from_this();
				//�����ģ�ȫ��ȡ������״̬
				m_spRoot->SetIsTopFocused(false, true);
				spRet->SetIsTopFocused(true);
				if (m_pActiveView)
				{
					m_pActiveView->Invalidate();
					//����ܹ�ֱ���л�����������༭״̬
					if (m_pActiveView->IsKindOf(RUNTIME_CLASS(CTestDrawPicView)))
					{
						CTestDrawPicView* pDrawView = (CTestDrawPicView*)m_pActiveView;
						pDrawView->SwitchToEdit(spRet);
					}
				}
			}
		}
	}
	*pResult = 0;
}
