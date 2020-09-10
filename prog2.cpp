#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	const char *myfifo = ".myfifo";
	const char* myfifo2 = ".myfifo2";

	// create the named pipe (FIFO)
	mkfifo(myfifo, 0666);
	mkfifo(myfifo2, 0666);

	const int MAX = 1024;
	char rd_data[MAX], wr_data[MAX];

	printf("%s: READY!\n", argv[0]);

	// prog2: read first
	int fd = open(myfifo, O_RDONLY);
	int	fd2 = open(myfifo2, O_WRONLY);

	while (true)
	{
		read(fd, rd_data, sizeof(rd_data));
		printf("prog2 received: %s", rd_data);

		printf("Enter a message: ");
		fgets(wr_data, MAX, stdin);
		write(fd2, wr_data, strlen(wr_data) + 1);
	}
}