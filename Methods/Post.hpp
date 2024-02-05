/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onaciri <onaciri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 09:09:48 by onaciri           #+#    #+#             */
/*   Updated: 2024/02/05 18:58:29 by onaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POST_HPP
#define POST_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <map>
#include <vector>
#include <fstream>
#include <bits/stdc++.h> 
#include "Method.hpp"
#include <iostream>
#include <ctime>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

class Post : public Method
{
    private:
        /*function*/
		void mimeType();
    private:
        /* data */
		int crfile;
		int MethodType;
        int special;
        int rare;
        size_t left_over;
        int chunk_ctl;
		size_t size_len;
        size_t total_Body;
        size_t chunk_left;
        int find_sep;
        int file_hang;
        int is_first;
        std::string buffer;
        std::string buff_chunk;
		std::map<std::string, std::string> mime;
		std::fstream outFile;
        std::fstream out;
        std::string sep;
        std::string sep_end;
        std::string sep_found;
    public:
        Post();
        Post(const Post& post);
        Post& operator=(const Post& post);
        ~Post();
        
        int process(std::string body, size_t body_size);
		void openFile(std::string body, size_t body_size);
		void normalFile(std::string body, size_t body_size);
        
        void chunked_file(std::string body, size_t body_size);
        void chunk_write(std::string body, size_t body_size);

        void    ft_boundary(std::string& body);

};


#endif