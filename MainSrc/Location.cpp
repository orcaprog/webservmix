/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouassi <abouassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 18:23:03 by abouassi          #+#    #+#             */
/*   Updated: 2024/01/31 16:07:30 by abouassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"



void Location::FillValid()
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
     Vstrvalid.push_back("#");
     Vstrvalid.push_back("allow_methods");
     Vstrvalid.push_back("autoindex");
     Vstrvalid.push_back("upload");
     Vstrvalid.push_back("cgi_path");
}
void Location::checkValidation()
{
    std::vector<std::string>::iterator iter;
    for (size_t i = 0; i < vlocation.size() ; i++)
    {
        iter = std::find(Vstrvalid.begin(),Vstrvalid.end(),vlocation[i][0]);
        if (iter == Vstrvalid.end())
        {
            throw "Error : Invalid Derecties '"+vlocation[i][0]+"'\n";
        }
    }
}


int Location::pathIsFile(std::string path)
{
    struct stat fileStat;
    if (stat(path.c_str(), &fileStat) == 0)
    {
        if (fileStat.st_mode & S_IFREG)
        {
            return 2;
        }
        else if (fileStat.st_mode & S_IFDIR)
        {
            return 3;
        }
        else
            return 4;
    }
    else
        return 0;
}

int Location::pathExists(std::string path) {
    struct stat fileStat;
    return stat(path.c_str(), &fileStat) == 0;
}
size_t Location::GetIndex(std::string dir)
{
    size_t i ;
    for (i = 0; i < vlocation.size(); i++)
    {
        if (vlocation[i][0] == dir)
        {
            return i;
        }
    }
    return (i);
}
int Location::checkDup(std::string der,int & index)
{
    int dup = 0;
    size_t i = 0;
    while ( i < vlocation.size())
    {
        
        if (vlocation[i][0] == der)
        {
            index = i;
            dup++;
        }
        if (dup > 1)
        {
           throw "Error duplicate derective \n";
        }
        i++;
    }
    return (dup);
}
void Location::desplayLocation()
{

    // Printtwodom(vlocation,"location");
    cout<<"PATH :"<<path[0]<<endl;
    cout<<"permession:"<<permession<<endl;
    cout<<"Root :"<<root[0]<<endl;
    map<string,string>::iterator iter = cgi_path.begin();
    while (iter != cgi_path.end())
    {
        cout<<"cgi_path :'"<<iter->first<<"' '"<<iter->second<<"'\n";
        iter++;
    }

    std::cout<<"index  :"<<index[0]<<std::endl;
}
void Location::SetAllDir()
{
    FillValid();
    // checkValidation();
    SetRoot();
    SetAllowMethods();
    SetUpload();
    SetCgiPath();
    SetAutoindex();
    SetIndex();
    SetPath();
}
void Location::SetIndex()
{
    int i;
    int num = checkDup("index", i);
    std::string arg;
    if (num == 0)
    {
        index.push_back(Servindex);
        return;
    }
    if (vlocation[i].size() != 2)
    {
        throw "invalid port in of the server_name directive \n";
    }
    arg = vlocation[i][1];
    index.push_back(arg);
}
void Location::Printtwodom(const std::vector<std::vector<std::string> > & matrix,std::string data)
{
    std::cout<<data<<"   :\n";
    for (std::vector<std::vector<std::string> >::const_iterator row = matrix.begin(); row != matrix.end(); ++row) {
        
    for (std::vector<std::string>::const_iterator value = row->begin(); value != row->end(); ++value) {
        std::cout <<"   :"<< *value << "|";
    }
    std::cout << std::endl;
    } 
}

 void Location::SetIndexRoot(string root,string index)
 {
    Servindex = index;
    ServRoot = root;
 }

void Location::SetRoot()
{
    int i;
    int num = checkDup("root",i);
    std::string arg;
    if (num == 0)
    {
        root.push_back(ServRoot);
        return ;
    }
    if (vlocation[i].size() != 2 )
    {
         throw "invalid root directive \n";
    }
    arg = vlocation[i][1];
    if (!pathExists(arg)) 
    {
        throw ("Path '"+arg+"' does not exist.\n");
    }
    root.push_back(arg);
}
void Location::SetPath()
{
    int i;
    int num = checkDup("location",i);
    std::string arg;
    if (num == 0)
    {
        path.push_back("");
        return ;
    }
    if (vlocation[i].size() != 2 )
    {
         throw "invalid location directive \n";
    }
    arg = vlocation[i][1];
    // if (!pathExists(arg))
    // {
    //     throw ("Path '"+arg+"' does not exist.\n");
    // }
    path.push_back(arg);
}

void Location::CheckMethods(std::string methd)
{
    std::vector<std::string> allMethds;
    std::vector<std::string>::iterator iter;
    if ((get && methd == "GET" ) ||( post && methd == "POST" )|| (dele && methd == "DELETE") )
    {
        throw "Error duplicate methods "+methd+" \n";
    }
    
    allMethds.push_back("POST");
    allMethds.push_back("GET");
    allMethds.push_back("DELETE");
    iter = std::find(allMethds.begin(),allMethds.end(),methd);
    if (iter == allMethds.end())
    {
        throw "error no method "+methd+ " exists\n";
    }
    if (methd == "POST")
        post = 1;
    if (methd == "GET")
        get = 1;
    if (methd == "DELETE")
        dele = 1;
}
void Location::SetAllowMethods()
{
    int i;
    int num = checkDup("allow_methods",i);
    post = 0;
    get = 0;
    dele = 0;
    std::string arg;
    std::vector<std::string> vallow;
    if (num == 0)
    {
        return ;
    }
    if (vlocation[i].size() > 4 || vlocation[i].size() < 2 )
    {
         throw "invalid methods arguments directive \n";
    }
    vallow =  vlocation[i];

    
    std::vector<std::string>::iterator iter;
    iter = vallow.begin();
    iter++;
    while (iter != vallow.end())
    {
        CheckMethods(*iter);
        permession = (POST * post) | (DELETE * dele) | (GET * get);
        iter++;
    }    
}
void Location::SetAutoindex()
{
    int i;
    int num = checkDup("autoindex",i);
    std::string arg;
    if (num == 0)
    {
        return ;
    }
    if (vlocation[i].size() != 2 )
    {
         throw "invalid autoindex directive \n";
    }
    arg = vlocation[i][1];
    if (arg != "on" &&  arg != "off")
    {
        throw "The argument  '"+arg+ "' is not valid\n";
    }
    if (arg == "on")
        permession += AUTOINDEX;
}
void Location::SetUpload()
{
    int i;
    int num = checkDup("upload",i);
    std::string arg;
    if (num == 0)
    {
        return ;
    }
    if (vlocation[i].size() != 2 )
    {
         throw "invalid upload directive \n";
    }
    arg = vlocation[i][1];
    if (arg != "on" &&  arg != "off")
    {
        throw "The argument  '"+arg+ "' is not valid\n";
    }
    if (arg == "on")
        permession += UPLOAD;
}
// void Location::check_Status(std::string status)
// {
//     std::vector<std::string>::iterator iter;

//     iter = std::find(s_erorr.begin(),s_erorr.end(),status);
//     if (iter == s_erorr.end())
//     {
//         throw "Error : Invalid status '"+status +"' code  Derecties\n";
//     }
// }


void Location::SetCgiPath()
{
    std::string extantion;
    std::string path;
    for (size_t i = 0; i < vlocation.size(); i++)
    {
        if (vlocation[i][0] == "cgi_path")
        {
            if (vlocation[i].size() != 3)
            {
                throw "Error in error_page\n";
            }
            extantion = vlocation[i][1];
            path = vlocation[i][2];
            // check_extantion(extantion);
            if (!pathExists(path)) 
            {
                throw ("Path '"+path+"' does not exist.\n");
            }
            cgi_path[extantion] = path;
        }
    }
}

/*__________________________________________*/
/*__________________________________________*/
/*__________________________________________*/


Location::Location()
{
    permession = 0;
}

Location::~Location()
{
}
