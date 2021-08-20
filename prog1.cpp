#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "my_const.h"

int main() {

	// create the named pipe (FIFO) if not exist
	int f1 = mkfifo(myfifo_1to2, 0666);
	int f2 = mkfifo(myfifo_2to1, 0666);
	printf("@p1: f1 = %d  f2 = %d\n", f1, f2);

	char rd_data[MAX], wr_data[MAX];

	printf("waiting for named pipes open ... \n");

	// prog1: write first
	// open() will be blocked until the other side is open
	int fd = open(myfifo_1to2, O_WRONLY);
	if (fd < 0) {
		printf("%s open error with code %d\n", myfifo_1to2, fd);
		exit(1);
	}
	int	fd2 = open(myfifo_2to1, O_RDONLY);
	if (fd2 < 0) {
		printf("%s open error with code %d\n", myfifo_2to1, fd2);
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
	unlink(myfifo_1to2);
	unlink(myfifo_2to1);
}