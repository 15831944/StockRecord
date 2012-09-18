
// StockRecordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StockRecord.h"
#include "StockRecordDlg.h"
#include "afxdialogex.h"

#include "FieldNamesMap.h"

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
	, m_nDBStatus(-1)
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
	ON_COMMAND(ID_MENU_HOLD_RECORD, &CStockRecordDlg::OnMenuHoldRecord)
	ON_COMMAND(ID_MENU_BUY_RECORD, &CStockRecordDlg::OnMenuBuyRecord)
	ON_COMMAND(ID_MENU_SELL_RECORD, &CStockRecordDlg::OnMenuSellRecord)
	ON_COMMAND(ID_MENU_MONEY_RECORD, &CStockRecordDlg::OnMenuMoneyRecord)
	ON_BN_CLICKED(IDC_BT_EXIT, &CStockRecordDlg::OnBnClickedExit)
	ON_NOTIFY(NM_RCLICK, IDC_GRID, &CStockRecordDlg::OnGridRClick)
	ON_COMMAND_RANGE(IDM_STOCKBUY_REMOVE, IDM_STOCKMONEY_REMOVE, &CStockRecordDlg::OnMenuRemoveRecord)
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

	/* Some features of grid's column and row. */
	m_GridCtrl.SetFixedBkColor(RGB(0xFF, 0xFF, 0x00));
	m_GridCtrl.SetColumnResize(TRUE);		// Column can resize.
	m_GridCtrl.SetRowResize(TRUE);			// Row cannot resize.
	m_GridCtrl.SetAutoSizeStyle(GVS_BOTH);	// Auto size
	m_GridCtrl.SetEditable(FALSE);			// Cannot edit	

	OnMenuHoldRecord();						// Query hold record when startup.

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

/*
 *	Set gird data queried from sqlite3_get_table.
 *  nRow - How many rows of the data.
 *  nCol - How many columns of the data.
 *  result - All the queried data are stored in memory pointed by result.
 */
int CStockRecordDlg::SetGirdData( int nRow, int nCol, char** result )
{
	// TODO: Show info when there is no data in the database
	if (nRow <= 0 || nCol <= 0 || !result) {
		m_GridCtrl.DeleteAllItems();
		m_GridCtrl.ShowWindow(SW_HIDE);
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
	m_GridCtrl.SetColumnCount(nCol);
	m_GridCtrl.SetRowCount(nRow + 1);	// + 1 to display the field names.
	m_GridCtrl.SetFixedRowCount(1);
	m_GridCtrl.SetFixedColumnCount(1);

	// TODO: How to set font.

	/* Clear corresponding m_vecStock*Ids to prepare to make a new display. */
	ClearRecordIds();

	/**
	 *	1. Show name of fields, before 'nCol' of result.
	 */
	for (int colIdx = 0; colIdx < nCol; ++colIdx) {
				
		char* data = result[colIdx];
		if (!data || strlen(data) == 0)
			return ERR;

		/* 1.1 Set left top item */
		if (colIdx == 0) {			// Do not display left-top "id" string.
			SetLeftTopItemData();
			continue;
		}

		/*  1.2 Show field names. Convert English field name to Chinese name. */
		string strOutName("");
		strOutName = FieldNamesMap::GetChNameByEnName(data);
		m_GridCtrl.SetItemText(0, colIdx, strOutName.c_str());

		/**
		 *	1.2.1 Set gird's column's width according strOutName's width.
		 *  Maybe have no effect because setting of data's font format.
		 */
		CSize sz;
		CClientDC dc(this);
		sz = dc.GetTextExtent(strOutName.c_str(), strOutName.length());
		if (sz.cx > 0)
			m_GridCtrl.SetColumnWidth(colIdx, (int)sz.cx - 5);
	}

	/**
	 *	2. Show data, data is starting from 'nCol' of 'result'.
	 *  Data in column 0 is the ids, which are no need to display.
	 *  Display 'seqNo' in grid view instead of 'id' from database.
	 */
	int seqNo = 1;					// sqe_no is started from 1.
	for (int rowIdx = 1; rowIdx < nRow + 1; ++rowIdx) {
		for (int colIdx = 0; colIdx < nCol; ++colIdx) {

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
				char* idStr = result[rowIdx * nCol + colIdx];
				int id = -1;
				if (idStr)
					id = atoi(idStr);
				if (id >= 0)
					StoreRecordId(id);
				continue;
			}

			/* 2.2 Display data, Convert UTF8 word to GB2312 format. */
			string strOut;			
			char* data = result[rowIdx * nCol + colIdx];
			string strData = CChineseCodeLib::UTF8ToGB2312(data);
			m_GridCtrl.SetItemText(rowIdx, colIdx, strData.c_str());
		}
	}

	return OK;
}

