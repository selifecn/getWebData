
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

//����ADO�Ķ�̬���ӿ�,��Ҫ�����ռ�,��EOF��ΪrsEOF
#import "c:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF") rename("BOF","adoBOF")

class CADOWeb  //webdata
{
public:
	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pRecordset;
public:
	//bool Addinf();
	CADOWeb(); //���캯��
	virtual ~CADOWeb(); //��������
	 //��ʼ���������ݿ�
	bool OnInitADOConnect();
	_RecordsetPtr&GetRecordSet(_bstr_t bstrSQL); //ִ��SQL������ؼ�¼��
	bool ExecuteSQL(_bstr_t bstrSQL); //ִ��SQL����޷��ؼ�¼��
	void ExitConnect(); //�Ͽ����ݿ�����

};

using namespace std;//�����ռ�

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
��DLL�Ĺ��ܣ��ṩ�ͻ��˷���ASP����ASP��վ���ݿ�Ľӿ�
GetShellCode ������ȡ��������
login  ��¼ASP��վ
logout �˳�ASP��վ
*/
class WEBDATA_API  CWebData {
public:
	CWebData();//��ʼ��
	~CWebData();
				
public:
	int login(const CString strSerURL,const CString strLogname,const CString strPassword);//��¼��
	void logout(const CString strSerURL,int nLogID);
	CStringA GetShellCode(const int nTaskID,const TCHAR * pName,const TCHAR * pMailStyle,const int nGetCodeStyle);  //��ȡ��������

public:
//	int			logID;				//��¼ID
	int			m_nNews;				//�洢����Ϣ������
	CString		m_strSerURL;			//web��������ַ
	CADOWeb		m_cADOweb;				//�����������ݿ����Ҫ����ADOConn.hͷ����
	CStringA	m_strArecv;				//��ŷ�����Ϣ
	
				//�˳�
	bool GetNews(bool bload = 0);	//��ȡ�������ݵ���������bload=1ʱ��������Ϣ


public:
	bool WebUpToMDB();				//����ȫ�����ݵ��������ݿ�
	void SendSql(CString SQL);		//ֱ��ִ��SQL��䵽�������ݿ�
	bool delID(TCHAR * form,int ID);	//ɾ��ָ�����е�ָ����	


public:
	bool viewinf(CListCtrl &m_list);          //ֱ����ʾ
	bool viewinfformMDB(CListCtrl &m_list,const int nLogID);   //�����ݿ��ȡ
	bool viewjsweb(CListCtrl &m_list,const int nLogID);
	bool viewuser(CListCtrl &m_list,const int nLogID);
	bool viewcook(CListCtrl &m_list,const int nLogID);
	bool viewcookformMDB(CListCtrl &m_list,const int nLogID);

protected:

	CStringA strxml;					//�洢XML����

	void SendURL(CString strurl,bool brecv); //����ASP����brecv�Ƿ���ʾ�������ݣ�m_strArecv���淵������
	bool LoadXML(int startID = 0);	//��WEB.mdb�ϴθ��¼�¼֮����������
	void CreateXML(TCHAR * path);	//дXML�ļ�
	bool WriteXMLtoMDB(TCHAR * path,TCHAR * rss,TCHAR * form,TCHAR * title[15],int listnub);//��XML����д�����ݿ�
	bool ReadXML(TCHAR * path,TCHAR * title[15],int len,TCHAR * rss,CListCtrl &m_list); //��ȡXML��listctrl�ؼ�

									//��ȡָ����XML�ļ�������ʾ��m_list��
	// TODO: add your methods here.
};


#endif // !defined(WEBDATA_H__F89D7D90_A2C7_4BEA_A1CC_C5F422422E7C__INCLUDED_)


