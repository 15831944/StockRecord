
#include "StdAfx.h"
#include "StockModel.h"



int DeleteRecordById( sqlite3* db, const char* tableName, int id )
{
	if (!db || !tableName || strlen(tableName) == 0 || id <= 0) {
		AfxMessageBox("Cannot delete record by id.");
		return ERR;
	}

	/* Setup sql string */
	string sql("");
	char strId[8] = "";

	_itoa_s(id, strId, 10);
	sql = sql	
		+ " BEGIN TRANSACTION; " 
		+ " DELETE FROM " + tableName
		+ " WHERE id = " + strId 
		+ "; END TRANSACTION;";

	/* Delete record by id */
	char* errmsg = NULL;
	int ret = 0;
	ret = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errmsg);

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

/* Insert a record into stock_buy table, the data is stored in model. 
 * db MUST be valid, and the database must be opened. 
 */
int
InsertBuyRecord( sqlite3* db, const char* strTable, const CStockBuyModel& model )
{
	if (!db || !strTable || strlen(strTable) == 0) {
		AfxMessageBox("Cannot insert record into table");
		return ERR;
	}

	string sql("");
	sql = sql
		+ " BEGIN TRANSACTION; " 
		+ " INSERT INTO " + strTable
		+ " (code, name, buy_price, buy_amount, buy_date) "
		+ " VALUES( "
		+ " \"" + (LPCTSTR)model.code + "\", "
		+ " \"" + (LPCTSTR)model.name + "\", "
		+  (LPCTSTR)model.buy_price + ", "
		+  (LPCTSTR)model.buy_amount + ", "
		+ "date(\"" + (LPCTSTR)model.buy_date + "\")"
		+ " ); "
		+ " END TRANSACTION; ";

	char* errmsg = NULL;
	int ret = 0;
	ret = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errmsg);

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

int
InsertHoldRecord(sqlite3* db, const char* strTable, const CStockHoldModel& model)
{
	if (!db || !strTable || strlen(strTable) == 0) {
		AfxMessageBox("Cannot insert record into table");
		return ERR;
	}

	string sql("");
	sql = sql
		+ " BEGIN TRANSACTION; " 
		+ " INSERT INTO " + strTable
		+ " (code, name, buy_price, hold_cost, hold_amount, even_price) "
		+ " VALUES( "
		+ " \"" + (LPCTSTR)model.code + "\", "
		+ " \"" + (LPCTSTR)model.name + "\", "
		+  (LPCTSTR)model.buy_price + ", "
		+  (LPCTSTR)model.hold_cost + ", "
		+  (LPCTSTR)model.hold_amount + ", "
		+  (LPCTSTR)model.even_price
		+ " ); "
		+ " END TRANSACTION; ";

	char* errmsg = NULL;
	int ret = 0;
	ret = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errmsg);

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

/**
 *	Record id is container in model.id.
 */
int 
UpdateHoldRecord( sqlite3* db, const char* strTable, const CStockHoldModel& model )
{
	if (!db || !strTable || strlen(strTable) == 0) {
		AfxMessageBox("Cannot insert record into table");
		return ERR;
	}

	char idStr[8] = "";
	_itoa_s(model.id, idStr, 10);

	string sql("");
	sql = sql
		+ " BEGIN TRANSACTION; " 
		+ " UPDATE " + strTable
		+ " SET code = " + "\"" + (LPCTSTR)model.code + "\", "
		+ " name = " + "\"" + (LPCTSTR)model.name + "\", "
		+ " buy_price = " + (LPCTSTR)model.buy_price + ", "
		+ " hold_cost = " + (LPCTSTR)model.hold_cost + ", "
		+ " hold_amount = " + (LPCTSTR)model.hold_amount + ", "
		+ " even_price = " + (LPCTSTR)model.even_price + 
		+ " WHERE id = " + idStr + "; "
		+ " END TRANSACTION; ";
	
	char* errmsg = NULL;
	int ret = 0;
	ret = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errmsg);

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

/**
 *	para is OUT parameter, which will hold the record queried from database.
 *  para's actual type is CStockHoldModel;
 */
