#ifndef REQUEST_HPP
#define REQUEST_HPP

// #include "../Methods/Post.hpp"
#include "../Methods/Cgi.hpp"

using namespace std;


class Cgi;

enum errors{
    Method_Unkounu = 1,
    Httpv_Unkounu = 2,
    Invalid_Header = 4,
};

class Request
{
std::string req_h;
std::string root_path;
Cgi cgi;
int parce_req(const std::string& req1);
int parce_line(const std::string& line);
int parce_rline(const std::string& line);
int parce_key(const std::string& key);
int spl_reqh_body(std::string s1);
Method* create_method(const string &type);
public:
    Servers serv;
    int error;
    Method* method;
    size_t body_size;
    int body_state;
    std::string type;
    std::string r_path;
    std::string uri;
    std::string req_path;
    std::string http_v;
    std::string body;
    std::map<std::string, std::string> headers;

    Request();
    Request(const Servers &ser);
    Request(const Request& req);
    Request& operator=(const Request& oth);
    void process_req(const string &req, size_t read_len, int event);
    string get_respons() const;
    int resp_done();
    ~Request();
};

#endif