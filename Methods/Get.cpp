#include "Get.hpp"
#include <sys/stat.h>


Get::Get(){
    set_extentions();
    end = 0;
    opened = 0;
}

Get::Get(const Get& oth){
    set_extentions();
    *this = oth;
}

Get& Get::operator=(const Get& oth){
    cout<<"GET COPY ASSIGNMENT"<<endl;
    if (this != &oth){
        serv = oth.serv;
        http_v = oth.http_v;
        uri = oth.uri;
        headers = oth.headers;
        req_path = oth.req_path;
        fullUri_path = oth.fullUri_path;
    }
    return *this;
}

void Get::set_extentions(){
    types["html"] = "text/html";
    types["htm"] = "text/html";
    types["css"] = "text/css";
    types["jpeg"] = "image/jpeg";
    types["jpg"] = "image/jpeg";
    types["png"] = "image/png";
    types["gif"] = "image/gif";
    types["json"] = "application/json";
    types["mp4"] = "video/mp4";
    types["mp3"] = "audio/mpeg";
    types["js"] = "application/javascript";
    types["bmp"] = "image/bmp";
    types["ico"] = "image/x-icon";
    types["pdf"] = "application/pdf";
    types["txt"] = "text/plain";
    types["xml"] = "application/xml";
    types["zip"] = "application/zip";
    types["tar"] = "application/x-tar";
    types["gz"] = "application/gzip";
}

int Get::extension_search(const string& f_name){
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

void Get::set_content_type(const string& file_name){
    extension_search(file_name);
    if (serv.Is_cgi){
        content_type = "text/html";
    }
    cout<<"extension1: "<<extension<<endl;
    if (types.find(extension) != types.end())
        content_type = types.find(extension)->second;
    else
        content_type = "application/octet-stream";
}

void Get::set_headers(const string& file_name){
    int hed = 0;
    set_content_type(file_name);
    respons = "HTTP/1.1 " + serv.status;
    respons += string("\r\nContent-Type: ");
    respons += content_type+string("\r\n");
    respons += string("Content-Length: ");
    string line;
    getline(src_file,line);
    head_size = line.size()+1;
    while (line.size() && line[line.size()-1] == '\r' && !hed){
        getline(src_file,line);
        head_size += line.size()+1;
        if (line.size() && line == "\r")
            hed = 1;
    }
    if (hed)
        file_len -= head_size;
    stringstream ss;
    ss<<file_len;
    respons += ss.str()+string("\r\n");
    if (!hed)
        respons += "\r\n";
    src_file.seekg(0, std::ios::beg);
}

void Get::open_file(const string& file_name){
    src_file.open(file_name.c_str(), ios::in);
    opened = 1;
    if (!src_file.is_open()){
        cout<<"can't open file: "<<file_name<<endl;
        opened = -1;
        return;
    }
    src_file.seekg(0, std::ios::end);
    file_len = src_file.tellg();
    src_file.seekg(0, std::ios::beg);
    set_headers(file_name);
    cout<<"content_len: "<<file_len<<endl;
    cout<<"content_type: "<<content_type<<endl;
}

void Get::get(const string& file_name){
    ssize_t r_len, max_r = 1000;
    
    if (!opened)
        open_file(file_name); 
    if (opened == 1){
        string res;
        res.resize(max_r);
        src_file.read(&res[0], max_r);
        r_len = src_file.gcount();
        res.resize(r_len);
        respons += res;
        if (src_file.eof())
            end = 1;
        // cout<<"res_size = "<<respons.size()<<"\nres: <<"<<respons<<">>"<<endl;
    }
    if (opened == -1){
        opened = 0;
        end = 1;
        get(serv.error_page["404"]);
    }
    if (end)
        src_file.close();
}

int Get::process(string _body, size_t event){
    body = _body;
    if (event == EPOLLIN)
        return 0;
    respons.clear();
    get(fullUri_path);
    return 0;
}


Get::~Get(){
}