#include "Get.hpp"
#include <sys/stat.h>

Get::Get(){
    set_extentions();
    end = 0;
    opened = 0;
    content_len = -1;
}


Get::Get(const Get& oth){
    set_extentions();
    content_len = -1;
    *this = oth;
}


Get& Get::operator=(const Get& oth){
    if (this != &oth){
        serv = oth.serv;
        http_v = oth.http_v;
        uri = oth.uri;
        headers = oth.headers;
        req_path = oth.req_path;
        fullUri_path = oth.fullUri_path;
        content_len = oth.content_len;
        types = oth.types;
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

int Get::is_tpye_supported(const string& file_name){
    string exten = extension_search(file_name, '.');
    if (types.find(exten) != types.end())
        return 1;
    return 0;
}

string Get::extension_search(const string& f_name, int spl){
    string extnsion = "";
    size_t tmp = f_name.find(spl);
    pos = tmp;
    while (tmp != string::npos){
        pos = tmp;
        tmp = f_name.find(spl,pos+1);
    }
    if (pos != string::npos && pos+1 < f_name.size()){
        if (spl == '/')
            extnsion = f_name.substr(0,pos);
        else
            extnsion = f_name.substr(pos+1);
    }
    return extnsion;
}


int Get::set_content_type(const string& file_name){
    extension = extension_search(file_name, '.');
    cout<<"extension: "<<extension<<endl;
    if (types.find(extension) != types.end())
        content_type = types.find(extension)->second;
    else if (extension == "")
        content_type = "application/octet-stream";
    else{
        serv.status = "415";
        get(serv.error_page["415"]);
        return 0;
    }
    return 1;
}

void Get::set_content_length(string line){
    if (content_len != -1)
        return ;
    stringstream sl;
    string key;
    string value;
    sl<<line;
    getline(sl, key, ':');
    if (key == "Content-Length"){
        getline(sl,value,'\r');
        content_len = (size_t)strtod(value.c_str(), NULL);
    }
    return ;
}

int Get::check_headers(){
    int hed = 0;
    string line;
    getline(src_file,line);
    res_h += line + "\n";
    head_size = line.size()+1;
    while (line.size() && line[line.size()-1] == '\r' && !hed){
        getline(src_file,line);
        res_h += line + "\n";
        set_content_length(line);
        head_size += line.size()+1;
        if (head_size > 5000)
            break;
        if (line.size() && line == "\r")
            hed = 1;
    }
    return hed;
}

void Get::set_headers(){
    int hed = 0;
    respons = "HTTP/1.1 " + serv.status;
    respons += string("\r\nContent-Type: ");
    respons += content_type+string("\r\n");
    respons += string("Content-Length: ");
    hed = check_headers();
    if (hed)
        file_len -= head_size;
    if (content_len > 0){
        if ((size_t)content_len < file_len)
            file_len = content_len;
        content_len += head_size;
    }
    stringstream ss;
    ss<<file_len;
    respons += ss.str()+string("\r\n");
    if (!hed){
        respons += "\r\n";
        respons += res_h;
    }
    else
        respons += res_h;
}


void Get::open_file(const string& file_name){
    if (!set_content_type(file_name))
        return ;
    src_file.open(file_name.c_str(), ios::in);
    opened = 1;
    if (!src_file.is_open()){
        src_file.clear();
        serv.status = "500";
        get_err_page(serv.error_page["500"]);
        opened = -1;
        return;
    }
    src_file.seekg(0, std::ios::end);
    file_len = src_file.tellg();
    src_file.seekg(0, std::ios::beg);
    set_headers();
}


void Get::get(const string& file_name){
    respons.clear();
    if (file_name == ""){
        respons = "HTTP/1.1 " + serv.status + "\r\n\r\n";
        end = 1;
        return;
    }
    if (!opened)
        open_file(file_name);
    if (opened == 1 && file_len && !end)
        read_file();
    if (opened == -1 || !file_len)
        end = 1;
}

void Get::read_file(){
    ssize_t r_len, max_r = 1024;
    string res;

    if (content_len != -1 && content_len > max_r)
        content_len -= max_r;
    else if (content_len != -1){
        max_r = content_len;
        content_len = 0;
    }
    res.resize(max_r);
    src_file.read(&res[0], max_r);
    r_len = src_file.gcount();
    res.resize(r_len);
    respons += res;
    if (r_len < max_r || !content_len){
        src_file.close();
        end = 1;
    }
    // if (respons.size())
    //     cout<<"\n\nres:"<<respons<<"\nres_end\n"<<endl;
}

int Get::process(string _body, int event){
    body = _body;
    if (event == EPOLLIN)
        return 0;
    get(fullUri_path);
    return 0;
}

void Get::get_err_page(const string& err_p_name){
    string res;
    fstream err_page;

    res.resize(1000);
    err_page.open(err_p_name.c_str(), ios::in);
    if (err_page.is_open()){
        src_file.seekg(0, std::ios::end);
        file_len = src_file.tellg();
        src_file.seekg(0, std::ios::beg);
        set_headers();
        err_page.read(&res[0],1000);
        res.resize(err_page.gcount());
        respons += res;
        end = 1;
    }
}

Get::~Get(){
}