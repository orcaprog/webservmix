
#ifndef GET_HPP
#define GET_HPP

#include <cstring>

using namespace std;

#include "Method.hpp"

class Get : public Method
{
int head_size;
int opened;
size_t pos;
string body;
size_t file_len;
fstream src_file;
map<string,string> types;
string content_type;
string extension;

int set_content_type(const string& file_name);
void set_extentions();
void open_file(const string& file_name);
void set_headers();
int extension_search(const string& f_name);
public:
    Get();
    void get(const string& file_name);
    Get(const Get& oth);
    Get& operator=(const Get& oth);
    int process(std::string body, int event);
    ~Get();
};

#endif