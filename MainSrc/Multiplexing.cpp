/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplexing.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouassi <abouassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 18:04:44 by abouassi          #+#    #+#             */
/*   Updated: 2024/01/29 16:30:35 by abouassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Multiplexing.hpp"

Multiplexing::Multiplexing(std::string  configfile)
{
    server.TakeAndParce(configfile);
    

}

Multiplexing::~Multiplexing()
{
}



void Multiplexing::Out_Events(int n)
{
       
    mClients[events[n].data.fd].second.process_req(string(""),0,EPOLLOUT);

    string res = mClients[events[n].data.fd].second.get_respons();

    write(events[n].data.fd , res.c_str(), res.size());
    if (mClients[events[n].data.fd].second.resp_done())
    {
        epoll_ctl(epollfd,EPOLL_CTL_DEL,events[n].data.fd,&ev);
        close(events[n].data.fd);
        mClients.erase(events[n].data.fd);
        cout<<"close connections :"<<events[n].data.fd<<endl;
    }
    
}

void Multiplexing::In_Events(int n)
{
    char buffer[1024];
    ssize_t bytesRead = 0;
    bytesRead = read(events[n].data.fd,buffer,1024);
    
    if (bytesRead == -1)
    {
        perror("Error read");
        return ;
    }
    if (bytesRead == 0) 
    {
        close(events[n].data.fd);
        std::cout<<"Connection closed by client\n"; 
    }
    else 
    {
        std::map<int ,std::pair<Servers,Request> >::iterator iter2 = mClients.find(events[n].data.fd);
        if (iter2 != mClients.end())
            mClients[events[n].data.fd].second.process_req(string("").append(buffer, bytesRead),bytesRead,EPOLLIN);
    }
}

void Multiplexing::Connect_And_Add(int n)
{
    int adrlen;
    std::map<int, Servers>::iterator iter = server.msockets.find(events[n].data.fd );
    if (iter != server.msockets.end()) 
    {   
        adrlen = sizeof(iter->second.address);
        conn_sock = accept(iter->first, (struct sockaddr *)&iter->second.address, (socklen_t*)&adrlen);
        if (conn_sock == -1) 
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        mClients[conn_sock].first = iter->second;
        Request req(mClients[conn_sock].first);
        mClients[conn_sock].second = req;
        std::cout<<"Fd Server :"<<iter->first<<std::endl;
        ev.events = EPOLLIN | EPOLLOUT;
        ev.data.fd = conn_sock;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,&ev) == -1){
            perror("epoll_ctl: conn_sock");
            exit(EXIT_FAILURE);
        }
    }
    else 
    {
        if (events[n].events & EPOLLIN)
            In_Events(n);
        else if (events[n].events & EPOLLOUT
            && mClients.find(events[n].data.fd) !=  mClients.end()){
            Out_Events(n);
        }
    }
}


void Multiplexing::CreatMUltiplex()
{
    std::ifstream inputFile;
    inputFile.open("index.html");
    std::string line;
    std::string data= "";
    while (std::getline(inputFile, line)) {
        data += line;
    }
    // std::string headers = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 1000000\r\n\r\n";
    // std::string httprespose = headers + data;
    // const char *hello = httprespose.c_str();

    epollfd = epoll_create(1);
    if (epollfd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
    std::map<int,Servers>::iterator iter;

    for (iter = server.msockets.begin(); iter != server.msockets.end(); iter++)
    {
        ev.events = EPOLLIN;
        ev.data.fd = iter->first;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, iter->first, &ev) == -1) {
            perror("epoll_ctl: listen_sock");
            exit(EXIT_FAILURE);
        }
    }
    while (true)
    {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        // std::cout<<"nfds   :"<<nfds<<std::endl;
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for (int n = 0; n < nfds ; ++n) 
        {
            Connect_And_Add(n);
        }
    }
}