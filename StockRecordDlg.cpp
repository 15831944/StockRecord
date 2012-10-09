
// StockRecordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StockRecord.h"
#include "StockRecordDlg.h"
#include "afxdialogex.h"

#include "FieldNamesMap.h"
#include "StockCalculate.h"

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
public:
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void CAboutDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

// CStockRecordDlg 对话框




CStockRecordDlg::CStockRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStockRecordDlg::IDD, pParent)
	, m_pTrayIcon(NULL)
	, m_bIsWndHidden(false)
	, m_bIsPlanSell(false)
	, m_pPlanBuyDlg(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CStockRecordDlg::~CStockRecordDlg(void)
{
	delete(m_pTrayIcon);
	m_pTrayIcon = NULL;

	delete m_pPlanBuyDlg;
	m_pPlanBuyDlg = NULL;
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
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_MENU_HOLD_RECORD, &CStockRecordDlg::OnMenuHoldRecord)
	ON_COMMAND(ID_MENU_BUY_RECORD, &CStockRecordDlg::OnMenuBuyRecord)
	ON_COMMAND(ID_MENU_SELL_RECORD, &CStockRecordDlg::OnMenuSellRecord)
	ON_COMMAND(ID_MENU_MONEY_RECORD, &CStockRecordDlg::OnMenuMoneyRecord)
	ON_BN_CLICKED(IDC_BT_EXIT, &CStockRecordDlg::OnBnClickedExit)
	ON_NOTIFY(NM_RCLICK, IDC_GRID, &CStockRecordDlg::OnGridRClick)
	ON_NOTIFY(NM_DBLCLK, IDC_GRID, &CStockRecordDlg::OnGridDBClick)
	ON_COMMAND_RANGE(IDM_STOCKBUY_REMOVE, IDM_STOCKMONEY_REMOVE,	\
		&CStockRecordDlg::OnMenuRemoveRecord)
	ON_COMMAND(IDM_STOCKBUY_ADD, &CStockRecordDlg::OnStockbuyAdd)
	ON_COMMAND(IDM_STOCKHOLD_PLAN_SELL, &CStockRecordDlg::OnStockholdPlanSell)
	ON_COMMAND(IDM_STOCKHOLD_SELL, &CStockRecordDlg::OnStockholdSell)
	ON_COMMAND(IDM_STOCKMONEY_INOUT, &CStockRecordDlg::OnStockmoneyInout)
	ON_BN_CLICKED(IDC_BT_ABOUT, &CStockRecordDlg::OnBnClickedBtAbout)
	ON_COMMAND(IDM_ABOUT, &CStockRecordDlg::OnMenuAbout)
	ON_COMMAND(IDM_ALWAYS_TOP, &CStockRecordDlg::OnMenuAlwaysTop)
	ON_WM_CREATE()
	ON_MESSAGE(WM_ICON_NOTIFY, &CStockRecordDlg::OnTrayNotification)
	ON_COMMAND(ID_MENU_TRAYICON_SHOWWD, &CStockRecordDlg::OnMenuTrayiconShowHideWnd)
	ON_COMMAND(ID_MENU_TRAYICON_EXIT, &CStockRecordDlg::OnMenuTrayiconExit)
	ON_COMMAND(ID_MENU_PLANBUY, &CStockRecordDlg::OnMenuPlanbuy)
	ON_COMMAND(IDM_STOCKHOLD_PLANBUY, &CStockRecordDlg::OnStockholdPlanbuy)
	ON_MESSAGE(WM_HOTKEY, &CStockRecordDlg::OnHotKey)
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

	/* Register hot key: Alt + Shift + R. */
	if (!RegisterHotKey(this->GetSafeHwnd(), WM_STOCK_RECORD_HIDESHOW, 
		MOD_ALT | MOD_SHIFT , 'R'/*0x52*/)) {
		MessageBox("Register hot key failed.");
	}

	SetupDBTableNames();
	m_dbConn.Connect();

	/* Some features of grid's column and row. */
	m_GridCtrl.SetFixedBkColor(RGB(0xCD, 0xEE, 0xB7));
	m_GridCtrl.SetGridLineColor(RGB(0xFC, 0xD7, 0x4F));
	m_GridCtrl.SetColumnResize(TRUE);		// Column can resize.
	m_GridCtrl.SetRowResize(TRUE);			// Row cannot resize.
	m_GridCtrl.SetAutoSizeStyle(GVS_BOTH);	// Auto size.
	m_GridCtrl.SetEditable(FALSE);			// Cannot edit.
	m_GridCtrl.EnableDragAndDrop(FALSE);
	// TODO: color related. // m_GridCtrl.SetBkColor(RGB(0xFF, 0xFF, 0xFF)); 

	OnMenuHoldRecord();						// Query hold record when startup.
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

void CStockRecordDlg::SetLeftTopItemData( void )
{
	string str("");

	switch (m_dbConn.GetActiveTable()) {
	case ACTIVE_TABLE_BUY:
		str.assign("BUY");
		break;
	case ACTIVE_TABLE_HOLD:
		str.assign("HOLD");
		break;
	case ACTIVE_TABLE_SELL:
		str.assign("SELL");
		break;
	case ACTIVE_TABLE_MONEY:
		str.assign("MONEY");
		break;
	default:
		str.assign("ERR");
		break;
	}

	// Set data of (0, 0) item.
	m_GridCtrl.SetItemText(0, 0, str.c_str());

	// Set column width.
	CSize sz = m_GridCtrl.GetTextExtent(0, 0, str.c_str());
	if (sz.cx > 0) {
		m_GridCtrl.SetColumnWidth(0, (int)sz.cx + 1);
	}
}

void CStockRecordDlg::ClearRecordIds( void )
{
	switch (m_dbConn.GetActiveTable()) {
	case ACTIVE_TABLE_BUY:
		m_vecStockBuyIds.clear();
		break;
	case ACTIVE_TABLE_HOLD:
		m_vecStockHoldIds.clear();
		break;
	case ACTIVE_TABLE_SELL:
		m_vecStockSellIds.clear();
		break;
	case ACTIVE_TABLE_MONEY:
		m_vecStockMoneyIds.clear();
		break;
	default:
		break;
	}
}

void CStockRecordDlg::StoreRecordId( int id )
{
	switch (m_dbConn.GetActiveTable()) {
	case ACTIVE_TABLE_BUY:
		m_vecStockBuyIds.push_back(id);
		break;
	case ACTIVE_TABLE_HOLD:
		m_vecStockHoldIds.push_back(id);
		break;
	case ACTIVE_TABLE_SELL:
		m_vecStockSellIds.push_back(id);
		break;
	case ACTIVE_TABLE_MONEY:
		m_vecStockMoneyIds.push_back(id);
		break;
	default:
		break;
	}
}

/*
 *	Set gird data queried from sqlite3_get_table.
 *  nRow - How many rows of the data.
 *  nCol - How many columns of the data.
 *  result - All the queried data are stored in memory pointed by result.
 */
int 
CStockRecordDlg::SetGridData( int nRowCount, int nColCount, char** result )
{
	/* No data in database, ask to add */
	if (nRowCount <= 0 || nColCount <= 0 || !result) {
		m_GridCtrl.DeleteAllItems();
		m_GridCtrl.ShowWindow(SW_HIDE);

		if (m_dbConn.GetActiveTable() == ACTIVE_TABLE_BUY) {
			if (MessageBox("没有记录，是否要增加记录？", "Confirm", MB_YESNO) == IDYES) {
				OnStockbuyAdd();
			}
		} else if (m_dbConn.GetActiveTable() == ACTIVE_TABLE_MONEY) {
			if (MessageBox("没有记录，是否要增加记录？", "Confirm", MB_YESNO) == IDYES) {
				OnStockmoneyInout();
			}
		}
		return ERR;
	}
	
	/**
	 *	Init Grid with column and row count.
	 *
	 *  Fixed row 0 is to display name of fields. Which should be mapped 
	 *  into Chinese column names according its English names.
	 *
	 *  Fixed column 0 is to display the sequence number that are only valid 
	 *  in the grid's view. Data in 'id' column in database won't be displayed.
	 *  That is, the 'id' from database will be replaced with 'seqNo' in view.
	 */
	m_GridCtrl.DeleteAllItems();		// delete all to display new data.
	m_GridCtrl.ShowWindow(SW_SHOW);
	m_GridCtrl.SetColumnCount(nColCount);
	m_GridCtrl.SetRowCount(nRowCount + 1);	// + 1 to display the field names.
	m_GridCtrl.SetFixedRowCount(1);
	m_GridCtrl.SetFixedColumnCount(1);

	/* Clear corresponding m_vecStock*Ids to prepare to make a new display. */
	ClearRecordIds();

	/* 1. Show name & set width of fields, before 'nCol' of result. */
	for (int colIdx = 0; colIdx < nColCount; ++colIdx) {
				
		const char* data = result[colIdx];
		if (!data || strlen(data) == 0)
			return ERR;

		/* 1.1 Set left top item */
		if (colIdx == 0) {			// Do not display left-top "id" string.
			SetLeftTopItemData();
			continue;
		}

		/*  1.2.0 Show field names. Convert English field name to Chinese name. */
		string strOutName("");
		strOutName = FieldNamesMap::GetChNameByEnName(data);
		m_GridCtrl.SetItemText(0, colIdx, strOutName.c_str());
		if (string::npos != strOutName.find(_T("保本价格"))
			&& m_dbConn.GetActiveTable() == ACTIVE_TABLE_HOLD) {
			m_GridCtrl.SetColumnTextClrWithoutFixedCells(colIdx, RGB(0xFF, 0x00, 0x00));
		}

		/**
		 *	1.2.1 Set gird's column's width according strOutName's width.
		 *  Maybe have no effect because setting of data's font format.
		 */
		CSize sz;
		CClientDC dc(this);
		sz = dc.GetTextExtent(strOutName.c_str(), strOutName.length());
		int columnLen = sz.cx;

		if (string::npos != strOutName.find(_T("日期")) || 
			string::npos != strOutName.find(_T("date")))
			columnLen += 10;	/* buy_date & sell_date columns have wider width. */
		m_GridCtrl.SetColumnWidth(colIdx, columnLen - 5);
	}

	/**
	 *	2. Show data, data is starting from 'nCol' of 'result'.
	 *  Data in column 0 is the ids, which are no need to display.
	 *  Display 'seqNo' in grid view instead of 'id' from database.
	 */
	int seqNo = 1;					// sqe_no is started from 1.
	for (int rowIdx = 1; rowIdx < nRowCount + 1; ++rowIdx) {
		if (rowIdx % 2 == 0)		// Set row background color.
			m_GridCtrl.SetRowBkClrWithoutFixedCells(rowIdx, RGB(0xBC, 0xC7, 0xD8));
		for (int colIdx = 0; colIdx < nColCount; ++colIdx) {

			/* 2.1 Display 'seqNo' in col 0, instead of 'id'. */
			if (colIdx == 0) {
				char strSeqNo[8] = "";
				_itoa_s(seqNo++, strSeqNo, 10);
				m_GridCtrl.SetItemText(rowIdx, colIdx, strSeqNo);

				/**
				 *	Store 'id' value to later operation, like removing record.
				 *  Make a relation between 'id' and 'seqNo'.
				 *  Which will cause that the gird can not order by column.
				 */
				const char* idStr = result[rowIdx * nColCount + colIdx];
				int id = -1;
				if (idStr)
					id = atoi(idStr);
				if (id > 0)
					StoreRecordId(id);
				else
					MessageBox("Record id is <= 0.", "ERROR!");
				continue;
			}

			/* 2.2 Display data, Convert UTF8 word to GB2312 format. */
			string strOut;			
			const char* data = result[rowIdx * nColCount + colIdx];
			string strData = CChineseCodeLib::UTF8ToGB2312(data);
			m_GridCtrl.SetItemText(rowIdx, colIdx, strData.c_str());
		}
	}

	return OK;
}

int CStockRecordDlg::GetAndShowTableData(ENUMACTIVETABLE whichTable)
{
	m_dbConn.SetActiveTable(whichTable);
	CStockTableDataInfo tableDataInfo = 
		m_dbConn.QueryWholeTableData(whichTable);
	SetGridData(tableDataInfo.m_nRow, tableDataInfo.m_nCol, tableDataInfo.m_pData);
	m_dbConn.ReleaseWholeTableData(tableDataInfo.m_pData);
	return OK;
}

int CStockRecordDlg::ReloadRecords( void )
{
	int ret = 0;

	switch (m_dbConn.GetActiveTable()) {

	case ACTIVE_TABLE_BUY:
		OnMenuBuyRecord();
		break;
	case ACTIVE_TABLE_HOLD:
		OnMenuHoldRecord();
		break;
	case ACTIVE_TABLE_SELL:
		OnMenuSellRecord();
		break;
	case ACTIVE_TABLE_MONEY:
		OnMenuMoneyRecord();
		break;
	default:
		ret = ERR;
	}

	return ret;
}

int CStockRecordDlg::SetupDBTableNames(void)
{
	m_dbConn.SetDatabaseName("StockRecord.db");
	m_dbConn.SetBuyTableName("stock_buy");
	m_dbConn.SetHoldTableName("stock_hold");
	m_dbConn.SetSellTableName("stock_sell");
	m_dbConn.SetMoneyTableName("stock_money");
	return OK;
}

void CStockRecordDlg::OnClose(void)
{
	CDialogEx::OnClose();
}

void CStockRecordDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	
	m_dbConn.DisConnect();
	UnregisterHotKey(GetSafeHwnd(), WM_STOCK_RECORD_HIDESHOW);
}

