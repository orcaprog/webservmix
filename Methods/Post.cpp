/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onaciri <onaciri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 10:40:02 by onaciri           #+#    #+#             */
/*   Updated: 2024/02/15 15:39:42 by onaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Post.hpp"

Post::Post()
{
	crfile = 0;
    enter_cgi = 0;
    chunk_ctl = 0;
    total_Body = 0;
    size_len = 0;
    left_over = 0;
    chunk_left = 0;
    rare = 0;
    special = 0;
    end = 0;
    MethodType = 0;
    find_sep = 0;
    is_first = 0;
    sep = "";
	sep_found = "";
    ran_file = "";
    the_file = "";
    file_hang = 0;
    here_is = 0;
    first_time = 0;
    first_run = 0;
    cgi_exe = 0;
    error = 0;
    mimeType();
}


Post::Post(const Post& post)
{
    *this = post;
}

Post& Post::operator=(const Post& post)
{
    (void)post;
    return *this;
}

Post::~Post()
{

}


/*work*/

std::string Post::creat_file_name(int ret)
{
    
    std::time_t currentTime = std::time(NULL);
    // Convert time to struct tm in local time
    std::tm* timeInfo = std::localtime(&currentTime);
    // Format the time into a string
    char time_B[80];  // Sufficiently large time_B
    std::strftime(time_B, sizeof(time_B), "%Y-%m-%d_%H-%M-%S", timeInfo);
    // Get the resulting string
    std::string currentTimeString = time_B;
    std::string root;
    if (ret)
        return currentTimeString;
    if (serv.Is_cgi)
    {
        
        size_t i;
        if (!fullUri_path.size())
        {
            std::cout << "extention Problem\n";
            return std::string("");       
        }
        for (i = fullUri_path.size() - 1; i >= 0 && fullUri_path[i]!= '.'; i--);
        
        if (fullUri_path[i] == '.')
            for (i -= 1 ; i >= 0 && fullUri_path[i]!= '/'; i--);
        if (!i)
            i = fullUri_path.size();
        root = fullUri_path.substr(0, i);
    }
    else
        root = serv.rootUri;
    return  root + "/"+ currentTimeString;
}
void Post::mimeType()
{
   std::string str =
    "  text/html html\n"
    "text/css   css\n"
    "text/xml  xml\n"
    "image/gif gif\n"
    "image/jpeg jpeg\n"
    "application/javascript   js\n"
    "application/atom+xml  atom\n"
    "application/rss+xml  rss\n"
    "text/mathml    mml\n"
    "text/plain txt\n"
    "text/vnd.sun.j2me.app-descriptor    jad\n"
    "text/vnd.wap.wml  wml\n"
    "text/x-component htc\n"
    "image/avif avif\n"
    "image/png  png\n"
    "image/svg+xml  svg\n"
    "image/tiff  tiff\n"
    "image/vnd.wap.wbmp wbmp\n"
    "image/webp webp\n"
    "image/x-icon ico\n"
    "image/x-jng  jng\n"
    "image/x-ms-bmp  bmp\n"
    "font/woff   woff\n"
    "font/woff2  woff2\n"
    "application/java-archive jar\n"
    "application/json json\n"
    "application/mac-binhex40  hqx\n"
    "application/msword   doc\n"
    "application/pdf   pdf\n"
    "application/postscript                           ps\n"
    "application/rtf                                  rtf\n"
    "application/vnd.apple.mpegurl                    m3u8\n"
    "application/vnd.google-earth.kml+xml             kml\n"
    "application/vnd.google-earth.kmz                 kmz\n"
    "application/vnd.ms-excel                         xls\n"
    "application/vnd.ms-fontobject                    eot\n"
    "application/vnd.ms-powerpoint                    ppt\n"
    "application/vnd.oasis.opendocument.graphics      odg\n"
    "application/vnd.oasis.opendocument.presentation  odp\n"
    "application/vnd.oasis.opendocument.spreadsheet   ods\n"
    "application/vnd.oasis.opendocument.text          odt\n"
    "application/vnd.openxmlformats-officedocument.presentationml.presentation pptx\n"
    "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet xlsx\n"
    "application/vnd.openxmlformats-officedocument.wordprocessingml.document docx\n"
    "application/vnd.wap.wmlc  wmlc\n"
    "application/wasm   wasm\n"
    "application/x-7z-compressed  7z\n"
    "application/x-cocoa cco\n"
    "application/x-java-archive-diff  jardiff\n"
    "application/x-java-jnlp-file jnlp\n"
    "application/x-makeself   run\n"
    "application/x-perl   pl\n"
    "application/x-pilot  pdb\n"
    "application/x-rar-compressed  rar\n"
    "application/x-redhat-package-manager    rpm\n"
    "application/x-sea     sea\n"
    "application/x-shockwave-flash     swf\n"
    "application/x-stuffit  sit\n"
    "application/x-tcl    tcl\n"
    "application/x-x509-ca-cert   crt\n"
    "application/x-xpinstall                          xpi\n"
    "application/xhtml+xml                            xhtml\n"
    "application/xspf+xml                             xspf\n"
    "application/zip                                  zip\n"
    "application/octet-stream bin\n"
    "audio/midi  midi\n"
    "audio/mpeg mp3\n"
    "audio/ogg ogg\n"
    "audio/x-m4a       m4a\n"
    "audio/x-realaudio    ra\n"
    "video/3gpp  3gpp\n"
    "video/mp2t   ts\n"
    "video/mp4    mp4\n"
    "video/mpeg   mpeg\n"
    "video/quicktime   mov\n"
    "video/webm   webm\n"
    "video/x-flv  flv\n"
    "video/x-m4v  m4v\n"
    "video/x-mng  mng\n"
    "video/x-ms-asf     asf\n"
    "video/x-ms-wmv    wmv\n"
    "video/x-msvideo   avi\n"
    "text/x-c c";
    std::istringstream input(str);
    std::string line;
    str.clear();
    while (input >> line >> str)
        mime[line] = str;
    
}

