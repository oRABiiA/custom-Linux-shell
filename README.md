# Custom Linux Shell for Academic Management

A Linux shell program developed in C for managing students, lecturers, and courses. The project simulates a simple academic management system with login authentication, user directories, and secure account handling.

## Features

- **Student & Lecturer Login:** Secure login system with password validation and account blocking after multiple failed attempts.
- **Directory-based Storage:** Each student has a personal directory containing status files and related information.
- **Process Management:** Uses `fork` and `execl` to run separate processes for student and lecturer actions.
- **Error Handling:** Validates user input, handles incorrect credentials, and manages blocked accounts.
- **Modular Code:** Functions for string parsing, authentication, file navigation, and input handling.

## Technologies Used

- **Language:** C  
- **Platform:** Linux  
- **System Calls:** `fork`, `execl`, `read`, `write`, `lseek`, `mkdir`, `chdir`  
- **File Handling:** Reading and writing student/lecturer files  

## Installation

1. Clone the repository:  
   ```bash
   git clone https://github.com/yourusername/custom-linux-shell.git

2. Compile the program:
   ```bash
   gcc Locked_shell.c -o LockShell

3. Enter commands in the shell prompt:
   ```bash
   LockShell> LoginStudent <username> <password>
   LockShell> LoginLecturer <username> <password>
   LockShell> exit
  The shell manages account validation, process execution, and user directories automatically.

## Screenshots / Demo

Here are some example outputs of the program in action:

<img width="602" height="599" alt="Picture1" src="https://github.com/user-attachments/assets/4a89ea27-3fca-43c0-8a7b-f58cf2be0849" />
<img width="602" height="757" alt="Picture2" src="https://github.com/user-attachments/assets/9f36be00-e058-416e-afbb-442f53d7a0e7" />
<img width="580" height="683" alt="Picture3" src="https://github.com/user-attachments/assets/b1519744-7242-4f00-a48d-4ba53430b49e" />
