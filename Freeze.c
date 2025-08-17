#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define LONG 100

int lineCount(int fd) { // returns how many bits there are in the line in file
	int line = 0, tmp;
	char buffer[2] = { '\0' };
	while (buffer[0] != '\n') {
		tmp = read(fd, buffer, 1);
		if (tmp == -1) {
			perror("ERROR reading from student file");
			exit(EXIT_FAILURE);
		}
		if (tmp == 0) { break; }
		if (buffer[0] != '\n') {
			line++;
		}
	}
	return line;
}


int main(int argc, char* argv[]) {

	if (argc < 2) { fprintf(stdout, "Invalid Number Of Parameters\n"); exit(1); }

	int ch, fd_tus, bitCount, writeTo, rd;
	char path[50], buffer[LONG] = { '\0' };


	if ((ch = chdir("/home/braude/Student")) == -1) { // go to Student dir
		fprintf(stdout, "Could not find dir\n");
		exit(EXIT_FAILURE);
	}

	if ((ch = chdir(argv[1])) == -1) { // go to the specific student dir
		fprintf(stdout, "No Student with ID:%s found\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if ((fd_tus = open("Status.txt", O_RDWR)) == -1) { // open the status file for the student
		perror("faild to open status file");
		exit(EXIT_FAILURE);
	}

	bitCount = lineCount(fd_tus);

	if (bitCount == 0) { // if the status file is empty
		writeTo = write(fd_tus, "Blocked", 7); // write blocked in the status file, meaning we freeze the student
		if (writeTo == -1) { perror("Could not write to status file from Freeze"); exit(1); }
		else {
			fprintf(stdout, "Student ID: %s is Blocked\n", argv[1]);
			exit(1);
		}
	}

	else { // the student is already blocked
		lseek(fd_tus, 0, SEEK_SET);
		rd = read(fd_tus, buffer, 20);
		if (rd == -1) { perror("read from status"); exit(1); }
		if (strcmp(buffer, "Blocked") == 0) {
			fprintf(stdout, "Student is already Blocked\n");
			exit(1);
		}

		else {
			lseek(fd_tus, 0, SEEK_SET);
			writeTo = write(fd_tus, "Blocked", 7);
			fprintf(stdout, "Student ID: %s is Blocked\n", argv[1]);
			exit(1);
		}

	}
	return 0;
}