
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "File.h"

using namespace std;

//read file to set
bool File::read( set<string>& desSet )
{
	if( m_strPath.empty() )
		return false;

	//���ļ�
	FILE *fp;
	fp = fopen( m_strPath.c_str(), "rb");
	if (!fp)
	{
		cerr << "can't open " << m_strPath << endl;
		return false;
	}

	//�����ļ�����
	long fileLen;
	fseek( fp, 0, SEEK_END );			//��ָ���ƶ����ļ�ĩβ
	fileLen = ftell( fp );				//�����ļ�����
	if( fileLen == 0 ) {
        fclose(fp);
		return true;
    }
	fseek( fp, 0, SEEK_SET );			//���½�ָ���ƶ����ļ���ͷ
	char *buf = new char[fileLen + 1];
	
	if (!buf)
	{
		cerr << "new memory error" << endl;
        fclose(fp);
		return false;
	}

	//��ȡ�ļ�
	fread( buf, fileLen, 1, fp );	//��ȡ�ļ�
	buf[fileLen] = '\0';			//�ļ���β�����ַ���������
	
	//�����з���Ϊ�ַ��������ַ���ע�⣺�����Linuxϵͳ����ΪLinux���з�Ϊ\n��WindowsΪ\r\n
	for( int i = fileLen - 1; i >= 0; --i )		
		if( buf[i] == g_lineEnd )
			buf[i] = '\0';

	char *line = buf;					//ָ���е�ָ��
	int lineLen, pos;					//�г���
	while ( line <= buf + fileLen ) 
	{
		lineLen = strlen( line );		
		
		//������а��������ַ������
		for( pos = 0; pos < lineLen && ( line[ pos ] == ' ' || line[ pos ] == '\t'|| line[ pos ] == g_lineEnd  ) ; ++pos );
			if( pos < lineLen )
			desSet.insert( line );
		

		line += lineLen + g_lineEndLen;			//�絽��һ�п�ʼ�ַ�
	}

	fclose(fp);
	delete[] buf;
	buf = NULL;

	return true;
}

//���ĵ�����ÿ�ж���vector
bool File::read( vector<string>& desVec )
{
	
	if( m_strPath.empty() )
		return false;

	//���ļ�
	FILE *fp;
	fp = fopen( m_strPath.c_str(), "rb");
	if (!fp)
	{
		cerr << "can't open " << m_strPath << endl;
		return false;
	}

	//�����ļ�����
	long fileLen;
	fseek( fp, 0, SEEK_END );			//��ָ���ƶ����ļ�ĩβ
	fileLen = ftell( fp );				//�����ļ�����
	if( fileLen == 0 ) {
        fclose(fp);
		return true;
    }
	fseek( fp, 0, SEEK_SET );			//���½�ָ���ƶ����ļ���ͷ
	char *buf = new char[fileLen + 1];
	
	if (!buf)
	{
        fclose(fp);
		cerr << "new memory error" << endl;
		return false;
	}

	//��ȡ�ļ�
	fread( buf, fileLen, 1, fp );	//��ȡ�ļ�
	buf[fileLen] = '\0';			//�ļ���β�����ַ���������
	
	//�����з���Ϊ�ַ��������ַ���ע�⣺�����Linuxϵͳ����ΪLinux���з�Ϊ\n��WindowsΪ\r\n
	int lineNum = 1;
	for( int i = fileLen - 1; i >= 0; --i )		
	if( buf[i] == g_lineEnd )
	{
			buf[i] = '\0';
		++lineNum;
	}
	desVec.reserve( desVec.size() + lineNum );

	char *line = buf;					//ָ���е�ָ��
	int lineLen, pos;					//�г���
	while ( line <= buf + fileLen ) 
	{
		lineLen = strlen( line );		
		
		//������а��������ַ������
		for( pos = 0; pos < lineLen && ( line[ pos ] == ' ' || line[ pos ] == '\t'|| line[ pos ] == g_lineEnd  ) ; ++pos );
			if( pos < lineLen )
				desVec.push_back( line );

		line += lineLen + g_lineEndLen;			//�絽��һ�п�ʼ�ַ�
	}

	fclose(fp);
	delete[] buf;
	buf = NULL;
	return true;
}

//��fwriteд���ļ�
bool File::write(const string &content )
{
	if( m_strPath.empty() )
		return false;

	FILE *fp;
	fp = fopen( m_strPath.c_str(), "w" );
	if( !fp )
	{
		cerr << "can't open file:" << m_strPath << endl;
		return false;
	}

	fwrite( content.c_str(), content.size(), 1, fp );

	fclose(fp);
	return true;
}

//write vector to file
bool File::write( vector<string>& srcVec )
{
	if( m_strPath.empty() )
		return false;

	ofstream outFile( m_strPath.c_str() );
	if( !outFile )
		return false;

	vector<string>::iterator iter, iterEnd = srcVec.end();
	for( iter = srcVec.begin(); iter != iterEnd; ++iter )
	{
		//if( !iter->empty() && iter->find_first_not_of( " \t" ) != string::npos )		//ȥ�����к�û�����ֵ���
		outFile << (*iter).c_str() << endl;
	}
	outFile.close();
	return true;

}


bool File::write( const deque<string> &srcQueue )
{
	if( m_strPath.empty() )
		return false;

	string content;
	deque<string>::const_iterator dIter, dIter_end = srcQueue.end();
	for( dIter = srcQueue.begin(); dIter != dIter_end; ++dIter )
	{
		content += *dIter;
	}

	FILE *fp;
	fp = fopen( m_strPath.c_str(), "w" );
	if( !fp )
	{
		cerr << "can't open file:" << m_strPath << endl;
		return false;
	}

	fwrite( content.c_str(), content.size(), 1, fp );

	fclose(fp);
	return true;
}
