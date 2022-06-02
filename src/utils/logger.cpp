/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 09:55:33 by mmondell          #+#    #+#             */
/*   Updated: 2022/04/12 10:01:17 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include <fstream>
#include <ostream>
#include <sys/stat.h>
#include <sys/types.h>

bool checkDir(const char* folderPath) {
    struct stat sb;

    if (stat(folderPath, &sb) == 0 && S_ISDIR(sb.st_mode))
        return true;
    return false;
}

void logToFile(time_t& t, std::stringstream& ss) {

    struct tm* now = localtime(&t);

    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d.", now);

    if (!checkDir("/log"))
        mkdir("log", ACCESSPERMS);

    std::string path("log/log_file[");

    path.append(buffer);
    path.append("].txt");

    std::ofstream myfile;
    myfile.open(path.c_str(), std::ios_base::app);
    if (!myfile.good())
        perror("[LOGGER] log_file error");

    myfile << ss.rdbuf();
    myfile.close();
}

std::string logEvent(const std::string& msg) {

    std::stringstream ss;
    time_t timeNow = time(0);

    std::string date = (ctime(&timeNow));
    date.resize(date.length() - 1);

    ss << "[" << date << "] " << msg;

    logToFile(timeNow, ss);
    return ss.str();
}
