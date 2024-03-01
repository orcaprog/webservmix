/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouassi <abouassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 14:49:58 by abouassi          #+#    #+#             */
/*   Updated: 2024/02/01 09:26:30 by abouassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELETE_HPP
#define DELETE_HPP

#include <stdio.h>
#include <dirent.h>
#include <iostream>
#include<cstring>
#include "Method.hpp"
#include "../MainSrc/ParceConf.hpp"
class Delete : public Method
{

public:
    bool deleted;
    Delete();
    int  my_remove(std::string file);
    int status;
    void RemoveAllPath(std::string path);
    int pathExists(std::string path);
    int process(string body, int event) ;
    ~Delete();
};
#endif