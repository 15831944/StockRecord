// ChineseCodeLib.cpp: implementation of the CChineseCodeLib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChineseCodeLib.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChineseCodeLib::CChineseCodeLib()
{

}

CChineseCodeLib::~CChineseCodeLib()
{

}


void CChineseCodeLib::UTF_8ToUnicode(WCHAR* pOut,char *pText)
{
	char* uchar = (char *)pOut;
	
	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);
	
	return;
}
CString HexToBin(CString string)//将16进制数转换成2进制
{
	if( string == "0") return "0000";
	if( string == "1") return "0001";
	if( string == "2") return "0010";
	if( string == "3") return "0011";
	if( string == "4") return "0100";
	if( string == "5") return "0101";
	if( string == "6") return "0110";
	if( string == "7") return "0111";
	if( string == "8") return "1000";
	if( string == "9") return "1001";
	if( string == "a") return "1010";
	if( string == "b") return "1011";
	if( string == "c") return "1100";
	if( string == "d") return "1101";
	if( string == "e") return "1110";
	if( string == "f") return "1111";
	return "";
}

int BinToInt(CString string)//2进制字符数据转换成10进制整型
{
	int len =0;
	int tempInt = 0;
	int strInt = 0;
	for(int i =0 ;i < string.GetLength() ;i ++)
	{
	        tempInt = 1;
	        strInt = (int)string.GetAt(i)-48;
	        for(int k =0 ;k < 7-i ; k++)
	        {
			tempInt = 2*tempInt;
	        }
	        len += tempInt*strInt;
	}
	return len;
}      
void CChineseCodeLib::UTF_8ToUnicode2(WCHAR* pOut,char *pText)
{// 转换2字节的UTF-8编码为Unicode
	char* uchar = (char *)pOut;
	WCHAR x;
	x = ((pText[0] & 0x1F) << 6) + (pText[1] & 0x3F);
	uchar[1]=HIBYTE(x);
	uchar[0]=LOBYTE(x);
	return;
}

void CChineseCodeLib::UnicodeToGb2312(char* pOut,unsigned short uData)
{
	WideCharToMultiByte(CP_ACP,NULL,(LPCWSTR)&uData,1,pOut,sizeof(WCHAR),NULL,NULL);//CP_UTF8
	
	return;
}
void CChineseCodeLib::UnicodeToGb23122(char *pOut, WCHAR szUnicodeString)
{
	UINT nCodePage = 936; //GB2312 
	int nLength=WideCharToMultiByte(nCodePage,0,&szUnicodeString,-1,NULL,0,NULL,NULL);
	char* pBuffer=new char[nLength+1];
	WideCharToMultiByte(nCodePage,0,&szUnicodeString,-1,pBuffer,nLength,NULL,NULL);
	pBuffer[nLength]=0;
	pOut = pBuffer;
//#ifndef _DEBUG
		delete []pBuffer;
//#endif
}
void CChineseCodeLib::UnicodeToBig5(char* pOut,unsigned short uData)
{
	UINT nCodePage = 950;
	WideCharToMultiByte(nCodePage,0,(LPCWSTR)&uData,1,pOut,sizeof(WCHAR),NULL,NULL);
	return;
}

void CChineseCodeLib::Gb2312ToUnicode(WCHAR* pOut, char* szGBString)
{
    UINT nCodePage = 936; //GB2312 
    ::MultiByteToWideChar(nCodePage,0,szGBString,2,pOut,1);
	return;
}
void CChineseCodeLib::Big5ToUnicode(WCHAR* pOut, char* szGBString)
{
    UINT nCodePage = 950; //BIG5 
    ::MultiByteToWideChar(nCodePage,0,szGBString,2,pOut,1);
	return;
}

void CChineseCodeLib::UnicodeToUTF_8(BYTE* pOut,WCHAR* pText)
{
	// 注意 WCHAR高低字的顺序,低字节在前，高字节在后
	char* pchar = (char *)pText;
	
	pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
	pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[1] & 0xC0) >> 6);
	pOut[2] = (0x80 | (pchar[0] & 0x3F));
	
	return;
}
void CChineseCodeLib::UniToUTF_8(BYTE *buff,const WCHAR *wstrIn)
{
    ASSERT(wstrIn);
    int wLen=wcslen(wstrIn);
    int i=0;
#define putchar(a)     buff[i++]=(BYTE)a;
	
    for(int j=0;(DWORD)j<wcslen(wstrIn),i<3;j++)
    {
        WCHAR c=wstrIn[j];
        if (c < 0x80)
        {
            putchar (c);
        }
        else if (c < 0x800)
        {
            putchar (0xC0 | c>>6);
            putchar (0x80 | c & 0x3F);
        }
        else if (c < 0x10000)
        {
            putchar (0xE0 | c>>12);
            putchar (0x80 | c>>6 & 0x3F);
            putchar (0x80 | c & 0x3F);
        }
        else if (c < 0x200000)
        {
            putchar (0xF0 | c>>18);
            putchar (0x80 | c>>12 & 0x3F);
            putchar (0x80 | c>>6 & 0x3F);
            putchar (0x80 | c & 0x3F);
        }
    }
}

