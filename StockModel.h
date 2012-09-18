/**
 *	Define model class to hold values queried from or written to database.
 *  The model can be defined as class or struct.
 *  Also, provide some functions to manipulate the database.
 */

#ifndef STOCK_RECORD_MODEL_H
#define STOCK_RECORD_MODEL_H

extern "C" {
	#include <sqlite3.h>	// SQLite support.
};

/**
 *	Stock buy record model.
 */
class CStockBuyModel
{
public:
 	CStockBuyModel(void);

	/**
	 *	NOTE: You MUST provide the following 2 functions if you want to 
	 *  return an object in a function call.
	 */
	CStockBuyModel(const CStockBuyModel& model);
 	~CStockBuyModel(void);
	CStockBuyModel& operator = (const CStockBuyModel& model);

public:
	CString code;
	CString name;
	CString buy_price;
	CString buy_amount;
	CString buy_date;
};

/**
 *	Stock hold record model.
 */
class CStockHoldModel
{
public:
	CStockHoldModel(void);
	~CStockHoldModel(void);
	CStockHoldModel(const CStockHoldModel& model);
	CStockHoldModel& operator = (const CStockHoldModel& model);
};

/**
 *	Stock sell record model.
 */
class CStockSellModel
{

};

/**
 *	Stock money record model.
 */
class CStockMoneyModel
{

};

/** 
 *  Delete record by id in tableName.
 *  NOTE: db must be valid and points to database which is already opened.
 */
int DeleteRecordById(sqlite3* db, const char* tableName, int id);

#endif