/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmondell <mmondell@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 09:55:33 by mmondell          #+#    #+#             */
/*   Updated: 2022/01/18 13:10:11 by mmondell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

void logEvent(std::string &msg) {

	time_t timeNow = time(0);

	std::string date = (ctime(&timeNow));
	date.resize(date.length() - 1);
	std::cout << msg << std::endl;
}
