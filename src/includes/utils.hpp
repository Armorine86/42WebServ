#pragma once

#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>

#include "colors.hpp"
#include "defines.hpp"

typedef std::vector<std::string> StringVector;

//	Utility functions
std::string logEvent(const std::string& msg);
std::string format_line(std::string& line, int& line_num);
bool ends_with(std::string const& value, std::string const& ending);

// File / Folder Utils
bool valid_Folder_File(std::string path);
bool fileExist(std::string path);
bool folderExist(std::string path);
void createFile();

//	Split
StringVector split(std::string line, std::string delimiter);

//	Trimmer
std::string& left_trim(std::string& line, const char* delimiters);
std::string& right_trim(std::string& line, const char* delimiters);
std::string& both_trim(std::string& line, const char* delimiters);
std::string& left_word_trim(std::string& line, std::string s_delimiters);

// Convert Numerical type to String
template <typename T>
std::string IntToString(const T& num) {
    std::stringstream ss;
    std::string str;

    ss << num;
    ss >> str;

    return str;
}
