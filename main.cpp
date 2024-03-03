/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouassi <abouassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 17:36:57 by abouassi          #+#    #+#             */
/*   Updated: 2024/01/24 18:42:50 by abouassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MainSrc/ParceConf.hpp"
#include "MainSrc/Multiplexing.hpp"
#include <exception>

string decooding_uri(string uri){
    string new_uri;
    for (size_t i = 0; i < uri.size(); i++){
        if (uri[i] == '%' && i+2 < uri.size()){
            stringstream hexa;
            int h;
            hexa<<uri[i+1];
            hexa<<uri[i+2];
            hexa>>hex>>h;
            new_uri.push_back(h);
            i += 2;
        }
        else
            new_uri.push_back(uri[i]);
    }
    return new_uri;
}

int main(int ac , char **av)
{
    (void)ac;
    string uri = decooding_uri("file%20%51%o1%");
    cout<<"new_uri: "<<uri<<endl;
    // string conf;
    // try
    // {
    //     if(ac != 2)
    //         conf = "";
    //     else 
    //         conf = av[1];
    //     Multiplexing mult(conf);
    //     mult.CreatMUltiplex();
    
    // }
    // catch(std::string  e)
    // {
    //     std::cerr << e<< '\n';
    // }
    // catch(const char * e)
    // {
    //     std::cerr << e<< '\n';
    // }
    // catch(exception & ex)
    // {
    //     cout<<ex.what()<<endl;
    //     cout<<string::npos<<endl;
    // }
    
}