/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouassi <abouassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 18:23:05 by abouassi          #+#    #+#             */
/*   Updated: 2024/01/27 09:14:01 by abouassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP


#include <algorithm> 
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include <map>
#include <sys/stat.h>

enum PERMESSION {
    GET=1,
    POST=2,
    DELETE=4,
    UPLOAD=8,
    AUTOINDEX=16
    };
using namespace std;
class Location
{
private:
        void CheckMethods(std::string methd);
        std::vector<std::string>  AddcgiPaths(std::string status,std::string path);
        size_t GetIndex(std::string dir);
        int  checkDup(std::string der,int & index);

        int pathIsFile(std::string path);
    
        void FillValid();
        void checkValidation();
        std::vector<std::string> Vstrvalid;
        void SetIndex();
        void SetPath();
        void SetAllowMethods();
        void SetAutoindex();
        void SetUpload();
        void SetCgiPath();
        void SetRoot();
        void Printtwodom(const std::vector<std::vector<std::string> > & matrix,std::string data);
        int pathExists(std::string path);
public:
        std::vector<std::string> path;
        std::vector<std::string> root;
        std::vector<std::string> index;
        map<string ,string> cgi_path;
        bool get;
        bool post;
        bool dele;
        

        int permession;
        /*________________Cgi___________________*/
        
        void desplayLocation();
        void SetAllDir();
        std::string & GetPath();
        std::string & GetRoot();
        std::string & GetindexLocation();
        void SetIndexRoot(string root,string index);
        string ServRoot;
        string Servindex;
        
        std::vector<std::vector<std::string> > vlocation;

        Location();
        ~Location();
};





#endif