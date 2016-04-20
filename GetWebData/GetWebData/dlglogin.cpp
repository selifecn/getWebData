// dlglogin.cpp : 实现文件
//

#include "stdafx.h"
#include "GetWebSource.h"
#include "dlglogin.h"


// Cdlglogin 对话框

CString m_strIniFile;

void  GetIniFile()
{
	TCHAR	inipath[MAX_PATH];

	ZeroMemory( inipath ,MAX_PATH*sizeof(TCHAR));
	GetModuleFileName(NULL, inipath, MAX_PATH);
	//GetCurrentDirectory(MAX_PATH,inipath);  //找到当前进程的当前目录
	TCHAR *pFind = _tcsrchr(inipath, _T('\\'));
	if ( pFind != NULL)
	{
		lstrcpy(pFind, _T("\\"));
	}

	lstrcat(inipath,_T("setting.ini"));
	m_strIniFile = inipath;
}

CString GetConfig( LPCTSTR lpAPP,LPCTSTR lpKey)
{
	TCHAR	lpValue[1024];
	lpValue[0]=0;
	GetPrivateProfileString( lpAPP,lpKey,_T(""),lpValue,1024,m_strIniFile );
	return lpValue;
}

int GetConfigInt( LPCTSTR lpAPP,LPCTSTR lpKey )
{
	return GetPrivateProfileInt( lpAPP,lpKey,0,m_strIniFile );
}

void SaveConfig( LPCTSTR lpAPP,LPCTSTR lpKey,LPCTSTR lpValue )
{
	WritePrivateProfileString( lpAPP,lpKey,lpValue,m_strIniFile);
}
void SaveConfigInt( LPCTSTR lpAPP,LPCTSTR lpKey,int nValue )
{
	CString strTemp;
	strTemp.Format( _T("%d"),nValue );
	SaveConfig( lpAPP,lpKey,strTemp );
}


IMPLEMENT_DYNAMIC(Cdlglogin, CDialog)

Cdlglogin::Cdlglogin(CWnd* pParent /*=NULL*/)
	: CDialog(Cdlglogin::IDD, pParent)
	, logurl(_T(""))
	, logusername(_T(""))
	, logpassword(_T(""))
{
	GetIniFile();
//	GetDlgItem(IDC_USERNAME)->SetWindowText(_T("admin"));
//	GetDlgItem(IDC_PASSWORD)->SetWindowText(_T("admin"));

	logurl = GetConfig( _T("Config"),_T("URL") );

}

Cdlglogin::~Cdlglogin()
{
}

void Cdlglogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_URL, logurl);
	DDX_Text(pDX, IDC_USERNAME, logusername);
	DDX_Text(pDX, IDC_PASSWORD, logpassword);
}


BEGIN_MESSAGE_MAP(Cdlglogin, CDialog)
	ON_BN_CLICKED(IDOK, &Cdlglogin::OnBnClickedOk)
END_MESSAGE_MAP()


// Cdlglogin 消息处理程序

void Cdlglogin::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	SaveConfig( _T("Config"),_T("URL"),logurl);
	OnOK();
}

