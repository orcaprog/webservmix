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
int main(int ac , char **av)
{
    (void)ac;

    try
    {
        Multiplexing mult(av[1]);
        mult.CreatMUltiplex();
    
    }
    catch(std::string  e)
    {
        std::cerr << e<< '\n';
    }
        catch(const char * e)
    {
        std::cerr << e<< '\n';
    }
    catch(exception & ex)
    {
        cout<<ex.what()<<endl;
        cout<<string::npos<<endl;
    }
    
}