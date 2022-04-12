#include "Response.hpp"

void Response::responsePOST()
{
	if (request->getContentType().find("multipart/form-data") != std::string::npos)
		responseMultipart();
	else
		handleCGI();
}

void Response::responseMultipart()
{
	std::string boundary("");
	size_t start = 0, pos = 0;

	boundary = request->getContentType();
	left_word_trim(boundary, "--");
	boundary.insert(0, "--");
	boundary.append("--");

	if (server->bin_boundary == "")
		server->bin_boundary = boundary;

	if (server->isChunked == false){
		if (config.client_max_body_size < request->getContentLength()){
			status_code = "413";
			makeHeader(status_code);
			server->tropBeaucoup = true;
			return;
		}
		setFilename();
		start = findBodyStart();
		pos = start;

		if (fileExist(path)) {
			handleFile();
		}
		if (server->bytes == RECV_BUFSIZE)
			server->isChunked = true;
	}
	
	pos = findBodyEnd(pos, server->bin_boundary);

	writeToFile(start, pos);

	if (server->bytes != RECV_BUFSIZE)
	{
		makeAutoindex("./resources/upload/"); //!this is hardcoded and I hate it
		makeHeader(status_code);
		server->isChunked = false;
	}
}

size_t Response::findBodyStart()
{
	size_t start = 0;
	std::string content(request->buffer);

	start = content.find("filename=", start);
	start = content.find("\r\n\r\n", start);
	start += 4;

	return start;
}

void Response::setFilename() 
{
	size_t start = 0, pos = 0;
	std::string content;

	content = request->buffer;
	pos = content.find("filename=\"", pos);
	if (pos != std::string::npos)
	{
		pos += 10;
		start = content.find('"', pos);
		if (start != std::string::npos)
		{
			for (size_t i = 0; i < config.locations.size(); i++)
			{
				if (config.locations.at(i).upload_directory != "")
				{
					path.assign(config.locations.at(i).upload_directory);
					break;
				}
			}
			if (!folderExist(path))
				mkdir(path.c_str(), ACCESSPERMS);

			path.append(content.substr(pos, start - pos));
			server->upload_path.assign(path);
		}
	}
}

void Response::handleFile()
{
	std::ifstream infile(server->upload_path.c_str());

	if (infile.good()) {
		logEvent("File Already Exists");
		if (remove(server->upload_path.c_str()) == -1)
			logEvent("Could Not Delete File");
		logEvent("File Deleted Succesfully");
	}
}

void Response::writeToFile(size_t start, size_t pos) 
{
	std::ofstream ofs(server->upload_path.c_str(),
			std::ofstream::out | std::ofstream::app | std::ofstream::binary);

	if (!ofs.good() || !ofs.is_open())
		std::cerr << BRED << logEvent("OFSTREAM Error in filepath") << END << std::endl;

	const char *addr = &request->buffer[start];

	size_t len = 0;
	if (server->bytes != RECV_BUFSIZE)
		len = (pos - 5) - start;
	else
		len = pos - start;

	ofs.write(addr, len);
	ofs.close();

	if (!ofs.good())
		std::cerr << BRED << logEvent("OFSTREAM Error in writing") << END << std::endl;
}

size_t Response::findBodyEnd(size_t pos, std::string boundary) 
{
	char *ptr;
	size_t max_length = 0;
	while (pos < sizeof(request->buffer))
	{
		max_length = sizeof(request->buffer) - pos;
		ptr = (char *)memchr(request->buffer + pos, '-', max_length);
		if (ptr == NULL || pos > sizeof(request->buffer))
		{
			pos = server->bytes;
			return pos;
		}
		pos = ptr - request->buffer + 1;
		if (memcmp(ptr, boundary.c_str(), boundary.size()) == 0)
			break;
	}
	return pos + 1;
}
