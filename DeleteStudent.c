#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define LONG 100


int main(int argc, char* argv[]) {

	if (argc < 2) { fprintf(stdout, "Invalid Number Of Parameters\n"); exit(1); }

	int ch, rm;
	char path[50];

	strcat(path, "/home/braude/Student/");
	strcat(path, argv[1]);

	if ((ch = chdir(path)) == -1) { // change the current working directory type to path
		fprintf(stdout, "No Student with ID:%s found\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	// remove the files in student dir
	remove("Status.txt");
	remove("Schedule.txt");
	if ((ch = chdir("/home/braude/Student/")) == -1) { // change the current working directory type to /home/braude
		perror("Error changing dir");
		exit(EXIT_FAILURE);
	}
	rm = rmdir(path); // delete the direcotry
	if (rm == 0) { fprintf(stdout, "Student has been deleted successfully\n"); exit(1); }
	if (rm < 0) { perror("Delete Student has failed"); exit(1); }

	return 0;
}