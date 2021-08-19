#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	// myfifo: prog1 => prog2
	// myfifo2: prog2 => prog1
	const char* myfifo = ".myfifo";
	const char* myfifo2 = ".myfifo2";

	// create the named pipe (FIFO) if not exist
	mkfifo(myfifo, 0666);
	mkfifo(myfifo2, 0666);

	const int MAX = 1024;
	char rd_data[MAX], wr_data[MAX];

	printf("waiting for named pipes open ... \n");

	// prog1: write first
	// open() will be blocked until the other side is open
	int fd = open(myfifo, O_WRONLY);
	if (fd < 0) {
		printf("%s open error with code %d\n", myfifo, fd);
		exit(1);
	}
	int	fd2 = open(myfifo2, O_RDONLY);
	if (fd2 < 0) {
		printf("%s open error with code %d\n", myfifo2, fd2);
		exit(1);
	}

	printf("named pipes opened and ready\n");

	while (true) {
		printf("Enter a message (Q to quit): ");
		fgets(wr_data, MAX, stdin);
		wr_data[strlen(wr_data) - 1] = '\0'; // '\n' is replaced by NULL ('\0')
		write(fd, wr_data, strlen(wr_data) + 1);
		if (strcmp(wr_data, "Q") == 0)
			break;

		read(fd2, rd_data, sizeof(rd_data));
		printf("received: %s\n", rd_data);
	}
	close(fd);
	close(fd2);
	unlink(myfifo);
	unlink(myfifo2);
}