void CChineseCodeLib::Gb2312ToUTF_8(string& pOut,char *pText, DWORD pLen)
{
	char buf[4];
	char* rst = new char[pLen*2];
	
	memset(buf,0,4);
	memset(rst,0,pLen *2);
	
	DWORD i = 0;
	DWORD j = 0;	
	while(i < pLen)
	{
		//如果是英文直接复制就可以
		if( *(pText + i) >= 0)
		{
			rst[j++] = pText[i++];
		}
		else
		{
			WCHAR pbuffer;
			//pbuffer=(WCHAR*)malloc(128);
			Gb2312ToUnicode(&pbuffer,pText+i);
			//BIG5ToUnicode(&pbuffer,pText+i);
			
			UniToUTF_8((BYTE*)buf,&pbuffer);
			//UnicodeToUTF_8((BYTE*)buf,&pbuffer);
			
			unsigned short int tmp = 0;
			tmp = rst[j] = buf[0];
			tmp = rst[j+1] = buf[1];
			tmp = rst[j+2] = buf[2];
			
			
			j += 3;
			i += 2;
		}
	}
	rst[j] = '\0';
	
	//返回结果
	pOut = rst;		
//#ifndef _DEBUG	
	delete []rst;	
//#endif	
	return;
}
void CChineseCodeLib::Big5ToUTF_8(string& pOut,char *pText, DWORD pLen)
{
	char buf[4];
	char* rst = new char[pLen*2];
	
	memset(buf,0,4);
	memset(rst,0,pLen*2);
	
	DWORD i = 0;
	DWORD j = 0;	
	while(i < pLen)
	{
		//如果是英文直接复制就可以
		if( *(pText + i) >= 0)
		{
			rst[j++] = pText[i++];
		}
		else
		{
			WCHAR pbuffer;
			//pbuffer=(WCHAR*)malloc(128);
			//Gb2312ToUnicode(pbuffer,pText+i);
			Big5ToUnicode(&pbuffer,pText+i);
			
			UniToUTF_8((BYTE*)buf,&pbuffer);
			//UnicodeToUTF_8(buf,&pbuffer);
			
			unsigned short int tmp = 0;
			tmp = rst[j] = buf[0];
			tmp = rst[j+1] = buf[1];
			tmp = rst[j+2] = buf[2];
			
			
			j += 3;
			i += 2;
		}
	}
	rst[j] = '\0';
	
	//返回结果
	pOut = rst;	
//#ifndef _DEBUG	
	delete []rst;	
//#endif	
	return;
}

void CChineseCodeLib::UTF_8ToGb2312(string &pOut, char *pText, DWORD pLen,bool *noUnicode)
{
	char * newBuf = new char[pLen];
	char Ctemp[4];
	memset(Ctemp,0,4);
	
	DWORD i =0;
	DWORD j = 0;
	
	while(i < pLen)
	{
		if(pText[i] > 0)
		{
			newBuf[j++] = pText[i++];			
		}
		else                 
		{
			if (*noUnicode)
				*noUnicode = false;
			/*
			U-00000000 - U-0000007F:  0xxxxxxx  
			U-00000080 - U-000007FF:  110xxxxx 10xxxxxx  
			U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx  
			U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx  
			U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
			U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
			*/
			WCHAR Wtemp;    //if ((BYTE)*(pText+i)==0xc2&&(BYTE)*(pText+i+1)==0xb7)//if (((BYTE)*(pText+i+2))&0x80==0)//2 byte utf-8 code
			if (((BYTE)*(pText+i))>>5==6&&((BYTE)*(pText+i+1))>>6==2)
			{ 
				UTF_8ToUnicode2(&Wtemp,pText + i);
				i+=2;
			}
			else
			{
				UTF_8ToUnicode(&Wtemp,pText + i);
				i += 3;    
			}
			UnicodeToGb2312(Ctemp,Wtemp);

			newBuf[j] = Ctemp[0];
			newBuf[j + 1] = Ctemp[1];
			if(i<pLen&&(Ctemp[0]==0||Ctemp[1]==0))
			{
				if (Ctemp[0]==0)
					newBuf[j]=' ';
				else if (Ctemp[1]==0)
					newBuf[j+1]=' ';
			}
			j += 2;  
		}
	}
	newBuf[j] = '\0';
	
	pOut = newBuf;
//#ifndef _DEBUG	
	delete [] newBuf;	
//#endif	
	
	return; 
}
void CChineseCodeLib::UTF_8ToBig5(string &pOut, char *pText, DWORD pLen)
{
	char * newBuf = new char[pLen];
	char Ctemp[4];
	memset(Ctemp,0,4);
	
	DWORD i =0;
	DWORD j = 0;
	
	while(i < pLen)
	{
		if(pText[i] > 0)
		{
			newBuf[j++] = pText[i++];			
		}
		else                 
		{
			WCHAR Wtemp;
			UTF_8ToUnicode(&Wtemp,pText + i);
			
			UnicodeToGb2312(Ctemp,Wtemp);
			
			newBuf[j] = Ctemp[0];
			newBuf[j + 1] = Ctemp[1];
			
			i += 3;    
			j += 2;   
		}
	}
	newBuf[j] = '\0';
	
	pOut = newBuf;
//#ifndef _DEBUG	
	delete []newBuf;	
//#endif	
	
	return; 
}

std::string CChineseCodeLib::UTF8ToGB2312(const char* str)
{
	std::string result;
	WCHAR *strSrc = NULL;
	TCHAR *szRes = NULL;

	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i+1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new TCHAR[i+1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete []strSrc;
	delete []szRes;

	return result;
} 

std::string CChineseCodeLib::GB2312ToUTF8( const char* str )
{
	std::string result;
	WCHAR *strSrc = NULL;
	TCHAR *szRes = NULL;

	// TODO: Look details of these functions. The args.
	int i = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i+1];
	MultiByteToWideChar(CP_ACP, 0, str, -1, strSrc, i);

	i = WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new TCHAR[i+1];
	int j=WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete []strSrc;
	delete []szRes;

	return result;
}