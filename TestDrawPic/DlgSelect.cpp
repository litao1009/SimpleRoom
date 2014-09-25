// DlgSelect.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "DlgSelect.h"
#include "afxdialogex.h"

#include <tuple>

// CDlgSelect 对话框

IMPLEMENT_DYNAMIC(CDlgSelect, CBaseDialog)

CDlgSelect::CDlgSelect(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgSelect::IDD, pParent)
{
	
}

CDlgSelect::~CDlgSelect()
{
	m_bmpDefaultBitmap.Detach();
}

void CDlgSelect::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBX_GTYPE, m_cbxGType);
	DDX_Control(pDX, IDC_LIST_OBJ, m_lstObj);
	DDX_Control(pDX, IDC_RDO_EDIT_ZONE, m_rdoEditZone);
	DDX_Control(pDX, IDC_RDO_EDIT_SKIN, m_rdoEditSkin);
}


BEGIN_MESSAGE_MAP(CDlgSelect, CBaseDialog)
	ON_CBN_SELCHANGE(IDC_CBX_GTYPE, &CDlgSelect::OnCbnSelchangeCbxGtype)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RDO_EDIT_ZONE, &CDlgSelect::OnBnClickedRdoEditZone)
	ON_BN_CLICKED(IDC_RDO_EDIT_SKIN, &CDlgSelect::OnBnClickedRdoEditSkin)
END_MESSAGE_MAP()


// CDlgSelect 消息处理程序

void CDlgSelect::InitObjList(ENUM_Group_Type eGTypeID)
{
	m_imgList.DeleteImageList();
	
	m_nImgWidth=110;
	m_nImgHeight = 120;
	m_imgList.Create(m_nImgWidth, m_nImgHeight, ILC_COLOR24|ILC_MASK,0,1);
	m_imgList.SetBkColor(GetSysColor( COLOR_WINDOW));
	m_lstObj.SetImageList(&m_imgList,LVSIL_NORMAL);
	m_lstObj.SetIconSpacing(CSize(120,130));
	m_lstObj.DeleteAllItems();
	m_arrObjList.clear();
	if (eGTypeID<EGT_RoomDoor)
	{
		std::vector<CSkinDAL::StSkinInfo> arrSkin;
		CDataManager::GetInstance().m_spSkinMgr->GetSkinList(0,0,0, eGTypeID, arrSkin);
		//皮肤
		for (auto& curItem:arrSkin)
		{
			StObjInfo stInfo;
			stInfo.GTypeID = eGTypeID;
			stInfo.ValueID = curItem.SkinID;
			stInfo.XLen = curItem.XLen;
			stInfo.YLen = curItem.YLen;
			stInfo.ZLen = 0.0f;
			_tcscpy_s(stInfo.ObjName, curItem.SkinName);
			CString strPath;
			strPath.Format(_T("../data/resource/skin/%s"), curItem.SkinPicPath);
			_tcscpy_s(stInfo.ShowPicPath, strPath);
			_tcscpy_s(stInfo.SkinPath, strPath);
			_tcscpy_s(stInfo.ModelPath, _T(""));
			m_arrObjList.push_back(stInfo);
		}
	}
	else
	{
		std::vector<CDecorDAL::StDecorInfo> arrDecor;
		CDataManager::GetInstance().m_spDecorMgr->GetDecorList(0,0,0, eGTypeID, arrDecor);
		//模型
		for (auto& curDecor: arrDecor)
		{
			StObjInfo stInfo;
			stInfo.GTypeID = eGTypeID;
			stInfo.ValueID = curDecor.DecorID;
			stInfo.XLen = curDecor.XLen;
			stInfo.YLen = curDecor.YLen;
			stInfo.ZLen = curDecor.ZLen;
			CString strTmp;
			_tcscpy_s(stInfo.ObjName, curDecor.DecorName);
			strTmp.Format(_T("../data/resource/Skin/%s"), curDecor.ShowPicPath );
			_tcscpy_s(stInfo.ShowPicPath, strTmp);
			_tcscpy_s(stInfo.SkinPath, strTmp);
			strTmp.Format(_T("../data/resource/models/%s"), curDecor.ModelFilePath);
			_tcscpy_s(stInfo.ModelPath, strTmp);
			m_arrObjList.push_back(stInfo);
		}
	}
	int i=0;
	for (auto& it: m_arrObjList)
	{
		int nRet = this->CreateImage(it.ShowPicPath, m_nImgWidth, m_nImgHeight, m_imgList);
		if (nRet==-1)
		{
			i = m_imgList.Add(&m_bmpDefaultBitmap,RGB(120,120,120));
		}
		if (m_lstObj.InsertItem(i, it.ObjName, i))
		{
		}
		i++;
	}
}

