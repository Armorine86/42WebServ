#include "Response.hpp"

void Response::responseGET()
{
	body.clear();
	if (DEBUG)
		std::cout << BRED << path << END << std::endl;

	if (request->isCGIRequest())
	{
		handleCGI();
		return;
	}
	if ((!is_dir(path) && request->getURL().find("/upload") != std::string::npos) ||
		request->getURL().find("/image") != std::string::npos ||
		request->getURL().find("favicon.ico") != std::string::npos)
		makeImage();
	else if (request->getURL().find("/420") != std::string::npos)
		status_code = "420";
	else if (autoindex)
		makeAutoindex(path);
	else if (redirection)
		status_code = "301";
	else
		readHTML(path);
	if (bodySize > config.client_max_body_size)
		status_code = "413";
	makeHeader(status_code);
}

// Retrieves the requested image binary and sets the informations in a pair
std::pair<char *, std::streampos> Response::getImageBinary(const char *path)
{
	ImgInfo img_info;
	std::ifstream f(path, std::ios::in | std::ios::binary | std::ios::ate);

	if (!f.is_open())
	{
		status_code = "404";
		img_info.first = NULL;
		return img_info;
	}

	img_info.second = f.tellg();

	img_info.first = new char[static_cast<long>(img_info.second)];

	f.seekg(0, std::ios::beg);
	f.read(img_info.first, img_info.second);
	f.close();

	return img_info;
}

// Find the location of the requested image
// Gets the Binary
// Writes the content to the Response body
void Response::makeImage()
{
	LocationVector location = config.locations;

	ImgInfo img = getImageBinary(path.c_str());

	body.clear();
	body.write(img.first, img.second);
	bodySize = img.second;
	status_code = "200";

	delete[] img.first;
}

// Used to respond with passed HTML file
void Response::readHTML(std::string filepath)
{
	std::string line;
	std::fstream myfile;

	myfile.open(filepath.c_str(), std::ios::in);

	if (!myfile.good())
	{
		status_code = "404";
		if (DEBUG)
			std::cout << logEvent("file cannot open!\n") << std::endl;
		return;
	}
	status_code = "200";
	while (getline(myfile, line))
		body << line << '\n';
	bodySize = body.str().length();
}

void Response::makeAutoindex(std::string path)
{
	DIR *dir;
	dirent *dirent;
	std::string line, value;

	dir = opendir(path.c_str());
	if (!dir)
	{
		status_code = "404";
		std::cout << BRED << "Autoindex error" << END << std::endl;
		return;
	}
	status_code = "200";
	value.assign("<html>\n<head>\n<meta charset=\"utf-8\">\n"
				 "<title>Directory Listing</title>\n</head>\n<body>\n<h1>" +
				 path + "</h1>\n<ul>");
	left_word_trim(path, "/upload");
	while ((dirent = readdir(dir)) != NULL)
	{
		value.append("<li><a href=\"");
		value.append(path);
		if (value[value.size() - 1] != '/')
			value.append("/");
		value.append(dirent->d_name);
		if (dirent->d_type == DT_DIR)
			value.append("/");
		value.append("\"> ");
		value.append(dirent->d_name);
		if (dirent->d_type == DT_DIR)
			value.append("/");
		value.append("</a></li>\n");
	}
	value.append("</ul></body></html>");
	closedir(dir);

	body << value;
	bodySize = body.str().length();
}
