#include "Cgi.hpp"

Cgi::Cgi(){
    cmds = new char *[3];
    cmds[0] = NULL;
    cmds[1] = NULL;
    cmds[2] = NULL;
    env = new char *[4];
    env[0] = NULL;
    env[1] = NULL;
    env[2] = NULL;
    env[3] = NULL;
    cgi_execueted = 0;
}

Cgi::Cgi(Servers _serv, const string& m_type){
    serv = _serv;
    method_type = m_type;
    cmds = new char *[3];
    cmds[0] = NULL;
    cmds[1] = NULL;
    cmds[2] = NULL;
    env = new char *[4];
    env[0] = NULL;
    env[1] = NULL;
    env[2] = NULL;
    env[3] = NULL;
    cgi_execueted = 0;
}

Cgi& Cgi::operator=(const Cgi& oth){
    if (this != &oth){
        serv = oth.serv;
        method_type = oth.method_type;
    }
    return *this;
}

int Cgi::extension_search(const string& f_name){
    extension = "";
    size_t tmp = f_name.find(".");
    pos = tmp;
    while (tmp != string::npos){
        pos = tmp;
        tmp = f_name.find(".",pos+1);
    }
    if (pos != string::npos && pos+1 < f_name.size()){
        extension = f_name.substr(pos+1);
        return 1;
    }
    return 0;
}

void Cgi::execute(const string& fullUri_path){
    FILE *file;
    char * cmd;
    int exit_stat;

    set_cmd(fullUri_path);
    set_env();
    cmd = cmds[0];
    pid = fork();
    if(pid < 0){
      perror("fork fail");
      exit(EXIT_FAILURE);
    }
    if (pid == 0){
        // cout<<"file_name: "<<cmds[1]<<endl;
        file = fopen("out.html", "w");
        dup2(file->_fileno,STDOUT_FILENO);
        if(execve(cmd,cmds,env) == -1){
            std::cout<<"No exec\n";
            perror("execve");
            exit(1);
        }
    }
    waitpid(pid,&exit_stat,0);
    cgi_execueted = 1;
    cout<<WEXITSTATUS(exit_stat)<<endl;
}

void Cgi::set_env(){
    string senv("QUERY_STRING=");

    senv += serv.querys;
    env[0] = new char[senv.length()+1];
    strcpy(env[0],senv.c_str());
    senv = "PATH_INFO=";
    senv += "None";
    env[1] = new char[senv.length()+1];
    strcpy(env[1],senv.c_str());
    // senv = "REQUEST_METHOD=";
    // senv += "GET";
    // env[2] = new char[senv.length()+1];
    // strcpy(env[2],senv.c_str());
}

void Cgi::set_cmd(const string& fullUri_path){

    extension_search(fullUri_path);
    cout<<"cgi_exten: "<<extension<<endl;
    map<string,string>::iterator it;
    it = serv.UriLocation.cgi_path.find(extension);
    if (it == serv.UriLocation.cgi_path.end()){
        cerr<<"No Cgi Extension"<<endl;
        // get(serv.error_page["404"]);
        return ;
    }
    string cmdCgi = it->second;
    cmds[0] =  new char[cmdCgi.length() + 1];
    strcpy(cmds[0],cmdCgi.c_str());
    cmds[1] = new char[ fullUri_path.length() + 1];
    strcpy(cmds[1],fullUri_path.c_str());
}

Cgi::~Cgi(){
    for (int i = 0; cmds && cmds[i]; i++)
	    delete [] cmds[i];
    delete [] cmds;

    for (int i = 0; env[i]; i++)
	    delete [] env[i];
    delete [] env;
}
