#include <unistd.h>
#include <iostream>

int main(void) {

	execlp("python3", "python3", "./src/cgi/form.py", NULL);
	
	std::cout << "Whelp it didn't work!" << std::endl;
}
