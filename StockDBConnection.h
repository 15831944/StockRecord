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
	STOCK_TABLE_BUY = 0,	/* table stock_buy */
	STOCK_TABLE_HOLD,		/* table stock_hold */
	STOCK_TABLE_SELL,		/* table stock_sell */
	STOCK_TABLE_MONEY		/* table stock_money */
};

class CDBTableDataInfo;

int StockHoldRecordCallback( void* para, int nCol, char** colValue, char** colName ); 
int FutureHoldRecordCallback( void* para, int nCol, char** colValue, char** colName );

/**
 *	Hold information of a whole table, like how many rows and columns,
 *  where the table data is starting from.
 *  Used when querying data by using sqlite3_get_table().
 */
class CDBTableDataInfo
{
public:
	CDBTableDataInfo(void);
	~CDBTableDataInfo(void);
public:
	int m_nRow;		// How many rows
	int m_nCol;		// How many columns
	char** m_pData;
	int m_errcode;
};

class CDataBaseConnection
{
public:
	CDataBaseConnection(void);
	~CDataBaseConnection(void);

private:
	string m_sDBName;		/* database name */
	sqlite3* m_pDatabase;	/* database connection */
	int m_nDBStatus;
	ENUMACTIVETABLE m_eActiveTable;

private:
	/* DB file doesn't exist, init its tables. */
	int InitDatabaseStockTables(void);
	int InitDatabaseFutureTables(void);
	BOOL IsTableNamesValie(void);
	string GetActiveTableName(ENUMACTIVETABLE activeTable);

public:
	void SetDatabaseName(string dbName)		{m_sDBName = dbName;}
	void SetActiveTable(ENUMACTIVETABLE activeTable){m_eActiveTable = activeTable;}
	ENUMACTIVETABLE GetActiveTable(void) const		{return m_eActiveTable;}
	void SetDBStatus(int status)					{m_nDBStatus = status;}
	int GetDBStatus(void) const						{return m_nDBStatus;}

	/* Connect and disconnect to database. */
	int Connect(void);
	int DisConnect(void);
	sqlite3* GetConnect(void) const					{ return m_pDatabase;}
	
	/* You have to call ReleaseWholeTableData() to release the data 
	 * you have obtained by calling QueryWholeTableData().
	 */
	CDBTableDataInfo QueryWholeTableData(ENUMACTIVETABLE whichTable);
	int ReleaseWholeTableData(char** pData);
	int RemoveRecordByTableId(ENUMACTIVETABLE whichTable, int id);


	/************************************************************************/
	/*    ============ operations for stock related tables =============    */
	/************************************************************************/
private:
	/* Stock related tables' names */
	string m_sStockBuyTableName;
	string m_sStockHoldTableName;
	string m_sStockSellTableName;
	string m_sStockMoneyTableName;
	
public:
	void SetStockBuyTableName(string tableName)	{m_sStockBuyTableName = tableName;}
	void SetStockHoldTableName(string tableName){m_sStockHoldTableName = tableName;}
	void SetStockSellTableName(string tableName){m_sStockSellTableName = tableName;}
	void SetStockMoneyTableName(string tableName){m_sStockMoneyTableName = tableName;}

	/* Select, insert, delete, update stock record. */
	int InsertStockBuyRecord	(const CStockModelBuy& buyModel);
	int InsertStockHoldRecord(const CStockModelHold& holdModel);
	int InsertStockSellRecord(const CStockModelSell& sellModel);
	int InsertStockMoneyRecord(const CStockModelMoney& moneyModel);
	CStockModelHold SelectStockHoldModelByCode(const char* code);
	CStockModelHold SelectStockHoldModelById(int id);
	int UpdateStockHoldModel(const CStockModelHold& holdModel);
	int UpdateStockSellTotalEarn(string strEachValue);


	/************************************************************************/
	/*    =========== operations for future related tables =============    */
	/************************************************************************/
private:


};

#endif