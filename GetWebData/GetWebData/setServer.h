#pragma once

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CDHtmlDialog。"
#endif 

// CsetServer 对话框

class CsetServer : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CsetServer)

public:
	CsetServer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CsetServer();
	CString strurl;
// 重写
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// 对话框数据
	enum { IDD = IDD_DIALOG1, IDH = IDR_HTML_SETSERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};