/**
 *	Use sqlite3_get_table() to get all data in the table.
 *  This function MUST be called by OnMenu*Record() functions.
 *  tableName - which table to read data from.
 */
int CStockRecordDlg::QueryRecordsByTableName(const char* tableName)
{
	if (!tableName || strlen(tableName) == 0) {
		MessageBox("Table name is empty.", "Tips");
		return BAD_DB_TABLE_NAME;
	}

	if (!m_pDatabase && m_nDBStatus != DB_STATUS_OPENED) {
		return DB_STATUS_CLOSED;
	}
	
	int ret = 0;
	CString msg("");
	int nCol = 0;
	int nRow = 0;
	char** result = NULL;
	char* errmsg = NULL;
	string sql("");

	sql.clear();
	sql = sql + "SELECT * FROM " + tableName;
	
	/**
	 *	1. Query all records by using sqlite3_get_table() func.
	 */
	ret = sqlite3_get_table(m_pDatabase, sql.c_str(), &result, &nRow, &nCol, &errmsg);
	if (ret != SQLITE_OK) {
		if (errmsg) {
			CString str;
			str.Format("Cannot open table %s. %s",tableName ,errmsg);
			MessageBox(str, "ERROR!");
			sqlite3_free(errmsg);
			errmsg = NULL;
		}
		return ret;
	}

	/** Set grid data */
	SetGirdData(nRow, nCol, result);

	sqlite3_free_table(result);		// Never forget to free result table.
	result = NULL;
	return OK;
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
	if (m_pDatabase && m_nDBStatus == DB_STATUS_OPENED) {	// already opened.
		MessageBox("Database is already opened.", "Tip");
		return OK;
	}

	if (m_strDBName.empty()) {
		MessageBox("Database's name is empty.", "Oops");
		return BAD_DB_TABLE_NAME;
	}

	int ret = 0;
	CString msg("");
	ret = sqlite3_open_v2(m_strDBName.c_str(), &m_pDatabase, SQLITE_OPEN_READWRITE, NULL);
	if (ret == SQLITE_OK) {
		m_nDBStatus = DB_STATUS_OPENED;
		return ret;
	}

	/**
	 *	Open Read-only database failed, because db does not exist.
	 *  Now create a new database.
	 */
	m_nDBStatus = DB_STATUS_DOESNOT_EXIST;
	ret = sqlite3_open_v2(m_strDBName.c_str(), &m_pDatabase,
		SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	if (ret != SQLITE_OK) {
		msg.Format(_T("Cannot create & open database : %s."), m_strDBName.c_str());
		return ret;
	}

	/**
	 *	Initiate new database's tables.
	 */
	m_nDBStatus = DB_STATUS_OPENED;
	ret = InitDatabaseTables();

	if (ret == SQLITE_OK)
		MessageBox("Database does not exist.\nIts tables has been inited.");

	return ret;
}

int CStockRecordDlg::CloseDatabase( void )
{
	if (!m_pDatabase && m_nDBStatus != DB_STATUS_OPENED)
		return OK;
	
	int ret = 0;
	ret = sqlite3_close_v2(m_pDatabase);
	if (ret != SQLITE_OK) {
		return ret;
	}

	m_nDBStatus = DB_STATUS_CLOSED;
	m_pDatabase = NULL;
	return OK;
}

void CStockRecordDlg::OnClose(void)
{
	CDialogEx::OnClose();
}

void CStockRecordDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	
	CloseDatabase();		/* Close database */
}

