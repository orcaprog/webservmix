#include "Request.hpp"

Request::Request()
{
    method = NULL;
    body_state = 0;
    body_size = 0;
    error = 0;
}


Request::Request(const vector<Servers>& vser){
    method = NULL;
    body_state = 0;
    body_size = 140;
    error = 0;
    ser_vec = vser;
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
        ser_vec = oth.ser_vec;
    }
    return *this;
}

void Request::set_serv(){
    string serv_name;
    if (headers.find("Host") == headers.end()){
        serv = ser_vec[0];
        return ;
    }
    serv_name = headers.find("Host")->second;
    vector<Servers>::iterator it;
    it = find(ser_vec.begin(), ser_vec.end(), serv_name);
    if (it != ser_vec.end())
        serv = *it;
    else
        serv = ser_vec[0];
}

int Request::spl_reqh_body(string s1)
{
    if (body_state){
        body = s1;
        body_size += body.size();
        return 0;
    }
    size_t pos = s1.find("\r\n\r\n", 0);
    if (pos != s1.npos)
    {
        body = s1.substr(pos + 4);
        cout << "--_______Lheaders Te9raw Kolhom________--\n" << endl;
        req_h += s1.substr(0, pos);
        body_state = 1;
        body_size = body.size();
    }
    else
        req_h += s1;
    if (req_h.length() > 10000){
        error = error | Headers_Too_Large;
        return 0;
    }
    return body_state;
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
    uri = tmp;
    if (uri.size() > 1000){
        error = Uri_Too_Long;
        return 0;
    }
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
        error = error | Invalid_Header;
        return 0;
    }
    if (value.size() == 0){
        error = error | Invalid_Header;
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
    set_serv();
    serv.FillData(uri,type);
    if (!(serv.UriLocation.permession & method_type)){
        error = error | Not_Allowed_Method;
        return 0;
    }
    method = create_method(type);
    cgi.set_arg(serv, type, headers);
    return 1;
}

void Request::check_for_error(){
    if (!error || error_resp.size())
        return;
    
    cout<<"ERROR: "<<error<<endl;
    string err_page_name;
    Get get;
    if (error == Method_Unkounu || error == Invalid_Header)
        err_page_name = "400";
    else if (error & Not_Allowed_Method)
        err_page_name = "405";
    else if (error & Uri_Too_Long)
        err_page_name = "414";
    else if (error & Httpv_Unkounu)
        err_page_name = "505";
    else if (error & Headers_Too_Large)
        err_page_name = "413";
    get.serv.status = err_page_name;
    get.get(serv.error_page[err_page_name]);
    error_resp = get.respons;
}

void    Request::process_req(const string &req, int event){
    if (!parce_req(req) || error){
        check_for_error();
        return ;
    }
    if (body_state && method){
        if (serv.Is_cgi)
            cgi.execute(method, event);
        else{
            if (type == "GET")
                method->process(body, event);
            else
                method->process(body, event);
        }
    }
}

int Request::resp_done(){
    if (error)
        return 1;
    if (serv.Is_cgi && type == "GET"){
        if (cgi.resp_done){
            cout<<"cgi_done"<<endl;
            return 1;
        }
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
