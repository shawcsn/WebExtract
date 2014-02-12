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
	
	//将规则文件和过滤词文件读入m_vecRule和m_vecWords中
	TitleExtract( const char *path, const char *url, char *ruleFile = "", char *filterFile = "" );
	// char *ruleFile = "titletag.ini", char *filterFile = "filterwords.ini"
	~TitleExtract();
	
	void extract( char *&title, char *&content);
	//新增链接提取函数
	void extractherf( string &pageUrl, TiXmlNode *const root, string &herf );
	void herfextract( char *&herf);
	//提取标题
	void extractTitle( string &pageUrl, TiXmlNode *const root, string &herf );
	
	//string extrCertainUrl( string &pageUrl, TiXmlNode *const root, vector< pair< pair< string, string >, pair< string, string > > > &vecRule );
protected:
	void deleteWords( string &title );
	
	string strToLower( string srcStr );
	
	string extrCertainUrl( string &pageUrl, TiXmlNode * const root );
	//从textline中提出出strRet后面跟的内容
	bool selText( string &textline, string &tag, string &strRet );
private:
	//存储对于特定网页的过滤规则
	vector< pair< pair< string, string >, pair< string, string > > > m_vecRule;
	//存储对于title中可能出现的噪声词
	vector< string > m_vecWords;
	char *m_title;
	char *m_content;
	char *m_herf;
	string m_url;
};

#endif