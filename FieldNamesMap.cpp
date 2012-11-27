#include "stdafx.h"
#include "StockRecord.h"
#include "FieldNamesMap.h"

/**
 *	Static member parameter of class MUST be inited outside of the class.
 */
std::map<string, string> FieldNamesMap::m_StockFieldNamesMap;
bool FieldNamesMap::m_isMapInited = false;

FieldNamesMap::FieldNamesMap( void )
{
	
}

int FieldNamesMap::InitStockFieldNamesMap( void )
{
	int ret = 0;

	m_StockFieldNamesMap.clear();

	m_StockFieldNamesMap["code"]		=	"股票代码";
	m_StockFieldNamesMap["name"]		=	"股票名称";
	m_StockFieldNamesMap["buy_price"]	=	"买入价格";
	m_StockFieldNamesMap["buy_amount"]	=	"买入数量";
	m_StockFieldNamesMap["buy_date"]	=	"买入日期";

	m_StockFieldNamesMap["hold_cost"]	=	"持仓成本";
	m_StockFieldNamesMap["hold_amount"]	=	"持仓数量";
	m_StockFieldNamesMap["even_price"]	=	"保本价格";

	m_StockFieldNamesMap["sell_price"]	=	"卖出价格";
	m_StockFieldNamesMap["sell_amount"]	=	"卖出数量";
	m_StockFieldNamesMap["sell_date"]	=	"卖出日期";
	m_StockFieldNamesMap["each_earn"]	=	"每次盈利";
	m_StockFieldNamesMap["total_earn"]	=	"总体盈利";

	m_StockFieldNamesMap["each_money"]	=	"输入/输出金额";
	m_StockFieldNamesMap["money_date"]	=	"输入/输出日期";
	m_StockFieldNamesMap["remark"]		=	"资金备注";
	m_StockFieldNamesMap["total_money"]	=	"全部资金";

	return ret;
}

/**
 *	If Chinese names are used in table's fields, the map from EnName to ChName
 *  won't be necessary any more.
 */
std::string FieldNamesMap::GetChNameByEnName( string enName )
{
	if (!m_isMapInited) {		// Map has not been inted.
		InitStockFieldNamesMap();
		m_isMapInited = true;
	}


	if (enName.empty())
		return "";

	string chName ("");					// Chinese name
	map<string, string>::iterator it;

	it = m_StockFieldNamesMap.find(enName);
	if (it == m_StockFieldNamesMap.end())	// No related value.
		return enName;

	return it->second;
}

FieldNamesMap::~FieldNamesMap()
{
	m_StockFieldNamesMap.clear();
	m_isMapInited = false;
}