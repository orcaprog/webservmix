/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParceConf.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouassi <abouassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 17:54:31 by abouassi          #+#    #+#             */
/*   Updated: 2024/01/26 11:02:48 by abouassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParceConf.hpp"


void ParceConf::FillValid()
{
    Vstrvalid.push_back("listen");
    Vstrvalid.push_back("server");
    Vstrvalid.push_back("server_name");
    Vstrvalid.push_back("host");
    Vstrvalid.push_back("root");
    Vstrvalid.push_back("error_page");
    Vstrvalid.push_back("client_max_body_size");
    Vstrvalid.push_back("index");
    Vstrvalid.push_back("location");
    Vstrvalid.push_back("{");
    Vstrvalid.push_back("}");
    Vstrvalid.push_back("return");
    Vstrvalid.push_back("upload_path");
    Vstrvalid.push_back("allow_methods");
    Vstrvalid.push_back("autoindex");
    Vstrvalid.push_back("upload");
    Vstrvalid.push_back("cgi_path");
}
std::vector<std::string> ParceConf::Split_line(std::string line)
{
    std::stringstream ss;
    std::vector<std::string> vline;
    index = 0;
    ss << line;
    std::string chunks;
    while (std::getline(ss, chunks, ' '))
    {
        if (!chunks.empty())
        {
            if(strncmp(chunks.c_str(),"#",1) == 0)
            {
                break;
            }
            vline.push_back(chunks);
        }
    }
    return vline;
}

void ParceConf::DefaultServer()
{
    Servers ser;
    ser.CreatSocketServer(msockets);
    // ser.UriLocation.permession = 7;
    msockets[ser.server_fd].push_back(ser);
}
void ParceConf::TakeAndParce(std::string confgfile)
{
    std::ifstream configfile;
    std::string line;
    std::vector<std::string> _split;
    std::vector<string>::iterator iterfind;
    if(confgfile.empty())
        DefaultServer();
    else
    {
        configfile.open(confgfile.c_str(), std::ios::in);
        if (configfile.is_open())
        {
            while (getline(configfile, line))
            {
                
                if (!line.empty() )
                {
                    _split = Split_line(line);
                    if (!_split.empty())
                    {
                        Vconf.push_back(_split);
                    }
                }
            }
            configfile.close();
        }
        desplay();
    }        
}
ParceConf::ParceConf()
{
    FillValid();
}

Servers ParceConf::FirstFill()
{
    Servers server;
    std::vector<std::string>::iterator iter;
    int bracket = 0;
    if (Vconf[index][0] != "server" || Vconf[index].size() > 1)
    {
        throw "Error :'"+Vconf[index][0]+"' no server derectires ";
    }
    else
        server.servconf.push_back(Vconf[index]);
    index++;

    if (index < Vconf.size()  && Vconf[index][0] != "{")
    {
        throw "Error : no open brackets \n";
    }
    else
        server.servconf.push_back(Vconf[index]);
    index++;
    bracket++;


    while (index < Vconf.size() && bracket)
    {
        if (Vconf[index][0] == "{" )
        {
            if(Vconf[index - 1 ][0] == "location")
                bracket++;
            else
                throw "open no '"+Vconf[index][0] +"'loaction \n";
                
        }
        if (Vconf[index][0] == "}")
        {
            bracket--;
        }
        iter = std::find(Vstrvalid.begin(),Vstrvalid.end(),Vconf[index][0]);
        if (iter == Vstrvalid.end())
        {
            throw "Error :  unknown directive '"+Vconf[index][0]+"'\n";
        }
        server.servconf.push_back(Vconf[index]);
        index++;
    }

    if(bracket != 0)
    {
        throw "Error :unexpected end of file , expecting '}'";
    }
    return server;
}
void ParceConf::FillServers()
{
    std::vector<std::string> Vcol;
    std::vector<std::string>::iterator iter;

    while (index < Vconf.size())
    {
        Vservers.push_back(FirstFill());
    }
}

void ParceConf::desplay()
{
    FillServers();
    size_t i = 0;
    vector<string> ser_names;
    vector<Servers>::iterator iter;
    vector<Servers> hold;
    while (i < Vservers.size())
    {
        Vservers[i].SetAllDir(ser_names);
        ser_names.push_back(Vservers[i].server_name[0]);
        Vservers[i].CreatSocketServer(msockets);
        msockets[Vservers[i].server_fd].push_back(Vservers[i]);
        hold = msockets[Vservers[i].server_fd];
        iter = find(hold.begin(),hold.end(),"");
        if(iter != hold.end())
            throw "Error : Conflicting same host and port and no exsist of servername\n";
        i++;
    }
}

ParceConf::~ParceConf()
{
}
