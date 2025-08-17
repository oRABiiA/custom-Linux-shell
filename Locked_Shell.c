#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define LONG 100

int checkInfo(char* info, char* acc, char* pass) { // checks if the pass & acc are in info

	// to seperate & save the info into different strings
	char userAcc[LONG];
	char userPass[LONG];
	char userID[LONG];
	char userName[LONG];

	// to break the info string 
	char save[4][LONG];
	char* token = strtok(info, " ");

	int i = 0;
	while (token != NULL) {
		strcpy(save[i++], token);
		token = strtok(NULL, " ");
	}

	strcpy(userAcc, save[0]);
	strcpy(userPass, save[1]);
	strcpy(userID, save[2]);
	strcpy(userName, save[3]);

	// check the validation of the user input
	if (strcmp(userAcc, acc) == 0) {
		if (strcmp(userPass, pass) == 0) { // user is in info		
			return 0;
		}

		if (strcmp(userPass, pass) != 0) { // user password is incorrect		
			return 1;
		}
	}
	if (strcmp(userPass, acc) != 0) { // user is not in info
		return 2;
	}

}

void clearString(char* str) { // gets a string and clears it
	int  i;
	for (i = strlen(str) - 1; i >= 0; i--) {
		str[i] = '\0';
	}
}

int lineCount(int fd) { // calculates how many bits are there in a line in a file
	int line = 0, tmp;
	char buffer[2] = { '\0' }; // store one char at a time
	while (buffer[0] != '\n') {
		tmp = read(fd, buffer, 1); // get one char at a time until we reach an ENTER character
		if (tmp == -1) {
			perror("ERROR reading from student file");
			exit(EXIT_FAILURE);
		}
		if (tmp == 0) { break; }
		if (buffer[0] != '\n') {
			line++;
		}
	}
	return line; // return the amount of bits
}

void breakString(char* acc, char* pass, char* id, char* name, char* info) { // gets info and breaks it into 4 different strings, acc, pass, id & name
	char save[4][LONG]; // to store the sub-strings in info
	char* token = strtok(info, " ");
	int i = 0;
	while (token != NULL) {
		strcpy(save[i++], token);
		token = strtok(NULL, " ");
	}
	strcpy(acc, save[0]);
	strcpy(pass, save[1]);
	strcpy(id, save[2]);
	strcpy(name, save[3]);
}

int howMany(int fd) { // returns how many lines are there in a file
	int count = 0, tmp, header;
	char buffer[2] = { '\0' };
	header = lineCount(fd); // skip the header in stu/lec files
	lseek(fd, header + 1, SEEK_SET); // skip the header in stu/lec files
	while (1) {
		tmp = lineCount(fd);
		if (tmp != 0) { count++; }
		else { break; }
	}
	lseek(fd, 0, SEEK_SET); // return the pointer to the start
	return count;
}


void makeUserInput(char* input, char* acc, char* pass, char* directory) { // seperates input into acc, pass & directory

	char tmp[LONG];
	char save[3][LONG]; // to store the data from input

	int k;
	for (k = 0; k < 3; k++) {
		clearString(save[k]);
	}

	// seperate the string
	strcpy(tmp, input);
	char* token = strtok(tmp, " ");
	int i = 0;
	while (token != NULL) {
		strcpy(save[i++], token);
		token = strtok(NULL, " ");
	}

	strcpy(directory, save[0]);
	strcpy(acc, save[1]);
	strcpy(pass, save[2]);
}



