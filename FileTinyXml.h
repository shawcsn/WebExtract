#ifndef FILETINYXML_H
#define FILETINYXML_H
#include <iostream>
#include <string>
#include <deque>
#include <set>
#include <vector>
#include "tinyxml.h"
#include "File.h"
#include "FileTidy.h"

#ifdef _WIN32
// specially for Visual C++ 6.0
#define STL_USING_ALL
//#include <stl>
#endif

using namespace std;

class FileTinyXml 
{
public:
	FileTinyXml(){};
	FileTinyXml( const char * filePath )//m_result = NULL;
		:m_srcFilePath(filePath){  }
	~FileTinyXml(){ m_doc.Clear();  }//delete[] m_result;
	bool loadSrcFile();
	bool saveToFile( string& desFilePath );
	
	//正文提取的核心调用函数
	//char * extract();
	
	//提取正文
	string extractContent( deque<string> &queText );
	
	//提取subtree下标签为strtag并含有子节点的第一个节点,在模块中分配的内存返回到模块外要用引用
	//bool extractNode( TiXmlNode *const subTree, string &strTag, TiXmlNode *&reNode );
	void extractNode( TiXmlNode *const subTree, string &strTag, vector< TiXmlNode * > &vecNode );

	//获取m_doc
	const TiXmlDocument &getDocument(){ return m_doc; }
	
protected:

	//删除标签包含sdeltag内容的节点
	bool deleteNode( TiXmlNode* const subTree, set<string> &sDelTag );

	//计算某一子树的权重
	float caculateWeight( TiXmlNode * const subTree );

	//计算subTree下面的strTag标签的权重
	void maxWeightBlock( TiXmlNode *const subTree, set<string> &setTag, TiXmlNode *&resultNode, float &maxWeight, vector<string> &vecPunct );

	//查找subtree下strTag标签的数量
	int countNode( TiXmlNode * const subTree, string &strTag );
	
	//查找text中指定字符串的长度,如果sCountString为空则返回text的长度
	int countStrInText( TiXmlNode * const subTree, vector<string> &vecCount );

	//删除subTree中的链接群
	void delLinkGroup( TiXmlNode *const subTree, string &strTag, float threshold );

	//提取树节点下的文字信息
	void extractText( TiXmlNode * const subTree, deque<string> &queMainText, int &direction );

	//提取subtree下非sepTree的<p>标签下含标点的内容按顺序存放到deque中
	void extractPText( TiXmlNode * const subTree, TiXmlNode *sepTree, deque< string > &mainText, int &direction, vector<string> &vecPunct, int &maxLink );

	const string m_srcFilePath;
	TiXmlDocument m_doc;
	
private:
	
	//char *m_result;
};

#endif