int CStockRecordDlg::InitDatabaseTables( void )
{
	if (!m_pDatabase || m_nDBStatus != DB_STATUS_OPENED) {
		return ERR;
	}

	ASSERT(IsTableNamesValid());

	int ret = 0;
	char* errmsg = NULL;
	string sql("");

	/**
	 *	Init stock_buy table.
	 */
	sql.clear();
	sql = sql 
		+ "CREATE TABLE "
		+ m_strBuyTableName 
		+ " (	id INTEGER PRIMARY KEY AUTOINCREMENT, "
		+ "		code		VARCHAR(6) NOT NULL, "
		+ "		name		VARCHAR(10), "
		+ "		buy_price	FLOAT, "
		+ "		buy_amount	INTEGER, "
		+ "		buy_date	DATE "
		+ ")";
	ret = sqlite3_exec(m_pDatabase, sql.c_str(), NULL, NULL, &errmsg);
	if (ret != SQLITE_OK) {
		if (errmsg) {
			CString str;
			str.Format("%s", str);
			MessageBox(str, "ERROR!");
			sqlite3_free(errmsg);
			errmsg = NULL;
		}
		return ret;
	}

	/**
	 *	Init stock_hold table.
	 */
	sql.clear();
	sql = sql
		+ "CREATE TABLE "
		+ m_StrHoldTableName
		+ " (	id INTEGER PRIMARY KEY AUTOINCREMENT, "
		+ "		code		VARCHAR(6) NOT NULL, "
		+ "		name		VARCHAR(10), "
		+ "		buy_price	FLOAT, "
		+ "		hold_cost	FLOAT, "
		+ "		hold_amount	INTEGER, "
		+ "		even_price	FLOAT "
		+ ")";
	ret = sqlite3_exec(m_pDatabase, sql.c_str(), NULL, NULL, &errmsg);
	if (ret != SQLITE_OK) {
		if (errmsg) {
			CString str;
			str.Format("%s", str);
			MessageBox(str, "ERROR!");
			sqlite3_free(errmsg);
			errmsg = NULL;
		}
		return ret;
	}

	/**
	 *	Init stock_sell table.
	 */
	sql.clear();
	sql = sql 
		+ "CREATE TABLE "
		+ m_strSellTableName
		+ " (	id INTEGER PRIMARY KEY AUTOINCREMENT, "
		+ "		code		VARCHAR(6) NOT NULL, "
		+ "		name		VARCHAR(10), "
		+ "		buy_price	FLOAT, "
		+ "		sell_price	FLOAT, "
		+ "		sell_amount	INTEGER, "
		+ "		sell_date	DATE, "
		+ "		even_price	FLOAT, "
		+ "		each_earn	FLOAT, "
		+ "		total_earn	FLOAT "
		+ ")";
	ret = sqlite3_exec(m_pDatabase, sql.c_str(), NULL, NULL, &errmsg);
	if (ret != SQLITE_OK) {
		if (errmsg) {
			CString str;
			str.Format("%s", str);
			MessageBox(str, "ERROR!");
			sqlite3_free(errmsg);
			errmsg = NULL;
		}
		return ret;
	}

	/**
	 *	Init stock_money table.
	 */
	sql.clear();
	sql = sql
		+ "CREATE TABLE "
		+ m_strMoneyTableName
		+ " (	id INTEGER PRIMARY KEY AUTOINCREMENT, "
		+ "		each_money		FLOAT, "
		+ "		money_date		DATE, "
		+ "		remark			VACHAR, "
		+ "		total_money		FLOAT "
		+ ")";
	ret = sqlite3_exec(m_pDatabase, sql.c_str(), NULL, NULL, &errmsg);
	if (ret != SQLITE_OK) {
		if (errmsg) {
			CString str;
			str.Format("%s", str);
			MessageBox(str, "ERROR!");
			sqlite3_free(errmsg);
			errmsg = NULL;
		}
		return ret;
	}

	return ret;
}

BOOL CStockRecordDlg::IsTableNamesValid( void )
{
	if (m_strBuyTableName.empty() || m_StrHoldTableName.empty()
		|| m_StrHoldTableName.empty() || m_strMoneyTableName.empty()) {

		return FALSE;
	}

	return TRUE;	
}

void CStockRecordDlg::OnMenuHoldRecord()
{
	m_enumRecordTable = T_STOCKHOLD;
	QueryRecordsByTableName(m_StrHoldTableName.c_str());
}

void CStockRecordDlg::OnMenuBuyRecord()
{
	m_enumRecordTable = T_STOCKBUY;
	QueryRecordsByTableName(m_strBuyTableName.c_str());
}

