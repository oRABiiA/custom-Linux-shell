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

	int ch, fd_tus, bitCount, writeTo;
	char path[50], buffer[LONG] = { '\0' };

	if ((ch = chdir("/home/braude/Student")) == -1) { // go to student dir
		fprintf(stdout, "Could not find dir\n");
		exit(EXIT_FAILURE);
	}

	if ((ch = chdir(argv[1])) == -1) { // go to the student ID dir
		fprintf(stdout, "No Student with ID:%s found\n", argv[1]);
		perror("");
		exit(EXIT_FAILURE);
	}

	if ((fd_tus = open("Status.txt", O_RDWR)) == -1) { // open the status file for this student
		perror("faild to open status file");
		exit(EXIT_FAILURE);
	}

	bitCount = lineCount(fd_tus);

	if (bitCount == 0) { fprintf(stdout, "The Student is already approved\n"); exit(1); } // if the status file for the student is empty

	else {
		remove("Status.txt"); // delete the current status file for the student
		close(fd_tus);
		if ((fd_tus = open("Status.txt", O_APPEND | O_RDWR | O_CREAT, 0644)) == -1) { // make a new empty status file for the student
			perror("ERROR status file creating in approve");
			exit(EXIT_FAILURE);
		}
		close(fd_tus);
		fprintf(stdout, "Student ID:%s is now approved\n", argv[1]);
	}
	return 0;
}