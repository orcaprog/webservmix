/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouassi <abouassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 17:42:27 by abouassi          #+#    #+#             */
/*   Updated: 2024/01/28 09:58:44 by abouassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERS_HPP
#define SERVERS_HPP
#include <arpa/inet.h>

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <algorithm> 
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include <map>
#include <sys/stat.h>
#include "Location.hpp"
#include <stack>
#include <dirent.h>
#include <iostream>
#include<cstring>
#include <ctime>
#include <stdlib.h>
#include <limits.h>


class Servers
{
private:
    int  checkDup(std::string der,int & index);
    bool check_isdigit(std::string str);
    void FillValid();
    void check_Status(std::string status);
    int pathIsFile(std::string path);
    size_t GetIndex(std::string dir);
    Location FirstFill(size_t & index);
    void SetPorts (); 
    void SetServerName(vector<string> & ser_names); 
    void SetHost(); 
    void SetRoot();
    void SetError_page();  
    void SetClient_max_body_size(); 
    void SetIndex(); 
    void FillQuerys(string & uri);
    void SetReturn();
    void SetRederectionResp(vector<string> & redirect);

public:

    vector<string> redirect;
    vector<string> rStatus;
    std::vector<int> port;
    std::vector<std::string> server_name; 
    std::vector<std::string> host; 
    std::vector<std::string> root;
    std::vector<std::string> index;
    std::vector<long long int > client_max_body_size;
    map<string,string> error_page;
    std::vector<std::string> s_erorr;
    bool  operator== (const Servers& ser);
    bool  operator== (const string & servername);
    int server_fd;
    bool sercheck;
    struct sockaddr_in address;
    std::vector<std::string> Vstrvalid;
    std::vector<std::vector<std::string> > servconf;
    std::vector<Location> locations;
    void SetAllDir(vector<string> & ser_names);
    void desplay();
    int getLocation(std::string path);
    void SetDefaultError();
    void CreatSocketServer( std::map<int,vector<Servers> > & msockets);
    Servers();
    void SetIndex_Of(string path);
    int searchPathLocation(string & uri);
    int fillFromLocation(int &in, string &uri,string & method);
    void FillData(string uri,string mehtod);
    string rootUri;
    bool Is_cgi;
    string querys;
    string status;
    void SetUriRoot(int i,string & uri);
    Location UriLocation;
    void FillLocation();
    bool MatchingWithRoot(string & rootPlusUri,string &rootPath);
    ~Servers();
};


#endif