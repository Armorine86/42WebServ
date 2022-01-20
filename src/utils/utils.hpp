/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 14:47:52 by mmondell          #+#    #+#             */
/*   Updated: 2022/01/20 12:05:15 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <sstream>
#include <ctime>
#include <iostream>
#include "colors.hpp"

//Split
//StringVector split(std::string& line, std::string delimiter);

std::string logEvent(const std::string &msg);
std::string format_line(std::string &line);

// Trimmer
std::string& left_trim(std::string &line, const char* delimiters);
std::string& right_trim(std::string &line, const char* delimiters);
std::string& both_trim(std::string &line, const char* delimiters);

