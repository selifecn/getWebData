// setServer.cpp : 实现文件
//

#include "stdafx.h"
#include "GetWebSource.h"
#include "setServer.h"


// CsetServer 对话框

IMPLEMENT_DYNCREATE(CsetServer, CDHtmlDialog)

CsetServer::CsetServer(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CsetServer::IDD, CsetServer::IDH, pParent)
{

}

CsetServer::~CsetServer()
{
}

void CsetServer::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CsetServer::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
//	Navigate(_T("file:///")+strUrl);
//	Navigate(_T("http://10.1.1.196/viewserverinf.asp?infserver=qishanxiaolu"));
	strurl="http://127.0.0.1";
	CString url=strurl+"/viewserverinf.asp?infserver=xxxxxxxx";
	Navigate(url);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(CsetServer, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CsetServer)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CsetServer 消息处理程序

HRESULT CsetServer::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CsetServer::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}
