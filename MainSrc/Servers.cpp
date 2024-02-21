/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouassi <abouassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 17:43:44 by abouassi          #+#    #+#             */
/*   Updated: 2024/01/30 14:43:35 by abouassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Servers.hpp"
bool isValidIpAddress(string & ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr));
    return result != 0;
}
int  Servers::getLocation(std::string path)
{
    size_t i;
    for (i = 0; i < locations.size(); i++)
    {
        if (locations[i].path[0] == path)
        {
            return i;
        }
    }
    return -1;
}


int Servers::pathIsFile(std::string path)
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

int Servers::pathExists(std::string path)
{
    struct stat fileStat;
    return stat(path.c_str(), &fileStat) == 0;
}

bool Servers::check_isdigit(std::string str)
{
    int i = 0;
    while (str[i])
    {
        if (!isdigit(str[i]))
        {
            return (0);
        }
        i++;
    }
    return (1);
}

void Servers::Printtwodom(const std::vector<std::vector<std::string> > &matrix, std::string data)
{
    for (std::vector<std::vector<std::string> >::const_iterator row = matrix.begin(); row != matrix.end(); ++row)
    {
        std::cout << data << "   :";
        for (std::vector<std::string>::const_iterator value = row->begin(); value != row->end(); ++value)
        {
            std::cout << *value << "|";
        }
        std::cout << std::endl;
    }
}

/*_____________________________________________________________*/
/*_________________________SET_________________________________*/
/*_____________________________________________________________*/

