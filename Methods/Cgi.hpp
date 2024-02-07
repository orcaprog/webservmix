#ifndef CGI_HPP
#define CGI_HPP

#include "Get.hpp"


class Get;

class Cgi
{
int pid;
int cgi_execueted;
size_t pos;
// size_t file_len;
char **cmds;
char **env;
string extension;
string method_type;
Servers serv;
public:
    Cgi();
    Cgi(Servers _serv, const string& m_type);
    Cgi& operator=(const Cgi& oth);
    void execute(const string& fullUri_path);
    void set_env();
    void set_cmd(const string& fullUri_path);
    int extension_search(const string& f_name);
    ~Cgi();
};

#endif