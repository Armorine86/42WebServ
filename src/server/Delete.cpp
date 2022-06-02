#include "Response.hpp"

void Response::responseDELETE() {
    deletePath(path);
    status_code = "200";
    makeHeader(status_code);
}

// Recursive function that will get inside all the branch of the upload directory
// delete all files and finally all directories
void Response::deletePath(std::string path) {
    DIR* dir;
    dirent* dirent;
    int ret = 0;
    std::string line;

    dir = opendir(path.c_str());
    if (!dir) {
        status_code = "404";
        std::cout << BRED << "Deleting error" << END << std::endl;
        return;
    }
    while ((dirent = readdir(dir)) != NULL) {
        line.clear();
        line = path;
        if (line[line.size() - 1] != '/')
            line.append("/");
        line.append(dirent->d_name);
        if (line[line.size() - 1] == '.')
            continue;
        if (dirent->d_type == DT_DIR)
            line.append("/");

        if (folderExist(line)) {
            deletePath(line);
            ret = rmdir(line.c_str());
        } else
            ret = unlink(line.c_str());
        if (ret < 0)
            std::cout << "Deleting error: " << errno << std::endl;
    }
    closedir(dir);
}
