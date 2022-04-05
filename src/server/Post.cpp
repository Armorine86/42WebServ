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
	std::string boundary;
	size_t start = 0, pos = 0;

	boundary = request->getContentType();
	left_word_trim(boundary, "--");
	boundary.append(boundary + "--");

	if (server->isChunked == false){
		if (config.client_max_body_size < request->getContentLength()){
			status_code = "413";
			makeHeader(status_code);
			server->tropBeaucoup = true;
			return;
		}
		start = setFilename();
		pos = start;
	}

	pos = findBodyEnd(pos, boundary);
	writeToFile(start, pos);

	if (server->bytes != RECV_BUFSIZE)
	{
		makeAutoindex("./resources/upload/"); //!this is hardcoded and I hate it
		makeHeader(status_code);
		server->isChunked = false;
	}
}

size_t Response::setFilename() 
{
	size_t start = 0, pos = 0;
	std::string content, filepath;

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
					filepath.assign(config.locations.at(i).upload_directory);
					break;
				}
			}
			filepath.append(content.substr(pos, start - pos));
			server->upload_path.assign(filepath);
		}
		start = content.find("\r\n\r\n", start);
		start += 4;
		std::ifstream infile(server->upload_path.c_str());
		if (infile.good())
			unlink(server->upload_path.c_str());
		if (server->bytes == RECV_BUFSIZE)
			server->isChunked = true;
	}
	return start;
}

void Response::writeToFile(size_t start, size_t pos) 
{
	std::ofstream ofs(server->upload_path.c_str(),
			std::ofstream::out | std::ofstream::app | std::ofstream::binary);

	if (!ofs.good() || !ofs.is_open())
		std::cout << BRED << "OFSTREAM Error in filepath" << END << std::endl;

	const char *addr = &request->buffer[start];

	size_t len = 0;
	if (server->bytes != RECV_BUFSIZE)
		len = (pos - 5) - start;
	else
		len = pos - start;

	ofs.write(addr, len);
	ofs.close();

	if (!ofs.good())
		std::cout << BRED << "OFSTREAM Error in writing" << END << std::endl;
}

size_t Response::findBodyEnd(size_t pos, std::string boundary) 
{
	char *ptr;
	while (true)
	{
		ptr = (char *)memchr(request->buffer + pos, '-', sizeof(request->buffer));
		pos = ptr - request->buffer + 1;
		if (memcmp(ptr, boundary.c_str(), boundary.size()) == 0)
			break;
		if (pos > sizeof(request->buffer))
		{
			pos = server->bytes;
			break;
		}
		// faire dequoi aussi si ca trouve pas le boundary
		// setter aussi un file too big
	}
	return pos;
}
