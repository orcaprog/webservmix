/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouassi <abouassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 09:43:27 by abouassi          #+#    #+#             */
/*   Updated: 2024/02/01 09:28:00 by abouassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Delete.hpp"
int Delete::process(string body, int event) 
{
    (void)body;
    if(deleted) 
    {
        cout<<"Enter in "<<serv.rootUri<<" \n";
        RemoveAllPath(serv.rootUri);
        deleted = 0;
    }
    else if(event == EPOLLOUT)
    {
        Get get;
        cout<<"status :"<<status<<endl;
        if(status == 0)
        {
            cout<<"entere here \n";
            respons = "HTTP/1.1 204\r\n\r\n";
            end = 1;
        }
        else if (status == 1)
        {
            get.serv.status = "403";
            get.get(serv.error_page["403"]);
            respons = get.respons;
            std::cout<<respons<<endl;
            end = 1;
        }
        else
        {
            get.serv.status = "500";
            get.get(serv.error_page["500"]);
            respons = get.respons;
            std::cout<<respons<<endl;
            end = 1;
        }
    }
    return 0;
}

int Delete::pathExists(std::string path) {
    struct stat fileStat;
    return stat(path.c_str(), &fileStat) == 0;
}

Delete::Delete()
{
    cout<<"call constructor \n";
    status = 0;
    deleted = 1;
}

int  Delete::my_remove(std::string file)
{
    if (std::remove(file.c_str()) != 0) 
    {
        perror(("Error removing file  :" + file).c_str());
        return 1;
    } 
    else 
    {
        std::puts(("File removed successfully:" + file).c_str());
        return 0;
    }
}

string PatentOfFile(string & fullPath)
{
    size_t len = fullPath.length();
    len--;
    while (len >= 0 && fullPath[len] != '/')
    {
        len--;
    }
    char buffer[fullPath.size()];
    std::size_t length = fullPath.copy(buffer,len,0);
    buffer[length]='\0';
    return string(buffer);
    
}
void Delete::RemoveAllPath(std::string path)
{
    struct stat stat_info;
    std::string path_plus;
    stat(path.c_str(),&stat_info);
    if (stat_info.st_mode & S_IFREG)
    {
        cout<<PatentOfFile(path);
        stat(PatentOfFile(path).c_str(),&stat_info);
        if (stat_info.st_mode & S_IWOTH)
        {
            my_remove(path);
        }
        else 
        {
            status = 1;
        }
        return ;
    }

    DIR *dir = opendir(path.c_str());
    if (dir == NULL) {
        status = 2;
        perror("Error opening directory");
        cout<<status<<endl;
        return ;
    }
    if (!(stat_info.st_mode & S_IWOTH))
    {
        status = 1;
        return ;
    }
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR)
        {
            if (std::strcmp(entry->d_name  , ".") && std::strcmp(entry->d_name ,"..") )
            {
                path_plus = path + "/"+entry->d_name;
                RemoveAllPath(path_plus);
                // std::cout<<"dir :"<<entry->d_name<<std::endl;
            }
        }
        else
        {
            std::string filePath = std::string(path) + "/" + entry->d_name;
            // std::cout<<"file :"<<filePath<<std::endl;
             stat(path_plus.c_str(),&stat_info);
             my_remove(filePath );

        }
    }
    closedir(dir);
    my_remove(path);
}


Delete::~Delete()
{
}