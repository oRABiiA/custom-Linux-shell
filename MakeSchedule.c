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

int howMany(int fd) { // returns how many lines are there in a file
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

int lineCount(int fd) { // returns how many bits are there in a single line in a file
	int line = 0, tmp;
	char buffer[2] = { '\0' };
	while (buffer[0] != '\n') { // stop when we get to an "Enter" character
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

void getCodeID(char* input, char* dest) { // gets the course code from input
	char temp[5];
	int i;
	for (i = 0; i < 6; i++) {
		temp[i] = input[i];
	}
	temp[5] = '\0';
	strcpy(dest, temp);
}

int checkLineFile(int fd, char* insert) {// checks if insert is in file fd

	int bitCount, rd, i, count;
	char buffer[LONG], tmp[LONG];

	strcpy(tmp, insert);
	tmp[strlen(tmp) - 1] = '\0';

	lseek(fd, 0, SEEK_SET);
	count = howMany(fd); // get how many lines are in file *fd*

	for (i = 0; i < count; i++) {
		bitCount = lineCount(fd);
		lseek(fd, -(bitCount + 1), SEEK_CUR);
		rd = read(fd, buffer, bitCount + 1); // read a line from the file
		if (rd == -1) { perror("read in function checkLineFile"); exit(1); }
		buffer[strlen(buffer) - 1] = '\0';
		if (strcmp(buffer, tmp) == 0) { // check if the line is in file
			return 1; // exists
		}
	}
	return 0; // not exist
}


int main(int argc, char* argv[]) {

	if (argc < 1) { fprintf(stdout, "Invalid Number Of Parameters\n"); exit(1); }

	int bitCount, ch, fd_crs, fd_sch, fd_tus, rd, writeTo, lines, check = 0, go, flag = 1, ok = 0, fileCheck;
	char answer[20], temp[5], course[LONG], status[50];
	char path[50];

	clearString(path);
	strcat(path, "Student/");
	strcat(path, argv[1]);

	if ((ch = chdir(path)) == -1) { // change the current working directory type to the specific student directory
		perror("Error changing dir");
		exit(EXIT_FAILURE);
	}

	if ((fd_tus = open("Status.txt", O_RDONLY)) == -1) { // open the status file for this student
		perror("ERROR oppening Status from makeSchedule");
		exit(EXIT_FAILURE);
	}

	rd = read(fd_tus, status, 7); // read whats written in the status file
	if (rd == -1) { perror("read from status"); exit(1); }
	if (strcmp(status, "Blocked") == 0) { // if blocked is written, then the student cant do the command
		fprintf(stdout, "Blocked Account! permission denied\n");
		exit(1);
	}
	else {
		close(fd_tus);
		rd = 0;
	}

	if ((ch = chdir("/home/braude/Courses")) == -1) { // go to the courses direcotry
		perror("Error changing dir");
		exit(EXIT_FAILURE);
	}

	if ((fd_crs = open("Courses.txt", O_RDONLY)) == -1) { // open the courses file to see all the currently available courses
		fprintf(stdout, "There are no Courses yet\n");
		exit(EXIT_FAILURE);
	}

	if ((ch = chdir("/home/braude")) == -1) { // go back to /home/braude
		perror("Error changing dir");
		exit(EXIT_FAILURE);
	}

	if ((ch = chdir(path)) == -1) { // switch to this student directory
		perror("Error changing dir");
		exit(EXIT_FAILURE);
	}

	if ((fd_sch = open("Schedule.txt", O_APPEND | O_RDWR | O_CREAT, 0644)) == -1) { // open a new file named schedule for this student
		perror("ERROR new file oppening");
		exit(EXIT_FAILURE);
	}


	lines = howMany(fd_crs); // get how many courses are there
	clearString(answer);
	clearString(course);
	fprintf(stdout, "Insert Course ID:\n"); // ask for input from the student

	while (1) {
		fscanf(stdin, "%s", answer); // get input from the student
		if (strcmp(answer, "Exit") == 0) { break; }
		if (strcmp(answer, "Confirm") == 0) {

			if (ok == 0) { fprintf(stdout, "Choose an existing course first, Enter a Course ID to Procceed:\n"); flag = 0; go = 0; } // if he chose Confirm, without choosing a course first
			else { // if he chose confirm, check if he chose this course before then update the value
				if (ok == 1) {
					fileCheck = checkLineFile(fd_sch, course); // check if he already has this course in his schedule
					lseek(fd_sch, 0, SEEK_END);
					if (fileCheck == 1) { // he has the course
						fprintf(stdout, "Can't append course. The course is already scheduled\n");
						flag = 0;
						go = 0;
					}
					else if (fileCheck == 0) { // he doesnt have the course
						lseek(fd_sch, 0, SEEK_END);
						writeTo = write(fd_sch, course, bitCount + 1); // append the course to his schedule
						flag = 0;
						go = 0;
					}
				}
			}
		}
		int k;
		if (flag == 1) { // check for valid input from the student
			for (k = 0; k < strlen(answer); k++) {
				if (answer[k] < 48 || answer[k] > 57) {
					go = 0;
					fprintf(stdout, "Please insert Confirm,Exit or Course ID only\n");
					break;
				}
				else { go = 1; }
			}
		}
		if (go != 0) { // search for the course ID in Courses file
			int i;
			for (i = 0; i < lines; i++) {
				bitCount = lineCount(fd_crs);
				lseek(fd_crs, -(bitCount + 1), SEEK_CUR);
				rd = read(fd_crs, course, bitCount + 1); // read a course from Courses
				if (rd == -1) { perror("read from courses"); exit(1); }
				course[bitCount + 1] = '\0';
				getCodeID(course, temp); // get the code from the course
				if (strcmp(answer, temp) == 0) { // if course is found, show it to the student
					fprintf(stdout, "%s", course);
					check = 1;
					ok = 1;
					break;
				}
			}
			if (check == 0) { fprintf(stdout, "Course ID not found\n"); fprintf(stdout, "Insert Course ID:\n"); ok = 0; } // if course is not found

		}
		flag = 1;
		check = 0;
		lseek(fd_crs, 0, SEEK_SET);
	}
	// handle a situation where the student didnt append any courses and his schedule is empty, so delete his schedule file from his directory
	lseek(fd_sch, 0, SEEK_SET);
	bitCount = lineCount(fd_sch);
	if (bitCount == 0) {
		remove("Schedule.txt");
	}
	else {
		lseek(fd_sch, 0, SEEK_SET);
	}
	close(fd_sch);
	close(fd_crs);
	return 0;
}


