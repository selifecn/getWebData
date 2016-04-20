#pragma once


// Cdlglogin 对话框

class Cdlglogin : public CDialog
{
	DECLARE_DYNAMIC(Cdlglogin)

public:
	Cdlglogin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Cdlglogin();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	CString logurl;
public:
	CString logusername;
public:
	CString logpassword;
};
