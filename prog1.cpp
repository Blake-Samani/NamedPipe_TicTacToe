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

	const int MAX = 256;
	char rd_data[MAX];
	string wr_string;

	// prog1: write first

	while (true) {
		cout << "Enter a message: ";
		getline(cin, wr_string);

		fd = open(myfifo, O_WRONLY);
		const char* wr_data = wr_string.c_str();
		write(fd, wr_data, strlen(wr_data) + 1);
		close(fd);

		fd = open(myfifo, O_RDONLY);
		read(fd, rd_data, sizeof(rd_data));
		string rd_string(rd_data);
		cout << "prog1 received: " << rd_string << endl;
		close(fd);
	}
}