#include "StdAfx.h"
#include "StockDBConnection.h"


CStockTableDataInfo::CStockTableDataInfo(void)
	: m_nRow(-1)
	, m_nCol(-1)
	, m_pData(NULL)
	, m_errcode(OK)
{
}


CStockTableDataInfo::~CStockTableDataInfo(void)
{
	m_pData = NULL;
}


CStockDBConnection::CStockDBConnection(void)
	: m_pDatabase(NULL)
	, m_nDBStatus(-1)
{
}


CStockDBConnection::~CStockDBConnection(void)
{
	DisConnect();
}

int CStockDBConnection::Connect( void )
{
	if (m_pDatabase && m_nDBStatus == DB_STATUS_OPENED) 	// already opened.
		return OK;

	if (m_sDBName.empty()) 
		return BAD_DB_TABLE_NAME;

	/* Database file exists, and read-only open successfully. */
	int ret = 0;
	ret = sqlite3_open_v2(m_sDBName.c_str(), &m_pDatabase, SQLITE_OPEN_READWRITE, NULL);
	if (ret == SQLITE_OK) {
		m_nDBStatus = DB_STATUS_OPENED;
		return ret;
	}

	/**
	 *	Open Read-only database failed, because db does not exist.
	 *  Now create a new database.
	 */
	m_nDBStatus = DB_STATUS_DOESNOT_EXIST;
	ret = sqlite3_open_v2(m_sDBName.c_str(), &m_pDatabase,
		SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	if (ret != SQLITE_OK) 
		return ret;

	/**
	 *	Initiate new database's tables.
	 */
	m_nDBStatus = DB_STATUS_OPENED;
	ret = InitDatabaseStockTables();
	ret = InitDatabaseFutureTables();

	if (ret == SQLITE_OK)
		AfxMessageBox("Database does not exist.\nIts tables has been inited.");

	return ret;
}

int CStockDBConnection::DisConnect( void )
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

int CStockDBConnection::InitDatabaseStockTables( void )
{
	if (!m_pDatabase || m_nDBStatus != DB_STATUS_OPENED) {
		return ERR;
	}

//	ASSERT(IsTableNamesValid());

	int ret = 0;
	char* errmsg = NULL;
	string sql("");

	/**
	 *	Init stock_buy table.
	 */
	sql.clear();
	sql = sql 
		+ "CREATE TABLE "
		+ m_sBuyTableName 
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
			str.Format("%s", errmsg);
			AfxMessageBox(str);
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
		+ m_sHoldTableName
		+ " (	id INTEGER PRIMARY KEY AUTOINCREMENT, "
		+ "		code		VARCHAR(6) NOT NULL, "
		+ "		name		VARCHAR(10), "
		+ "		buy_price	FLOAT, "
		+ "		hold_cost	FLOAT, "
		+ "		hold_amount	INTEGER, "
		+ "		even_price	FLOAT, "
		+ "		buy_date	DATE "
		+ ")";
	ret = sqlite3_exec(m_pDatabase, sql.c_str(), NULL, NULL, &errmsg);
	if (ret != SQLITE_OK) {
		if (errmsg) {
			CString str;
			str.Format("%s", errmsg);
			AfxMessageBox(str);
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
		+ m_sSellTableName
		+ " (	id INTEGER PRIMARY KEY AUTOINCREMENT, "
		+ "		code		VARCHAR(6) NOT NULL, "
		+ "		name		VARCHAR(10), "
		+ "		buy_price	FLOAT, "
		+ "		sell_price	FLOAT, "
		+ "		sell_amount	INTEGER, "
		+ "		even_price	FLOAT, "
		+ "		buy_date	DATE, "
		+ "		sell_date	DATE, "
		+ "		each_earn	FLOAT, "
		+ "		total_earn	FLOAT "
		+ ")";
	ret = sqlite3_exec(m_pDatabase, sql.c_str(), NULL, NULL, &errmsg);
	if (ret != SQLITE_OK) {
		if (errmsg) {
			CString str;
			str.Format("%s", errmsg);
			AfxMessageBox(str);
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
		+ m_sMoneyTableName
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
			str.Format("%s", errmsg);
			AfxMessageBox(str);
			sqlite3_free(errmsg);
			errmsg = NULL;
		}
		return ret;
	}

	return ret;
}

BOOL CStockDBConnection::IsTableNamesValie( void )
{
	if (m_sBuyTableName.empty() || m_sHoldTableName.empty()
		|| m_sHoldTableName.empty() || m_sMoneyTableName.empty()) {

		return FALSE;
	}

	return TRUE;	
}

std::string CStockDBConnection::GetActiveTableName( ENUMACTIVETABLE activeTable )
{
	switch (activeTable) {
	case ACTIVE_TABLE_BUY:
		return m_sBuyTableName;
		break;
	case ACTIVE_TABLE_HOLD:
		return m_sHoldTableName;
		break;
	case ACTIVE_TABLE_SELL:
		return m_sSellTableName;
		break;
	case ACTIVE_TABLE_MONEY:
		return m_sMoneyTableName;
		break;
	default:
		return "";
	}

	return "";		// Never goes here.
}

CStockTableDataInfo 
CStockDBConnection::QueryWholeTableData( ENUMACTIVETABLE whichTable )
{
	CStockTableDataInfo tableDataInfo;

	if (!m_pDatabase || m_nDBStatus != DB_STATUS_OPENED) {
		tableDataInfo.m_errcode = DB_STATUS_CLOSED;
		return tableDataInfo;
	}

	string tableName = GetActiveTableName(whichTable);
	if (tableName.empty()) {
		tableDataInfo.m_errcode = BAD_DB_TABLE_NAME;
		return tableDataInfo;
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
	 *	Query all records by using sqlite3_get_table() func.
	 */
	ret = sqlite3_get_table(m_pDatabase, sql.c_str(), &result, &nRow, &nCol, &errmsg);
	if (ret != SQLITE_OK) {
		if (errmsg) {
			CString str;
			str.Format("Cannot open table %s. %s",tableName ,errmsg);
			AfxMessageBox(str);
			sqlite3_free(errmsg);
			errmsg = NULL;
		}
		tableDataInfo.m_errcode = ret;
		return tableDataInfo;
	}

	/* Assign fields of CStockTableDataInfo */
	tableDataInfo.m_nRow = nRow;
	tableDataInfo.m_nCol = nCol;
	tableDataInfo.m_pData = result;
	tableDataInfo.m_errcode = ret;
	return tableDataInfo;
}

int CStockDBConnection::ReleaseWholeTableData( char** pData )
{
	if (!pData)
		return ERR;

	sqlite3_free_table(pData);	/* Never forget to free result table. */
	pData = NULL;
	return OK;
}

int CStockDBConnection::RemoveRecordByTableId( ENUMACTIVETABLE whichTable, int id )
{
	string strTableName = GetActiveTableName(whichTable);
	if (!m_pDatabase || m_nDBStatus != DB_STATUS_OPENED || strTableName.empty())
		return ERR;

	/* Setup sql string */
	string sql("");
	char strId[8] = "";

	_itoa_s(id, strId, 10);
	sql = sql	
		+ " BEGIN TRANSACTION; " 
		+ " DELETE FROM " + strTableName
		+ " WHERE id = " + strId 
		+ "; END TRANSACTION;";



	/* Delete record by id */
	char* errmsg = NULL;
	int ret = 0;
	ret = sqlite3_exec(m_pDatabase, sql.c_str(), NULL, NULL, &errmsg);

	/* Handle error message when error occurs. */
	if (errmsg) {
		CString str;
		str.Format("%s", errmsg);
		AfxMessageBox(str);
		sqlite3_free(errmsg);
		errmsg = NULL;
		return ret;
	}

	return ret;
}

int CStockDBConnection::InsertBuyRecord( const CStockModelBuy& model )
{
	if (!m_pDatabase || m_sBuyTableName.empty()) {
		AfxMessageBox("Cannot insert record into stock_buy table");
		return ERR;
	}

	string sql("");
	sql = sql
		+ " BEGIN TRANSACTION; " 
		+ " INSERT INTO " + m_sBuyTableName
		+ " (code, name, buy_price, buy_amount, buy_date) "
		+ " VALUES( "
		+ " \"" + (LPCTSTR)model.code + "\", "
		+ " \"" + (LPCTSTR)model.name + "\", "
		+ " \"" + (LPCTSTR)model.buy_price + "\", "
		+ " \"" + (LPCTSTR)model.buy_amount + "\", "
		+ "date(\"" + (LPCTSTR)model.buy_date + "\")"
		+ " ); "
		+ " END TRANSACTION; ";

	char* errmsg = NULL;
	int ret = 0;
	ret = sqlite3_exec(m_pDatabase, sql.c_str(), NULL, NULL, &errmsg);

	/* Handle error message when error occurs. */
	if (errmsg) {
		CString str;
		str.Format("%s", errmsg);
		AfxMessageBox(str);
		sqlite3_free(errmsg);
		errmsg = NULL;
		return ret;
	}

	return ret;
}

int CStockDBConnection::InsertHoldRecord( const CStockModelHold& model )
{
	if (!m_pDatabase || m_sHoldTableName.empty()) {
		AfxMessageBox("Cannot insert record into stock_hold table");
		return ERR;
	}

	string sql("");
	sql = sql
		+ " BEGIN TRANSACTION; " 
		+ " INSERT INTO " + m_sHoldTableName
		+ " (code, name, buy_price, hold_cost, hold_amount, even_price, buy_date) "
		+ " VALUES( "
		+ " \"" + (LPCTSTR)model.code + "\", "
		+ " \"" + (LPCTSTR)model.name + "\", "
		+ " \"" + (LPCTSTR)model.buy_price + "\", "
		+ " \"" + (LPCTSTR)model.hold_cost + "\", "
		+ " \"" + (LPCTSTR)model.hold_amount + "\", "
		+ " \"" + (LPCTSTR)model.even_price + "\", "
		+ "date(\"" + (LPCTSTR)model.buy_date + "\")"
		+ " ); "
		+ " END TRANSACTION; ";

	char* errmsg = NULL;
	int ret = 0;
	ret = sqlite3_exec(m_pDatabase, sql.c_str(), NULL, NULL, &errmsg);

	/* Handle error message when error occurs. */
	if (errmsg) {
		CString str;
		str.Format("%s", errmsg);
		AfxMessageBox(str);
		sqlite3_free(errmsg);
		errmsg = NULL;
		return ret;
	}

	return ret;
}

int CStockDBConnection::InsertSellRecord( const CStockModelSell& model )
{
	if (!m_pDatabase || m_sSellTableName.empty()) {
		AfxMessageBox("Cannot insert record into stock_sell table");
		return ERR;
	}

	string sql("");
	sql = sql
		+ " BEGIN TRANSACTION; " 
		+ " INSERT INTO " + m_sSellTableName
		+ " (code, name, buy_price, sell_price, sell_amount, "
		+ " even_price, buy_date, sell_date, each_earn, total_earn) "
		+ " VALUES( "
		+ " \"" + (LPCTSTR)model.code + "\", "
		+ " \"" + (LPCTSTR)model.name + "\", "
		+ " \"" + (LPCTSTR)model.buy_price + "\", "
		+ " \"" + (LPCTSTR)model.sell_price + "\", "
		+ " \"" + (LPCTSTR)model.sell_amount + "\", "
		+ " \"" + (LPCTSTR)model.even_price + "\", "
		+ " date(\"" + (LPCTSTR)model.buy_date + "\"), "
		+ " date(\"" + (LPCTSTR)model.sell_date + "\"), "
		+ " \"" + (LPCTSTR)model.each_earn + "\", "
		+ " \"" + (LPCTSTR)model.total_earn  + "\" "
		+ " ); "
		+ " END TRANSACTION; ";

	char* errmsg = NULL;
	int ret = 0;
	ret = sqlite3_exec(m_pDatabase, sql.c_str(), NULL, NULL, &errmsg);

	/* Handle error message when error occurs. */
	if (errmsg) {
		CString str;
		str.Format("%s", errmsg);
		AfxMessageBox(str);
		sqlite3_free(errmsg);
		errmsg = NULL;
		return ret;
	}

	return ret;
}

int CStockDBConnection::InsertMoneyRecord( const CStockModelMoney& model )
{
	return 0;
}

int StockHoldRecordCallback( void* para, int nCol, char** colValue, char** colName )
{
	if (!para || !colName || !colName)
		return ERR;

	/* assign value according to its column name */
	CStockModelHold* pModel = (CStockModelHold*) para;
	pModel->id = !colValue[0] ? -1 : atoi(colValue[0]);
	pModel->code.Format("%s", (!colValue[1] ? "" : colValue[1]));
	pModel->name.Format("%s", (!colValue[2] ? "" :	colValue[2]));
	pModel->buy_price.Format("%s", (!colValue[3] ? "" : colValue[3]));
	pModel->hold_cost.Format("%s", (!colValue[4] ? "" : colValue[4]));
	pModel->hold_amount.Format("%s", (!colValue[5] ? "" : colValue[5]));
	pModel->even_price.Format("%s", (!colValue[6] ? "" : colValue[6]));
	pModel->buy_date.Format("%s", (!colValue[7] ? "" : colValue[7]));
	pModel->SetEncodeStyle(ENCODE_STYLE_UTF8);

	return OK;
}

CStockModelHold CStockDBConnection::SelectHoldModelByCode( const char* code )
{
	if (!m_pDatabase || m_sHoldTableName.empty() || !code || strlen(code) == 0)
		return CStockModelHold();

	CStockModelHold holdModel;
	string sql("");
	sql = sql 
		+ "SELECT * FROM "
		+ m_sHoldTableName
		+ " WHERE code = "
		+ "\"" + code + "\" LIMIT 1;";

	char* errmsg = NULL;
	int ret = 0;
	ret = sqlite3_exec(m_pDatabase, sql.c_str(), StockHoldRecordCallback, \
		&holdModel, &errmsg);

	/* Handle error message when error occurs. */
	if (errmsg) {
		CString str;
		str.Format("%s", errmsg);
		AfxMessageBox(str);
		sqlite3_free(errmsg);
		errmsg = NULL;
		return CStockModelHold();
	}

	return holdModel;
}

CStockModelHold CStockDBConnection::SelectHoldModelById( int id )
{
	if (!m_pDatabase || m_sHoldTableName.empty() || id <= 0)
		return CStockModelHold();

	char idStr[10] = "";
	_itoa_s(id, idStr, 10);

	CStockModelHold holdModel;
	string sql("");
	sql = sql 
		+ "SELECT * FROM "
		+ m_sHoldTableName
		+ " WHERE id = "
		+ "\"" + idStr + "\" LIMIT 1;";

	char* errmsg = NULL;
	int ret = 0;
	ret = sqlite3_exec(m_pDatabase, sql.c_str(), StockHoldRecordCallback, \
		&holdModel, &errmsg);

	/* Handle error message when error occurs. */
	if (errmsg) {
		CString str;
		str.Format("%s", errmsg);
		AfxMessageBox(str);
		sqlite3_free(errmsg);
		errmsg = NULL;
		return CStockModelHold();
	}

	return holdModel;
}

int CStockDBConnection::UpdateHoldModel( const CStockModelHold& model )
{
	if (!m_pDatabase || m_sHoldTableName.empty()) {
		AfxMessageBox("Cannot update record in stock_hold table");
		return ERR;
	}

	char idStr[8] = "";
	_itoa_s(model.id, idStr, 10);

	string sql("");
	sql = sql
		+ " BEGIN TRANSACTION; " 
		+ " UPDATE " + m_sHoldTableName
		+ " SET code = " + "\"" + (LPCTSTR)model.code + "\", "
		+ " name = " + "\"" + (LPCTSTR)model.name + "\", "
		+ " buy_price = " + "\"" + (LPCTSTR)model.buy_price + "\", "
		+ " hold_cost = " + "\"" + (LPCTSTR)model.hold_cost + "\", "
		+ " hold_amount = " + "\"" + (LPCTSTR)model.hold_amount + "\", "
		+ " even_price = " + "\"" + (LPCTSTR)model.even_price + "\", "
		+ " buy_date = " + "date(\"" + (LPCTSTR)model.buy_date + "\")"
		+ " WHERE id = " + idStr + "; "
		+ " END TRANSACTION; ";

	char* errmsg = NULL;
	int ret = 0;
	ret = sqlite3_exec(m_pDatabase, sql.c_str(), NULL, NULL, &errmsg);

	/* Handle error message when error occurs. */
	if (errmsg) {
		CString str;
		str.Format("%s", errmsg);
		AfxMessageBox(str);
		sqlite3_free(errmsg);
		errmsg = NULL;
		return ret;
	}

	return ret;
}

int CStockDBConnection::UpdateSellTotalEarn( void )
{
	if (!m_pDatabase || m_sSellTableName.empty()) {
		AfxMessageBox("Cannot update record in stock_sell table");
		return ERR;
	}

	string sql ("");
	sql = sql 
		+ " BEGIN TRANSACTION; " 
		+ " UPDATE " + m_sSellTableName
		+ " SET total_earn = (SELECT SUM(each_earn) FROM " + m_sSellTableName + ")"
		+ " WHERE id = (SELECT id FROM " + m_sSellTableName + " LIMIT 1);"
		+ " END TRANSACTION; ";

	char* errmsg = NULL;
	int ret = 0;
	ret = sqlite3_exec(m_pDatabase, sql.c_str(), NULL, NULL, &errmsg);

	/* Handle error message when error occurs. */
	if (errmsg) {
		CString str;
		str.Format("%s", errmsg);
		AfxMessageBox(str);
		sqlite3_free(errmsg);
		errmsg = NULL;
		return ret;
	}

	return ret;
}


// TODO: Create tables if db doesnot exist.
int CStockDBConnection::InitDatabaseFutureTables( void )
{
	int ret = 0;

	return ret;
}



