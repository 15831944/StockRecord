
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
 * db MUST be valid, and the database must be opened. */
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
		/* TOBE optimized, only convert name to UTF8 from GB2312 when inserting */
		+ " \"" + CChineseCodeLib::GB2312ToUTF8((LPCTSTR)model.name) + "\", "
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

	int ret = 0;
	// TODO: insert hold record.
	/* 1. Check out whether stock(code) is in stock_hold table. */

	/* 2.1 If stock is in stock_hold table, update hold record. */

	/* 2.2 If stock is not in stock_hold table, insert hold record. */

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
	pModel->code.Format("%s", (!colValue[1] ? "" : colValue[1]));
	/* To be opted, convert format for all members if they are Chinese words. */
	pModel->name.Format("%s", (!colValue[2] ? "" :	\
		CChineseCodeLib::UTF8ToGB2312(colValue[2]).c_str()));
	pModel->buy_price.Format("%s", (!colValue[3] ? "" : colValue[3]));
	pModel->hold_cost.Format("%s", (!colValue[4] ? "" : colValue[4]));
	pModel->hold_amount.Format("%s", (!colValue[5] ? "" : colValue[5]));
	pModel->even_price.Format("%s", (!colValue[6] ? "" : colValue[6]));

	return OK;
}

/**
 *	Note that the records in table stock_hold are unique by code.
 *  So this function will always query one record if there is.
 *  If no record, return an emply object.
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

/**
 *	Stock buy model
 */ 
CStockBuyModel::CStockBuyModel( void )
	: code(_T(""))
	, name(_T(""))
	, buy_price(_T(""))
	, buy_amount(_T(""))
	, buy_date(_T(""))
	, stock_type()
{
}

CStockBuyModel::CStockBuyModel( const CStockBuyModel& model)
{
	*this = model;	// will call operator=()
}

CStockBuyModel::~CStockBuyModel( void )
{
}

/* If you want to assign an object to another, operator= MUST be provided. */
CStockBuyModel& CStockBuyModel::operator=( const CStockBuyModel& model )
{
	if (this == &model)
		return *this;

	this->code = model.code;
	this->name = model.name;
	this->buy_price = model.buy_price;
	this->buy_amount = model.buy_amount;
	this->buy_date = model.buy_date;

	return *this;
}

/**
 *	Stock hold model
 */ 
CStockHoldModel::CStockHoldModel( void )
	: code(_T(""))
	, name(_T(""))
	, buy_price(_T(""))
	, hold_cost(_T(""))
	, hold_amount(_T(""))
	, even_price(_T(""))
{
}

CStockHoldModel::CStockHoldModel( const CStockHoldModel& model )
{
	*this = model;
}
CStockHoldModel::~CStockHoldModel( void )
{
}

CStockHoldModel& CStockHoldModel::operator=( const CStockHoldModel& model )
{
	if (this == &model)
		return *this;
	
	this->code = model.code;
	this->name = model.name;
	this->buy_price = model.buy_price;
	this->hold_cost = model.hold_cost;
	this->hold_amount = model.hold_amount;

	return *this;
}

/* Stock sell model */