void CStockRecordDlg::OnMenuSellRecord()
{
	m_enumRecordTable = T_STOCKSELL;
	QueryRecordsByTableName(m_strSellTableName.c_str());
}

void CStockRecordDlg::OnMenuMoneyRecord()
{
	m_enumRecordTable = T_STOCKMONEY;
	QueryRecordsByTableName(m_strMoneyTableName.c_str());
}

void CStockRecordDlg::OnBnClickedExit()
{
	CDialogEx::OnCancel();
}

void CStockRecordDlg::SetLeftTopItemData( void )
{
	string str("");

	switch (m_enumRecordTable) {
	case T_STOCKBUY:
		str.assign("BUY");
		break;
	case T_STOCKHOLD:
		str.assign("HOLD");
		break;
	case T_STOCKSELL:
		str.assign("SELL");
		break;
	case T_STOCKMONEY:
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
	switch (m_enumRecordTable) {
	case T_STOCKBUY:
		m_vecStockBuyIds.clear();
		break;
	case T_STOCKHOLD:
		m_vecStockHoldIds.clear();
		break;
	case T_STOCKSELL:
		m_vecStockSellIds.clear();
		break;
	case T_STOCKMONEY:
		m_vecStockMoneyIds.clear();
		break;
	default:
		break;
	}
}

void CStockRecordDlg::StoreRecordId( int id )
{
	switch (m_enumRecordTable) {
	case T_STOCKBUY:
		m_vecStockBuyIds.push_back(id);
		break;
	case T_STOCKHOLD:
		m_vecStockHoldIds.push_back(id);
		break;
	case T_STOCKSELL:
		m_vecStockSellIds.push_back(id);
		break;
	case T_STOCKMONEY:
		m_vecStockMoneyIds.push_back(id);
		break;
	default:
		break;
	}
}

/**
 *	Handle right click on grid.
 *  Popup a menu and show the operations as the menu items.
 */
void CStockRecordDlg::OnGridRClick( NMHDR *pNotifyStruct, LRESULT* pResult )
{
	//char* itemData = NULL;
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	// itemData = m_GridCtrl.GetItemText(pItem->iRow, pItem->iColumn);

	/* DO NOT popup submenu on fixed column or row */
	if (m_GridCtrl.IsCellFixed(pItem->iRow, pItem->iColumn)) {
		*pResult = 1;
		return;
	}

// 	/* Make the cell (where right click occurs) selected */
// 	CCellID cellID(pItem->iRow, pItem->iColumn);
// 	m_GridCtrl.

	/* Get position where right click occurs. */
	DWORD dwPos = GetMessagePos();	
	CPoint point (LOWORD(dwPos), HIWORD(dwPos));

	CMenu menu;
	CMenu* popupMenu(NULL);		// MUST be inited.
	menu.LoadMenuA(IDR_MENU2);

	/* Get related submenu according to current opened table */
	switch (m_enumRecordTable) {
	case T_STOCKBUY:
		popupMenu = menu.GetSubMenu(0);
		break;
	case T_STOCKHOLD:
		popupMenu = menu.GetSubMenu(1);
		break;
	case T_STOCKSELL:
		popupMenu = menu.GetSubMenu(2);
		break;
	case T_STOCKMONEY:
		popupMenu = menu.GetSubMenu(3);
		break;
	default:
		break;
	}

	/* Popup the submenu */
	if (popupMenu) {
		popupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);
	}

	menu.DestroyMenu();
	*pResult = 0;
}

