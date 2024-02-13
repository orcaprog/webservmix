#include "Request.hpp"

Request::Request()
{
    method = NULL;
    body_state = 0;
    body_size = 0;
    error = 0;
}


Request::Request(const Servers &ser){
    method = NULL;
    body_state = 0;
    body_size = 0;
    error = 0;
    serv = ser;
}

Request::Request(const Request& req1){
    method = NULL;
    *this = req1;
}

Request& Request::operator=(const Request& oth){
    if (this != &oth){
        delete method;
        error = oth.error;
        method = oth.method;
        body_state = oth.body_state;
        body_size = oth.body_size;
        type = oth.type;
        r_path = oth.r_path;
        http_v = oth.http_v;
        body = oth.body;
        headers = oth.headers;
        uri = oth.uri;
        serv = oth.serv;
    }
    return *this;
}

int Request::spl_reqh_body(string s1)
{
    if (body_state){
        body = s1;
        body_size += body.size();
        return 0;
    }
    if (s1.find("\r\n\r\n", 0) != s1.npos)
    {
        body = s1.substr(s1.find("\r\n\r\n", 0) + 4);
        cout << "--_______Lheaders Te9raw Kolhom________--\n" << endl;
        req_h += s1.substr(0, s1.find("\r\n\r\n", 0));
        body_state = 1;
        body_size = body.size();
        return 1;
    }
    req_h += s1;
    return 0;
}

int Request::parce_key(const string &key)
{
    if (key.size() > 0 && !isalpha(key[0]))
        return 0;
    for (size_t i = 0; i < key.size(); i++)
    {
        if (!isalnum(key[i]) && key[i] != '_' && key[i] != '-')
            return 0;
    }
    return 1;
}

int Request::parce_rline(const string &rline){
    stringstream ss;
    string tmp;
    ss<<rline;
    getline(ss, tmp, ' ');
    if (tmp != "GET" && tmp != "POST" && tmp != "DELETE"){
        error = Method_Unkounu;
        return 0;
    }
    method_type = (tmp == "GET") + (tmp == "POST")*2 + (tmp == "DELETE")*4;
    type = tmp;
    getline(ss, tmp, ' ');
    r_path = uri = tmp;
    getline(ss, tmp);
    if (tmp != "HTTP/1.1\r" && tmp != "HTTP/1.1"){
        error = Httpv_Unkounu;
        return 0;
    }
    http_v = tmp;
    return 1;
}

int Request::parce_line(const string &line)
{
    stringstream ss;
    string key;
    string value;

    ss<<line;
    getline(ss, key, ':');
    getline(ss, value, ' ');
    value.clear();
    getline(ss, value, '\r');
    if (!parce_key(key) && value.size()){
        error = Invalid_Header;
        return 0;
    }
    if (value.size() == 0){
        cout << "ERROR: No Value For Key " << key << endl;
        error = Invalid_Header;
        return 0;
    }
    headers[key] = value;
    return 1;
}

int Request::parce_req(const string &req)
{
    std::stringstream sstr;
    string line;

    if (!spl_reqh_body(req))
        return 1;
    sstr << req_h;
    getline(sstr,line);
    if (!parce_rline(line))
        return 0;
    while (sstr.peek() != -1)
    {
        getline(sstr, line);
        if (line.size() && !parce_line(line))
            return 0;
    }
    serv.FillData(uri,type);
    if (!(serv.UriLocation.permession & method_type)){
        error = error | NotAllowedMethod;
        return 0;
    }
    method = create_method(type);
    cgi.set_arg(serv, type);
    return 1;
}

void Request::check_for_error(){
    if (!error)
        return;
    Get get;
    if (error == Method_Unkounu || error == Httpv_Unkounu || error == Invalid_Header){
        get.serv.status = "400";
        get.get(serv.error_page["400"]);
    }
    if (error & NotAllowedMethod){
        get.serv.status = "405";
        get.get(serv.error_page["405"]);
    }
    error_resp = get.respons;
}

void    Request::process_req(const string &req, size_t read_len, int event){
    if (!parce_req(req)){
        check_for_error();
        return ;
    }
    if (body_state && method){
        if (serv.Is_cgi)
            cgi.execute(method);
        else{
            if (type == "GET")
                method->process(body, event);
            else
                method->process(body, read_len);
        }
    }
}

int Request::resp_done(){
    if (error)
        return 1;
    if (serv.Is_cgi){
        if (cgi.resp_done)
            return 1;
    }
    else{
        if (method && method->end)
            return 1;
    }
    return 0;
}

Method* Request::create_method(const string &type){
    Method* m = NULL;
    if (type == "GET")
        m = new Get();
    else if (type == "POST")
        m = new Post();
    //else if (type == "DELETE")
    //     m = new Delete();
    else
        cerr<<"Cannot Create Method: "<<"|"<<type<<"|"<<endl;
    if (m){
        m->headers = headers;
        m->http_v = http_v;
        m->uri = uri;
        m->fullUri_path = serv.rootUri;
        m->serv = serv;
    }
    return (m);
}

string Request::get_respons() const{
    if (error)
        return error_resp;
    if (!method)
        return("");
     if (serv.Is_cgi)
            return cgi.get.respons;
    return (method->respons);
}

Request::~Request(){
    delete method;
}