void Post::openFile(std::string body, size_t body_size)
{
	std::string mimeVal;

	if (headers.find("Transfer-Encoding") != headers.end() && headers.find("Content-Type") != headers.end())
    {
        std::string tmp_C = (headers.find("Content-Type"))->second;
        if ((headers.find("Transfer-Encoding"))->second == "chunked"&& tmp_C.find("boundary=") == std::string::npos)
		{
			MethodType = 1;
		}
    }
    else if (!MethodType && headers.find("Content-Type") != headers.end() && !serv.Is_cgi)
    {
        std::cout << "chose onme \n" << "serv is "<< serv.Is_cgi<<std::endl;
        // exit(1);
        std::string tmp_C = (headers.find("Content-Type"))->second;
		if (tmp_C.find("boundary=") != std::string::npos)
		{
			buffer = tmp_C.substr(tmp_C.find("boundary=") + strlen("boundary=") , tmp_C.size() - tmp_C.find("boundary=") - strlen("boundary="));
			MethodType = 3;
			sep = "--";
			sep.append(buffer,0, buffer.size());
            sep_end = sep + "--";
			buffer = "";
			crfile = 1;
			ft_boundary(body);
			return ;
		}
    }
    else if (!MethodType && headers.find("Content-Type") != headers.end() && serv.Is_cgi)
    {
        std::string tmp_C = (headers.find("Content-Type"))->second;
		if (tmp_C.find("boundary=") != std::string::npos)
		{
			buffer = tmp_C.substr(tmp_C.find("boundary=") + strlen("boundary=") , tmp_C.size() - tmp_C.find("boundary=") - strlen("boundary="));
			MethodType = 4;
			sep = "--";
			sep.append(buffer,0, buffer.size());
            sep_end = sep + "--";
			buffer = "";
			crfile = 1;
            ft_boundary_cgi(body);
			return ;
		}
    }
    if (!MethodType)
    {
        MethodType = 2;

    }
	if (MethodType != 3 && headers.find("Content-Type") != headers.end())
    {
        if (mime.find( headers.find("Content-Type")->second) != mime.end())
		{
			mimeVal = mime.find((headers.find("Content-Type")->second))->second;
		}
		else
		{
			mimeVal = "x";
		}
    }
    else if (MethodType != 3)
    {
        std::cout << "No Content Type\n";
		crfile = -2;
        end = 1;
        error = 3;
        return ;
    }
	if (MethodType != 3)
	{
        std::string time_B = creat_file_name(0);
		std::string fileName = time_B;
		std::string dot  = ".";
		fileName = time_B + dot;
		fileName = fileName + mimeVal;
        the_file = fileName;
        content_type = mimeVal;
		outFile.open(fileName.c_str(), std::ios::out | std::ios::binary);
        if (!outFile.is_open())
        {
            error = 4;
            end = 1;
            std::cout << "Could not open the output file"<< the_file << std::endl << outFile.is_open()<<std::endl;
            return ;
        }
	}
    if (headers.find("Content-Length") != headers.end())
    {
        size_len =  (size_t)std::atoi((headers.find("Content-Length")->second).c_str());
    }
    else
    {
        if (headers.find("Transfer-Encoding") == headers.end())
        {
            end = 1;
            error = 3;
        }
        return ;
    }
	if (MethodType != 3 && outFile.is_open())
	{
        std::cout << "lll >>> " << body.size()<<std::endl;
        crfile = 1;
        body_size =  body.size();
        if (MethodType == 2)
            normalFile(body, body_size);
        else
            chunked_file(body, body_size);        
    }
}

