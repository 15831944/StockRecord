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
// 	CStockModelBase(const CStockModelBase& base);
// 	CStockModelBase& operator= (const CStockModelBase& base);

private:
	enum EncodeStyle m_encodeStyle;

public:
	virtual  void SetEncodeStyle(int encodeStyle);
	virtual  int GetEncodeStyle(void) const;

};

/**
 *	Stock buy record model.
 */
class CStockBuyModel : public CStockModelBase
{
public:
 	CStockBuyModel(void);

	/**
	 *	NOTE: You MUST provide the following 2 functions if you want to 
	 *  1. return an object in a function call.		{ return object;}
	 *  2. pass an object to a function parameter.	Func(X object){}
	 *  3. init an object with another.				X objA(objB);
	 *  4. assign an object with another.			objA = objB;
	 */
	//CStockBuyModel(const CStockBuyModel& model);
 	//CStockBuyModel& operator = (const CStockBuyModel& model);
	~CStockBuyModel(void);

public:
	int id;
	CString code;
	CString name;
	CString buy_price;
	CString buy_amount;
	CString buy_date;
	bool stock_type;

public:
	void ConvertEncodeFormat(int targetEncode);
};

/**
 *	Stock hold record model.
 */
class CStockHoldModel : public CStockModelBase
{
public:
	CStockHoldModel(void);
	~CStockHoldModel(void);
	//CStockHoldModel(const CStockHoldModel& model);
	//CStockHoldModel& operator = (const CStockHoldModel& model);

	void ConvertEncodeFormat(int targetEncode);

public:
	/**
	 * id will be inited to -1, to indicate there is no record in db. 
	 * If id is not -1, the model represents a record which already exists in db.
	 * In this case, id is the value of actual record id in database.
	 */
	int id;		
	CString code;
	CString name;
	CString buy_price;
	CString hold_cost;
	CString hold_amount;
	CString even_price;
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

/** 
 * Insert / update records held in model into table stored in sqlite database.
 * The model's member's format MUST be UTF8. 
 */
int InsertBuyRecord(sqlite3* db, const char* buyTable, const CStockBuyModel& model);
int InsertHoldRecord(sqlite3* db, const char* strTable, const CStockHoldModel& model);
int UpdateHoldRecord(sqlite3* db, const char* strTable, const CStockHoldModel& model);

int StockHoldCallback(void* para, int nCol, char** colValue, char** colName);
CStockHoldModel 
SelectHoldRecordByCode(sqlite3*db, const char* strTable, const char* code);
CStockHoldModel
SelectHoldRecordById(sqlite3*db, const char* strTable, int id);

#endif