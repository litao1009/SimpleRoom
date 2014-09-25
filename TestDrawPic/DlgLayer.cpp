// DlgLayer.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "DlgLayer.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "TestDrawPicView.h"
// CDlgLayer 对话框

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


// CDlgLayer 消息处理程序


BOOL CDlgLayer::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_treeObj.ModifyStyle(NULL,TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT); 
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgLayer::Refresh(CView* pActiveView, CBaseODLSPtr spObj)
{
	m_pActiveView = pActiveView;
	m_treeObj.DeleteAllItems();
	//HTREEITEM hItem = m_treeObj.InsertItem(_T("场景"));
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
			strName=_T("相机");
		}
		break;
	case EOBT_OBJ_Group:
		{
			strName=_T("分组");
		}
		break;
	case EOBT_Wall:
		{
			int nIndex = spSub->GetIndex();
			strName.Format(_T("墙%d"),  nIndex);
		}
		break;
	case EOBT_Wall_Face:
		{
			//判断上级对象类型
			CBaseODLSPtr spParent = spSub->GetParent().lock();
			switch (spParent->GetType())
			{
			case EOBT_Wall:
				{
					int nIndex = spSub->GetIndex();
					if ((nIndex!=1) && (nIndex!=3))
					{
						//第一、3面墙可以编辑，其他不编辑
						return;
					}
					strName.Format(_T("墙面%d"), nIndex);
				}
				break;
			case EOBT_Beam:
				{
					if (spSub->GetSide()==ESP_Up)
					{
						//顶上不能编辑
						return;
					}
					int nIndex = spSub->GetIndex();
					strName.Format(_T("梁面%d"), nIndex);
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
					strName.Format(_T("柱面%d"), nIndex);
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
			strName=_T("墙面区域");
		}
		break;
	case EOBT_Beam:
		{
			strName=_T("梁");
		}
		break;
	case EOBT_Pillar:
		{
			strName=_T("柱");
		}
		break;
	case EOBT_Floor:
		{
			strName=_T("地板");
		}
		break;
	case EOBT_Floor_Zone:
		{
			strName=_T("地板区域");
		}
		break;
	case EOBT_Sky:
		{
			strName=_T("天花板");
		}
		break;
	case EOBT_Sky_Zone:
		{
			strName=_T("天花板区域");
		}
		break;
	case EOBT_Decor:
		{
			strName=_T("装饰品");
		}
		break;
	case EOBT_Design:
		{
			strName=_T("设计方案");
		}
		break;
	default:
		{
			strName=_T("其他");
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
	// TODO: 在此添加控件通知处理程序代码
	
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
				//其他的，全部取消焦点状态
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
				//其他的，全部取消焦点状态
				m_spRoot->SetIsTopFocused(false, true);
				spRet->SetIsTopFocused(true);
				if (m_pActiveView)
				{
					m_pActiveView->Invalidate();
					//最好能够直接切换这个东西到编辑状态
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