int StockHoldCallback( void* para, int nCol, char** colValue, char** colName )
{
	if (!para || !colName || !colName)
		return ERR;

	/* assign value according to its column name */
	CStockHoldModel* pModel = (CStockHoldModel*) para;
	pModel->id = !colValue[0] ? -1 : atoi(colValue[0]);
	pModel->code.Format("%s", (!colValue[1] ? "" : colValue[1]));
	pModel->name.Format("%s", (!colValue[2] ? "" :	colValue[2]));
	pModel->buy_price.Format("%s", (!colValue[3] ? "" : colValue[3]));
	pModel->hold_cost.Format("%s", (!colValue[4] ? "" : colValue[4]));
	pModel->hold_amount.Format("%s", (!colValue[5] ? "" : colValue[5]));
	pModel->even_price.Format("%s", (!colValue[6] ? "" : colValue[6]));
	pModel->SetEncodeStyle(ENCODE_STYLE_UTF8);

	return OK;
}

/**
 *	Note that the records in table stock_hold are unique by code.
 *  So this function will always query one record if there is.
 *  If no record, return an empty object.
 */
CStockHoldModel 
SelectHoldRecordByCode( sqlite3*db, const char* strTable, const char* code)
{
	if (!db || !strTable || strlen(strTable) == 0
		|| !code || strlen(code) == 0)
		return CStockHoldModel();

	CStockHoldModel holdModel;
	string sql("");
	sql = sql 
		+ "SELECT * FROM "
		+ strTable
		+ " WHERE code = "
		+ "\"" + code + "\" LIMIT 1;";

	char* errmsg = NULL;
	int ret = 0;
	ret = sqlite3_exec(db, sql.c_str(), StockHoldCallback, &holdModel, &errmsg);

	/* Handle error message when error occurs. */
	if (errmsg) {
		CString str;
		str.Format("%s", errmsg);
		AfxMessageBox(str);
		sqlite3_free(errmsg);
		errmsg = NULL;
		return CStockHoldModel();
	}

	return holdModel;
}


CStockHoldModel SelectHoldRecordById( sqlite3*db, const char* strTable, int id )
{
	if (!db || !strTable || strlen(strTable) == 0
		|| id <= 0)
		return CStockHoldModel();

	char idStr[10] = "";
	_itoa_s(id, idStr, 10);

	CStockHoldModel holdModel;
	string sql("");
	sql = sql 
		+ "SELECT * FROM "
		+ strTable
		+ " WHERE id = "
		+ "\"" + idStr + "\" LIMIT 1;";

	char* errmsg = NULL;
	int ret = 0;
	ret = sqlite3_exec(db, sql.c_str(), StockHoldCallback, &holdModel, &errmsg);

	/* Handle error message when error occurs. */
	if (errmsg) {
		CString str;
		str.Format("%s", errmsg);
		AfxMessageBox(str);
		sqlite3_free(errmsg);
		errmsg = NULL;
		return CStockHoldModel();
	}

	return holdModel;
}

int ConvertEncodeFormat( CStockModelBase* pModel, int targetEncode )
{
	if (!pModel || pModel->GetEncodeStyle() == targetEncode)
		return ERR;

	switch (targetEncode) {

	// TO BE opted. Convert more members.
	case ENCODE_STYLE_UTF8:
		pModel->name.Format("%s", 
			CChineseCodeLib::GB2312ToUTF8((LPCTSTR)pModel->name).c_str());
		pModel->SetEncodeStyle(ENCODE_STYLE_UTF8);
		break;

	case ENCODE_STYLE_GB2312:
		pModel->name.Format("%s", 
			CChineseCodeLib::UTF8ToGB2312((LPCTSTR)pModel->name).c_str());
		pModel->SetEncodeStyle(ENCODE_STYLE_GB2312);
		break;

	default:
		break;
	}

	return 0;
}
/* ===================== Stock sell model =====================  */

CStockModelBase::CStockModelBase(void)
	: m_encodeStyle(ENCODE_STYLE_GB2312)
	, name(_T("基类name"))
	, code(_T("基类code"))
{
}

CStockModelBase::~CStockModelBase(void)
{
}

void CStockModelBase::SetEncodeStyle( int encodeStyle )
{
	switch (encodeStyle) {

	case ENCODE_STYLE_GB2312:
		m_encodeStyle = ENCODE_STYLE_GB2312;
		break;

	case ENCODE_STYLE_UTF8:
		m_encodeStyle = ENCODE_STYLE_UTF8;
		break;

	default:
		break;
	}
}

