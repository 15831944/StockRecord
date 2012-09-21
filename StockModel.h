/**
 *	Define model class to hold values queried from or written to database.
 *  The model can be defined as class or struct.
 *  Also, provide some functions to manipulate the database.
 *
 *  NOTE: The model's members should store values converted into GB2312 from UTF8.
 */

#ifndef STOCK_RECORD_MODEL_H
#define STOCK_RECORD_MODEL_H

extern "C" {
	#include <sqlite3.h>	// SQLite support.
};

/**
 * Base model 
 */
class CStockModelBase
{
public:
	CStockModelBase(void);
	~CStockModelBase(void);

public:
	CString code;
	CString name;

private:
	enum EncodeStyle m_encodeStyle;

public:
	void SetEncodeStyle(int encodeStyle);
	int  GetEncodeStyle(void) const;

	virtual int ConvertEncodeFormat(int targetEncode);

};

/**
 *	Stock buy record model.
 */
class CStockBuyModel : public CStockModelBase
{
public:
 	CStockBuyModel(void);
	~CStockBuyModel(void);

public:
	int id;
	CString buy_price;
	CString buy_amount;
	CString buy_date;
	bool stock_type;

public:
};

/**
 *	Stock hold record model.
 */
class CStockHoldModel : public CStockModelBase
{
public:
	CStockHoldModel(void);
	~CStockHoldModel(void);


public:
	/**
	 * id will be inited to -1, to indicate there is no record in db. 
	 * If id is not -1, the model represents a record which already exists in db.
	 * In this case, id is the value of actual record id in database.
	 */
	int id;		
	CString buy_price;
	CString hold_cost;
	CString hold_amount;
	CString even_price;
};

/**
 *	Stock sell record model.
 */
class CStockSellModel : public CStockModelBase
{
public:
	CStockSellModel(void);
	~CStockSellModel(void);


public:
	int id;
	CString buy_price;
	CString sell_price;
	CString sell_amount;
	CString sell_date;
	CString even_price;
	CString each_earn;
	CString total_earn;
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

/** 
 * Insert / update records held in model into table stored in sqlite database.
 * The model's member's format MUST be UTF8. 
 */
int InsertBuyRecord(sqlite3* db, const char* buyTable, const CStockBuyModel& model);
int InsertHoldRecord(sqlite3* db, const char* strTable, const CStockHoldModel& model);
int UpdateHoldRecord(sqlite3* db, const char* strTable, const CStockHoldModel& model);
int InsertSellRecord(sqlite3* db, const char* strTable, const CStockSellModel& model);
int UpdateSellTotalEarn(sqlite3* db, const char* strTable);

int StockHoldCallback(void* para, int nCol, char** colValue, char** colName);
CStockHoldModel 
SelectHoldRecordByCode(sqlite3*db, const char* strTable, const char* code);
CStockHoldModel
SelectHoldRecordById(sqlite3*db, const char* strTable, int id);

#endif