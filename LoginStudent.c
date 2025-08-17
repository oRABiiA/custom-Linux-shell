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

int main(int argc, char* argv[]) {

	if (argc < 3) { fprintf(stdout, "Invalid Number Of Parameters\n"); exit(1); }

	char* quit = "LogOut";
	char stdInput[LONG], ID[10];
	int child, status, ch;

	strcpy(ID, argv[3]);

	if ((ch = chdir("Student")) == -1) { // change the current working directory type to Student directory
		perror("");
		exit(EXIT_FAILURE);
	}

	if ((ch = chdir(ID)) == -1) { // check if the student we logged in to exists in the system or not
		fprintf(stdout, "Student Not Found\n");
		exit(EXIT_FAILURE);
	}

	if ((ch = chdir("/home/braude")) == -1) { // change the current working directory type to /home/braude
		perror("");
		exit(EXIT_FAILURE);
	}

	// get input from the user
	fprintf(stdout, "StdShell> ");
	fscanf(stdin, "%s", stdInput);

	while ((strcmp(stdInput, quit)) != 0) {

		if ((strcmp(stdInput, "ShowCourses")) == 0) {
			child = fork(); // make a child proccess
			if (child == 0) {
				execl("/home/braude/ShowCourses", "ShowCourses", NULL);
				perror("execl child trying to open showcourses"); // if error end the child
				exit(1);
			}
			if (child < 0) {
				perror("child fork from loginstudent");
				exit(1);
			}
			waitpid(child, &status, 0); // make the parent wait for the child to finish
		}

		else if ((strcmp(stdInput, "GetNewCourse")) == 0) {
			child = fork(); // make a child proccess
			if (child == 0) {
				execl("/home/braude/GetNewCourses", "GetNewCourses", NULL);
				perror("execl child trying to open getnewcourses"); // if error end the child
				exit(1);
			}
			if (child < 0) {
				perror("child fork from loginstudent");
				exit(1);
			}
			waitpid(child, &status, 0); // make the parent wait for the child to finish
		}

		else if ((strcmp(stdInput, "MakeSchedule")) == 0) {
			child = fork(); // make a child proccess
			if (child == 0) {
				execl("/home/braude/MakeSchedule", "MakeSchedule", ID, NULL);
				perror("execl child trying to open makeschedule"); // if error end the child
				exit(1);
			}
			if (child < 0) {
				perror("child fork from loginstudent");
				exit(1);
			}
			waitpid(child, &status, 0); // make the parent wait for the child to finish
		}

		else if ((strcmp(stdInput, "GetSchedule")) == 0) {
			child = fork(); // make a child proccess
			if (child == 0) {
				execl("/home/braude/GetSchedule", "GetSchedule", ID, NULL);
				perror("execl child trying to open getschedule"); // if error end the child
				exit(1);
			}
			if (child < 0) {
				perror("child fork from loginlecturer");
				exit(1);
			}
			waitpid(child, &status, 0); // make the parent wait for the child to finish
		}

		else {
			fprintf(stdout, "Not Supported\n"); // any other input is not supported
		}

		// get input from the user again
		fprintf(stdout, "StdShell> ");
		fscanf(stdin, "%s", stdInput);

	}

	child = fork(); // make a child proccess to finish the program
	if (child == 0) {
		execl("/home/braude/LogOut", "LogOut", NULL);
		perror("execl child trying to open getschedule"); // if error end the child
		exit(1);
	}
	if (child < 0) {
		perror("child fork from loginlecturer");
		exit(1);
	}
	waitpid(child, &status, 0); // make the parent wait for the child to finish the program
	return 0;
}