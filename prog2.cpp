#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "my_const.h"

int main()
{
	// create the named pipe (FIFO) if not created yet
	int f1 = mkfifo(myfifo_1to2, 0666);
	int f2 = mkfifo(myfifo_2to1, 0666);
	printf("@p2: f1 = %d  f2 = %d\n", f1, f2);

	char rd_data[MAX], wr_data[MAX];

	printf("waiting for named pipes open ... \n");

	// prog2: read first
	// order of open is important to unblock process
	int fd = open(myfifo_1to2, O_RDONLY);
	if (fd < 0) {
		printf("%s open error with code %d\n", myfifo_1to2, fd);
		exit(1);
	}
	int	fd2 = open(myfifo_2to1, O_WRONLY);
	if (fd2 < 0) {
		printf("%s open error with code %d\n", myfifo_2to1, fd2);
		exit(1);
	}

	printf("named pipes opened and ready\n");

	while (true)
	{
		read(fd, rd_data, sizeof(rd_data));
		printf("received: %s\n", rd_data);
		if (strcmp(rd_data, "Q") == 0)
			break;

		printf("Enter a message: ");
		fgets(wr_data, MAX, stdin);
		wr_data[strlen(wr_data) - 1] = '\0';
		write(fd2, wr_data, strlen(wr_data) + 1);
	}
}