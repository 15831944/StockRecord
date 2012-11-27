/**
 *	Class for connecting database, and operations like select, delete, update.
 */

#ifndef  STOCK_DBCONNECTION_H
#define  STOCK_DBCONNECTION_H

#include <sqlite3.h>		/* SQLite support. */
#include "StockModel.h"		/* Model to hold values. */
#include <string>
using namespace std;

enum ENUMACTIVETABLE		/* Which table is displayed now. */
{
	ACTIVE_TABLE_BUY = 0,	/* table stock_buy */
	ACTIVE_TABLE_HOLD,		/* table stock_hold */
	ACTIVE_TABLE_SELL,		/* table stock_sell */
	ACTIVE_TABLE_MONEY		/* table stock_money */
};

class CStockTableDataInfo;
int StockHoldRecordCallback( void* para, int nCol, char** colValue, char** colName ); 


/**
 *	Hold information of a whole table, like how many rows and columns,
 *  where the table data is starting from.
 *  Used when querying data by using sqlite3_get_table().
 */
class CStockTableDataInfo
{
public:
	CStockTableDataInfo(void);
	~CStockTableDataInfo(void);
public:
	int m_nRow;		// How many rows
	int m_nCol;		// How many columns
	char** m_pData;
	int m_errcode;
};

class CStockDBConnection
{
public:
	CStockDBConnection(void);
	~CStockDBConnection(void);

private:
	/* database and tables' names */
	string m_sDBName;
	string m_sBuyTableName;
	string m_sHoldTableName;
	string m_sSellTableName;
	string m_sMoneyTableName;

	/* database connection */
	sqlite3* m_pDatabase;
	int m_nDBStatus;
	ENUMACTIVETABLE m_eActiveTable;
	
public:
	void SetDatabaseName(string dbName)		{m_sDBName = dbName;}
	void SetBuyTableName(string tableName)	{m_sBuyTableName = tableName;}
	void SetHoldTableName(string tableName)	{m_sHoldTableName = tableName;}
	void SetSellTableName(string tableName)	{m_sSellTableName = tableName;}
	void SetMoneyTableName(string tableName){m_sMoneyTableName = tableName;}

public:
	void SetActiveTable(ENUMACTIVETABLE activeTable){m_eActiveTable = activeTable;}
	ENUMACTIVETABLE GetActiveTable(void) const		{return m_eActiveTable;}
	void SetDBStatus(int status)					{m_nDBStatus = status;}
	int GetDBStatus(void) const						{return m_nDBStatus;}

	/* Connect and disconnect to database. */
	int Connect(void);
	int DisConnect(void);
	sqlite3* GetConnect(void) const			{ return m_pDatabase;}
	
	/* You have to call ReleaseWholeTableData() to release the data 
	 * you have obtained by calling QueryWholeTableData().
	 */
	CStockTableDataInfo QueryWholeTableData(ENUMACTIVETABLE whichTable);
	int ReleaseWholeTableData(char** pData);

	/* Select, insert, delete, update record. */
	int RemoveRecordByTableId(ENUMACTIVETABLE whichTable, int id);
	int InsertBuyRecord	(const CStockModelBuy& buyModel);
	int InsertHoldRecord(const CStockModelHold& holdModel);
	int InsertSellRecord(const CStockModelSell& sellModel);
	int InsertMoneyRecord(const CStockModelMoney& moneyModel);
	CStockModelHold SelectHoldModelByCode(const char* code);
	CStockModelHold SelectHoldModelById(int id);
	int UpdateHoldModel(const CStockModelHold& holdModel);
	int UpdateSellTotalEarn(void);

private:
	/* DB file doesn't exist, init its tables. */
	int InitDatabaseStockTables(void);
	int InitDatabaseFutureTables(void);
	BOOL IsTableNamesValie(void);
	string GetActiveTableName(ENUMACTIVETABLE activeTable);
};

#endif