void Post::normalFile(std::string body, size_t body_size)
{
    (void)body_size;
    std::cout << "in normal\n";
    std::cout << "is file open " << outFile.is_open() << std::endl;
    std::cout << "the file " << the_file<< std::endl;
	outFile.write(body.c_str(), body.size());
    total_Body += body.size();
    if (total_Body >= size_len)
    {
        // out.close();
        outFile.close();
        crfile = -2;
        end = 1;
    }
}


void Post::chunk_write(std::string body, size_t body_size)
{
    if (chunk_ctl >= (int)body.size())
    {
        // std::cout << "Me1\n";
        (void)body_size;
        outFile.write(body.c_str(), body.size());

        buffer = "";
        left_over = 0;
        if ((int)body.size() == chunk_ctl)
        {
             rare = 1;
             chunk_ctl = 0;
             return;
        }
        chunk_ctl = chunk_ctl - body.size();
    }
    else
    {
                // std::cout << "Me2\n";

        std::string tmp = "";
        std::string chunks_s;
        std::stringstream ss;

        buffer = "";
        tmp.append(body, 0, chunk_ctl);
        if (body.size() - chunk_ctl == 1)
        {
            outFile.write(tmp.c_str(), tmp.size());

            buff_chunk = body.substr(chunk_ctl, 1);
            left_over = 1;
            chunk_ctl = 0;
            rare = 1;
            return ;
        }
        buffer = body.substr(chunk_ctl + 2, body.size() - chunk_ctl - 2);
        if (buffer.find("\r\n") != std::string::npos)
        {
            chunks_s = buffer.substr(0, buffer.find("\r\n"));
            buffer = buffer.substr(buffer.find("\r\n") + 2, buffer.size() -  buffer.find("\r\n") - 2);
            ss << chunks_s;
            ss >> std::hex >> chunk_ctl;
            if (!chunk_ctl)
            {
                outFile.write(tmp.c_str(), tmp.size());
                outFile.close();
                crfile = -2;
                end = 1;
                out.close();
                return ;
            }
            tmp.append(buffer, 0, buffer.size());
            outFile.write(tmp.c_str(), tmp.size());
            chunk_ctl = chunk_ctl - buffer.size();
        }
        else
        {
            outFile.write(tmp.c_str(), tmp.size());
            buff_chunk = buffer;
            left_over = buffer.size();
            chunk_ctl = 0;
        }

    }

}

void Post::chunked_file(std::string body, size_t body_size)
{
    std::stringstream ss;
    if (!body.size()&& !left_over)
        return ;
    // std::cout << "in chunked\n";
    if (left_over)
    {
        buffer = "";
        buffer.append(buff_chunk, 0, left_over);
        buffer.append(body, 0, body.size());
        body = buffer;
        body_size = left_over + body.size();
        left_over = 0;
    }
    if (chunk_ctl)
        chunk_write(body, body_size);
    else
    {
        std::string chunks_s;
        if (rare)
        {
            body = body.substr(2, body.size() - 2);
            rare = 0;
        }
        if (body.find("\r\n") == std::string::npos)
        {
            std::cout << "not found \n";
            end = 1;
        }
        chunks_s = body.substr(0, body.find("\r\n"));
        body = body.substr(body.find("\r\n") + 2, body.size() -  body.find("\r\n") - 2);
        ss << chunks_s;
        ss >> std::hex >> chunk_ctl;
		if (!chunk_ctl)
		{
           	outFile.close();
           	crfile = -2;
            end = 1;
            out.close();
            return ;
		}
        body_size = body_size - chunks_s.size() - 2;
        chunk_write(body, body_size);
    }
}