BOOL CDlgSelect::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	m_nImgWidth = 116;
	m_nImgHeight = 87;
	//图显示
	m_lstObj.SetView(LV_VIEW_ICON);

	//m_lstSelMat.SetBkColor(RGB(0,0,0));

	::SetWindowLong( m_lstObj.GetSafeHwnd(), GWL_STYLE, WS_CHILD | WS_VISIBLE | WS_HSCROLL); 
	m_lstObj.ModifyStyle(NULL, LVS_SHOWSELALWAYS | LVS_SINGLESEL);
	m_lstObj.SetExtendedStyle(m_lstObj.GetExtendedStyle()|LVS_EX_GRIDLINES);
	CString strDefImage=_T("");
	strDefImage.Format(_T("%s\\default.png"), _T("../Data/Resource/"));
	CreateImage(strDefImage, m_nImgWidth, m_nImgHeight, m_bmpDefaultBitmap);
	
	
	m_arrGType.emplace_back(EGT_GroundTiles, _T("地板砖"));
	m_arrGType.emplace_back(EGT_FloorBoard, _T("木地板"));
	m_arrGType.emplace_back(EGT_Buckle, _T("扣条"));
	m_arrGType.emplace_back(EGT_WallPaper, _T("墙纸"));
 	m_arrGType.emplace_back(EGT_WallBrick, _T("墙砖"));
	m_arrGType.emplace_back(EGT_BeltCourse, _T("腰线"));
	m_arrGType.emplace_back(EGT_RoomDoor, _T("房间门"));
	m_arrGType.emplace_back(EGT_RoomWindow, _T("房间窗"));
	m_arrGType.emplace_back(EGT_BayWindow, _T("飘窗台"));
// 	m_arrGType.push_back(_T("自定义墙砖组合"));
// 	m_arrGType.push_back(_T("自定义地板砖组合"));
	int i=0;
	ENUM_Group_Type eGTypeID = EGT_GroundTiles;
	for (auto& it: m_arrGType)
	{
		auto elm = std::get<1>(it);
 		m_cbxGType.InsertString(i, elm);
 		i++;
	}
	if (i>0)
	{
		m_cbxGType.SetCurSel(0);
		eGTypeID = std::get<0>(m_arrGType[0]);
	}
	m_rdoEditZone.SetCheck(1);
	m_rdoEditSkin.SetCheck(0);
	InitObjList(eGTypeID);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgSelect::OnCbnSelchangeCbxGtype()
{
	//切换
	int nCurSel = m_cbxGType.GetCurSel();
	if (nCurSel>=0)
	{
		auto& nGTypeID = std::get<0>(m_arrGType[nCurSel]);
		InitObjList(nGTypeID);
	}
}


void CDlgSelect::OnSize(UINT nType, int cx, int cy)
{
	CBaseDialog::OnSize(nType, cx, cy);
	if (!m_lstObj.GetSafeHwnd())
	{
		return;
	}
	CRect rtMain;
	GetWindowRect(rtMain);

	CRect rtCbx;
	rtCbx.left = 5;
	rtCbx.right = rtMain.Width() - 10;
	rtCbx.top = 5;
	rtCbx.bottom = 500;
	m_cbxGType.MoveWindow(rtCbx);
	m_cbxGType.SetItemHeight(-1, 14);

	CRect rtRdo;
	rtRdo.left = 5;
	rtRdo.top = 25;
	rtRdo.right = rtRdo.left+ 150;
	rtRdo.bottom = rtRdo.top + 22;
	m_rdoEditZone.MoveWindow(rtRdo);

	rtRdo.left = rtRdo.right+5;
	rtRdo.right = rtRdo.left+ 150;
	m_rdoEditSkin.MoveWindow(rtRdo);
	
	CRect rtList;
	rtList.left = 5;
	rtList.right = rtMain.Width() - 10;
	rtList.top = rtRdo.bottom+5;
	rtList.bottom = rtMain.Height()-5;
	m_lstObj.MoveWindow(rtList);
}

#define FreePointer(s) if(s){delete s; s=nullptr;}
LRESULT CDlgSelect::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message==WM_USER+1000)
	{
		int nCurSel = -1;
		
		POSITION pos = m_lstObj.GetFirstSelectedItemPosition();
		if (pos != NULL)
		{
			while (pos)
			{
				nCurSel = m_lstObj.GetNextSelectedItem(pos);
				if (nCurSel>=0)
				{
					break;
				}
			}
		}
		if (nCurSel<0)
		{
			return 0;
		}
		StObjInfo* ddsInfo=new StObjInfo();

		CopyMemory(ddsInfo, &m_arrObjList[nCurSel], sizeof(StObjInfo));
		ddsInfo->IsSkin = m_rdoEditZone.GetCheck()==0;
		COleDataSource* pDS=new COleDataSource();
		INT nLen=sizeof(StObjInfo);
		HGLOBAL hData=GlobalAlloc(GHND|GMEM_SHARE,nLen);
		if (hData)
		{
			memcpy((void*)GlobalLock(hData),ddsInfo,nLen);
			GlobalUnlock(hData);
			pDS->CacheGlobalData(CF_TEXT,hData);
			DROPEFFECT dropeffect = pDS->DoDragDrop(DROPEFFECT_COPY|DROPEFFECT_MOVE);
		}
		GlobalFree(hData);
		hData=NULL;
		FreePointer(pDS);
		FreePointer(ddsInfo);
	}
	return CBaseDialog::DefWindowProc(message, wParam, lParam);
}


BOOL CDlgSelect::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	return CBaseDialog::PreTranslateMessage(pMsg);
}


void CDlgSelect::OnBnClickedRdoEditZone()
{
	if (m_rdoEditZone.GetCheck())
	{
		
	}
}


void CDlgSelect::OnBnClickedRdoEditSkin()
{
	if (m_rdoEditZone.GetCheck())
	{

	}
}
