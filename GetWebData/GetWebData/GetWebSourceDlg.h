// GetWebSourceDlg.h : header file
//

#include "..\dll\WebData.h"

#if !defined(AFX_GETWEBSOURCEDLG_H__CDA6F86A_0259_4BD7_9708_5650322BC0EF__INCLUDED_)
#define AFX_GETWEBSOURCEDLG_H__CDA6F86A_0259_4BD7_9708_5650322BC0EF__INCLUDED_

#pragma comment(lib,"..//dll//debug//webdata.lib")


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGetWebSourceDlg dialog


class CGetWebSourceDlg : public CDialog
{
// Construction
public:
	void logout(int userID);
	CGetWebSourceDlg(CWnd* pParent = NULL);	// standard constructor
	CString strtime;

	CWebData webdata;

	//////////////////////////
// Dialog Data
	//{{AFX_DATA(CGetWebSourceDlg)
	enum { IDD = IDD_GETWEBSOURCE_DIALOG };
	CListCtrl	m_list;
	int		m_intnews;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetWebSourceDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGetWebSourceDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void Onviewinf();
	afx_msg void OnCookies();
	afx_msg void OnViewUser();
	afx_msg void OnViewJsweb();
	afx_msg void Ondelinf();
	afx_msg void OnSendMail();
	afx_msg void OnSql();
	afx_msg void Oninf();
	afx_msg void OnAddToMdb();
	afx_msg void OnButNew();
	afx_msg void Oncook();
	afx_msg void Onbuser();
	afx_msg void Onbjsweb();
//	afx_msg void Ondell();
	afx_msg void Onshuxin();
	afx_msg void Ongetshellcode();
//	afx_msg void Ongetshellcode2();
//	afx_msg void Onreadweb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int intID;
public:
	afx_msg void OnBnClickedSetweb();

public:
	afx_msg void OnBnClickedButLog();
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETWEBSOURCEDLG_H__CDA6F86A_0259_4BD7_9708_5650322BC0EF__INCLUDED_)