void    Post::ft_boundary(std::string& body)
{
    size_t pos;
    size_t pos1;

    if (left_over)
    {
        // I add the new body with  what remain of the previous call 
        buffer.append(body, 0, body.size());
        left_over = 0;
        body = buffer;
        buffer = "";
    }
	if (body.find("\r") != std::string::npos && body.find("\r") + 3 - body.size() < sep.size())
	{
        //I put this call body in left over beceause it is not big enough to check sep or sep_end
        if (body.find(sep_end) != std::string::npos)
        {
            //special cas that probably does not happen anymore
            //it is if i have a body that end with spe_end with \r\n 
            //wich will cuse this function to always be called (look at if condation)
            if (body.find("\r\n") !=std::string::npos)
            {
                buffer = body.substr(0, body.size() - 2);
		        left_over = body.size() - 2;
                return ;
            }
        }
		buffer = body;
		left_over = body.size();
		return ;
	}
    if (body.find(sep_end) != std::string::npos)
    {
        //it all over so i check is there anythig to wirte then if not i close and out
		pos = body.find(sep_end);
        pos1 = body.find(sep);
        if (!pos || pos == 2)
        {
            end = 1;
            if (out.is_open())
            out.close();
            return ;
        }
        else if (pos == pos1)
        {
            // I check if pos and pos1 equal which will mean that sep_end was the only seprator
            //in this case there is something to be writen before closing
            out.write(body.c_str(), pos - 2);
            out.close();
            end = 1;
            return ;
        }
    }
	if (body.find(sep) != std::string::npos)
	{
        //first function to execute in normal cas
        pos = body.find(sep);
        if (body.find(sep, pos + 1) != std::string::npos)
        {
            //here i check if i found two or more body so i took only the first body aka until \r\n\rn
            //and safe the rest in left_ober== buffer
            if (!pos || pos <= 2)
            {
                //in this case i found sep at the beging neaning it will be nothing to write at the file and all what foulwed pos1
                //is for the next call
                //because pos1 satart at sep it will cause a rmain \r\n to stay in the body
                //that is handle by here_is 
                pos1 = body.find(sep, pos + 1);
                std::string buff_tmp = body.substr(0, pos1 );//problem in \r\n ;
                buffer = body.substr(pos1, body.size() - (pos1));
                left_over = body.size() - pos1 ;
                body = buff_tmp;
                here_is = 1;
            }
            else
            {
                //in case it is not firsr mean I should write then safe whatever left
                std::string buff_tmp = body.substr(0, pos);//problem in \r\n ;
                buffer = body.substr(pos, body.size() - pos);
                left_over = body.size() - pos;
                body = buff_tmp;
                out.write(body.c_str(), body.size());
                out.close();
                return ;
            }
        }
        if (body.find("\r\n\r\n") != std::string::npos)
		{
            
			sep_found = body.substr(pos, sep.size());
            if (out.is_open())
            {
                if (pos > 2)
                {
                    buffer = body.substr(0, pos - 2);
                    out.write(buffer.c_str(), buffer.size());
                    buffer = body.substr(pos, body.size() - pos);
                    left_over = buffer.size();
                    out.close();
                    return ;
                }
                else
                    out.close();
            }
			if (body.find("name") != std::string::npos)
			{
                std::string mimeVal;
				pos = body.find("name");
                if (body.find("\"", pos + strlen("name=") + 1) != std::string::npos)
                    pos1 = body.find("\"", pos + strlen("name=") + 1);
				if (pos1 <= 1)
					pos1 = 2;
				std::string file = body.substr(pos + strlen("name=") + 1, pos1 - ( strlen("name=") + 1) - pos);
                if (!file[0])
                {
                    std::string time_B = creat_file_name(0);
                    file = time_B;
                }
                else if (body.find("Content-Type:") != std::string::npos)
                {
                    std::string ext;
                    pos = body.find("Content-Type:");
                    pos1 = body.find("\r\n", pos);
                    ext = body.substr(pos + strlen("Content-Type: "), pos1 - (pos+ strlen("Content-Type: ")));
                    if (mime.find(ext) != mime.end())
	            		mimeVal = mime.find(ext)->second;
                    else
                        mimeVal = "x";
                }
	            else
                    mimeVal = "txt";
                file = file + ".";
                file += mimeVal;
                if (access(file.c_str(),F_OK ) == 0)
                {
                    //in case of duplcate ********************************************
                    end = 1;
                    crfile = -2;
                    error = 3;
                    return ;
                }
                out.open(file.c_str(), std::ios::out | std::ios::binary);
				if (out.is_open())
					std::cout << "FILE opened\n";
				else
                {
                    std::cout << "File Problem\n";
					error = 4;/////ERRRRRRRROE PAGES  
                    return ;
                }
				
			}
			else
			{
                std::string file;
				std::cout << "No File Name\n";
                std::string time_B = creat_file_name(0);
                std::stringstream ss1;
                ss1 << file_hang;
                file = time_B + ss1.str();
                file_hang++;
                std::string dot  = ".";
                file = file +  dot;
                file = file + "txt";
                ss1.str("");
                out.open(file.c_str(), std::ios::out | std::ios::binary);
				if (out.is_open())
					std::cout << "FILE opened\n" << file<<std::endl;
				else
                {
                    std::cout << file<<std::endl;
                    std::cout << "File Problem\n";
                    error = 4;
					return ;
                    
                }
				//make Error page 
				// return ;
			}
			buff_chunk = "";
			pos = body.find("\r\n\r\n");
            int end_sep;
            if (body.find(sep, pos) != std::string::npos)
            {
                pos1 = body.find(sep, pos);
                end_sep = pos1 - (pos + 4 + 2);
            }
            else 
                end_sep = body.size() - (pos + 4 );
            if (here_is)
                end_sep -= 2;
			buff_chunk.append(body, pos + 4, end_sep);
            out.write(buff_chunk.c_str(), buff_chunk.size() );
            here_is = 0;
		}
		else
		{
            if (left_over)
            {
                buffer.append(body, 0, body.size());
                left_over += body.size();   
            }
            else
            {
                buffer = body;
                left_over = body.size();
            }
            here_is = 0;
		}
	}
    else
    {
        if (out.is_open())
        {
            out.write(body.c_str(), body.size() );
        }
        here_is = 0;
    }
}
/*just focus herr*/
/**********************************************************************************************/

