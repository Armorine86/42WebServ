#include "Response.hpp"

void Response::errorBody(std::string& code)
{
	std::string line;
	std::fstream myfile;
	std::string error_path = config.error_pages.at(code);
	
	if (request.getURL().find("/image") != std::string::npos ||
		request.getURL().find("favicon.ico") != std::string::npos)
		makeImage(server->request);
	else {
		body.clear();
		myfile.open(error_path.c_str(), std::ios::in);
		
		if (!myfile.good()){
			server->status_code = "404";
			std::cout << logEvent("Ho no! Even the error file is not found!\n") << std::endl;
		}

		while (getline(myfile, line))
			body << line << '\n';
		bodySize = body.str().length();
	}
}
