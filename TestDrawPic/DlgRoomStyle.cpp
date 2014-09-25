// DlgRoomStyle.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDrawPic.h"
#include "DlgRoomStyle.h"
#include "afxdialogex.h"


// CDlgRoomStyle 对话框

IMPLEMENT_DYNAMIC(CDlgRoomStyle, CDialogEx)

CDlgRoomStyle::CDlgRoomStyle(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgRoomStyle::IDD, pParent)
{
	m_spRoomStyle=std::make_shared<CRoomStyleBLL>();
	m_pDefaultBitmap=new CBitmap();
	m_nImgWidth = 116;
	m_nImgHeight = 87;
	m_nIndex = -1;
}

CDlgRoomStyle::~CDlgRoomStyle()
{
	m_pDefaultBitmap->Detach();
	delete m_pDefaultBitmap;
}

void CDlgRoomStyle::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ROOM_STYLE, m_lstRoomStyle);
}


BEGIN_MESSAGE_MAP(CDlgRoomStyle, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgRoomStyle::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgRoomStyle 消息处理程序


BOOL CDlgRoomStyle::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_lstRoomStyle.SetView(LV_VIEW_ICON);

	//m_lstSelMat.SetBkColor(RGB(0,0,0));

	::SetWindowLong( m_lstRoomStyle.GetSafeHwnd(), GWL_STYLE, WS_CHILD | WS_VISIBLE | WS_HSCROLL); 
	m_lstRoomStyle.ModifyStyle(NULL, LVS_SHOWSELALWAYS | LVS_SINGLESEL);
	m_lstRoomStyle.SetExtendedStyle(m_lstRoomStyle.GetExtendedStyle()|LVS_EX_GRIDLINES);
	// TODO:  在此添加额外的初始化
	m_ImageList.DeleteImageList();
	m_ImageList.Create(m_nImgWidth, m_nImgHeight, ILC_COLOR24|ILC_MASK,0,1);
	m_ImageList.SetBkColor(RGB(123,45,7));
	m_lstRoomStyle.SetImageList(&m_ImageList,LVSIL_NORMAL);
	m_lstRoomStyle.SetIconSpacing(CSize(120,120));
	INT nIndex = 0;
	CDrawImageEngine::GetInstance().Create(m_lstRoomStyle.GetDC()->GetSafeHdc(), static_cast<Gdiplus::REAL>(m_nImgWidth),static_cast<Gdiplus::REAL>(m_nImgHeight));
	std::vector<CRoomStyleLineBLLSPtr> arrLine;
	m_spRoomStyle->GetLineList(arrLine);
	for (auto& curObj :  arrLine)
	{
		CArray<Gdiplus::Point> st;
		
		std::vector<Gdiplus::PointF> arrPoint;
		CRoomStyleLineBLLSPtr sp1 = std::dynamic_pointer_cast<CRoomStyleLineBLL>(curObj);
		sp1->GetPointList(nIndex, arrPoint);
		
		for (auto& curItem : arrPoint)
		{
			Gdiplus::Point pt;
			pt.X = static_cast<INT>(curItem.X);
			pt.Y = static_cast<INT>(curItem.Y);
			st.Add(pt);
		}
		nIndex = CDrawImageEngine::GetInstance().CreateImage(st, m_nImgWidth, m_nImgHeight, m_ImageList);
		m_lstRoomStyle.InsertItem(nIndex, _T(""), nIndex );
		nIndex++;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgRoomStyle::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_lstRoomStyle.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{

	}
	else
	{
		while (pos)
		{
			m_nIndex = m_lstRoomStyle.GetNextSelectedItem(pos);
			if (m_nIndex>=0)
			{
				break;
			}
		}
	}
	CDialogEx::OnOK();
}

void CDlgRoomStyle::GetPointList( std::vector<Gdiplus::PointF>& arrPoint )
{	
	std::vector<CRoomStyleLineBLLSPtr> arrLine;
	m_spRoomStyle->GetLineList(arrLine);
	if (arrLine.size()>0)
	{
		arrLine[m_nIndex]->GetPointList(m_nIndex, arrPoint);
	}
}
