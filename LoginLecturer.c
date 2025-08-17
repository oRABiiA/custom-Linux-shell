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

void checkSetCourses(char* input, char* dest) { // a specific function to check if the data is suitable for SetCourses command
	char temp[11];
	int i;
	for (i = 0; i < 10; i++) {
		temp[i] = input[i]; // SetCourses is of length 10
	}
	temp[10] = '\0';
	strcpy(dest, temp);
}

void getID(char* input, char* dest1, char* dest2) { // sets dest1 & dest2 to the first two values which input has
	char temp[LONG][LONG];
	char other[LONG];

	strcpy(other, input);
	char* token = strtok(other, " ");
	int i = 0;
	while (token != NULL) {
		strcpy(temp[i++], token);
		token = strtok(NULL, " ");
	}
	strcpy(dest1, temp[0]);
	strcpy(dest2, temp[1]);
}

int main(int argc, char* argv[]) {

	if (argc < 3) { fprintf(stdout, "Invalid Number Of Parameters\n"); exit(1); }


	char* quit = "LogOut";
	char dir[LONG], lecInput[LONG], ID[LONG], temp[LONG];
	int child, status;
	clearString(dir);
	clearString(lecInput);
	// get input from the user
	fprintf(stdout, "LecShell> ");
	fgets(dir, LONG, stdin);
	dir[strlen(dir) - 1] = '\0';

	while ((strcmp(dir, quit)) != 0) {
		getID(dir, temp, ID); // get the ID from the input
		checkSetCourses(temp, lecInput); // check if the input is for SetCourses command
		if ((strcmp(lecInput, "SetCourses")) == 0) {
			child = fork(); // create a child
			if (child == 0) {
				execl("/home/braude/SetCourses", "SetCourses", dir, NULL); // make the child run the command
				perror("execl child trying to open setcourses"); // if error end the child
				exit(1);
			}
			if (child < 0) {
				perror("child fork from loginlecturer");
				exit(1);
			}
			waitpid(child, &status, 0); // make the parent wait for the child to finish
		}

		else if ((strcmp(temp, "ShowCourses")) == 0) {
			child = fork(); // create a child
			if (child == 0) {
				execl("/home/braude/ShowCourses", "ShowCourses", NULL); // make the child run the command
				perror("execl child trying to open showcourses"); // if error end the child
				exit(1);
			}
			if (child < 0) {
				perror("child fork from loginlecturer");
				exit(1);
			}
			waitpid(child, &status, 0); // make the parent wait for the child to finish
		}

		else if ((strcmp(temp, "DeleteStudent")) == 0) {
			child = fork(); // create a child
			if (child == 0) {
				execl("/home/braude/DeleteStudent", "DeleteStudent", ID, NULL); // make the child run the command
				perror("execl child trying to open deletestudent"); // if error end the child
				exit(1);
			}
			if (child < 0) {
				perror("child fork from loginlecturer");
				exit(1);
			}
			waitpid(child, &status, 0); // make the parent wait for the child to finish

		}

		else if ((strcmp(temp, "Freeze")) == 0) {
			child = fork(); // create a child
			if (child == 0) {
				execl("/home/braude/Freeze", "Freeze", ID, NULL); // make the child run the command
				perror("execl child trying to open freeze"); // if error end the child
				exit(1);
			}
			if (child < 0) {
				perror("child fork from loginlecturer");
				exit(1);
			}
			waitpid(child, &status, 0); // make the parent wait for the child to finish
		}

		else if ((strcmp(temp, "Approve")) == 0) {
			child = fork(); // create a child
			if (child == 0) {
				execl("/home/braude/Approve", "Approve", ID, NULL); // make the child run the command
				perror("execl child trying to open approve"); // if error end the child
				exit(1);
			}
			if (child < 0) {
				perror("child fork from loginlecturer");
				exit(1);
			}
			waitpid(child, &status, 0); // make the parent wait for the child to finish
		}

		else if ((strcmp(temp, "GetSchedule")) == 0) {
			child = fork(); // create a child
			if (child == 0) {
				execl("/home/braude/GetSchedule", "GetSchedule", ID, NULL); // make the child run the command
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
			fprintf(stdout, "Not Supported\n"); // if the user enters a different command
		}

		// get input from the user again
		clearString(lecInput);
		fprintf(stdout, "LecShell> ");
		fgets(dir, LONG, stdin);
		dir[strlen(dir) - 1] = '\0';

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