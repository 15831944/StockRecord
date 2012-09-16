#include "stdafx.h"
#include "StockRecord.h"
#include "FieldNamesMap.h"

/**
 *	Static member parameter of class MUST be inited outside of the class.
 */
std::map<string, string> FieldNamesMap::m_FieldNamesMap;
bool FieldNamesMap::m_isMapInited = false;

FieldNamesMap::FieldNamesMap( void )
{
	
}

int FieldNamesMap::InitFieldNamesMap( void )
{
	int ret = 0;

	m_FieldNamesMap.clear();

	m_FieldNamesMap["code"]			=	"股票代码";
	m_FieldNamesMap["name"]			=	"股票名称";
	m_FieldNamesMap["buy_price"]	=	"买入价格";
	m_FieldNamesMap["buy_amount"]	=	"买入数量";
	m_FieldNamesMap["buy_date"]		=	"买入日期";

	m_FieldNamesMap["hold_cost"]	=	"持仓成本";
	m_FieldNamesMap["hold_amount"]	=	"持仓数量";
	m_FieldNamesMap["even_price"]	=	"保本价格";

	m_FieldNamesMap["sell_price"]	=	"卖出价格";
	m_FieldNamesMap["sell_amount"]	=	"卖出数量";
	m_FieldNamesMap["sell_date"]	=	"卖出时间";
	m_FieldNamesMap["each_earn"]	=	"每次盈利";
	m_FieldNamesMap["total_earn"]	=	"总体盈利";

	m_FieldNamesMap["each_money"]	=	"输入/输出金额";
	m_FieldNamesMap["money_date"]	=	"输入/输出时间";
	m_FieldNamesMap["remark"]		=	"资金备注";
	m_FieldNamesMap["total_money"]	=	"全部资金";

	return ret;
}

std::string FieldNamesMap::GetChNameByEnName( string enName )
{
	if (!m_isMapInited) {		// Map has not been inted.
		InitFieldNamesMap();
		m_isMapInited = true;
	}


	if (enName.empty())
		return "";

	string chName ("");					// Chinese name
	map<string, string>::iterator it;

	it = m_FieldNamesMap.find(enName);
	if (it == m_FieldNamesMap.end())	// No related value.
		return "";

	return it->second;
}

FieldNamesMap::~FieldNamesMap()
{
	m_FieldNamesMap.clear();
	m_isMapInited = false;
}