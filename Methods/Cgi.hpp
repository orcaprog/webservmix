#ifndef CGI_HPP
#define CGI_HPP

#include "Get.hpp"
#include "Post.hpp"



class Cgi
{
int pid;
int is_run;
int cgi_execueted;
int exit_stat;
size_t pos;
char *cmd;
char **cmds;
char **env;
FILE *file;
string extension;
string out_file;
string method_type;
Servers serv;
void set_env(const string& fullUri_path);
void generate_file_name();
void set_cmd(const string& fullUri_path);
int extension_search(const string& f_name);
void exec_cgi(const string& fullUri_path);
public:
    time_t start_time;
    int resp_done;
    Get get;
    Cgi();
    Cgi(Servers _serv, const string& m_type);
    Cgi& operator=(const Cgi& oth);
    void execute(Method *method);
    void set_arg(Servers srv, const string& mtype);
    ~Cgi();
};

#endif