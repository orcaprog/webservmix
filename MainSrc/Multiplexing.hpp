/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplexing.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouassi <abouassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 18:04:47 by abouassi          #+#    #+#             */
/*   Updated: 2024/01/24 18:43:43 by abouassi         ###   ########.fr       */
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
    std::map<int ,std::pair<Servers,Request> > mClients;
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
};



#endif

