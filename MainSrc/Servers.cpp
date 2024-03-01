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
bool isValidIpAddress(string &ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr));
    return result <= 0;
}
int Servers::getLocation(std::string path)
{
    size_t i;
    for (i = 0; i < locations.size(); i++)
    {
        if (locations[i].path == path)
            return i;
    }
    return -1;
}

int Servers::pathIsFile(std::string path)
{
    struct stat fileStat;
    if (stat(path.c_str(), &fileStat) == 0)
    {
        if (fileStat.st_mode & S_IFREG)
            return 2;
        else if (fileStat.st_mode & S_IFDIR)
            return 3;
        else
            return 4;
    }
    else
        return 0;
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
/*_____________________________________________________________*/
/*_________________________SET_________________________________*/
/*_____________________________________________________________*/

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
            throw "Error duplicate derective \n";
        i++;
    }
    return (dup);
}
void Servers::SetPorts()
{
    int i;
    std::string arg;

    int num = checkDup("listen", i);
    if (num == 0)
    {
        throw "Error: Port information is missing. Please enter a valid port number \n";
        return;
    }
    if (servconf[i].size() != 2)
        throw "Invalid number of arguments in 'listen' directive \n";

    arg = servconf[i][1];
    int myport = std::atoi(arg.c_str());
    if (arg.size() > 5 ||!check_isdigit(arg) || myport > 65535)
        throw("invalid port in '" + arg + "' of the  directive \n");

    port = myport;
}

void Servers::SetServerName(vector<string> &ser_names)
{
    int i;
    std::string arg;
    vector<string>::iterator iter;

    int num = checkDup("server_name", i);
    if (num == 0)
        return;
    if (servconf[i].size()  < 2)
        throw "Invalid number of arguments in 'server_name' directive \n";
    arg = servconf[i][1];
    if (find(ser_names.begin(), ser_names.end(), arg) != ser_names.end())
        throw "conflicting server name '" + arg + "' \n";
    server_name.clear();
    iter = servconf[i].begin();
    iter++;
    while (iter != servconf[i].end())
    {
        server_name.push_back(*iter);
        iter++;
    }
}

void Servers::SetHost()
{
    int i;
    std::string arg;

    int num = checkDup("host", i);
    if (num == 0)
    {
        throw "Error: Host not specified. Please enter a valid host.\n";
        return;
    }
    if (servconf[i].size() != 2)
        throw "Invalid number of arguments in 'host' directive \n";
    arg = servconf[i][1];
    if (isValidIpAddress(arg))
        throw "invalid host ip address \n";
    host = arg;
}

void Servers::SetRoot()
{
    int i;
    std::string arg;
    char resolvedPath[PATH_MAX];

    int num = checkDup("root", i);
    if (num == 0)
    {
        throw "Error: Root directory not specified. Please provide a valid root directory.\n";
        return;
    }
    if (servconf[i].size() != 2)
        throw "Invalid number of arguments in 'root' directive \n";
    arg = servconf[i][1];
    if (pathIsFile(arg) != 3)
        throw("Root path :'" + arg + "' does not exist or is not a directory.\n");
    realpath(arg.c_str(), resolvedPath);
    root = resolvedPath;
}
void Servers::SetIndex()
{
    int i;
    vector<string>::iterator iter;
    int num = checkDup("index", i);
    if (num == 0)
    {
        throw "Error: Index not provided. Please enter a valid index.\n";
        return;
    }
    if (servconf[i].size() < 2)
        throw "Invalid number of arguments in 'index' directive \n";
    index.clear();
    iter = servconf[i].begin();
    iter++;
    while (iter != servconf[i].end())
    {
        index.push_back(*iter);
        iter++;
    }
}
void Servers::SetClient_max_body_size()
{
    int i;
    long long int body_size;
    std::string arg;

    int num = checkDup("client_max_body_size", i);
    if (num == 0)
        return;
    if (servconf[i].size() != 2)
        throw "Invalid number of arguments in 'client_max_body_size' directive \n";
    arg = servconf[i][1];
    body_size = std::strtod(arg.c_str(), NULL);
    if (arg.size() > 20 || !check_isdigit(arg))
        throw "invalid  '" + arg + "' in client_max_body_size  directive \n";
    client_max_body_size = body_size;
}

