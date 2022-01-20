/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 09:55:33 by mmondell          #+#    #+#             */
/*   Updated: 2022/01/19 14:59:00 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::string logEvent(const std::string &msg) {

	std::stringstream ss;
	time_t timeNow = time(0);

	std::string date = (ctime(&timeNow));
	date.resize(date.length() - 1);
	
	ss << "[" << date << "] " << RED << msg;

	return ss.str();
}
