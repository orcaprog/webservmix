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
    SocketTimeout = 15;   

}

Multiplexing::~Multiplexing()
{
}


void Multiplexing::CloseClient(int & n)
{
    epoll_ctl(epollfd,EPOLL_CTL_DEL,events[n].data.fd,&ev);
    close(events[n].data.fd);
    mClients.erase(events[n].data.fd);
    cout<<"connections with client["<<events[n].data.fd<<"]  is closed from server"<<endl;
}

void Multiplexing::Out_Events(int n)
{
    mClients[events[n].data.fd].process_req(string(""),EPOLLOUT);
    string res = mClients[events[n].data.fd].get_respons();
    ssize_t bytesWritten = write(events[n].data.fd , res.c_str(), res.size());
    
    if (bytesWritten < 0 || mClients[events[n].data.fd].resp_done())
        CloseClient(n); 
}

void Multiplexing::In_Events(int n)
{
    char buffer[1024];
    ssize_t bytesRead = 0;

    bytesRead = read(events[n].data.fd,buffer,1024);
    if (bytesRead <= 0) {
        std::cout<<"Connection closed by client ["<<events[n].data.fd<<"]\n"; 
        CloseClient(n);
    }
    else 
    {
        std::map<int ,Request >::iterator iter2 = mClients.find(events[n].data.fd);
        if (iter2 != mClients.end())
            mClients[events[n].data.fd].process_req(string("").append(buffer, bytesRead),EPOLLIN);
    }
}

void Multiplexing::Connect_And_Add(int n)
{
    int adrlen;
    std::map<int, vector<Servers> >::iterator iter = server.msockets.find(events[n].data.fd );
    if (iter != server.msockets.end()) 
    {   
        std::cout<<"Fd Server :"<<iter->first<<std::endl;
        adrlen = sizeof(iter->second[0].address);
        conn_sock = accept(iter->first, (struct sockaddr *)&iter->second[0].address, (socklen_t*)&adrlen);
        if (conn_sock == -1) 
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        Request req(iter->second);
        mClients[conn_sock] = req;
        mClients[conn_sock].startTime = clock();
        ev.events = EPOLLIN | EPOLLOUT | EPOLLHUP;
        ev.data.fd = conn_sock;

        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,&ev) == -1) 
        {
            perror("epoll_ctl: conn_sock");
            exit(EXIT_FAILURE);
        }
    } 
    else 
    {
        if(events[n].events & EPOLLHUP)
            CloseClient(n);
        else if (events[n].events & EPOLLIN && !(mClients[events[n].data.fd].error & Body_SizeTooLarge)) 
        {
            In_Events(n);
            mClients[events[n].data.fd].startTime = clock();
        }
        else if (events[n].events & EPOLLOUT && mClients.find(events[n].data.fd) !=  mClients.end()) 
        {
            Out_Events(n);
            // mClients[events[n].data.fd].startTime = clock(); 
        }
    }
}

void Multiplexing::CheckTimeOut()
{
    clock_t CurrentTime = clock();
    std::map<int ,Request >::iterator iter  = mClients.begin();
    while (iter != mClients.end())
    {
        // cout<<"serv_time: "<<double(CurrentTime - iter->second.startTime) / CLOCKS_PER_SEC<<endl;
        if(( double(CurrentTime - iter->second.startTime) / CLOCKS_PER_SEC  ) > SocketTimeout)
        {
            std::cout<<CurrentTime<<endl;
            std::cout<<iter->second.startTime<<endl;
            std::cout<<( double(CurrentTime - iter->second.startTime)/ CLOCKS_PER_SEC  )<<endl;
            close(iter->first);
            cout<<"close connection  client TIme out fd :"<<"["<<iter->first<<"]"<<endl;
            mClients.erase(iter->first);
        }
        iter++;
    }
}

void Multiplexing::CreatMUltiplex()
{
    std::map<int,vector<Servers> >::iterator iter;
    epollfd = epoll_create(1);
    if (epollfd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
    for (iter = server.msockets.begin(); iter != server.msockets.end(); iter++)
    {
            ev.events = EPOLLIN;
            ev.data.fd = iter->first;
            if (epoll_ctl(epollfd, EPOLL_CTL_ADD, iter->first, &ev) == -1) 
            {
                perror("epoll_ctl: listen_sock");
                exit(EXIT_FAILURE);
            }
    }
    while (true)
    {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }
        // cout<<"EPOLLIN  :"<<EPOLLIN<<endl;
        // cout<<"EPOLLOUT :"<<EPOLLOUT<<endl;
        for (int n = 0; n < nfds ; ++n) 
        {
            // cout<<"fd  : ["<<events[n].data.fd<<"]  : ";
            // cout<<"event  : ["<<events[n].events<<"]"<<endl;
            Connect_And_Add(n);
        }
        // CheckTimeOut();
    }
}