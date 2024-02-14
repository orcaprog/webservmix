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
int Delete::process(string body, size_t body_size)
{
    RemoveAllPath(serv.rootUri);
    
}

int Delete::pathExists(std::string path) {
    struct stat fileStat;
    return stat(path.c_str(), &fileStat) == 0;
}

Delete::Delete()
{

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
    string parentPath= "";
    len--;
    while (len >= 0 && fullPath[len] != '/')
    {
        len--;
    }
    char buffer[++len];
    std::size_t length = fullPath.copy(buffer,len,0);
    buffer[length]='\0';
    return string(buffer);
    
}
void Delete::RemoveAllPath(std::string path)
{

    int check_rm = 0;
    struct stat stat_info;
    std::string path_plus;
    std::cout<<"path   :"<<path<<std::endl;
    stat(path.c_str(),&stat_info);
    std::cout<<stat_info.st_mode<<endl;
    std::cout<<S_IFREG<<endl;
    if (stat_info.st_mode & S_IFREG)
    {
        cout<<PatentOfFile(path);
        stat(PatentOfFile(path).c_str(),&stat_info);
        if (stat_info.st_mode & S_IWOTH)
        {
             my_remove(path);
        }
        return ; 
    }
    

    DIR *dir = opendir(path.c_str());
    if (dir == NULL) {
        perror("Error opening directory");
        return ;
    }
    if (!(stat_info.st_mode & S_IWOTH))
    {
        cout<<"Forbidan \n";
        return;
    }
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR)
        {

            if (std::strcmp(entry->d_name  , ".") && std::strcmp(entry->d_name ,"..") )
            {
                path_plus = path + "/"+entry->d_name;
                 RemoveAllPath(path_plus);
                std::cout<<"dir :"<<entry->d_name<<std::endl;
            }
        }
        else
        {
            std::string filePath = std::string(path) + "/" + entry->d_name;
            std::cout<<"file :"<<filePath<<std::endl;
             stat(path_plus.c_str(),&stat_info);
            check_rm += my_remove(filePath );

        }
    }
    closedir(dir);
    my_remove(path);
}


Delete::~Delete()
{
}