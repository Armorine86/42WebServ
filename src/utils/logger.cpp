/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 09:55:33 by mmondell          #+#    #+#             */
/*   Updated: 2022/03/10 20:55:25 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include <ostream>
#include <fstream>

void logToFile(time_t& t, std::stringstream& ss) {
	
	struct tm * now = localtime(&t);

	char buffer [80];
	strftime (buffer,80,"%Y-%m-%d.",now);

	std::string path("log/log[");
	
	path.append(buffer);
	path.append("].txt");
	
	std::ofstream myfile;
	myfile.open(path.c_str());

	myfile << ss.rdbuf();
	myfile.close();
}

std::string logEvent(const std::string &msg) {

	std::stringstream ss;
	time_t timeNow = time(0);

	std::string date = (ctime(&timeNow));
	date.resize(date.length() - 1);
	
	ss << "[" << date << "] " << msg;

	logToFile(timeNow, ss);
	return ss.str();
}

