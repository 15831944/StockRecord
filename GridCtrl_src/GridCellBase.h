/////////////////////////////////////////////////////////////////////////////
// GridCellBase.h : header file
//
// MFC Grid Control - Grid cell base class header file
//
// Written by Chris Maunder <chris@codeproject.com>
// Copyright (c) 1998-2005. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
// An email letting me know how you are using it would be nice as well. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with CGridCtrl v2.22+
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDCELLBASE_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_)
#define AFX_GRIDCELLBASE_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CGridCtrl;

// 单元格状态
#define GVIS_FOCUSED            0x0001	// 单元格成为焦点
#define GVIS_SELECTED           0x0002	// 选择单元格
#define GVIS_DROPHILITED        0x0004	// 单元格是 Drop highlighted
#define GVIS_READONLY           0x0008	// 设置只读，不能编辑
#define GVIS_FIXED              0x0010	// 单元格锁定
#define GVIS_FIXEDROW           0x0020	// 单元格是锁定行的一部分
#define GVIS_FIXEDCOL           0x0040	// 单元格是锁定列的一部分
#define GVIS_MODIFIED           0x0080	// 单元格被修改过

// 单元格数据掩码
#define GVIF_TEXT               LVIF_TEXT		// 存取单元格文本
#define GVIF_IMAGE              LVIF_IMAGE		// 存取单元格图片数量
#define GVIF_PARAM              LVIF_PARAM		// 存取单元格用户数据（lParam）
#define GVIF_STATE              LVIF_STATE		// 存取单元格状态
#define GVIF_BKCLR              (GVIF_STATE<<1)	// 存取单元格背景颜色
#define GVIF_FGCLR              (GVIF_STATE<<2)	// 存取单元格前景颜色
#define GVIF_FORMAT             (GVIF_STATE<<3)	// 存取单元格格式
#define GVIF_FONT               (GVIF_STATE<<4)	// 存取单元格字体
#define GVIF_MARGIN             (GVIF_STATE<<5)	// 存取单元格边缘信息
#define GVIF_ALL                (GVIF_TEXT|GVIF_IMAGE|GVIF_PARAM|GVIF_STATE|GVIF_BKCLR|GVIF_FGCLR| \
                                 GVIF_FORMAT|GVIF_FONT|GVIF_MARGIN)	// 所有信息

// Used for Get/SetItem calls.
typedef struct _GV_ITEM {
    int      row,col;     // 对象的行和列
    UINT     mask;        // 用于设置/获取单元格数据的一种掩码
    UINT     nState;      // 单元格状态（如焦点/突出显示等）
    DWORD    nFormat;     // 单元格格式（默认使用CDC::DrawText格式）
    int      iImage;      // index of the list view item icon
    COLORREF crBkClr;     // 背景颜色 (或者 CLR_DEFAULT)
    COLORREF crFgClr;     // 前景颜色 (或者 CLR_DEFAULT)
    LPARAM   lParam;      // 32-bit value to associate with item
    LOGFONT  lfFont;      // 单元格字体
    UINT     nMargin;     // Internal cell margin
    CString  strText;     // 单元格文本
} GV_ITEM;


// Each cell contains one of these. Fields "row" and "column" are not stored since we
// will usually have acces to them in other ways, and they are an extra 8 bytes per
// cell that is probably unnecessary.

// 这个类是所有 Grid 单元格的基类，包含每个单元格的信息。同时，它还定义了大量的方法供 Grid 调用，
// 例如画图和打印。几乎所有的方法和函数都是虚拟模式，类本身不能直接使用，只能被引用。
// 作为 Grid 控件的默认单元格类 CGridCell 就是基于 CGridCellBase 的。
class CGridCellBase : public CObject
{
    friend class CGridCtrl;
    DECLARE_DYNAMIC(CGridCellBase)

// Construction/Destruction
public:
    CGridCellBase();
    virtual ~CGridCellBase();

// Attributes
public:
    virtual void SetText(LPCTSTR /* szText */)              = 0 ;
    virtual void SetImage(int /* nImage */)                 = 0 ;
    virtual void SetData(LPARAM /* lParam */)               = 0 ;
    virtual void SetState(DWORD nState)                     { m_nState = nState; }
    virtual void SetFormat(DWORD /* nFormat */)             = 0 ;
    virtual void SetTextClr(COLORREF /* clr */)             = 0 ;
    virtual void SetBackClr(COLORREF /* clr */)             = 0 ;
    virtual void SetFont(const LOGFONT* /* plf */)          = 0 ;
    virtual void SetMargin( UINT /* nMargin */)             = 0 ;
    virtual void SetGrid(CGridCtrl* /* pGrid */)            = 0 ;
    virtual void SetCoords( int /* nRow */, int /* nCol */) = 0 ;

