
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

/* Stock buy model */
CStockBuyModel::CStockBuyModel( void )
	: code(_T(""))
	, name(_T(""))
	, buy_price(_T(""))
	, buy_amount(_T(""))
	, buy_date(_T(""))
{
}

CStockBuyModel::CStockBuyModel( const CStockBuyModel& model)
{
	*this = model;
}

CStockBuyModel::~CStockBuyModel( void )
{
}

/* If you want to assign an object to another, operator= MUST be provided. */
CStockBuyModel& CStockBuyModel::operator=( const CStockBuyModel& model )
{
	if (this == &model)
		return *this;

	code = model.code;
	name = model.name;
	buy_price = model.buy_price;
	buy_amount = model.buy_amount;
	buy_date = model.buy_date;

	return *this;
}

/* Stock hold model */
CStockHoldModel::CStockHoldModel( void )
{
}

CStockHoldModel::CStockHoldModel( const CStockHoldModel& model )
{
}
CStockHoldModel::~CStockHoldModel( void )
{
}

CStockHoldModel& CStockHoldModel::operator=( const CStockHoldModel& model )
{
	if (this == &model)
		return *this;
	
}