std::string CStockRecordDlg::GetActiveTableName( void )
{
	std::string strTableName("");

	switch (m_enumRecordTable) {
	case T_STOCKBUY:
		strTableName = m_strBuyTableName;
		break;
	case T_STOCKHOLD:
		strTableName = m_StrHoldTableName;
		break;
	case T_STOCKSELL:
		strTableName = m_strSellTableName;
		break;
	case T_STOCKMONEY:
		strTableName = m_strMoneyTableName;
		break;
	default:
		strTableName = "";
	}

	return strTableName;
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

	switch (m_enumRecordTable) {

	case T_STOCKBUY:
		if (m_vecStockBuyIds.empty() || seqNo > (int)m_vecStockBuyIds.size())
			return -1;
		else
			id = m_vecStockBuyIds.at(seqNo - 1);
		break;

	case T_STOCKHOLD:
		if (m_vecStockHoldIds.empty() || seqNo > (int)m_vecStockHoldIds.size())
			return -1;
		else
			id = m_vecStockHoldIds.at(seqNo - 1);
		break;

	case T_STOCKSELL:
		if (m_vecStockSellIds.empty() || seqNo > (int)m_vecStockSellIds.size())
			return -1;
		else
			id = m_vecStockSellIds.at(seqNo - 1);
		break;

	case T_STOCKMONEY:
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


/**
 *	Handler for menu item - inserting record to all tables.
 *  uid contains the actual ID of menu item, like IDM_STOCKBUY_REMOVE... (no use)
 *  NOTE: The IDs of four menu items 'remove record' MUST be sequential.
 *  When the deleting action is going on, DONOT reload the data from database.
 */
void CStockRecordDlg::OnMenuRemoveRecord( UINT uid )
{
	/**
	 *	1. Get selected cells and focused cell.
	 *  Note that the selected cells may not be in succession.
	 *  So you should check every cell to see whether it is really selected.
	 *  When a cell is selected, the record in the row is to be deleted.
	 */
	CCellID focusedCell = m_GridCtrl.GetFocusCell();
	CCellRange cellRange(-1, -1, -1, -1);
	int selCount = m_GridCtrl.GetSelectedCount();
	cellRange = m_GridCtrl.GetSelectedCellRange();
	int ret = 0;

	/**
	 * 1.1 No selected cells, or the selected cell range is not valid,
	 * just delete record where focused cell is located. 
	 */
	if (selCount <= 0 || !IsCellRangeValid(cellRange)) {
		int id = GetActiveRecordIdBySeqNo(focusedCell.row);
		if (id > 0)
			ret = DeleteRecordById(id);
		return;
	}

	/**
	 * 1.2 Focused cell is not in the range of selected cell range.
	 * Firstly, delete focused record, 
	 * then, the selected records.
	 */
	int id = 0;
	if (!IsFocusedCellInSelectedRange(focusedCell, cellRange)) {

		/* 1.2.1 Firstly, delete the record where focused cell is located. */
		id = GetActiveRecordIdBySeqNo(focusedCell.row);
		if (id > 0)
			ret = DeleteRecordById(id);
	}

	/**
	 *	1.2.2 Secondly, delete selected records by traversing every cell
	 *  in the range to see whether it is really selected.
	 */
	for (int rowIdx = cellRange.GetMinRow(); 
		 rowIdx < cellRange.GetMaxRow(); 
		 ++rowIdx) {
		// TODO: DELETE selected records in the cell range.
	}


}

int CStockRecordDlg::DeleteRecordById( int id )
{
	if (id <= 0)
		return ERR;

	if (!m_pDatabase || m_nDBStatus != DB_STATUS_OPENED)
		return ERR;

	/* Get active table name. */
	string strTableName = GetActiveTableName();
	if (strTableName.empty())
		return ERR;

	/* Setup sql string */
	string sql("");
	char strId[8] = "";

	_itoa_s(id, strId, 10);
	sql = sql	+ "BEGIN TRANSACTION; " 
		+ " DELETE FROM " + strTableName
		+ " WHERE id = " + strId + "; END TRANSACTION;";

	/* Delete record by id */
	char* errmsg = NULL;
	int ret = 0;
	ret = sqlite3_exec(m_pDatabase, sql.c_str(), NULL, NULL, &errmsg);

	/* Handle error message when error occurs. */
	if (errmsg) {
		CString str;
		str.Format("%s", errmsg);
		MessageBox(str, "ERROR!");
		sqlite3_free(errmsg);
		errmsg = NULL;
		return ret;
	}

	return ret;
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

/** Whether is cell in the range of cellRange. */
BOOL CStockRecordDlg::IsFocusedCellInSelectedRange( 
	const CCellID& cell, const CCellRange& cellRange )
{
	if (cell.row < 0 || cell.col < 0 || !IsCellRangeValid(cellRange))
		return FALSE;

	if (cellRange.GetMinRow() <= cell.row && cell.row <= cellRange.GetMaxRow() &&
		cellRange.GetMinCol() <= cell.col && cell.col <= cellRange.GetMaxCol()) {
			return TRUE;
	}

	return FALSE;
}







