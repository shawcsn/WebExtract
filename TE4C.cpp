#include "FileTinyXml.h"
#include "TitleExtract.h"
#include "TE4C.h"
#include <iostream>

using namespace std;

const size_t default_title_buf_length   = 100;
const size_t default_content_buf_length = 100000;

char *title_buf        = (char*)malloc(sizeof(char*)*default_title_buf_length);
char *content_buf      = (char*)malloc(sizeof(char*)*default_content_buf_length);

size_t title_buf_len   = default_title_buf_length;
size_t content_buf_len = default_content_buf_length;


extern "C" void TextExtract(char *src_file, char *url, char **title, char **content) {
    TitleExtract titleextract(src_file, url);
    titleextract.extract();

    if ( titleextract.m_title.size()+1 > title_buf_len ) {
        title_buf = (char*)realloc(title_buf, titleextract.m_title.size()+1);
        if(!title_buf) {
            title_buf_len = 0;
            cerr<<"memory allocation failed"<<endl;
        }
        else {
           title_buf_len = titleextract.m_title.size() + 1;
           strcpy(title_buf,titleextract.m_title.c_str());
        }
    }
    else {
        strcpy(title_buf,titleextract.m_title.c_str());
    }

    if ( titleextract.m_content.size()+1 > content_buf_len ) {
        content_buf = (char*)realloc(content_buf, titleextract.m_content.size()+1);
        if(!content_buf) {
            content_buf_len = 0;
            cerr<<"memory allocation failed"<<endl;
        }
        else {
            content_buf_len = titleextract.m_content.size()+1;
            strcpy(content_buf,titleextract.m_content.c_str());
        }
    }
    else {
        strcpy(content_buf,titleextract.m_content.c_str());
    }

    *title   = title_buf;
    *content = content_buf;
}