int main(int argc, char* argv[]) {

	if (argc < 1) { fprintf(stdout, "Parameter count is not accurate"); exit(1); }

	int ch, status, pid, fd_stu, fd_lec, fd_tus, bitCount = 0, header = 0, flag, numberOfStudents, numberOfLec, runner, writeTo;
	char dir[LONG], password[LONG], account[LONG], ID[10], name[LONG], input[50], student[LONG],
		lecturer[LONG], tmpAcc[LONG], tmpPass[LONG], path[50], getUser[LONG];
	char* quit = "exit";
	char buffer[2] = { '\0' };

	if ((fd_stu = open("std_pass.txt", O_RDONLY)) == -1) { // open the students file for read only
		perror("open from reading file has failed");
		exit(EXIT_FAILURE);
	}

	if ((fd_lec = open("lec_pass.txt", O_RDONLY)) == -1) { // open the lecturers file for read only
		perror("open from reading file has failed");
		exit(EXIT_FAILURE);
	}

	if ((mkdir("Courses", 0777)) == -1) { // create a new  Courses directory
		perror("Error dir Courses");
		exit(EXIT_FAILURE);
	}

	if ((mkdir("Student", 0777)) == -1) { // create a new Students directory
		perror("Error dir student");
		exit(EXIT_FAILURE);
	}

	// make an array to save the students attempts
	numberOfStudents = howMany(fd_stu); // the amount of lines in stu file is how many students are there
	numberOfLec = howMany(fd_lec); // the amount of lines in lec file is how many students are there
	int* access = (int*)malloc(sizeof(int) * numberOfStudents); // an array to  store the failed attempts for each student trying to login to their accounts
	if (access == NULL) { fprintf(stdout, "malloc\n"); exit(EXIT_FAILURE); }
	int j;
	for (j = 0; j < numberOfStudents; j++) { access[j] = 0; }

	// make array's to save all the accounts into them
	char** studentNames;
	char** studentID;
	char** lecNames;
	studentNames = (char**)malloc(sizeof(char*) * numberOfStudents);
	studentID = (char**)malloc(sizeof(char*) * numberOfStudents);
	lecNames = (char**)malloc(sizeof(char*) * numberOfLec);
	if (studentNames == NULL) { fprintf(stdout, "malloc studentNames\n"); exit(EXIT_FAILURE); }
	if (studentID == NULL) { fprintf(stdout, "malloc studentID\n"); exit(EXIT_FAILURE); }
	if (lecNames == NULL) { fprintf(stdout, "malloc lecNames\n"); exit(EXIT_FAILURE); }
	int p;
	for (p = 0; p < numberOfStudents; p++) {
		studentNames[p] = (char*)malloc(sizeof(char) * LONG);
		studentID[p] = (char*)malloc(sizeof(char) * 10); // each ID is of length 9, the 10'th index is when '\0' is located at
	}
	for (p = 0; p < numberOfLec; p++) {
		lecNames[p] = (char*)malloc(sizeof(char) * LONG);
	}

	// make for each student a directory in students directory
	header = lineCount(fd_stu);
	lseek(fd_stu, header + 1, SEEK_SET); // skip the header in students file
	int k;
	for (k = 0; k < numberOfStudents; k++) {
		bitCount = lineCount(fd_stu);
		lseek(fd_stu, -(bitCount + 1), SEEK_CUR);
		clearString(student);
		int tmp;
		tmp = read(fd_stu, student, bitCount); // read a student line in stu file
		if (tmp == -1) {
			perror("ERROR reading from student file");
			exit(EXIT_FAILURE);
		}
		lseek(fd_stu, 1, SEEK_CUR); // move to the next student
		char help[LONG];
		strcpy(help, student);
		breakString(account, password, ID, name, help); // get the student info

		if ((ch = chdir("Student")) == -1) { // change the current working directory type to Student file
			perror("Error changing dir");
			exit(EXIT_FAILURE);
		}

		if ((mkdir(ID, 0777)) == -1) { // create a new directory with name ID of the student in Studeny dir
			perror("Error dir student");
			exit(EXIT_FAILURE);
		}

		if ((ch = chdir(ID)) == -1) { // go inside the new ID direcory which we created for the student
			perror("Error changing dir");
			exit(EXIT_FAILURE);
		}

		if ((fd_tus = open("Status.txt", O_APPEND | O_RDWR | O_CREAT, 0644)) == -1) { // create a status txt file in student dir, which will hold the status of the student in our system
			perror("ERROR status file creating");
			exit(EXIT_FAILURE);
		}

		if ((ch = chdir("/home/braude")) == -1) { // change the current working directory type to /home/braude
			perror("Error changing dir");
			exit(EXIT_FAILURE);
		}

		close(fd_tus);
		strcpy(studentNames[k], account); // save the student in studentNames
		strcpy(studentID[k], ID); // save the sutdent ID in studentID
	}
	lseek(fd_stu, 0, SEEK_SET);

	// clear the strings for future use
	clearString(account);
	clearString(password);
	clearString(ID);
	clearString(name);

	header = lineCount(fd_lec);
	lseek(fd_lec, header + 1, SEEK_SET); // skip the header in lec file
	for (k = 0; k < numberOfLec; k++) {
		bitCount = lineCount(fd_lec);
		lseek(fd_lec, -(bitCount + 1), SEEK_CUR);
		clearString(lecturer);
		int tmp;
		tmp = read(fd_lec, lecturer, bitCount); // get a lecturer from lec file
		if (tmp == -1) {
			perror("ERROR reading from student file");
			exit(EXIT_FAILURE);
		}
		lseek(fd_lec, 1, SEEK_CUR);
		char help[LONG];
		strcpy(help, lecturer);
		breakString(account, password, ID, name, help); // get the lecturer info
		strcpy(lecNames[k], account); // save the lecturer in lecNames
	}

	// clear the strings for future use
	clearString(tmpAcc);
	clearString(tmpPass);
	clearString(dir);


	// get input from the user
	fprintf(stdout, "LockShell> ");
	fgets(getUser, LONG, stdin);
	getUser[strlen(getUser) - 1] = '\0';

	while ((strcmp(getUser, quit)) != 0) { // a condition to end the program
		makeUserInput(getUser, tmpAcc, tmpPass, dir); // seperate the user input to check data entry

		if (strlen(tmpPass) == 0 && (strcmp(dir, "LoginStudent") == 0 || strcmp(dir, "LoginLecturer") == 0)) { fprintf(stdout, "Lack of parameters\n"); } // if the user didnt enter a full input

		else if ((strcmp(dir, "LoginStudent")) == 0) { // when dir == LoginStudent
			header = lineCount(fd_stu);
			lseek(fd_stu, header + 1, SEEK_SET); // skip the header in stu file
			while (1) {
				runner = 0; // to store each student index
				bitCount = lineCount(fd_stu);
				if (bitCount == 0) { break; }
				lseek(fd_stu, -(bitCount + 1), SEEK_CUR);
				clearString(student);
				int tmp;
				tmp = read(fd_stu, student, bitCount); // read a line (student) in stu file
				if (tmp == -1) {
					perror("ERROR reading from student file");
					exit(EXIT_FAILURE);
				}
				if (tmp == 0) { break; }
				lseek(fd_stu, 1, SEEK_CUR); // move to the next line
				char stuCopy[LONG];
				strcpy(stuCopy, student);
				breakString(account, password, ID, name, stuCopy); // get the line (student) info
				int isIn = checkInfo(student, tmpAcc, tmpPass); // check data insertion type, return a specific value
				int w;
				if (isIn == 0) { // student found, if the data of the student in line match up with the user input data
					flag = 0;
					access[runner] = 0; // reset his attempts in the array, it still keeps his account on freeze mode if he is actually blocked
					lseek(fd_stu, 0, SEEK_SET);
					if ((pid = fork()) < 0) { // if fork didnt succeed end program
						perror("fork");
						exit(1);
					}
					if (pid == 0) { // child proccess
						execl("/home/braude/LoginStudent", "LoginStudent", account, password, ID, name, NULL); // make the child run the LoginStudent program
						perror("execl"); // if error end the child
						exit(1);
					}
					waitpid(pid, &status, 0); // make the parent wait until the child is finished
					break;
				}
				else if (isIn == 1) {// password is incorrect, if the password data of the student in line does not match up with the user password input 
					lseek(fd_stu, 0, SEEK_SET);
					flag = 0;
					access[runner]++; // increment his failed attempts to login to his account
					clearString(path);
					strcat(path, "Student/");
					strcat(path, ID);
					if ((ch = chdir(path)) == -1) { // change the current working directory type, go to that student directory 
						perror("Error changing dir from failed password1");
						exit(EXIT_FAILURE);
					}
					if ((fd_tus = open("Status.txt", O_RDWR)) == -1) { // open the students file for this student
						perror("open from reading file has failed");
						exit(EXIT_FAILURE);
					}
					int red;
					char get[LONG];
					red = read(fd_tus, get, 7); // read whats written in status file
					if (red == -1) { perror("open status file from lockedShell failed"); exit(1); }
					if (strcmp(get, "Blocked") == 0 && access[runner] >= 3) { // if the user is already blocked & he entered more than 3 times an incorrect password
						//fprintf(stdout,"%s account BLOCKED!\n",account);
						fprintf(stdout, "Incorrect Password\n");
						access[runner] = 0; // reset the attempts counter for the students, it keeps him blocked in the system
						close(fd_tus);
					}
					else if (access[runner] >= 3) { // if the user is not blocked yet and he entered 3 or more times an incorrect password
						fprintf(stdout, "%s account BLOCKED!\n", account);
						writeTo = write(fd_tus, "Blocked", 7); // write to the student status file "Blocked" to indicate that his account is on freeze mode		
						if (writeTo == -1) { perror("write to status file when blocked"); exit(1); }
						close(fd_tus);
					}
					else { fprintf(stdout, "Incorrect Password\n"); } // if only the pass in incorrect, show a proper message
					if ((ch = chdir("/home/braude")) == -1) { // change the current working directory type to /home/braude
						perror("Error changing dir");
						exit(EXIT_FAILURE);
					}
					break;
				}
				else if (isIn == 2) {// student not found in this line in stu file
					runner++; // increment the index
					flag = 1; // an indicator of such student exist or not
				}

			}
			if (flag == 1) { fprintf(stdout, "Student Not found!\n"); }

		}



		else if ((strcmp(dir, "LoginLecturer")) == 0) { // when dir == LoginLecturer
			header = lineCount(fd_lec);
			lseek(fd_lec, header + 1, SEEK_SET); // skip the header in lec file
			while (1) {
				bitCount = lineCount(fd_lec);
				if (bitCount == 0) { break; }
				lseek(fd_lec, -(bitCount + 1), SEEK_CUR);
				clearString(lecturer);
				int tmp;
				tmp = read(fd_lec, lecturer, bitCount); // get a line (lecturer) in lec file
				if (tmp == -1) {
					perror("ERROR reading from student file");
					exit(EXIT_FAILURE);
				}
				if (tmp == 0) { break; }
				lseek(fd_lec, 1, SEEK_CUR); // move to the next line
				char stuCopy[LONG];
				strcpy(stuCopy, lecturer);
				breakString(account, password, ID, name, stuCopy);
				int isIn = checkInfo(lecturer, tmpAcc, tmpPass); // check data insertion type, return a specific value
				if (isIn == 0) { // lecturer found
					flag = 0;
					lseek(fd_lec, 0, SEEK_SET);
					if ((pid = fork()) < 0) { // if fork didnt succeed end program
						perror("fork");
						exit(1);
					}
					if (pid == 0) {
						execl("/home/braude/LoginLecturer", "LoginLecturer", account, password, ID, name, NULL); // make the child run the path
						perror("execl"); // if error end the child
						exit(1);
					}
					waitpid(pid, &status, 0); // make the parent proccess wait for the child to finish
					break;
				}
				else if (isIn == 1) {// the entered password is incorrect
					lseek(fd_lec, 0, SEEK_SET);
					flag = 0;
					fprintf(stdout, "Incorrect Password\n");
					break;
				}
				else if (isIn == 2) { // lecturer not found in this line
					flag = 1; // an indicator if the lecturer exists or not
				}

			}
			if (flag == 1) { fprintf(stdout, "Lecturer Not found!\n"); }

		}




		else {
			fprintf(stdout, "Not Supported\n"); // any other input is not supported
		}

		// clean the input strings for another user to prevent override
		clearString(tmpAcc);
		clearString(tmpPass);
		clearString(dir);
		clearString(getUser);

		// get another input from the user
		fprintf(stdout, "LockShell> ");
		fgets(getUser, LONG, stdin);
		getUser[strlen(getUser) - 1] = '\0';


	}

	// if user entered quit, go to exit and end program
	if ((pid = fork()) < 0) { // if fork didnt succeed end program
		perror("fork");
		exit(1);
	}
	if (pid == 0) {
		execl("/home/braude/Exit", "Exit", NULL); // make the child run the path
		perror("execl"); // if error end the child
		exit(1);
	}
	waitpid(pid, &status, 0); // make the parent proccess wait for the child to finish the program
	close(fd_stu);
	close(fd_lec);
	return 0;
}