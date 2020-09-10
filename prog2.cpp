#include <iostream>
using namespace std;

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int main()
{
	const char *myfifo = "tmp/myfifo";
	const char* myfifo2 = "tmp/myfifo2";

	// create the named pipe (FIFO)
	mkfifo(myfifo, 0666);
	mkfifo(myfifo2, 0666);

	const int MAX = 256;
	char rd_data[MAX];
	string wr_string;

	// prog2: read first
	int fd = open(myfifo, O_RDONLY);
	int	fd2 = open(myfifo2, O_WRONLY);

	while (true)
	{
		read(fd, rd_data, sizeof(rd_data));
		string rd_string(rd_data);
		cout << "prog2 received: " << rd_string << endl;

		cout << "Enter a message: ";
		getline(cin, wr_string);

		const char* wr_data = wr_string.c_str();
		write(fd2, wr_data, strlen(wr_data) + 1);
	}
}