#pragma once

#ifdef _WIN32_WCE
#error "Windows CE ��֧�� CDHtmlDialog��"
#endif 

// CsetServer �Ի���

class CsetServer : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CsetServer)

public:
	CsetServer(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CsetServer();
	CString strurl;
// ��д
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// �Ի�������
	enum { IDD = IDD_DIALOG1, IDH = IDR_HTML_SETSERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};
