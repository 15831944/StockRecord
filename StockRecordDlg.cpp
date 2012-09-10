
// StockRecordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StockRecord.h"
#include "StockRecordDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CStockRecordDlg 对话框




CStockRecordDlg::CStockRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStockRecordDlg::IDD, pParent)
	, m_pDatabase(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStockRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_GRID, m_GridCtrl);
}

BEGIN_MESSAGE_MAP(CStockRecordDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CStockRecordDlg 消息处理程序

BOOL CStockRecordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	SetupDBTableNames();
	OpenDatabase();

	/* Some features of grid. */
	m_GridCtrl.SetFixedColumnCount(1);
	m_GridCtrl.SetFixedRowCount(1);
	m_GridCtrl.SetColumnCount(7);
	m_GridCtrl.SetRowCount();//
	// TODO: Set fixed bk & text color.
	// m_GridCtrl.SetFixedBkColor()

	m_GridCtrl.SetColumnResize(TRUE);		// Column can resize.
	m_GridCtrl.SetRowResize(FALSE);			// Row cannot resize.
	m_GridCtrl.SetAutoSizeStyle(GVS_BOTH);	// Auto size
	m_GridCtrl.SetEditable(FALSE);			// Cannot edit

	// TODO: in OnInitDialog() 
	/** Same as ReadHoldRecord
	 * 1. Read records in stock_buy table. 
	 * 2. Get how many columns and row of the data.
	 * 3. Set the number of grid's column and row.
	 * 4. Set the data.
	 */

	//m_GridCtrl.ShowWindow(SW_HIDE);		// If no data, make grid invisible.

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CStockRecordDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CStockRecordDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CStockRecordDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int CStockRecordDlg::ReadStockHoldRecords(void)
{
	
	return 0;
}


int CStockRecordDlg::SetupDBTableNames(void)
{
	m_strDBName.assign("StockRecord.db");
	m_strBuyTableName.assign("stock_buy");
	m_StrHoldTableName.assign("stock_hold");
	m_strSellTableName.assign("stock_sell");
	m_strMoneyTableName.assign("stock_money");
	return OK;
}

int CStockRecordDlg::OpenDatabase(void)
{
	if (m_strDBName.empty()) {
		MessageBox("Database'a name is empty.", "Oops");
		return BAD_DB_TABLE_NAME;
	}

	if (m_pDatabase != NULL) {
		MessageBox("Database is already opened.", "Tip");
		return OK;
	}

	// TODO: Open DB in read-only by using _v2 and ...
	int ret = 0;
	ret = sqlite3_open(m_strDBName.c_str(), &m_pDatabase);

	if (ret != SQLITE_OK) {
		MessageBox("Open database error.", "Error");
		return ret;
	}

	return OK;
}

int CStockRecordDlg::CloseDatabase( void )
{
	if (!m_pDatabase)
		return OK;
	
	int ret = 0;
	ret = sqlite3_close(m_pDatabase);
	if (ret == SQLITE_OK) {
		m_pDatabase = NULL;
		return SQLITE_OK;
	}
	return ret;
}

void CStockRecordDlg::OnClose(void)
{
	CDialogEx::OnClose();
}

void CStockRecordDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	/* Close database */
	CloseDatabase();
}
