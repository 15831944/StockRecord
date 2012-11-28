
#include "StdAfx.h"
#include "StockModel.h"

/* ===================== Stock sell model =====================  */

CRecordModelBase::CRecordModelBase(void)
	: m_encodeStyle(ENCODE_STYLE_GB2312)
	, id(-1)
	, code(_T("基类code"))
	, name(_T("基类name"))
{
}

CRecordModelBase::~CRecordModelBase(void)
{
}

void CRecordModelBase::SetEncodeStyle( int encodeStyle )
{
	switch (encodeStyle) {

	case ENCODE_STYLE_GB2312:
		m_encodeStyle = ENCODE_STYLE_GB2312;
		break;

	case ENCODE_STYLE_UTF8:
		m_encodeStyle = ENCODE_STYLE_UTF8;
		break;

	default:
		break;
	}
}

int CRecordModelBase::GetEncodeStyle( void ) const
{
	switch (m_encodeStyle) {
	case ENCODE_STYLE_GB2312:
		return ENCODE_STYLE_GB2312;
		break;

	case ENCODE_STYLE_UTF8:
		return ENCODE_STYLE_UTF8;
		break;

	default:
		return ENCODE_STYLE_GB2312;
		break;
	}
}

int CRecordModelBase::ConvertEncodeFormat( int targetEncode )
{
	if (GetEncodeStyle() == targetEncode)
		return ERR;

	switch (targetEncode) {

	// TO BE opted. Convert more members.
	case ENCODE_STYLE_UTF8:
		name.Format("%s", 
			CChineseCodeLib::GB2312ToUTF8((LPCTSTR)name).c_str());
		SetEncodeStyle(ENCODE_STYLE_UTF8);
		break;

	case ENCODE_STYLE_GB2312:
		name.Format("%s", 
			CChineseCodeLib::UTF8ToGB2312((LPCTSTR)name).c_str());
		SetEncodeStyle(ENCODE_STYLE_GB2312);
		break;

	default:
		break;
	}

	return 0;
}

/**
 *	Stock buy model
 */ 
CStockModelBuy::CStockModelBuy( void )
	: CRecordModelBase()		/* construct your base object firstly */
	, buy_price(_T(""))
	, buy_amount(_T(""))
	, buy_date(_T(""))
	, stock_type()
{
}

CStockModelBuy::~CStockModelBuy( void )
{
}

/**
 *	=====================  Stock hold model ===================== 
 */ 
CStockModelHold::CStockModelHold( void )
	: CRecordModelBase()
	, buy_price(_T(""))
	, hold_cost(_T(""))
	, hold_amount(_T(""))
	, even_price(_T(""))
	, buy_date(_T(""))
{
}

CStockModelHold::~CStockModelHold( void )
{
}

/* ========== Stock Sell Model ============ */

CStockModelSell::CStockModelSell( void )
	: CRecordModelBase()
	, buy_price(_T(""))
	, sell_price(_T(""))
	, sell_amount(_T(""))
	, sell_date(_T(""))
	, even_price(_T(""))
	, each_earn(_T(""))
	, total_earn(_T(""))
	, buy_date(_T(""))
{

}

CStockModelSell::~CStockModelSell( void )
{
}


/************************************************************************/
/*                   Future record models related                       */
/************************************************************************/

CFutureTakenModel::CFutureTakenModel( void )
	: taken_price(_T(""))
	, taken_amount(_T(""))
	, multiplier(_T(""))
	, margin_ratio(_T(""))
	, commision_ratio(_T(""))
	, taken_date(_T(""))
{
}

CFutureTakenModel::~CFutureTakenModel( void )
{
}

CFutureHoldModel::CFutureHoldModel( void )
	: taken_price(_T(""))
	, hold_amount(_T(""))
	, hold_cost(_T(""))
	, even_price(_T(""))
{
}

CFutureHoldModel::~CFutureHoldModel( void )
{
}

CFutureClosedModel::CFutureClosedModel( void )
{

}

CFutureClosedModel::~CFutureClosedModel( void )
{
}
