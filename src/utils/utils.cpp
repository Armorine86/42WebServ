/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 09:55:33 by mmondell          #+#    #+#             */
/*   Updated: 2022/01/18 13:27:45 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

void logEvent(const std::string &msg) {

	time_t timeNow = time(0);

	std::string date = (ctime(&timeNow));
	date.resize(date.length() - 1);
	
	std::cout << "[" << date << "] " << msg << std::endl;
}