void Post::ft_boundary_cgi(std::string &body)
{
    size_t pos;
    size_t pos1;

    if (left_over)
    {
        buffer.append(body, 0, body.size());
        left_over = 0;
        body = buffer;
        buffer = "";
    }
	if (body.find("\r") != std::string::npos && body.find("\r") + 3 - body.size() < sep.size())
	{
        if (body.find(sep_end) != std::string::npos)
        {
            if (body.find("\r\n") !=std::string::npos)
            {
                buffer = body.substr(0, body.size() - 2);
		        left_over = body.size() - 2;
                return ;
            }
        }
		buffer = body;
		left_over = body.size();
		return ;
	}
    if (body.find(sep_end) != std::string::npos)
    {
		pos = body.find(sep_end);
        pos1 = body.find(sep);
        if (!pos || pos == 2)
        {
            end = 1;
            crfile = -2;
            if (out.is_open())
                out.close();
            return ;
        }
        else if (pos == pos1)
        {
            out.write(body.c_str(), pos - 2);
            total_Body = total_Body + (pos - 2);
            out.close();
            crfile = -2;
            end = 1;
            return ;
        }
    }
	if (body.find(sep) != std::string::npos)
	{
        pos = body.find(sep);
        if (body.find(sep, pos + 1) != std::string::npos)
        {
            if (!pos || pos <= 2)
            {
                pos1 = body.find(sep, pos + 1);
                std::string buff_tmp = body.substr(0, pos1 );//problem in \r\n ;
                buffer = body.substr(pos1, body.size() - (pos1));
                left_over = body.size() - pos1 ;
                body = buff_tmp;
                here_is = 1;
            }
            else
            {
                std::string buff_tmp = body.substr(0, pos);//problem in \r\n ;
                buffer = body.substr(pos, body.size() - pos);
                left_over = body.size() - pos;
                body = buff_tmp;
                out.write(body.c_str(), body.size());
                total_Body += body.size();
                // out.close();
                return ;
            }
        }
        if (body.find("\r\n\r\n") != std::string::npos)
		{
            
			sep_found = body.substr(pos, sep.size());
            if (out.is_open())
            {
                if (pos > 2)
                {
                    buffer = body.substr(0, pos - 2);
                    out.write(buffer.c_str(), buffer.size());
                    buffer = body.substr(pos, body.size() - pos);
                    left_over = buffer.size();
                    // out.close();
                    return ;
                }
            }
			if (body.find("name") != std::string::npos && !first_time)
			{
                std::string mimeVal;
                first_time = 1;
				pos = body.find("name");
                if (body.find("\"", pos + strlen("name=") + 1) != std::string::npos)
                    pos1 = body.find("\"", pos + strlen("name=") + 1);
				if (pos1 <= 1)
					pos1 = 2;
				std::string file = body.substr(pos + strlen("name=") + 1, pos1 - ( strlen("name=") + 1) - pos);
                if (!file[0])
                {
                    std::string time_B = creat_file_name(0);
                    file = time_B;
                }
                else if (body.find("Content-Type:") != std::string::npos)
                {
                    std::string ext;
                    pos = body.find("Content-Type:");
                    pos1 = body.find("\r\n", pos);
                    ext = body.substr(pos + strlen("Content-Type: "), pos1 - (pos+ strlen("Content-Type: ")));
                    if (mime.find(ext) != mime.end())
                    {
	            		mimeVal = mime.find(ext)->second;
                        content_type = ext;
                    }
                    else
                    {
                        mimeVal = "x";
                        content_type = "";
                    }
                }
	            else
                {
                    mimeVal = "txt";
                    content_type = "text/plain";
                }
                file = file + ".";
                file += mimeVal;
                if (access(file.c_str(),F_OK ) == 0)
                {
                    //in case of duplcate ********************************************
                    end = 1;
                    crfile = -2;
                    error = 3;
                    return ;
                }
                out.open(file.c_str(), std::ios::out | std::ios::binary);
                the_file = file;
				if (out.is_open())
					std::cout << "FILE opened\n";
				else
                {
                    std::cout << "File Problem\n";
                    error  = 4;
					return ;/////ERRRRRRRROE PAGES  
                }
				
			}
			else if (!first_time)
			{
                std::string file;
				std::cout << "No File Name\n";
                std::string time_B = creat_file_name(0);
                std::stringstream ss1;
                ss1 << file_hang;
                file = time_B + ss1.str();
                file_hang++;
                std::string dot  = ".";
                file = file +  dot;
                file = file + "txt";
                ss1.str("");
                the_file = file;
                content_type = "text/plain";
                out.open(file.c_str(), std::ios::out | std::ios::binary);
				if (out.is_open())
					std::cout << "FILE opened\n" << file<<std::endl;
				else
                {
                    std::cout << file<<std::endl;
                    std::cout << "File Problem\n";
                    error = 4;
					return  ;
                    
                }
				//make Error page 
				// return ;
			}
			buff_chunk = "";
			pos = body.find("\r\n\r\n");
            int end_sep;
            if (body.find(sep, pos) != std::string::npos)
            {
                pos1 = body.find(sep, pos);
                end_sep = pos1 - (pos + 4 + 2);
            }
            else 
                end_sep = body.size() - (pos + 4 );
            if (here_is)
                end_sep -= 2;
			buff_chunk.append(body, pos + 4, end_sep);
            out.write(buff_chunk.c_str(), buff_chunk.size());
            total_Body += buff_chunk.size();
            buff_chunk = "";
            here_is = 0;
		}
		else
		{
            if (left_over)
            {
                buffer.append(body, 0, body.size());
                left_over += body.size();   
            }
            else
            {
                buffer = body;
                left_over = body.size();
            }
            here_is = 0;
		}
	}
    else
    {
        if (out.is_open())
        {
            out.write(body.c_str(), body.size() );
            total_Body += body.size();
        }
        here_is = 0;
    } 
}