void CStockRecordDlg::OnMenuHoldRecord()
{
	SetWindowText("Hold Record");
	m_dbConn.SetActiveTable(ACTIVE_TABLE_HOLD);
	MakeMenuItemCheckedByActiveTable();		
	GetAndShowTableData(ACTIVE_TABLE_HOLD);
}

void CStockRecordDlg::OnMenuBuyRecord()
{
	SetWindowText("Buy Record");
	m_dbConn.SetActiveTable(ACTIVE_TABLE_BUY);
	MakeMenuItemCheckedByActiveTable();
	GetAndShowTableData(ACTIVE_TABLE_BUY);
}

void CStockRecordDlg::OnMenuSellRecord()
{
	SetWindowText("Sell Record");
	m_dbConn.SetActiveTable(ACTIVE_TABLE_SELL);
	MakeMenuItemCheckedByActiveTable();
	GetAndShowTableData(ACTIVE_TABLE_SELL);
}

void CStockRecordDlg::OnMenuMoneyRecord()
{
	SetWindowText("Money Record");
	m_dbConn.SetActiveTable(ACTIVE_TABLE_MONEY);
	MakeMenuItemCheckedByActiveTable();
	GetAndShowTableData(ACTIVE_TABLE_MONEY);
}

void CStockRecordDlg::OnBnClickedExit()
{
	CDialogEx::OnCancel();
}

