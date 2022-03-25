/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 14:47:52 by mmondell          #+#    #+#             */
/*   Updated: 2022/03/22 20:12:45 by mmondell         ###   ########.fr       */
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

//	Split
StringVector split(std::string line, std::string delimiter);


//	Trimmer
std::string& left_trim(std::string &line, const char* delimiters);
std::string& right_trim(std::string &line, const char* delimiters);
std::string& both_trim(std::string &line, const char* delimiters);

// Convert Numerical type to String
template<typename T>
std::string IntToString(const T& num) {
	std::stringstream ss;
	std::string str;

	ss << num;
	ss >> str;
	
	return str;
}
