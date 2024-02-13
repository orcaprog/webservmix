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
    resp_done = 0;
    is_run = 0;
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
    resp_done = 0;
    is_run = 0;
}

Cgi& Cgi::operator=(const Cgi& oth){
    if (this != &oth){
        serv = oth.serv;
        method_type = oth.method_type;
    }
    return *this;
}

void Cgi::set_arg(Servers srv, const string& mtype){
    serv = srv;
    method_type = mtype;
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

void Cgi::exec_cgi(const string& fullUri_path){
    if (!is_run){
        set_cmd(fullUri_path);
        set_env();
        cmd = cmds[0];
        is_run = 1;
        start_time = clock();
        pid = fork();
        if(pid < 0){
          perror("fork fail");
          exit(EXIT_FAILURE);
        }
        if (pid == 0){
            file = fopen(out_file.c_str(), "w");
            dup2(file->_fileno,STDOUT_FILENO);
            if(execve(cmd,cmds,env) == -1){
                std::cout<<"Cannot execute cgi scripte\n";
                exit(1);
            }
        }
    }
    if (waitpid(pid,&exit_stat,WNOHANG)>0)
        cgi_execueted = 1;
    else{
        if ((clock()-start_time)/CLOCKS_PER_SEC > 10){
            kill(pid,SIGKILL);
            cgi_execueted = 1;
            get.get(serv.error_page["408"]);
            resp_done = 1;
        }
    }

}

void Cgi::execute(Method *method){
    if (!cgi_execueted){
        exec_cgi(method->fullUri_path);
    }
    else{
        get.get(string(out_file));
        if (get.end)
            resp_done = 1;
    }
}

void Cgi::generate_file_name(){
    time_t currentTime = std::time(NULL);
    std::tm* tm = std::localtime(&currentTime);
    char time_B[80];
    std::strftime(time_B, sizeof(time_B), "%Y-%m-%d-%H-%M-%S", tm);
    out_file = time_B;
    out_file += string(".html");
}

void Cgi::set_env(){

    generate_file_name();
    string senv("QUERY_STRING=");
    senv += serv.querys;
    env[0] = new char[senv.length()+1];
    strcpy(env[0],senv.c_str());
    senv = "PATH_INFO=";
    senv += "None";
    env[1] = new char[senv.length()+1];
    strcpy(env[1],senv.c_str());
}

void Cgi::set_cmd(const string& fullUri_path){

    extension_search(fullUri_path);
    cout<<"cgi_exten: "<<extension<<endl;
    map<string,string>::iterator it;
    it = serv.UriLocation.cgi_path.find(extension);
    if (it == serv.UriLocation.cgi_path.end()){
        cerr<<"No Cgi Command"<<endl;
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