/**
 *	Handle right click on grid.
 *  Popup a menu and show the operations as the menu items.
 */
void CStockRecordDlg::OnGridRClick( NMHDR *pNotifyStruct, LRESULT* pResult )
{
	/* Get position where right click occurs. */
	DWORD dwPos = GetMessagePos();	
	CPoint point (LOWORD(dwPos), HIWORD(dwPos));
	CMenu menu;
	CMenu* popupMenu(NULL);		// MUST be inited.
	menu.LoadMenuA(IDR_MENU2);
	
	/* Right click on invalid cell, or on fixed colomn,
	 * Only the "Add Record" menu item (if there is) can be displayed.
	 */
	bool isRClickOnDataCell = false;
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	if (m_GridCtrl.IsValid(pItem->iRow, pItem->iColumn) && 
		!m_GridCtrl.IsCellInFixedRow(pItem->iRow, pItem->iColumn)) {

		isRClickOnDataCell = true;
	}

	/* Get related submenu according to current opened table */
	switch (m_dbConn.GetActiveTable()) {
	case ACTIVE_TABLE_BUY:
		popupMenu = menu.GetSubMenu(0);
		if (!isRClickOnDataCell)
			popupMenu->DeleteMenu(IDM_STOCKBUY_REMOVE, MF_BYCOMMAND);
		break;

	case ACTIVE_TABLE_HOLD:
		if (isRClickOnDataCell)
			popupMenu = menu.GetSubMenu(1);
		break;

	case ACTIVE_TABLE_SELL:
		if (isRClickOnDataCell)
			popupMenu = menu.GetSubMenu(2);
		break;

	case ACTIVE_TABLE_MONEY:
		popupMenu = menu.GetSubMenu(3);
		if (!isRClickOnDataCell) {
			popupMenu->DeleteMenu(IDM_STOCKMONEY_REMOVE, MF_BYCOMMAND);
		}
		break;

	default:
		break;
	}

	/* Popup the submenu */
	if (popupMenu) {
		SetForegroundWindow();
		DrawMenuBar();
		popupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);
	}
	
	menu.DestroyMenu();
	*pResult = 0;
}

