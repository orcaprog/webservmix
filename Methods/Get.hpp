
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
ssize_t content_len;
size_t file_len;
string body;
string res_h;
fstream src_file;
map<string,string> types;
string content_type;
string extension;

int set_content_type(const string& file_name);
void set_extentions();
void open_file(const string& file_name);
void set_headers();
int check_headers();
void read_file();
void set_content_length(string line);
public:
    string extension_search(const string& file_name, int spl);
    Get();
    Get(const Get& oth);
    Get& operator=(const Get& oth);
    int process(std::string body, int event);
    void get(const string& file_name);
    int is_tpye_supported(const string& file_name);
    ~Get();
};

#endif