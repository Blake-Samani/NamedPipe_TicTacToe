#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	// myfifo: prog1 => prog2
	// myfifo2: prog2 => prog1
	const char* myfifo = ".myfifo";
	const char* myfifo2 = ".myfifo2";

	// create the named pipe (FIFO)
	mkfifo(myfifo, 0666);
	mkfifo(myfifo2, 0666);

	const int MAX = 1024;
	char rd_data[MAX], wr_data[MAX];

	printf("%s: READY!\n", argv[0]);

	// prog1: write first
	int fd = open(myfifo, O_WRONLY);
	int	fd2 = open(myfifo2, O_RDONLY);

	while (true) {
		printf("Enter a message: ");
		fgets(wr_data, MAX, stdin);

		write(fd, wr_data, strlen(wr_data) + 1);

		read(fd2, rd_data, sizeof(rd_data));
		printf("prog1 received: %s", rd_data);
	}
}