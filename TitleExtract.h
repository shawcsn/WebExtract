#ifndef TITLEEXTRACT_H
#define TITLEEXTRACT_H

#include <iostream>
#include <string>
#include <vector>
#include "tinyxml.h"
#include "FileTinyXml.h"

using namespace std;

class TitleExtract: public FileTinyXml
{
public:
	
	TitleExtract();
	
	//�������ļ��͹��˴��ļ�����m_vecRule��m_vecWords��
	TitleExtract( const char *path, const char *url, char *ruleFile = "", char *filterFile = "" );
	// char *ruleFile = "titletag.ini", char *filterFile = "filterwords.ini"
	~TitleExtract();
	
	void extract( char *&title, char *&content);
	//����������ȡ����
	void extractherf( string &pageUrl, TiXmlNode *const root, string &herf );
	void herfextract( char *&herf);
	//��ȡ����
	void extractTitle( string &pageUrl, TiXmlNode *const root, string &herf );
	
	//string extrCertainUrl( string &pageUrl, TiXmlNode *const root, vector< pair< pair< string, string >, pair< string, string > > > &vecRule );
protected:
	void deleteWords( string &title );
	
	string strToLower( string srcStr );
	
	string extrCertainUrl( string &pageUrl, TiXmlNode * const root );
	//��textline�������strRet�����������
	bool selText( string &textline, string &tag, string &strRet );
private:
	//�洢�����ض���ҳ�Ĺ��˹���
	vector< pair< pair< string, string >, pair< string, string > > > m_vecRule;
	//�洢����title�п��ܳ��ֵ�������
	vector< string > m_vecWords;
	char *m_title;
	char *m_content;
	char *m_herf;
	string m_url;
};

#endif