/**
 *	If stock_hold is active, then the double click will trigger the sell operation.
 */
void CStockRecordDlg::OnGridDBClick( NMHDR *pNotifyStruct, LRESULT* pResult )
{
	/* Ignore double click on invalid cells and fixed row cells. */
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	if (!m_GridCtrl.IsValid(pItem->iRow, pItem->iColumn) || 
		m_GridCtrl.IsCellInFixedRow(pItem->iRow, pItem->iColumn)) {

		*pResult = 1;
		return ;
	}
	
	if (m_dbConn.GetActiveTable() == ACTIVE_TABLE_HOLD)
		OnStockholdPlanSell();
	else if (m_dbConn.GetActiveTable() == ACTIVE_TABLE_BUY)
		OnStockbuyAdd();
}

/**
 *	Get record id according to seqNo.
 *  Record id is stored in the m_vecStock*Ids.
 *  Also note that the index of vector is started from 0.
 */
int CStockRecordDlg::GetActiveRecordIdBySeqNo( int seqNo )
{
	if (seqNo < 0)
		return -1;

	int id = -1;

	switch (m_dbConn.GetActiveTable()) {

	case ACTIVE_TABLE_BUY:
		if (m_vecStockBuyIds.empty() || seqNo > (int)m_vecStockBuyIds.size())
			return -1;
		else
			id = m_vecStockBuyIds.at(seqNo - 1);
		break;

	case ACTIVE_TABLE_HOLD:
		if (m_vecStockHoldIds.empty() || seqNo > (int)m_vecStockHoldIds.size())
			return -1;
		else
			id = m_vecStockHoldIds.at(seqNo - 1);
		break;

	case ACTIVE_TABLE_SELL:
		if (m_vecStockSellIds.empty() || seqNo > (int)m_vecStockSellIds.size())
			return -1;
		else
			id = m_vecStockSellIds.at(seqNo - 1);
		break;

	case ACTIVE_TABLE_MONEY:
		if (m_vecStockMoneyIds.empty() || seqNo > (int)m_vecStockMoneyIds.size())
			return -1;
		else
			id = m_vecStockMoneyIds.at(seqNo - 1);
		break;

	default:
		id = -1;
	}

	return id;
}

BOOL CStockRecordDlg::IsCellRangeValid( const CCellRange& cellRange )
{
	if (cellRange.GetMinRow() < 0 || cellRange.GetMinCol() < 0 ||
		cellRange.GetMaxRow() < 0 || cellRange.GetMaxCol() < 0) {
			return FALSE;
	}

	if (cellRange.GetMinRow() <= cellRange.GetMaxRow() &&
		cellRange.GetMinCol() <= cellRange.GetMaxCol()) {
			return TRUE;
	}
	return FALSE;
}

/** Whether is cell in the range of cellRange (rows range). */
BOOL CStockRecordDlg::IsFocusedCellInSelectedRows( 
	const CCellID& cell, const CCellRange& cellRange )
{
	if (cell.row < 0 || cell.col < 0 || !IsCellRangeValid(cellRange))
		return FALSE;

	/* Compare row numbers */
	if (cellRange.GetMinRow() <= cell.row && cell.row <= cellRange.GetMaxRow()) 
		return TRUE;

	return FALSE;
}

/**
 *	Handler for menu item - deleting record for all tables.
 *  uid contains the actual ID of menu item, like IDM_STOCKBUY_REMOVE... (no use)
 *  NOTE: The IDs of four menu items 'remove record' MUST be sequential.
 *  When the deleting action is going on, DONOT reload the data from database.
 */
