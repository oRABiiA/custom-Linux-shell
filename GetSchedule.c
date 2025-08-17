#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define LONG 100

void clearString(char* str) { // gets a string and clears it
	int  i;
	for (i = strlen(str) - 1; i >= 0; i--) {
		str[i] = '\0';
	}
}

int howMany(int fd) { // returns how many lines are in the file
	int count = 0, tmp;
	char buffer[2] = { '\0' };
	while (1) {
		tmp = lineCount(fd);
		if (tmp != 0) { count++; }
		else { break; }
	}
	lseek(fd, 0, SEEK_SET);
	return count;
}

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
		if (buffer[0] != '\n') { line++; }
	}
	return line;
}

int main(int argc, char* argv[]) {

	if (argc < 2) { fprintf(stdout, "Invalid Number Of Parameters\n"); exit(1); }

	int ch, fd_sch, bitCount, num, rd;
	char buffer[LONG] = { '\0' };

	if ((ch = chdir("/home/braude/Student")) == -1) { // go to student dir
		fprintf(stdout, "Could not find dir\n");
		exit(EXIT_FAILURE);
	}

	if ((ch = chdir(argv[1])) == -1) { // go to the student ID dir
		fprintf(stdout, "No Student with ID:%s found\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if ((fd_sch = open("Schedule.txt", O_RDWR)) == -1) { // open the schedule file for this student
		fprintf(stdout, "Student Not register!\n");
		exit(EXIT_FAILURE);
	}

	num = howMany(fd_sch); // get how many courses the student did register for
	int c;
	for (c = 0; c < num; c++) {
		bitCount = lineCount(fd_sch);
		lseek(fd_sch, -(bitCount + 1), SEEK_CUR);
		rd = read(fd_sch, buffer, bitCount + 1); // read a course from the student
		if (rd == -1) { perror("read from schedule.txt"); exit(1); }
		buffer[bitCount + 1] = '\0';
		fprintf(stdout, "%s", buffer); // show the course
	}
	lseek(fd_sch, 0, SEEK_SET);
	close(fd_sch);

	return 0;
}