void Servers::check_Status(std::string status)
{
    std::map<string, string>::iterator iter;
    iter = error_page.find(status);
    if (iter == error_page.end())
        throw "Error : Invalid status '" + status + "' code  Derecties\n";
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
                throw "Invalid number of arguments in 'error_page' directive \n";
            status = servconf[i][1];
            path = servconf[i][2];
            check_Status(status);
            if (pathIsFile(path) == 2)
                error_page[status] = path;
            else
            {
                path = root + "/" + path;
                if (pathIsFile(path) != 2)
                    throw "Invalid arguments  '" + path + "' in 'error_page'\n";
                error_page[status] = path;
            }
        }
    }
}

void Servers::SetAllDir(vector<string> &ser_names)
{
    FillValid();
    SetHost();
    SetRoot();
    SetPorts();
    SetIndex();
    SetServerName(ser_names);
    SetError_page();
    SetClient_max_body_size();
    FillLocation();
}

/*_____________________________________________________________*/
/*_________________________GET_________________________________*/
/*_____________________________________________________________*/

/*=======================================================================*/
/*=======================================================================*/
size_t Servers::GetIndex(std::string dir)
{
    size_t i;
    for (i = 0; i < servconf.size(); i++)
    {
        if (servconf[i][0] == dir)
            return i;
    }
    return (i);
}
Location Servers::FirstFill(size_t &i)
{
    Location loaction;
    std::vector<std::string>::iterator iter;
    if (servconf[i][0] != "location")
        throw "no location'" + servconf[i][0] + "' \n";
    loaction.vlocation.push_back(servconf[i]);
    i++;
    if (servconf[i][0] != "{")
        throw "no open bracket for location \n";
    while (i < servconf.size() && servconf[i][0] != "}")
    {
        iter = std::find(Vstrvalid.begin(), Vstrvalid.end(), servconf[i][0]);
        if (iter == Vstrvalid.end())
            throw "Error : '" + servconf[i][0] + "' directive is not allowed here \n";
        loaction.vlocation.push_back(servconf[i]);
        i++;
    }
    if (i < servconf.size())
        i++;
    loaction.SetIndexRoot(root, index);
    loaction.SetAllDir();
    return loaction;
}

