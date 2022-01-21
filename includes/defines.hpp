/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 14:52:27 by mmondell          #+#    #+#             */
/*   Updated: 2022/01/21 12:08:09 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstdlib>
#include <signal.h>
#include <vector>
#include <string>
#include <cstring>

// General Purpose defines
#define DEFAULT_CONFIG_FILE "config_files/default.conf"
#define WHITESPACES " \t\n\v\f\r"
#define CONFIG_BLOCKS_DELIMITERS "{};"
#define LOCALHOST "127.0.0.1"

// Error codes
#define PARSING_ERROR 3
#define FILESTREAM_ERROR 1

// Vector typedefs  (cuz writing std::vector<blablabla> is tedious)
typedef std::vector<std::string> StringVector;


