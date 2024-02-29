/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplexing.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onaciri <onaciri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 18:04:47 by abouassi          #+#    #+#             */
/*   Updated: 2024/02/29 09:05:45 by onaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MULIPLEXING_HPP
#define MULIPLEXING_HPP
#include "ParceConf.hpp"
class Multiplexing
{
private:

public:
    Multiplexing(std::string  configfile);
    ~Multiplexing();
    ParceConf server;
    std::map<int ,Request > mClients;
    clock_t SocketTimeout;
    struct epoll_event ev;
    struct epoll_event events[MAX_EVENTS];
    int  conn_sock;
    int nfds;
    int  epollfd;
    void CreatMUltiplex();
    void Connect_And_Add(int n);
    void In_Events(int n);
    void Out_Events(int n);
    void CheckTimeOut();
    int setnonblocking(int sockfd) ;
    void CloseClient(int & n);
};



#endif