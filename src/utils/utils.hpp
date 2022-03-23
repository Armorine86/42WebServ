/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcollet <gcollet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 14:47:52 by mmondell          #+#    #+#             */
/*   Updated: 2022/03/23 14:08:16 by gcollet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <sstream>
#include <ctime>
#include <iostream>

#include "defines.hpp"
#include "colors.hpp"

typedef std::vector<std::string> StringVector;


//	Utility functions
std::string logEvent(const std::string &msg);
std::string format_line(std::string &line, int& line_num);
bool ends_with(std::string const & value, std::string const & ending);

//	Split
StringVector split(std::string line, std::string delimiter);


//	Trimmer
std::string& left_trim(std::string &line, const char* delimiters);
std::string& right_trim(std::string &line, const char* delimiters);
std::string& both_trim(std::string &line, const char* delimiters);
