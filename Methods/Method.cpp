#include "Method.hpp"

Method::Method(){
    ;
}

Method::Method(const Method& oth){
    *this = oth;
}

Method& Method::operator=(const Method& oth){
    cout<<"METHOD COPY ASSIGNMENT"<<endl;
    if (this != &oth){
        end = oth.end;
        serv = oth.serv;
        uri = oth.uri;
        http_v = oth.http_v;
        headers = oth.headers;
        respons = oth.respons;
        fullUri_path = oth.fullUri_path;
    }
    return *this;
}


Method::~Method(){
    ;
}