int CStockModelBase::GetEncodeStyle( void ) const
{
	switch (m_encodeStyle) {
	case ENCODE_STYLE_GB2312:
		return ENCODE_STYLE_GB2312;
		break;

	case ENCODE_STYLE_UTF8:
		return ENCODE_STYLE_UTF8;
		break;

	default:
		return ENCODE_STYLE_GB2312;
		break;
	}
}

/**
 *	Stock buy model
 */ 
CStockBuyModel::CStockBuyModel( void )
	: CStockModelBase()		/* construct your base object firstly */
	, id(-1)
//	, code(_T(""))
//	, name(_T(""))
	, buy_price(_T(""))
	, buy_amount(_T(""))
	, buy_date(_T(""))
	, stock_type()
{
}

CStockBuyModel::~CStockBuyModel( void )
{
}

// void CStockBuyModel::ConvertEncodeFormat( int targetEncode )
// {
// 	if (GetEncodeStyle() == targetEncode)
// 		return ;
// 
// 	switch (targetEncode) {
// 
// 	// TO BE opted. Convert more members.
// 	case ENCODE_STYLE_UTF8:
// 		this->name.Format("%s", 
// 			CChineseCodeLib::GB2312ToUTF8((LPCTSTR)this->name).c_str());
// 		SetEncodeStyle(ENCODE_STYLE_UTF8);
// 		break;
// 
// 	case ENCODE_STYLE_GB2312:
// 		this->name.Format("%s", 
// 			CChineseCodeLib::UTF8ToGB2312((LPCTSTR)this->name).c_str());
// 		SetEncodeStyle(ENCODE_STYLE_GB2312);
// 		break;
// 
// 	default:
// 		break;
// 	}
// }

/**
 *	=====================  Stock hold model ===================== 
 */ 
CStockHoldModel::CStockHoldModel( void )
	: CStockModelBase()
	, id(-1)
//	, code(_T(""))
//	, name(_T(""))
	, buy_price(_T(""))
	, hold_cost(_T(""))
	, hold_amount(_T(""))
	, even_price(_T(""))
{
}

CStockHoldModel::~CStockHoldModel( void )
{
}

// void CStockHoldModel::ConvertEncodeFormat( int targetEncode )
// {
// 	if (GetEncodeStyle() == targetEncode)
// 		return ;
// 
// 	switch (targetEncode) {
// 
// 	// TO BE opted. Convert more members.
// 	case ENCODE_STYLE_UTF8:
// 		this->name.Format("%s", 
// 			CChineseCodeLib::GB2312ToUTF8((LPCTSTR)this->name).c_str());
// 		SetEncodeStyle(ENCODE_STYLE_UTF8);
// 		break;
// 
// 	case ENCODE_STYLE_GB2312:
// 		this->name.Format("%s", 
// 			CChineseCodeLib::UTF8ToGB2312((LPCTSTR)this->name).c_str());
// 		SetEncodeStyle(ENCODE_STYLE_GB2312);
// 		break;
// 
// 	default:
// 		break;
// 	}
// }

/* ========== Stock Sell Model ============ */

CStockSellModel::CStockSellModel( void )
	: CStockModelBase()
	, id(0)
//	, code(_T(""))
//	, name(_T(""))
	, buy_price(_T(""))
	, sell_price(_T(""))
	, sell_amount(_T(""))
	, sell_date(_T(""))
	, each_earn(_T(""))
{

}

CStockSellModel::~CStockSellModel( void )
{
}

// void CStockSellModel::ConvertEncodeFormat( int targetEncode )
// {
// 	if (GetEncodeStyle() == targetEncode)
// 		return ;
// 
// 	switch (targetEncode) {
// 
// 		// TO BE opted. Convert more members.
// 	case ENCODE_STYLE_UTF8:
// 		this->name.Format("%s", 
// 			CChineseCodeLib::GB2312ToUTF8((LPCTSTR)this->name).c_str());
// 		SetEncodeStyle(ENCODE_STYLE_UTF8);
// 		break;
// 
// 	case ENCODE_STYLE_GB2312:
// 		this->name.Format("%s", 
// 			CChineseCodeLib::UTF8ToGB2312((LPCTSTR)this->name).c_str());
// 		SetEncodeStyle(ENCODE_STYLE_GB2312);
// 		break;
// 
// 	default:
// 		break;
// 	}
// }