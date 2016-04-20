// setServer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GetWebSource.h"
#include "setServer.h"


// CsetServer �Ի���

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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BEGIN_MESSAGE_MAP(CsetServer, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CsetServer)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CsetServer ��Ϣ�������

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
