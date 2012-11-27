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

	m_StockFieldNamesMap["code"]		=	"��Ʊ����";
	m_StockFieldNamesMap["name"]		=	"��Ʊ����";
	m_StockFieldNamesMap["buy_price"]	=	"����۸�";
	m_StockFieldNamesMap["buy_amount"]	=	"��������";
	m_StockFieldNamesMap["buy_date"]	=	"��������";

	m_StockFieldNamesMap["hold_cost"]	=	"�ֲֳɱ�";
	m_StockFieldNamesMap["hold_amount"]	=	"�ֲ�����";
	m_StockFieldNamesMap["even_price"]	=	"�����۸�";

	m_StockFieldNamesMap["sell_price"]	=	"�����۸�";
	m_StockFieldNamesMap["sell_amount"]	=	"��������";
	m_StockFieldNamesMap["sell_date"]	=	"��������";
	m_StockFieldNamesMap["each_earn"]	=	"ÿ��ӯ��";
	m_StockFieldNamesMap["total_earn"]	=	"����ӯ��";

	m_StockFieldNamesMap["each_money"]	=	"����/������";
	m_StockFieldNamesMap["money_date"]	=	"����/�������";
	m_StockFieldNamesMap["remark"]		=	"�ʽ�ע";
	m_StockFieldNamesMap["total_money"]	=	"ȫ���ʽ�";

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