void CStockRecordDlg::OnMenuRemoveRecord( UINT uid )
{
	if (MessageBox("确定要删除记录？", "Confirm", MB_YESNO) != IDYES) {
		return;
	}

	/**
	 *	0. Get selected cells and focused cell.
	 *  Note that the selected cells may not be in succession.
	 *  So you should check every cell to see whether it is really selected.
	 *  When a cell is selected, the record in the row is to be deleted.
	 */
	CCellID focusedCell = m_GridCtrl.GetFocusCell();
	CCellRange cellRange(-1, -1, -1, -1);
	int selCount = m_GridCtrl.GetSelectedCount();
	cellRange = m_GridCtrl.GetSelectedCellRange();
	int ret = 0;

	/* 1. Firstly, delete the record where focused cell is located. */
	int id = GetActiveRecordIdBySeqNo(focusedCell.row);
	ret = m_dbConn.RemoveRecordByTableId(m_dbConn.GetActiveTable(), id);

	/**
	 * 1.1 No selected cells, or the selected cell range is not valid,
	 * just delete record where focused cell is located. 
	 */
	if (selCount <= 0 || !IsCellRangeValid(cellRange)) {
		ReloadRecords();
		return ;
	}
	
	/**
	 *	2. Secondly, delete selected records by traversing every cell
	 *  in the range to see whether it is really selected.
	 *  Except the focused one (if focused one is in the selected range), 
	 *  because it has been removed already.
	 */
	for (int rowIdx = cellRange.GetMinRow(); 
		 rowIdx <= cellRange.GetMaxRow(); 
		 ++rowIdx) {

		if (rowIdx == focusedCell.row)	// skip removed focused record.
			continue;

		for (int colIdx = cellRange.GetMinCol();
			colIdx <= cellRange.GetMaxCol();
			++colIdx) {

			/* Cell (rowIdx, colIdx) is selected, delete related record,
			 * and restart traversing cells in the next row. 
			 */
			if (m_GridCtrl.IsCellSelected(rowIdx, colIdx)) {
				int id = GetActiveRecordIdBySeqNo(rowIdx);
				ret = 
					m_dbConn.RemoveRecordByTableId(m_dbConn.GetActiveTable(), id);
				break;	/* check next row */
			}
		} // inner for
	} // outer for

	ReloadRecords();
}

/**
 * stock_buy table MUST be active. 
 * That is m_nDBStatus MUST be DB_STATUS_OPENED. 
 */
void CStockRecordDlg::OnStockbuyAdd()
{
	int ret = 0;
	CStockBuyDlg buyDlg;
	if (buyDlg.DoModal() != IDOK)
		return ;
	
	/* 1. Get the data from dialog, and convert the data to model data. 
	 * The window class is valid, but the window doesn't exist (buyDlg.hwnd is 0). 
	 */
	CStockModelBuy buyModel = ConvertDlgDataToBuyModel(buyDlg);

	/* 2. Insert record into stock_buy table */
	if (buyModel.GetEncodeStyle() == ENCODE_STYLE_GB2312)
		buyModel.ConvertEncodeFormat(ENCODE_STYLE_UTF8);
	ret = m_dbConn.InsertBuyRecord(buyModel);

	/* 3. Convert buy model into hold model to prepare to insert. */
	CStockModelHold holdModel = ConvertBuyModelToHoldModel(buyModel);

	/* 4. Insert record into stock_hold table. */
	if (holdModel.GetEncodeStyle() == ENCODE_STYLE_GB2312)
		holdModel.ConvertEncodeFormat(ENCODE_STYLE_UTF8);
	if (holdModel.id < 0) /* No same record in stock_hold, insert a new one */
		ret = m_dbConn.InsertHoldRecord(holdModel);
	else	/* same record exists in stock_hold, update it */
		ret = m_dbConn.UpdateHoldModel(holdModel);

	/* 5. Reload records. */
	ReloadRecords();
}

void CStockRecordDlg::OnStockholdPlanSell()
{
	/* Let m_bIsPlanSell be true, then call OnStockholdSell().
	 * m_bIsPlanSell's value will pass to sellDlg.
	 * SellDlg will disable the "Sell" button is m_bIsPlanSell is true.
	 */
	m_bIsPlanSell = true;
	OnStockholdSell();
}

