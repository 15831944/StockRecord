// ChineseCodeLib.h: interface for the CChineseCodeLib class.
//
//////////////////////////////////////////////////////////////////////
#include<string>
using namespace std;

/*
功能：汉字GB2312与UTF-8编码互转
作者：litz
Email:mycro@163.com
参考：吴康彬先生的文章《UTF-8与GB2312之间的互换》
http://www.vckbase.com/document/viewdoc/?id=1397
*/


#if !defined(__CCHINESECODELIB_H_)
#define __CCHINESECODELIB_H_

class CChineseCodeLib  
{
public:
	/**
	 *  !DONOT use this before the bug is fixed.
	 *	UTF_8ToGb2312() has ERROR when I used. Confused about pLen.
	 */
	static void UTF_8ToGb2312(string& pOut,char *pText, DWORD pLen,bool *noUnicode);
	static void UTF_8ToBig5(string& pOut,char *pText, DWORD pLen);
	static void Gb2312ToUTF_8(string& pOut,char *pText, DWORD pLen);
	static void Big5ToUTF_8(string& pOut,char *pText, DWORD pLen);
	// Unicode 转换成UTF-8
	static void UnicodeToUTF_8(BYTE* pOut,WCHAR* pText);
	static void UniToUTF_8(BYTE *buff,const WCHAR *wstrIn);
	// GB2312 转换成　Unicode
	static void Gb2312ToUnicode(WCHAR* pOut, char *gbBuffer);
	static void Big5ToUnicode(WCHAR* pOut, char *gbBuffer);
	// 把Unicode 转换成 GB2312
	static void UnicodeToGb2312(char* pOut, unsigned short uData);
	static void UnicodeToGb23122(char *pOut, WCHAR szUnicodeString);
	static void UnicodeToBig5(char* pOut, unsigned short uData);
	// 把UTF-8转换成Unicode
	static void UTF_8ToUnicode(WCHAR* pOut, char* pText);
	static void UTF_8ToUnicode2(WCHAR* pOut,char *pText);
	CChineseCodeLib();
	virtual ~CChineseCodeLib();

	/**
	 *	Added by [9/16/2012 XinLong].
	 */
	
	/**
	 *	Convert between UTF8 and GB2312.
	 */
	static std::string UTF8ToGB2312(const char* str);
	static std::string GB2312ToUTF8(const char* str); 
};

#endif