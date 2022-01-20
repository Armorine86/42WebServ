/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 09:26:11 by mmondell          #+#    #+#             */
/*   Updated: 2022/01/20 13:24:23 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "defines.hpp"

// Cleans whitespaces and gives back a formatted string
std::string format_line(std::string &line) {
	
	std::string new_string;
	size_t i = 0;

	while (!line.empty()) {
		if (strchr(CONFIG_BLOCKS_DELIMITERS, line[0])) {
			//new_string.push_back(' ');
			new_string.push_back(line[0]);
			break;
		}
		left_trim(line, WHITESPACES);
		i = line.find_first_not_of(WHITESPACES);
		if (i == 0) {
			i = line.find_first_of(WHITESPACES);
		}
		new_string.append(" " + line.substr(0, i));
		line.erase(0, i);
	}

	return left_trim(new_string, " ");
}

std::string& left_trim(std::string &line, const char* delimiters) {
	
	line.erase(0, line.find_first_not_of(delimiters));

	return line;
}

std::string& right_trim(std::string &line, const char* delimiters) {
	
	line.erase(line.find_last_not_of(delimiters) + 1);

	return line;
}

std::string& both_trim(std::string &line, const char* delimiters) {

	return left_trim(right_trim(line, delimiters), delimiters);	
}

StringVector split(std::string line, std::string delimiter) {
   
   StringVector result;
   
   size_t i = 0, j = 0;
   
   while (i < line.length() && j < line.length()) {
	   i = line.find_first_of(delimiter, j);
	   if (i == std::string::npos) {
		   i = line.length();
	   }
	   std::string str = line.substr(j, i - j);
	   if (!str.empty()) {
		   result.push_back(str);
	   }
	   j = i + 1;
   }

   return result;
}
