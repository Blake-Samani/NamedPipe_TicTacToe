#include <iostream>
using namespace std;

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int main() {
	int fd;
	const char* myfifo = "tmp/myfifo";

	// create the named pipe (FIFO)
	mkfifo(myfifo, 0666);

	char rd_data[80];
	string wr_data;

	// prog1: write first

	while (true) {
		cout << "PROG1: Enter a word: "; 
		cin >> wr_data;

		fd = open(myfifo, O_WRONLY);
		write(fd, wr_data.c_str(), strlen(wr_data.c_str()));
		close(fd);

		fd = open(myfifo, O_RDONLY);
		read(fd, rd_data, sizeof(rd_data));
		cout << "prog1 received: " << rd_data << endl;
		close(fd);
	}
}