void Servers::parceIp(std::string ip)
{
    std::stringstream ss;
    ss << ip;
    std::string line;
    int net = 0;
    int len = 0;
    while (getline(ss, line, '.'))
    {
        net = std::atoi(line.c_str());
        if (line.size() > 3 || !check_isdigit(line) || net > 255 || net < 0)
        {
            throw "error invalid ip host " + ip + " \n";
        }
        len++;
    }
    if (len != 4)
        throw "error invalid ip host " + ip + " \n";
}
int Servers::checkDup(std::string der, int &index)
{
    int dup = 0;
    size_t i = 0;
    while (i < GetIndex("location"))
    {
        if (servconf[i][0] == der)
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
void Servers::SetPorts()
{
    int i;
    int num = checkDup("listen", i);
    std::string arg;
    if (num == 0)
    {
        port.push_back(0);
        return;
    }
    if (servconf[i].size() != 2)
    {
        throw "invalid port in of the listen directive \n";
    }
    arg = servconf[i][1];
    int myport = std::atoi(arg.c_str());
    if (!check_isdigit(arg) || myport > 65535 || myport <= 1023)
    {
        throw("invalid port in '" + arg + "' of the  directive \n");
    }
    port.push_back(myport);
}

void Servers::SetServerName()
{
    int i;
    int num = checkDup("server_name", i);
    std::string arg;
    if (num == 0)
    {
        server_name.push_back("");
        return;
    }
    if (servconf[i].size() != 2)
    {
        throw "invalid port  directive \n";
    }
    arg = servconf[i][1];

    // check hna laykon chi check
    server_name.push_back(arg);
}

void Servers::SetHost()
{
    int i;
    int num = checkDup("host", i);
    std::string arg;
    if (num == 0)
    {
        host.push_back("");
        
        return;
    }
    if (servconf[i].size() != 2)
    {
        throw "invalid host in  directive \n";
    }
    arg = servconf[i][1];
    if(!isValidIpAddress(arg))
    {
        throw "invalid host ip address \n";
    }
    // parceIp(arg);
    host.push_back(arg);
}

void Servers::SetRoot()
{
    int i;
    int num = checkDup("root", i);
    std::string arg;
    if (num == 0)
    {
        root.push_back("");
        return;
    }
    if (servconf[i].size() != 2)
    {
        throw "invalid root directive \n";
    }
    arg = servconf[i][1];
    if (!pathExists(arg))
    {
        throw("Path '" + arg + "' does not exist.\n");
    }
    root.push_back(arg);
}
void Servers::SetIndex()
{
    int i;
    int num = checkDup("index", i);
    std::string arg;
    if (num == 0)
    {
        index.push_back("");
        return;
    }
    if (servconf[i].size() != 2)
    {
        throw "invalid port in of the server_name directive \n";
    }
    arg = servconf[i][1];
    // std::cout<<"index   :"<<arg<<endl;
    // if (pathIsFile(arg) != 2 )
    // {
    //     throw("Path '" + arg + "' does not exist or is not a file.\n");
    // }
    index.push_back(arg);
}
void Servers::SetClient_max_body_size()
{
    int i;
    int num = checkDup("client_max_body_size", i);
    std::string arg;
    if (num == 0)
    {
        client_max_body_size.push_back(0);
        return;
    }
    if (servconf[i].size() != 2)
    {
        throw "invalid client_max_body_size  directive \n";
    }
    arg = servconf[i][1];
    long long int body_size = std::strtod(arg.c_str(), NULL);
    if (!check_isdigit(arg))
    {
        throw "invalid port in '" + arg + "' of the  directive \n";
    }
    client_max_body_size.push_back(body_size);
}

void Servers::check_Status(std::string status)
{
    std::map<string,string>::iterator iter;

    iter = error_page.find(status);
    if (iter == error_page.end())
    {
        throw "Error : Invalid status '" + status + "' code  Derecties\n";
    }
}

void Servers::SetError_page()
{
    std::string status;
    std::string path;
    for (size_t i = 0; i < servconf.size(); i++)
    {
        if (servconf[i][0] == "error_page")
        {
            if (servconf[i].size() != 3)
            {
                throw "Error in error_page\n";
            }
            status = servconf[i][1];
            path = servconf[i][2];
            check_Status(status);
            if(pathIsFile(path) == 2)
                error_page[status] = path;
            else
            {
                path = root[0] +"/"+path;
                if (pathIsFile(path) != 2)
                {
                    throw "path is not file or nosush file '"+path+"'\n";
                }
                error_page[status] = path;
            }
        }
    }
}

void Servers::SetAllDir()
{
    // ParceServers();
    FillValid();
    FillLocation();
    // checkValidation();

    SetHost();
    SetRoot();
    SetPorts();
    SetIndex();
    SetServerName();
    SetError_page();
    SetClient_max_body_size();

    size_t i = 0;
    while (i < locations.size())
    {
        locations[i].SetIndexRoot(root[0],index[0]);
        locations[i].SetAllDir();
        i++;
    }
}
/*_____________________________________________________________*/
/*_________________________GET_________________________________*/
/*_____________________________________________________________*/

const int & Servers::GetPorts()
{
    return port[0];
}
const std::string &Servers::GetServerName()
{
    return server_name[0];
}
const std::string &Servers::GetHost()
{
    return host[0];
}
const std::string &Servers::GetRoot()
{
    return root[0];
}

const long long int &Servers::GetClient_max_body_size()
{
    return client_max_body_size[0];
}
const std::string &Servers::GetIndex()
{
    return index[0];
}

/*=======================================================================*/
/*=======================================================================*/
size_t Servers::GetIndex(std::string dir)
{
    size_t i;
    for (i = 0; i < servconf.size(); i++)
    {
        if (servconf[i][0] == dir)
        {
            return i;
        }
    }
    return (i);
}
Location Servers::FirstFill(size_t &index)
{
    Location loaction;
    std::vector<std::string>::iterator iter;
    if(servconf[index][0] != "location")
    {
        throw "no location'"+servconf[index][0]+"' \n";
    }
    loaction.vlocation.push_back(servconf[index]);
    index++;
    if(servconf[index][0] != "{" )
    {
        throw "no open bracket for location \n";
    }
    while (index < servconf.size() && servconf[index][0] != "}")
    {
        iter = std::find(Vstrvalid.begin(),Vstrvalid.end(),servconf[index][0]);
        if (iter == Vstrvalid.end())
        {
            throw "Error : '"+servconf[index][0]+"' directive is not allowed here \n";
        }
        loaction.vlocation.push_back(servconf[index]);
        index++;
    }
    if(index < servconf.size())
        index++;

    return loaction;
}

void Servers::FillLocation()
{
    size_t index = GetIndex("location");

    if (index == servconf.size())
    {
        return;
    }
    while (index < servconf.size() && servconf[index][0] != "}")
    {
        locations.push_back(FirstFill(index));
    }
}
/*=======================================================================*/
/*=======================================================================*/

void Servers::FillValid()
{
    Vstrvalid.push_back("root");
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


void Servers::desplay()
{
    // std::vector<std::vector<std::string> > matrix = servconf; 

    cout<<"Ports :"<<GetPorts()<<endl;
    cout<<"ServerName  :"<<GetServerName()<<endl;
    cout<<"Host :"<<GetHost()<<endl;
    cout<<"Root :"<<GetRoot()<<endl;
    map<string,string>::iterator iter = error_page.begin();
    while (iter != error_page.end())
    {
        cout<<"error Page :'"<<iter->first<<"' '"<<iter->second<<"'\n";
        iter++;
    }
    
    cout<<"Client_max_body_size :"<<endl;
    cout<<"Index :"<<GetIndex()<<endl;
    size_t i = 0;
    while (i < locations.size())
    {
        std::cout << "________________________\n";
        locations[i].desplayLocation();
        i++;
    }
}

/*#############################################################*/
/*CREATE SOKCET*/
/*#############################################################*/

void Servers::CreatSocketServer( std::map<int,vector<Servers> > & msockets)
{

    std::map<int,vector<Servers> >::iterator iter = msockets.begin();
    vector<Servers>::iterator ser;
    while (iter != msockets.end())
    {
        ser = find(iter->second.begin(),iter->second.end(),*this);
        if (ser != iter->second.end())
        {
            break;
        }
        iter++;
    }
    if (iter ==  msockets.end())
    {
        // cout<<"this is new server\n";
    }
    else 
    {
        server_fd = iter->second[0].server_fd;
        // cout<<"this is already exist\n";
        return ;
    }
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("connot create socket");
        return;
    }

    int reuseaddr = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1)
    {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(host[0].c_str()); //init_adder(host); 
    address.sin_port = htons(port[0]);
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        return;
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("“In listen”");
        exit(EXIT_FAILURE);
    }
    sercheck = 1;
}
/*#############################################################*/
void Servers::SetIndex_Of(string path)
{
    std::ofstream index_Of;
    std::string line;
    std::vector<std::string> _split;
    std::vector<string>::iterator iterfind;


    struct dirent *entry;
    DIR *dir = opendir(path.c_str());
    if (dir == NULL) {
        perror("Error opening directory");
        return ;
    }

    
    index_Of.open("index_of.html", std::ios::out);
    if (index_Of.is_open())
    {
        index_Of << "<html>" << endl;
        index_Of << "<head>" << endl;
        index_Of << "    <title>Index of /</title>" << endl;
        index_Of << "</head>" << endl;
        index_Of << "<body>" << endl;
        index_Of << "    <h1>Index of /</h1>" << endl;
        index_Of << "    <hr>" << endl;
        index_Of << "    <pre>" << endl;


        while ((entry = readdir(dir)) != NULL) 
        {
            if (std::strcmp(entry->d_name  , "."))
            {
                if (entry->d_type == DT_DIR)
                {
                    index_Of << "    <a href=\""+ string(entry->d_name) +"/\">"+ string(entry->d_name) +" /</a>" <<endl;
                }
                else
                {
                    index_Of << "    <a href=\""+ string(entry->d_name) +"\">"+ string(entry->d_name) +"</a>" <<endl;

                }
            }
        }
        index_Of << "    </pre>" << endl;
        index_Of << "    <hr>" << endl;
        index_Of << "</body>" << endl;
        index_Of << "</html>" << endl;
        index_Of.close();
    }
}
void Servers::SetDefaultError()
{
    error_page["400"] =  "error_pages/400.html";
    error_page["401"] =  "error_pages/401.html";
    error_page["402"] =  "error_pages/402.html";
    error_page["403"] =  "error_pages/403.html";
    error_page["404"] =  "error_pages/404.html";
    error_page["405"] =  "error_pages/405.html";
    error_page["406"] =  "error_pages/406.html";
    error_page["407"] =  "error_pages/407.html";
    error_page["408"] =  "error_pages/408.html";
    error_page["409"] =  "error_pages/409.html";
    error_page["410"] =  "error_pages/410.html";
    error_page["411"] =  "error_pages/411.html";
    error_page["412"] =  "error_pages/412.html";
    error_page["413"] =  "error_pages/413.html";
    error_page["414"] =  "error_pages/414.html";
    error_page["415"] =  "error_pages/415.html";
    error_page["416"] =  "error_pages/416.html";
    error_page["417"] =  "error_pages/417.html";
    error_page["500"] =  "error_pages/500.html";
    error_page["504"] =  "error_pages/504.html";
    error_page["505"] =  "error_pages/505.html";
    
}
bool  Servers::operator== (const Servers& ser)
{
    if (host[0] == ser.host[0] && port[0] == ser.port[0])
    {
        return 1;
    }
    return 0;
}
bool  Servers::operator== (const string & servername)
{
    if (server_name[0] == servername)
    {
        return 1;
    }
    return 0;
}
int Servers::searchPathLocation(string &uri)
{
    string pathL;
    for (size_t i = 0; i < locations.size(); i++)
    {
        pathL =  locations[i].path[0];
        if (pathL[pathL.size() - 1] != '/')
            pathL += "/";
        if (strncmp(uri.c_str(),pathL.c_str(),pathL.length()) == 0  && pathL != "/")
        {
            return i;
        }
    }
    return -1;
}

int  Servers::fillFromLocation(int &in, string &uri,string & method)
{
        rootUri = uri;
        rootUri.replace(0, locations[in].path[0].length(), locations[in].root[0]);
        string & hold = rootUri;
        if (pathIsFile(rootUri) == 3 && method == "GET")
        {
            cout<<rootUri<<endl;
            if (rootUri[rootUri.size() - 1] != '/')
            {
                rootUri = "";
                status = "301 Moved Permanently  \r\nLocation: "+uri+"/";
            }
            else
            {
                rootUri += locations[in].index[0];
                cout<<"from location :\n\n"<<rootUri<<endl<<endl;
                if (!pathExists(rootUri))
                {
                    if (locations[in].permession & AUTOINDEX)
                    {
                        SetIndex_Of(hold);
                        rootUri = "index_of.html";
                    }
                    else
                    {
                        rootUri = error_page["404"];
                        status = "404";
                    }
                    return 0;
                }        
            }
        }
        else if (!pathExists(rootUri))
        {
            rootUri = error_page["404"];
            status = "404";
            return 0;
        }
    return 1;
}
void Servers::SetUriRoot(int i,string & uri)
{
    rootUri = locations[i].root[0] + uri;
    if (pathIsFile(rootUri) == 3)
    {
        if (rootUri[rootUri.size() - 1] != '/')
        {
            rootUri = "";
            status = "301 Moved Permanently  \r\nLocation: "+uri+"/";
        }
        else
        {
            rootUri +=locations[i].index[0];
            if (!pathExists(rootUri))
            {
                if (locations[i].permession & AUTOINDEX)
                {
                    SetIndex_Of(locations[i].root[0] +"/"+uri);
                    rootUri = "index_of.html";
                }
                else
                {
                    rootUri = error_page["404"];
                    status = "404";
                }
            }
        }
    }
}

void Servers::FillQuerys(string & uri)
{
    size_t pos;
    cout<<"URI :"<<uri<<endl;
    int len;
    pos = uri.find("?");
    if (pos != std::string::npos)
    {
        len = uri.length() - pos;
        char q[len + 1];
        if(uri[pos + 1])
        {
            uri.copy(q,len - 1,pos + 1 );
            q[len - 1] = '\0';
            querys = q;
        }
        else
            querys = "";    
        uri.erase(pos,len);
    }
}
void Servers::FillData(string uri,string mehtod)
{
    FillQuerys(uri);
    int in = searchPathLocation(uri);
    Is_cgi = false;
    int def = 0;
    if (in == -1)
    {
        def = getLocation("/");
        if (def != -1)
        {
            rootUri = locations[def].root[0] + uri;
            if (pathIsFile(rootUri) == 3 && mehtod == "GET")
            {
               SetUriRoot(def,uri);
            }
            else if (!pathExists(rootUri))
            {
                rootUri = error_page["404"];
                status = "404";
            }
            UriLocation = locations[def];
        }
        else
        {
            cout<<"root[0]: "<<root.size()<<endl;
            rootUri = root[0] +uri +"/" + index[0];
            UriLocation.path.push_back("");
            UriLocation.root.push_back(root[0]);
            UriLocation.permession = 0;
            UriLocation.index.push_back(index[0]);
        }
    }
    else
    {
        std::cout<<"Path of location :"<<locations[in].path[0]<<endl;
        if (fillFromLocation(in, uri,mehtod) && locations[in].path[0] == "/cgi")
        {
            Is_cgi = true;
        }
        UriLocation = locations[in];
            
    }
    // cout<<"rootUri :"<<rootUri<<endl;
    // cout<<"is_cgi :"<<Is_cgi<<endl;
    // cout<<"querys :"<<querys<<endl;
    // cout<<"      ========\n";
    // cout<<"        ===\n";
    // cout<<"         =\n";
    // UriLocation.desplayLocation();
    // cout<<"         =\n";
    // cout<<"        ===\n";
    // cout<<"      ========\n";
}

Servers::Servers()
{
    // root.push_back("");
    sercheck = 0;
    SetDefaultError();
    status = "200 OK";
    
}

Servers::~Servers()
{
}