std::string Post::find_ext()
{
    size_t i;
    if (!fullUri_path.size())
    {
        std::cout << "extention Problem\n";
        return std::string("");       
    }
    for (i = fullUri_path.size() - 1; i >= 0 && fullUri_path[i]!= '.'; i--);

    if (!i || i == fullUri_path.size() - 1)
    {
        std::cout << "extention Problem\n";
        return std::string("");
    }
    std::string ext_ret = fullUri_path.substr(i + 1,fullUri_path.size() - i);
    return ext_ret;
}


char **Post::set_cmd(std::string& ext_path)
{
    char **cmd = new char*[3];
    cmd[0] = new char[ext_path.size() + 1];
    cmd[1] = new char[fullUri_path.size() + 1];
    cmd[2] = NULL;
    std::strcpy(cmd[0], ext_path.c_str());
    std::strcpy(cmd[1], fullUri_path.c_str());
    return cmd;
}


char **Post::set_env()
{
    std::string script_name;
    std::string content_len;
    std::string cont_type;
    std::string info_path;
    // std::string query_str;
    std::string serv_prt;
    std::stringstream ss; 
    
	char **env = new char*[7];
    env[0] = new char[strlen("REDIRECT_STATUS=HTTP/1.1 200 OK") + 1];
    std::strcpy(env[0], "REDIRECT_STATUS=HTTP/1.1 200 OK");
    // env[1] = new char[strlen("REQUEST_METHOD=Post") + 1];
    // std::strcpy(env[1], "REQUEST_METHOD=POST");
    script_name = "SCRIPT_NAME=" + fullUri_path;
    env[1] = new char[script_name.size() + 1];
    std::strcpy(env[1], script_name.c_str());
    ss << total_Body;
    ss >> content_len;
    content_len =  "CONTENT_LENGTH=" + content_len;
    env[2] = new char[content_len.size() + 1];
    std::strcpy(env[2], content_len.c_str());
    cont_type = "CONTENT_TYPE=" + content_type;
    env[3] = new char[cont_type.size() + 1];
    std::strcpy(env[3], cont_type.c_str());
    info_path = "PATH_INFO=" + fullUri_path;
    env[4] = new char[info_path.size() + 1];
    std::strcpy(env[4], info_path.c_str());
    serv_prt = "SERVER_PROTOCOL=HTTP/1.1";
    env[5] = new char[serv_prt.size() + 1];
    std::strcpy(env[5], serv_prt.c_str());
    env[6] = NULL;
    return env;
}
void Post::exe_cgi()
{
    if (!first_run)
    {
        std::string ext_path;
        char **cmd;
        char **env;

        end = 0;
        first_run = 1;
        ran_file = creat_file_name(1);
        ran_file = ran_file + ".html";
        std::string ext = find_ext();
        if (!ext.size())
            return ;//error page before
        std::cout << ext<<std::endl;
        if (serv.UriLocation.cgi_path.find(ext) != serv.UriLocation.cgi_path.end())
        {
            std::string ext_path = (serv.UriLocation.cgi_path.find(ext))->second;
            cmd = set_cmd(ext_path);
            env = set_env();
        }
        else
        {
            std::cout << "Problem in the extention\n";
            error = 3;
            return ;
        }
        start_time = clock();
        pid = fork();
        if (pid < 0)
        {
            perror("Fork failed");
             error = 4;
            exit(1);
        }
        if (pid == 0)
        {
            FILE *infile;
            FILE *outfile;

            infile = fopen(the_file.c_str(), "r");
            outfile = fopen(ran_file.c_str(), "w");
            if (!infile || !outfile)
            {
                std::cout << "Problem in opening the File\n";
                exit(1) ;
                //Error pages for problem
            }
            dup2(infile->_fileno, STDIN_FILENO);
            dup2(outfile->_fileno, STDOUT_FILENO);
            if (execve(cmd[0], cmd, env) == -1)
            {
                std::cout << "Failed to execute\n";
                exit(1);
            }
        }
    }
    if (waitpid(pid, &exit_status, WNOHANG) > 0)
    {
        cgi_exe = 1;
        int rem = std::remove(the_file.c_str());
        if (rem)
        {
            std::cout << " couldn't remove the TMP file \n";
            error = 4;
            return ;
        }
    }
    else if ((clock() - start_time) / CLOCKS_PER_SEC > 5)
    {
        kill(pid, SIGKILL);
        std::cout << "there is time out \n";
        int rem = std::remove(the_file.c_str());
        int ram = std::remove(ran_file.c_str());
        if (rem || ram)
        {
            std::cout << " couldn't remove the TMP file \n";
            error = 4;
            return ;
        }
        cgi_exe = 1;
        error = 2;
    }
}

