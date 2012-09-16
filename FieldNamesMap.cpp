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

	m_FieldNamesMap["code"]			=	"��Ʊ����";
	m_FieldNamesMap["name"]			=	"��Ʊ����";
	m_FieldNamesMap["buy_price"]	=	"����۸�";
	m_FieldNamesMap["buy_amount"]	=	"��������";
	m_FieldNamesMap["buy_date"]		=	"��������";

	m_FieldNamesMap["hold_cost"]	=	"�ֲֳɱ�";
	m_FieldNamesMap["hold_amount"]	=	"�ֲ�����";
	m_FieldNamesMap["even_price"]	=	"�����۸�";

	m_FieldNamesMap["sell_price"]	=	"�����۸�";
	m_FieldNamesMap["sell_amount"]	=	"��������";
	m_FieldNamesMap["sell_date"]	=	"����ʱ��";
	m_FieldNamesMap["each_earn"]	=	"ÿ��ӯ��";
	m_FieldNamesMap["total_earn"]	=	"����ӯ��";

	m_FieldNamesMap["each_money"]	=	"����/������";
	m_FieldNamesMap["money_date"]	=	"����/���ʱ��";
	m_FieldNamesMap["remark"]		=	"�ʽ�ע";
	m_FieldNamesMap["total_money"]	=	"ȫ���ʽ�";

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