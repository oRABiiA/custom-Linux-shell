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

void breakString(char* input, char* dir, char* code, char* name, char* day, char* start, char* end) { // breaks input into seperate strings
	char save[6][LONG];
	char* token = strtok(input, " ");
	int i;
	for (i = 0; i < 6; i++) { clearString(save[i]); }
	i = 0;
	while (token != NULL) {
		strcpy(save[i++], token);
		token = strtok(NULL, " ");
	}

	strcpy(dir, save[0]);
	strcpy(code, save[1]);
	strcpy(name, save[2]);
	strcpy(day, save[3]);
	strcpy(start, save[4]);
	strcpy(end, save[5]);
}


int main(int argc, char* argv[]) {

	if (argc < 2) { fprintf(stdout, "Missing Parameters!!\n"); exit(1); }

	char courseName[LONG], courseCode[LONG], courseDay[LONG], courseStart[LONG], courseEnd[LONG];
	char course[LONG], input[LONG];
	int fd_set, writeTo, len;

	clearString(courseCode);
	clearString(courseName);
	clearString(courseDay);
	clearString(courseStart);
	clearString(courseEnd);
	clearString(input);

	breakString(argv[1], input, courseCode, courseName, courseDay, courseStart, courseEnd);

	// check for missing parameters for the course
	if (courseCode[0] == '\0') { fprintf(stdout, "%s: Missing Parameters!\n", argv[0]); exit(1); }
	if (courseName[0] == '\0') { fprintf(stdout, "%s: Missing Parameters!\n", argv[0]); exit(1); }
	if (courseDay[0] == '\0') { fprintf(stdout, "%s: Missing Parameters!\n", argv[0]); exit(1); }
	if (courseStart[0] == '\0') { fprintf(stdout, "%s: Missing Parameters!\n", argv[0]); exit(1); }
	if (courseEnd[0] == '\0') { fprintf(stdout, "%s: Missing Parameters!\n", argv[0]); exit(1); }

	// build the course string
	clearString(course);
	strcat(course, courseCode);
	strcat(course, " ");
	strcat(course, courseName);
	strcat(course, " ");
	strcat(course, courseDay);
	strcat(course, " ");
	strcat(course, courseStart);
	strcat(course, " ");
	strcat(course, courseEnd);
	strcat(course, "\n");

	int ch;
	if ((ch = chdir("Courses")) == -1) { // gor to Courses directory
		perror("Error changing dir");
		exit(EXIT_FAILURE);
	}

	if ((fd_set = open("Courses.txt", O_RDWR | O_CREAT, 0644)) == -1) { // create/open courses txt file 
		perror("ERROR new file oppening");
		exit(EXIT_FAILURE);
	}

	lseek(fd_set, 0, SEEK_END);

	writeTo = write(fd_set, course, strlen(course)); // write the new course string to the courses txt file

	if (writeTo == -1) {
		perror("Error writing course to Courses.txt");
		exit(EXIT_FAILURE);
	}

	if ((ch = chdir("/home/braude")) == -1) { // change the current working directory type to /home/braude
		perror("Error changing dir from SetCourses");
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "Successfully created Course\n");
	close(fd_set);
	return 0;
}