void CStockRecordDlg::OnStockholdSell()
{
	/* 1. Query the hold record according to the focused cell's row. */
	int id = GetActiveRecordIdBySeqNo(m_GridCtrl.GetFocusCell().row);
	CStockModelHold holdModel = m_dbConn.SelectHoldModelById(id);

	/* 2. Popup sell stock dialog with appropriate values. */
	if (holdModel.GetEncodeStyle() == ENCODE_STYLE_UTF8)
		holdModel.ConvertEncodeFormat(ENCODE_STYLE_GB2312);
	int nHoldAmount = atoi((LPCTSTR)holdModel.hold_amount);
	float fHoldCost = (float)atof((LPCTSTR)holdModel.hold_cost);

	CStockSellDlg sellDlg;
	sellDlg.m_strCode = holdModel.code;	// init dialog's edit values.
	sellDlg.m_strName = holdModel.name;
	sellDlg.m_nSellAmount = nHoldAmount;
	sellDlg.SetHoldAmount(nHoldAmount);
	sellDlg.SetHoldCost(fHoldCost);
	sellDlg.SetIsPlanSell(m_bIsPlanSell);
	m_bIsPlanSell = false;/* Let m_bIsPlanSell be false regardless its origin value */
	
	if (sellDlg.DoModal() != IDOK)
		return ;

	/* 3. Convert to StockSellModel , insert record into table stock_sell,
	 * and Update total_earn (+= each_earn) on the first record in stock_sell */
	CStockModelSell sellModel = ConvertToSellModel(holdModel, sellDlg);
	if (sellModel.GetEncodeStyle() == ENCODE_STYLE_GB2312)
		sellModel.ConvertEncodeFormat(ENCODE_STYLE_UTF8);
	int ret = m_dbConn.InsertSellRecord(sellModel);
	ret = m_dbConn.UpdateSellTotalEarn();

	/* 4.1 If all of held stock is sold, delete record in stock_hold. 
	 * Comment this, because we don't delete the record now. */
	if (sellDlg.m_nSellAmount == nHoldAmount) {
		// ret = DeleteRecordById(m_pDatabase, m_StrHoldTableName.c_str(), id);
	}

	/* 4.2 If part of held stock is sold, update record in stock_hold. */
	else if (sellDlg.m_nSellAmount < nHoldAmount) {
		int remianAmount = nHoldAmount - sellDlg.m_nSellAmount;
		bool bStockType = ('6' == holdModel.code.GetAt(0)) ?	\
			STOCK_TYPE_SHANG_HAI : STOCK_TYPE_SHEN_ZHEN;

		CStockFees stockFees;
		float fNewRemainEvenPrice = 
			stockFees.CalcuEvenPriceByHold(bStockType, fHoldCost, remianAmount);
		
		holdModel.hold_amount.Format("%d", remianAmount);
		holdModel.even_price.Format("%.2f", Round(fNewRemainEvenPrice, 2));

		if (holdModel.GetEncodeStyle() == ENCODE_STYLE_GB2312)
			holdModel.ConvertEncodeFormat(ENCODE_STYLE_UTF8);
		ret = m_dbConn.UpdateHoldModel(holdModel);
	}

	ReloadRecords();
}

void CStockRecordDlg::OnStockmoneyInout()
{
}

CString CStockRecordDlg::ConvertOleDateTimeToDateStr( const COleDateTime& datetime )
{
	/* Make sure the date format is: YYYY-MM-DD. */
	int year = datetime.GetYear();
	int month = datetime.GetMonth();
	int day = datetime.GetDay();
	CString dateStr("");
	dateStr.Format("%04d-%02d-%02d", year, month, day);
	return dateStr;
}

CStockModelBuy 
CStockRecordDlg::ConvertDlgDataToBuyModel( const CStockBuyDlg& buyDlg )
{
	CStockModelBuy buyModel;

	buyModel.code = buyDlg.m_strCode;
	buyModel.name = buyDlg.m_strName;
	buyModel.buy_price.Format("%.2f", buyDlg.m_fBuyPrice);
	buyModel.buy_amount.Format("%d", buyDlg.m_nBuyAmount);
	buyModel.stock_type = buyDlg.m_bStockType;
	buyModel.buy_date = ConvertOleDateTimeToDateStr(buyDlg.m_oleDataTime);
	buyModel.SetEncodeStyle(ENCODE_STYLE_GB2312);

	return buyModel;
}

CStockModelHold 
CStockRecordDlg::ConvertBuyModelToHoldModel( const CStockModelBuy& buyModel )
{
	/* Checkout whether the stock(code) is already in stock_hold.
	 * If no same stock in table stock_hold, then the returned object
	 * will be an empty object (empty CString as its members).
	 * And its holdModel's id will be -1.
	 *
	 * If the stock is already in the stock_hold, then holdModel's id will
	 * be the actual id value in the database.
	 */
	CStockModelHold holdModel;
	holdModel = m_dbConn.SelectHoldModelByCode((LPCTSTR)buyModel.code);
	/* If stock_hold doesn't have the same record, its id is -1. */
	bool isHoldTableHasSameStock = holdModel.id <= 0 ? false : true;
	
	/* The first 3 fields will be written by data from buyModel, 
	 * no matter whether the stock exists in stock_hold table.
	 * Other fields' values will be recalculated if stock exists in stock_hold.
	 */
 	holdModel.code = buyModel.code;
 	holdModel.name = buyModel.name;
	holdModel.buy_price = buyModel.buy_price;
	holdModel.buy_date = buyModel.buy_date;

	CStockFees stockFees;
	float fBuyPrice = (float)atof((LPCTSTR)buyModel.buy_price);
	int nCurrBuyAmount = atoi((LPCTSTR)buyModel.buy_amount);
	int nNewHoldAmount = nCurrBuyAmount;		// init

	float fCurrHoldCost = stockFees.CalculateHoldCostByBuy(buyModel.stock_type, \
		fBuyPrice, nCurrBuyAmount);
	float fNewHoldCost = fCurrHoldCost;			// init

	float fEvenPrice = stockFees.CalcuEvenPriceByHold(buyModel.stock_type, \
		fCurrHoldCost, nCurrBuyAmount);
	
	/* 2.1 Stock has already existed in stock_hold. Recalculate its values. 
	 * Update the hold record 
	 */
	if (isHoldTableHasSameStock) {
		float fPreHoldCost = (float)atof((LPCTSTR)holdModel.hold_cost);
		int nPreHoldAmount = atoi((LPCTSTR)holdModel.hold_amount);

		/* new hold_amount */
		nNewHoldAmount = nPreHoldAmount + nCurrBuyAmount;

		/* new hold_cost */
		fNewHoldCost = (fPreHoldCost * nPreHoldAmount 
			+ fCurrHoldCost * nCurrBuyAmount) / nNewHoldAmount;
		stockFees.m_fHoldCost = fNewHoldCost;

		/* new even_price */
		fEvenPrice = stockFees.CalcuEvenPriceByHold(buyModel.stock_type,	\
			fNewHoldCost, nNewHoldAmount);
	} 

	holdModel.hold_amount.Format("%d", nNewHoldAmount);
	holdModel.hold_cost.Format("%.3f", (fNewHoldCost = Round(fNewHoldCost, 3)));
	holdModel.even_price.Format("%.2f", (fEvenPrice = Round(fEvenPrice, 2)));

	holdModel.SetEncodeStyle(buyModel.GetEncodeStyle()); // for name
	return holdModel;
}

