#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	const char *myfifo = ".myfifo";
	const char* myfifo2 = ".myfifo2";

	// create the named pipe (FIFO)
	mkfifo(myfifo, 0666);
	mkfifo(myfifo2, 0666);

	const int MAX = 1024;
	char rd_data[MAX], wr_data[MAX];

	printf("waiting for named pipes open ... \n");

	// prog2: read first
	// order of open is important to unblock process
	int fd = open(myfifo, O_RDONLY);
	if (fd < 0) {
		printf("%s open error with code %d\n", myfifo, fd);
		exit(1);
	}
	int	fd2 = open(myfifo2, O_WRONLY);
	if (fd2 < 0) {
		printf("%s open error with code %d\n", myfifo2, fd2);
		exit(1);
	}

	printf("named pipes opened and ready\n");

	while (true)
	{
		read(fd, rd_data, sizeof(rd_data));
		printf("received: %s\n", rd_data);
		if (strcmp(rd_data, "quit") == 0)
			break;

		printf("Enter a message: ");
		fgets(wr_data, MAX, stdin);
		wr_data[strlen(wr_data) - 1] = '\0';
		write(fd2, wr_data, strlen(wr_data) + 1);
	}
}