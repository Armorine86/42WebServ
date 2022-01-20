/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 14:52:27 by mmondell          #+#    #+#             */
/*   Updated: 2022/01/20 12:10:02 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstdlib>
#include <signal.h>
#include <vector>
#include <string>
#include <cstring>


#define DEFAULT_CONFIG_FILE "config_files/default.conf"
#define WHITESPACES " \t\n\v\f\r"
#define CONFIG_BLOCKS_DELIMITERS "{};"

typedef std::vector<std::string> StringVector;