    virtual LPCTSTR    GetText()       const                = 0 ;
    virtual LPCTSTR    GetTipText()    const                { return GetText(); } // may override TitleTip return
    virtual int        GetImage()      const                = 0 ;
    virtual LPARAM     GetData()       const                = 0 ;
    virtual DWORD      GetState()      const                { return m_nState;  }
    virtual DWORD      GetFormat()     const                = 0 ;
    virtual COLORREF   GetTextClr()    const                = 0 ;
    virtual COLORREF   GetBackClr()    const                = 0 ;
    virtual LOGFONT  * GetFont()       const                = 0 ;
    virtual CFont    * GetFontObject() const                = 0 ;
    virtual CGridCtrl* GetGrid()       const                = 0 ;
    virtual CWnd     * GetEditWnd()    const                = 0 ;
    virtual UINT       GetMargin()     const                = 0 ;

    virtual CGridCellBase* GetDefaultCell() const;

    virtual BOOL IsDefaultFont()       const                = 0 ;
    virtual BOOL IsEditing()           const                = 0 ;
    virtual BOOL IsFocused()           const                { return (m_nState & GVIS_FOCUSED);  }
    virtual BOOL IsFixed()             const                { return (m_nState & GVIS_FIXED);    }
    virtual BOOL IsFixedCol()          const                { return (m_nState & GVIS_FIXEDCOL); }
    virtual BOOL IsFixedRow()          const                { return (m_nState & GVIS_FIXEDROW); }
    virtual BOOL IsSelected()          const                { return (m_nState & GVIS_SELECTED); }
    virtual BOOL IsReadOnly()          const                { return (m_nState & GVIS_READONLY); }
    virtual BOOL IsModified()          const                { return (m_nState & GVIS_MODIFIED); }
    virtual BOOL IsDropHighlighted()   const                { return (m_nState & GVIS_DROPHILITED); }

// Operators
public:
    virtual void operator=(const CGridCellBase& cell);

// Operations
public:
    virtual void Reset();

    virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);
    virtual BOOL GetTextRect( LPRECT pRect);    // i/o:  i=dims of cell rect; o=dims of text rect
    virtual BOOL GetTipTextRect( LPRECT pRect) { return GetTextRect( pRect); }  // may override for btns, etc.
    virtual CSize GetTextExtent(LPCTSTR str, CDC* pDC = NULL);
    virtual CSize GetCellExtent(CDC* pDC);

    // Editing
    virtual BOOL Edit( int /* nRow */, int /* nCol */, CRect /* rect */, CPoint /* point */, 
                       UINT /* nID */, UINT /* nChar */) { ASSERT( FALSE); return FALSE;}
	virtual BOOL ValidateEdit(LPCTSTR str);
    virtual void EndEdit() {}

    // EFW - Added to print cells properly
    virtual BOOL PrintCell(CDC* pDC, int nRow, int nCol, CRect rect);

    // add additional protected grid members required of cells
    LRESULT SendMessageToParent(int nRow, int nCol, int nMessage);

protected:
    virtual void OnEndEdit();
    virtual void OnMouseEnter();
    virtual void OnMouseOver();
    virtual void OnMouseLeave();
    virtual void OnClick( CPoint PointCellRelative);
    virtual void OnClickDown( CPoint PointCellRelative);
    virtual void OnRClick( CPoint PointCellRelative);
    virtual void OnDblClick( CPoint PointCellRelative);
    virtual BOOL OnSetCursor();

protected:
    DWORD    m_nState;      // Cell state (selected/focus etc)
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDCELLBASE_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_)
