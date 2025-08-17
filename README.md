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
  # The shell manages account validation, process execution, and user directories automatically.

## Project Structure
.
├── Locked_shell.c       # Main shell program
├── std_pass.txt         # Students' credentials
├── lec_pass.txt         # Lecturers' credentials
├── Student/             # Automatically created student directories
├── Courses/             # Automatically created courses directory
└── README.md



