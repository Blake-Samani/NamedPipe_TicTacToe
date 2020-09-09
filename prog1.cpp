#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>

int main() {
	int fd;
	const char* myfifo = "tmp/myfifo";

	// create the named pipe (FIFO)
	mkfifo(myfifo, 0666);

	const int MAX = 256;
	char rd_data[MAX];
	char wr_data[MAX];

	// prog1: write first

	while (true) {
		printf("Enter a message: ");
		fgets(wr_data, MAX, stdin);

		fd = open(myfifo, O_WRONLY);
		write(fd, wr_data, strlen(wr_data) + 1);
		close(fd);

		fd = open(myfifo, O_RDONLY);
		read(fd, rd_data, sizeof(rd_data));
		printf("prog1 received: %s\n", rd_data);
		close(fd);
	}
}