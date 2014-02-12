#include "FileTinyXml.h"
#include "TitleExtract.h"

using namespace std;

int main(int argc,char* argv[])
{
	
	//	if (argc!=2)
	//	{
	//		return -1;
	//	}
	char *srcFile = argv[1];				//网页本地存放地址
	//	char *srcFile = "test.html";
	char *url =""; 
	TitleExtract titleextract( srcFile, url );
	char *content;										//网页正文
	char *title;										//网页标题
    char *herf;
	titleextract.extract( title, content );
	titleextract.herfextract(herf);
	FILE* fp=fopen("content.txt","w+");
    FILE* fp1=fopen("title.txt","w+");
	FILE* fp2=fopen("herf.txt","w+");
	fprintf(fp1,"%s",title);
	fprintf(fp,"\n");
	fprintf(fp,"%s",content);
	//fprintf(fp2,"\n");
	fprintf(fp2,"%s",herf);
	fclose(fp);
    fclose(fp1);
	fclose(fp2);
	return 0;
}

