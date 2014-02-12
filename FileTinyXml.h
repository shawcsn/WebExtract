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
	
	//������ȡ�ĺ��ĵ��ú���
	//char * extract();
	
	//��ȡ����
	string extractContent( deque<string> &queText );
	
	//��ȡsubtree�±�ǩΪstrtag�������ӽڵ�ĵ�һ���ڵ�,��ģ���з�����ڴ淵�ص�ģ����Ҫ������
	//bool extractNode( TiXmlNode *const subTree, string &strTag, TiXmlNode *&reNode );
	void extractNode( TiXmlNode *const subTree, string &strTag, vector< TiXmlNode * > &vecNode );

	//��ȡm_doc
	const TiXmlDocument &getDocument(){ return m_doc; }
	
protected:

	//ɾ����ǩ����sdeltag���ݵĽڵ�
	bool deleteNode( TiXmlNode* const subTree, set<string> &sDelTag );

	//����ĳһ������Ȩ��
	float caculateWeight( TiXmlNode * const subTree );

	//����subTree�����strTag��ǩ��Ȩ��
	void maxWeightBlock( TiXmlNode *const subTree, set<string> &setTag, TiXmlNode *&resultNode, float &maxWeight, vector<string> &vecPunct );

	//����subtree��strTag��ǩ������
	int countNode( TiXmlNode * const subTree, string &strTag );
	
	//����text��ָ���ַ����ĳ���,���sCountStringΪ���򷵻�text�ĳ���
	int countStrInText( TiXmlNode * const subTree, vector<string> &vecCount );

	//ɾ��subTree�е�����Ⱥ
	void delLinkGroup( TiXmlNode *const subTree, string &strTag, float threshold );

	//��ȡ���ڵ��µ�������Ϣ
	void extractText( TiXmlNode * const subTree, deque<string> &queMainText, int &direction );

	//��ȡsubtree�·�sepTree��<p>��ǩ�º��������ݰ�˳���ŵ�deque��
	void extractPText( TiXmlNode * const subTree, TiXmlNode *sepTree, deque< string > &mainText, int &direction, vector<string> &vecPunct, int &maxLink );

	const string m_srcFilePath;
	TiXmlDocument m_doc;
	
private:
	
	//char *m_result;
};

#endif
