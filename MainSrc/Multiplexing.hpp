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

    void Connect_And_Add(int n);
    void In_Events(int n);
    void Out_Events(int n);
    bool CheckTimeOut(int n);
    void CloseClient(int & n);
    int nfds;
    int epollfd;
    int conn_sock;
    struct epoll_event ev;
    struct epoll_event events[MAX_EVENTS];
    clock_t SocketTimeout;
    ParceConf server;
    std::map<int ,Request > mClients;

public:
    Multiplexing(std::string  configfile);
    void CreatMUltiplex();
    ~Multiplexing();
};



#endif