void Servers::FillLocation()
{
    size_t ind = GetIndex("location");
    while (ind < servconf.size() && servconf[ind][0] != "}")
        locations.push_back(FirstFill(ind));
    if (getLocation("/") == -1)
    {
        Location loc;
        loc.path = "/";
        loc.root = root;
        loc.index = index;
        loc.permession = 23;
        locations.push_back(loc);
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
    Vstrvalid.push_back("return");
    Vstrvalid.push_back("upload_path");
    Vstrvalid.push_back("allow_methods");
    Vstrvalid.push_back("autoindex");
    Vstrvalid.push_back("upload");
    Vstrvalid.push_back("cgi_path");
}

// void Servers::desplay()
// {
//     // std::vector<std::vector<std::string> > matrix = servconf;

//     cout << "Ports :"<< port  << endl;
//     cout << "ServerName  :" << server_name[0] << endl;
//     cout << "Host :" << host  << endl;
//     cout << "Root :" << root << endl;
//     map<string, string>::iterator iter = error_page.begin();
//     while (iter != error_page.end())
//     {
//         cout << "error Page :'" << iter->first << "' '" << iter->second << "'\n";
//         iter++;
//     }
//     cout << "Client_max_body_size :"<<client_max_body_size  << endl;
//     cout << "Index :" << index[0] << endl;
//     size_t i = 0;
//     while (i < locations.size())
//     {
//         std::cout << "________________________\n";
//         locations[i].desplayLocation();
//         i++;
//     }
// }

/*#############################################################*/
/*CREATE SOKCET*/
/*#############################################################*/

void Servers::CreatSocketServer(std::map<int, vector<Servers> > &msockets)
{

    std::map<int, vector<Servers> >::iterator iter = msockets.begin();
    vector<Servers>::iterator ser;
    while (iter != msockets.end())
    {
        ser = find(iter->second.begin(), iter->second.end(), *this);
        if (ser != iter->second.end())
            break;
        iter++;
    }
    if (iter != msockets.end())
    {
        server_fd = ser->server_fd;
        return;
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
        server_fd = -1;
        return;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(host.c_str());
    address.sin_port = htons(port);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        close(server_fd);
        server_fd = -1;
        return;
    }
    if (listen(server_fd, SOMAXCONN) < 0)
    {
        perror("“In listen”");
        close(server_fd);
        server_fd = -1;
        return;
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
    if (dir == NULL)
    {
        perror("Error opening directory");
        return;
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
            if (std::strcmp(entry->d_name, "."))
            {
                if (entry->d_type == DT_DIR)
                    index_Of << "    <a href=\"" + string(entry->d_name) + "/\">" + string(entry->d_name) + " /</a>" << endl;
                else
                    index_Of << "    <a href=\"" + string(entry->d_name) + "\">" + string(entry->d_name) + "</a>" << endl;
            }
        }
        index_Of << "    </pre>" << endl;
        index_Of << "    <hr>" << endl;
        index_Of << "</body>" << endl;
        index_Of << "</html>" << endl;
        index_Of.close();
    }
    closedir(dir);
}

void Servers::SetDefaultError()
{
    error_page["400"] = "error_pages/400.html";
    error_page["403"] = "error_pages/403.html";
    error_page["404"] = "error_pages/404.html";
    error_page["405"] = "error_pages/405.html";
    error_page["408"] = "error_pages/408.html";
    error_page["411"] = "error_pages/411.html";
    error_page["413"] = "error_pages/413.html";
    error_page["414"] = "error_pages/414.html";
    error_page["415"] = "error_pages/415.html";
    error_page["500"] = "error_pages/500.html";
    error_page["501"] = "error_pages/501.html";
    error_page["504"] = "error_pages/504.html";
    error_page["505"] = "error_pages/505.html";
}

bool Servers::operator==(const Servers &ser)
{
    if (host == ser.host && port == ser.port)
        return 1;
    return 0;
}
bool Servers::operator==(const string &servername)
{
    if (find(server_name.begin(),server_name.end(),servername) != server_name.end())
        return 1;
    return 0;
}
int Servers::searchPathLocation(string uri)
{
    string pathL;
    for (size_t i = 0; i < locations.size(); i++)
    {
        pathL = locations[i].path;
        if (pathL[pathL.size() - 1] != '/')
            pathL += "/";
        if (uri[uri.size() - 1] != '/')
            uri += "/";
        if (strncmp(uri.c_str(), pathL.c_str(), pathL.length()) == 0 && pathL != "/")
            return i;
    }
    return -1;
}

int Servers::JoinIndexRoot(int &in)
{
    vector<string>::iterator iter = locations[in].index.begin();
    string hold = rootUri;

    while (iter != locations[in].index.end())
    {
        hold += *iter;
        if (pathIsFile(hold) == 2)
        {
            rootUri = hold;
            return 1;
        }
        hold = rootUri;
        iter++;
    }
    return 0;
}
int Servers::fillFromLocation(int &in, string &uri, string &method)
{
    rootUri = uri;
    rootUri.replace(0, locations[in].path.length(), locations[in].root);
    string hold = rootUri;
    if (pathIsFile(rootUri) == 3)
    {
        if (locations[in].permession & REDIR)
        {
            SetRederectionResp(locations[in].redirect);
            return 0;
        }
        if (rootUri[rootUri.size() - 1] != '/')
        {
            rootUri = "";
            status = "301 Moved Permanently  \r\nLocation: " + uri + "/";
            return 0;
        }
        else if (MatchingWithRoot(rootUri, locations[in].root))
        {
            rootUri = error_page["403"];
            status = "403";
            return 0;
        }
        else if (method == "GET" || method == "POST")
        {
            if (!JoinIndexRoot(in))
            {
                if (locations[in].permession & REDIR)
                    SetRederectionResp(locations[in].redirect);
                else if (locations[in].permession & AUTOINDEX )
                {
                    SetIndex_Of(hold);
                    rootUri = "index_of.html";
                }
                else
                {
                    rootUri = error_page["403"];
                    status = "403";
                }
                return 0;
            }
        }
    }
    else if (!pathIsFile(rootUri))
    {
        if (locations[in].permession & REDIR)
            SetRederectionResp(locations[in].redirect);
        else
        {
            rootUri = error_page["404"];
            status = "404";
        }
        return 0;
    }
    return 1;
}
void Servers::SetUriRoot(int i, string &uri)
{
    if (rootUri[rootUri.size() - 1] != '/')
    {
        rootUri = "";
        status = "301 Moved Permanently  \r\nLocation: " + uri + "/";
    }
    else
    {
        if (!JoinIndexRoot(i))
        {
            if (locations[i].permession & AUTOINDEX)
            {
                SetIndex_Of(locations[i].root + "/" + uri);
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

void Servers::FillQuerys(string &uri)
{
    size_t pos;
    int len;
    pos = uri.find("?");
    if (pos != std::string::npos)
    {
        len = uri.length() - pos;
        char q[len + 1];
        if (uri[pos + 1])
        {
            uri.copy(q, len - 1, pos + 1);
            q[len - 1] = '\0';
            querys = q;
        }
        else
            querys = "";
        uri.erase(pos, len);
    }
}
bool Servers::MatchingWithRoot(string &rootPlusUri, string &rootPath)
{
    char resolvedPath[PATH_MAX];
    size_t pos;
    realpath(rootPlusUri.c_str(), resolvedPath);
    string hold = resolvedPath;
    cout << hold << endl;
    cout << rootPlusUri << endl;
    hold += "/";
    pos = hold.find(rootPath);
    if (pos != string::npos && pos == 0)
        return 0;
    return 1;
}
void Servers::SetRederectionResp(vector<string> &redirect)
{
    rootUri = "";
    status = "301 \r\nLocation: " + redirect[1] + "/";
}
void Servers::FillData(string uri, string mehtod)
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
            if (locations[def].permession & REDIR)
            {
                SetRederectionResp(locations[def].redirect);
                return ;
            }
            rootUri = locations[def].root + uri;
            if (pathIsFile(rootUri) == 3)
            {
                if (MatchingWithRoot(rootUri, locations[def].root))
                {
                    rootUri = error_page["403"];
                    status = "403";
                }
                else if (mehtod == "GET" || mehtod == "POST")
                    SetUriRoot(def, uri);
            }
            else if (!pathIsFile(rootUri))
            {
                rootUri = error_page["404"];
                status = "404";
            }
            UriLocation = locations[def];
        }
    }
    else
    {
        if (fillFromLocation(in, uri, mehtod) && (locations[in].path == "/cgi" || locations[in].path == "/cgi/"))
            Is_cgi = true;
        UriLocation = locations[in];
    }
    // cout << "rootUri :" << rootUri << endl;
    // cout << "is_cgi :" << Is_cgi << endl;
    // cout << "querys :" << querys << endl;
    // cout << "      ========\n";
    // cout << "        ===\n";
    // cout << "         =\n";
    // UriLocation.desplayLocation();
    // cout << "         =\n";
    // cout << "        ===\n";
    // cout << "      ========\n";
}

Servers::Servers()
{
    char resolvedPath[PATH_MAX];
    realpath("./html", resolvedPath);
    root = resolvedPath;
    client_max_body_size = 100000;
    host = "0.0.0.0";
    index.push_back("index_webserv.html");
    port = 8080;
    server_name.push_back("");
    sercheck = 0;
    SetDefaultError();
    status = "200 OK";
}

Servers::~Servers()
{
}