/* Convert sellDlg data & holdModel data to sellModel data to be inserted. */
CStockModelSell 
CStockRecordDlg::ConvertToSellModel( const CStockModelHold& holdModel, 
		const CStockSellDlg& sellDlg )
{
	CStockModelSell sellModel;
	sellModel.code = holdModel.code;
	sellModel.name = holdModel.name;
	sellModel.buy_price = holdModel.buy_price;
	sellModel.even_price = holdModel.even_price;
	sellModel.sell_price.Format("%.2f", sellDlg.m_fSellPrice);
	sellModel.sell_amount.Format("%d", sellDlg.m_nSellAmount);
	sellModel.buy_date = holdModel.buy_date;
	sellModel.sell_date = ConvertOleDateTimeToDateStr(sellDlg.m_oleSellDate);

	/* Calculate each earn according to input from dialog. */
	bool bStockType = ('6' == holdModel.code.GetAt(0)) ?	\
		STOCK_TYPE_SHANG_HAI : STOCK_TYPE_SHEN_ZHEN;
	float fHoldCost = (float)atof((LPCTSTR)holdModel.hold_cost);
	CStockFees stockFees;
	float fEachEarn = stockFees.CalcuEachEarn(bStockType, /*Round(fHoldCost, 2)*/\
		fHoldCost , sellDlg.m_nSellAmount, sellDlg.m_fSellPrice);

	sellModel.each_earn.Format("%.2f", Round(fEachEarn, 2, ROUND_ZSBR));
	sellModel.SetEncodeStyle(holdModel.GetEncodeStyle());
	
	return sellModel;
}

void CStockRecordDlg::MakeMenuItemCheckedByActiveTable(void)
{
	/* Make all menu items unchecked */
	GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_MENU_BUY_RECORD, MF_UNCHECKED);
	GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_MENU_HOLD_RECORD, MF_UNCHECKED);
	GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_MENU_SELL_RECORD, MF_UNCHECKED);
	GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_MENU_MONEY_RECORD, MF_UNCHECKED);

	/* Make active table menu itme checked */
	switch (m_dbConn.GetActiveTable()) {
	case ACTIVE_TABLE_BUY:
		GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_MENU_BUY_RECORD, MF_CHECKED);
		break;
	case ACTIVE_TABLE_HOLD:
		GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_MENU_HOLD_RECORD, MF_CHECKED);
		break;
	case ACTIVE_TABLE_SELL:
		GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_MENU_SELL_RECORD, MF_CHECKED);
		break;
	case ACTIVE_TABLE_MONEY:
		GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_MENU_MONEY_RECORD, MF_CHECKED);
		break;
	default:
		break;
	}
}

void CStockRecordDlg::OnBnClickedBtAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CStockRecordDlg::OnMenuAbout()
{
	OnBnClickedBtAbout();
}

void CStockRecordDlg::OnMenuAlwaysTop()
{
	CRect rect;
	GetWindowRect(&rect);
	UINT nCheckState = 
		GetMenu()->GetSubMenu(1)->GetMenuState(IDM_ALWAYS_TOP, MF_BYCOMMAND);

	if (MF_CHECKED == (nCheckState & MF_CHECKED)) {
		SetWindowPos(&wndNoTopMost, rect.left, rect.top, -1, -1, SWP_NOSIZE);
		GetMenu()->GetSubMenu(1)->CheckMenuItem(IDM_ALWAYS_TOP, MF_UNCHECKED);
	} else {
		SetWindowPos(&wndTopMost, rect.left, rect.top, -1, -1, SWP_NOSIZE);
		GetMenu()->GetSubMenu(1)->CheckMenuItem(IDM_ALWAYS_TOP, MF_CHECKED);
	}
}

void CStockRecordDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// The dialog other than the grid will receive the WM_RBUTTONDOWN message.
	// MessageBox("CStockRecordDlg::OnRButtonDown()");

	CDialogEx::OnRButtonDown(nFlags, point);
}


int CStockRecordDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pTrayIcon = new CTrayIcon(this, WM_ICON_NOTIFY, "StockRecord",	\
		AfxGetApp()->LoadIconA(IDR_MAINFRAME), IDR_MENU3, 1);
	if (m_pTrayIcon)
		m_pTrayIcon->SetTooltipText("隐藏主程序");

	return 0;
}

/**
 *	Response for mouse click or keyboard stroke.
 */
