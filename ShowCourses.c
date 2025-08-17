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
		if (buffer[0] != '\n') { line++; }
	}
	return line;
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

void clearString(char* str) { // gets a string and clears it
	int  i;
	for (i = strlen(str) - 1; i >= 0; i--) {
		str[i] = '\0';
	}
}

int main(int argc, char* argv[]) {

	if (argc < 1) { fprintf(stdout, "Parameters Count Not Valid\n"); exit(1); }

	char course[LONG];
	int ch, bitCount, rd, fd_crs, count;
	if ((ch = chdir("Courses")) == -1) { // go to courses directory
		perror("Error changing dir");
		exit(EXIT_FAILURE);
	}

	if ((fd_crs = open("Courses.txt", O_RDONLY)) == -1) { // open the courses file for read only
		fprintf(stdout, "There are no courses yet\n");
		exit(EXIT_FAILURE);
	}

	clearString(course);
	count = howMany(fd_crs); // get how many courses currently available
	int i;
	for (i = 0; i < count; i++) { // show all the courses
		bitCount = lineCount(fd_crs);
		lseek(fd_crs, -(bitCount + 1), SEEK_CUR);
		clearString(course);
		rd = read(fd_crs, course, bitCount + 1); // read a course
		if (rd == -1) {
			perror("ERROR reading from courses.txt");
			exit(EXIT_FAILURE);
		}
		course[bitCount + 1] = '\0';
		fprintf(stdout, "%s", course); // print the course
	}

	close(fd_crs);

	return 0;
}