int Post::process(std::string body, size_t body_size)
{
    // std::cout << "Post \n"<<std::endl;
    // std::cout << "body is " << body << "." << std::endl<< std::endl;
    // std::cout << "body size " << body.size() << std::endl<< std::endl;
    //  std::cout << "body size1 " << body.size() << std::endl<< std::endl;
    // std::cout << "rare " << rare << std::endl<< std::endl;
    // std::cout << "left over " << left_over<< std::endl<< std::endl;
    // std::cout << "buffer " << buffer << "."<<std::endl<< std::endl;
    // std::cout << "chunk ctl " << chunk_ctl <<std::endl<< std::endl;
    // std::cout << "****************************************************************\n";
    // if (!body.size() )
    // {
    //     std::cout << "nothing to do\n";
    // }
        // exit(1);
    if (error)
    {
        if (serv.Is_cgi)
        {
            if (access(the_file.c_str(),F_OK ) == 0)
            {
                int rem = std::remove(the_file.c_str());
                if (rem)
                {
                    std::cout << " couldn't remove the TMP file \n";
                    error = 4;
                    return 0;
                }
            }
            if (access(ran_file.c_str(),F_OK ) == 0)
            {
                 int rem = std::remove(ran_file.c_str());
                if (rem)
                {
                    std::cout << " couldn't remove the TMP file \n";
                    error = 4;
                    return 0;
                }
            }
        }
        std::cout << "Error " <<error << std::endl;
        if (error == 2)
        {
            get.serv.status = "408";
            get.get("error_pages/408.html");
            serv.status = "408";
        }
        else if (error == 3)
        {
            get.serv.status = "400";
            get.get("error_pages/400.html");
            serv.status = "400";
        }
        else if (error == 4)
        {
            get.serv.status = "509";
            get.get("error_pages/417.html");
            serv.status = "417";
        }
        respons = get.respons;
             if (get.end)
                end = 1;
        std::cout << "Found error\n";
        return 1;
    }
    if (crfile > 0 && body_size == 2 && MethodType == 1)
    {
        buff_chunk += body;
        left_over += body.size();
        return 1;
    } 
	 if (crfile > 0)
    {
		if (MethodType == 2)
			normalFile(body, body_size);
		else if (MethodType == 1)
			chunked_file(body, body_size);
		else if (MethodType == 3)
			ft_boundary(body);
        else
            ft_boundary_cgi(body);
    }
	else if (!crfile)
		openFile(body, body_size);
    if (end && !cgi_exe)
        enter_cgi = 1;
    if (enter_cgi && serv.Is_cgi&& !error)
        exe_cgi();
    if (cgi_exe && body_size == EPOLLOUT && !error)
    {
        std::cout << "in get\n";
        get.get(ran_file);
        respons = get.respons;
        std::cout << "response: " << respons<<std::endl;
        if (get.end)
        {
            end = 1;
            int x =  std::remove(ran_file.c_str());
            if (!x)
            {
                std::cout << "Erro in deleting file\n";
                error = 4;
            }
        }
        std::cout << "out of get\n";   
    }
    if (end && !serv.Is_cgi&& !error)
    {
        std::cout << "in Post resp\n";
        serv.status = "201";
        respons = "HTTP/1.1 " + serv.status;
        respons += string("\r\nContent-Type: text/html\r\n");
        respons += string("Content-Length: 13");
        respons += "\r\n\r\n";
        respons += "File created\n";
    }
    
    // std::cout << " >> " << body.size() << "recv read " << body_size << "chunked is " << chunk_ctl<<std::endl;
    return 1; 
}