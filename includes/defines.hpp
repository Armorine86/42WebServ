/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 14:52:27 by mmondell          #+#    #+#             */
/*   Updated: 2022/02/28 14:21:23 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstdlib>
#include <signal.h>
#include <vector>
#include <string>
#include <cstring>

#include "colors.hpp"
#include "utils.hpp"


// General Purpose defines
#define DEFAULT_CONFIG_FILE "config_files/default.conf"
#define WHITESPACES " \t\n\v\f\r"
#define CONFIG_BLOCKS_DELIMITERS "{};"
#define LOCALHOST "127.0.0.1"

// Error codes
#define FILESTREAM_ERROR 1
#define PARSING_ERROR 2
#define INV_SOCKET 3
#define BINDING_ERR 4
#define OPT_ERR 5
#define LISTEN_ERR 6

// Other Macros
#define SIG_EXIT 0

// Vector typedefs  (cuz writing std::vector<blablabla> is tedious)
typedef std::vector<std::string> StringVector;


