#pragma once


// Cdlglogin �Ի���

class Cdlglogin : public CDialog
{
	DECLARE_DYNAMIC(Cdlglogin)

public:
	Cdlglogin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Cdlglogin();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
