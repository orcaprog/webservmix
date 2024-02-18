#ifndef REQUEST_HPP
#define REQUEST_HPP

// #include "../Methods/Post.hpp"
#include "../Methods/Delete.hpp"
#include "../Methods/Cgi.hpp"

using namespace std;


class Cgi;

enum errors{
    Method_Unkounu = 1,
    Httpv_Unkounu = 2,
    Invalid_Header = 4,
    NotAllowedMethod = 8,
    Uri_Too_Long = 16,
};

class Request
{
string req_h;
string error_resp;
Cgi cgi;
int parce_req(const string& req1);
int parce_line(const string& line);
int parce_rline(const string& line);
int parce_key(const string& key);
int spl_reqh_body(string s1);
Method* create_method(const string &type);
void check_for_error();
public:
    Servers serv;
    int error;
    int method_type;
    Method* method;
    size_t body_size;
    int body_state;
    std::string type;
    std::string r_path;
    std::string uri;
    std::string http_v;
    std::string body;
    std::map<std::string, std::string> headers;

    Request();
    Request(const Servers &ser);
    Request(const Request& req);
    Request& operator=(const Request& oth);
    void process_req(const string &req, int event);
    string get_respons() const;
    int resp_done();
        clock_t startTime ;
    ~Request();
};

#endif