HRESULT CStockRecordDlg::OnTrayNotification( WPARAM wParam, LPARAM lParam )
{
	/* Returns quickly if it is not for this tray icon. */
	if (!m_pTrayIcon || (UINT)wParam != m_pTrayIcon->GetTrayIconID())
		return 0L;

	/* Response for rclick */
	if (LOWORD(lParam) == WM_RBUTTONDOWN) {	
		CMenu menu, *pSubMenu = NULL;
		if (!menu.LoadMenu(IDR_MENU3))			return 0L;
		if (!(pSubMenu = menu.GetSubMenu(0)))	return 0L;
		/* Make first menu item the default (bold font) */
		::SetMenuDefaultItem(pSubMenu->m_hMenu, 0, TRUE);

		pSubMenu->ModifyMenu(ID_MENU_TRAYICON_SHOWWD, MF_BYCOMMAND,		\
						ID_MENU_TRAYICON_SHOWWD, 
						m_bIsWndHidden ? _T("显示主界面") : _T("隐藏主界面"));
		CPoint pos;
		GetCursorPos(&pos);
		DrawMenuBar();
		SetForegroundWindow();
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pos.x, pos.y, this);
		pSubMenu->Detach();
		menu.DestroyMenu();
	} else if (LOWORD(lParam) == WM_LBUTTONDBLCLK) {
		/* Response for double left click */
		OnMenuTrayiconShowHideWnd();
	}

	return 1L;
}

/* Show or hide the dialog */
void CStockRecordDlg::OnMenuTrayiconShowHideWnd()
{
	AfxGetMainWnd()->ShowWindow(SW_MINIMIZE);
	AfxGetMainWnd()->ShowWindow(m_bIsWndHidden ? SW_RESTORE : SW_HIDE);
	if (m_pTrayIcon)
		m_pTrayIcon->SetTooltipText(m_bIsWndHidden ? _T("隐藏主界面") : _T("显示主界面"));
	m_bIsWndHidden = !m_bIsWndHidden;
}

void CStockRecordDlg::OnMenuTrayiconExit()
{
	DestroyWindow();	/* Send WM_DESTROY and WM_NCDESTROY msg. */
}

BOOL CStockRecordDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYUP == pMsg->message) {
		switch (pMsg->wParam) {

		case 0x42:	/* B + CTRL will activate stock_buy table */
			if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
				OnMenuBuyRecord();
				/* Return TRUE, TranslateMessage() won't get a chance to handle */
				return TRUE;
			}
			break;

		case 0x53:	/* S + CTRL will activate stock_sell table */
			if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
				OnMenuSellRecord();
				return TRUE;
			}
			break;

		case 0x48:	/* H + CONTRL will activate stock_hold table */
			if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
				OnMenuHoldRecord();
				return TRUE;
			}
			break;

		case 0x4D:	/* M + CONTRL will activate stock_money table */
			if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
				OnMenuMoneyRecord();
				return TRUE;
			}
			break;

		case 0x54:	/* T + CONTROL will hide the dialog (only hide) */
			if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
				OnMenuTrayiconShowHideWnd();
				return TRUE;
			}
			break;

		case 0x50:	/* P + CONTROL will active plan_buy dialog. */
			if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
				OnMenuPlanbuy();
				return TRUE;
			}
			break;

		default:
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

/* Plan buy dialog is a non-modal dialog, it is created dynamically. */
void CStockRecordDlg::OnMenuPlanbuy()
{
	if (!m_pPlanBuyDlg) {
		/* plan dialog has not been created. */
		m_pPlanBuyDlg = new CStockPlanBuyDlg();
		if (!m_pPlanBuyDlg)
			return ;
		m_pPlanBuyDlg->SetDBConnection(&m_dbConn);
		m_pPlanBuyDlg->Create(IDD_PLANBUY_DLG, this);
	}

	m_pPlanBuyDlg->SetCode("");
	m_pPlanBuyDlg->SetBuyAmount(0);
	m_pPlanBuyDlg->ShowWindow(SW_SHOW);
}

void CStockRecordDlg::OnStockholdPlanbuy()
{
	/* 1. Query the hold record according to the focused cell's row. */
	int id = GetActiveRecordIdBySeqNo(m_GridCtrl.GetFocusCell().row);
	CStockModelHold holdModel = m_dbConn.SelectHoldModelById(id);
	if (holdModel.GetEncodeStyle() == ENCODE_STYLE_UTF8)
		holdModel.ConvertEncodeFormat(ENCODE_STYLE_GB2312);

	/* Only one plan buy dialog can be shown at the same time. */
	if (!m_pPlanBuyDlg) {
		/* plan dialog has not been created. */
		m_pPlanBuyDlg = new CStockPlanBuyDlg();
		if (!m_pPlanBuyDlg)
			return ;
		m_pPlanBuyDlg->SetDBConnection(&m_dbConn);
		m_pPlanBuyDlg->Create(IDD_PLANBUY_DLG, this);
	}

	m_pPlanBuyDlg->SetCode(holdModel.code);
	m_pPlanBuyDlg->SetBuyAmount(atoi((LPCTSTR)holdModel.hold_amount));
	m_pPlanBuyDlg->ShowWindow(SW_SHOW);
}

/* Hander for global hot key, to hide/show main dialog. */
HRESULT CStockRecordDlg::OnHotKey(WPARAM wParam, LPARAM lParam)
{
	if (wParam == WM_STOCK_RECORD_HIDESHOW)
		OnMenuTrayiconShowHideWnd();

	return (HRESULT)0;
}
