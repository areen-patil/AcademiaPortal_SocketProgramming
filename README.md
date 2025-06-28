# AcademiaPortal_SocketProgramming
This is a academia portal built for socket programming project as part of operating system lab project
# College Course Management System (Client-Server, C)

A simple multi-threaded client-server application in C for managing students, faculty, courses, and enrollments in a college environment.  
Supports admin, faculty, and student roles with authentication and role-based operations.

---

## Features

- **Admin**
  - Add, remove, activate/deactivate students and faculty
  - Update student details
  - View all students, faculty, courses, and enrollments
  - Change admin password

- **Faculty**
  - Add and remove courses
  - View their courses and enrolled students
  - Change their password

- **Student**
  - Enroll/unenroll in courses
  - View enrolled courses
  - Change their password

- **Authentication**
  - Admin login by ID and password
  - Faculty and student login by email and password

- **Persistence**
  - All data is stored in binary files (`students.dat`, `faculty.dat`, `courses.dat`, `enrollments.dat`)
  - Data is loaded and saved automatically

- **Concurrency**
  - Multi-threaded server using POSIX threads
  - File access synchronized with semaphores

---

## File Structure

```
.
├── clientsocket.c      # Client application
├── serversocket.c      # Server application
├── students.dat        # Student data (binary)
├── faculty.dat         # Faculty data (binary)
├── courses.dat         # Course data (binary)
├── enrollments.dat     # Enrollment data (binary)
```

---

## Build & Run

### 1. Compile

```sh
gcc -o server serversocket.c -lpthread
gcc -o client clientsocket.c
```

### 2. Start the Server

```sh
./server
```

### 3. Start the Client (in another terminal)

```sh
./client
```

---

## Usage

- **Login as Admin, Faculty, or Student**
- **Follow the on-screen menu to perform actions**
- **All changes are persistent across sessions**

---

## Notes

- Default admin credentials:  
  - **ID:** `1234`  
  - **Password:** `admin123`
- Faculty and student IDs are auto-generated.
- Faculty and student login is via email and password.
- Data files are created automatically if not present.

---

## Code Highlights

- Uses `send`/`recv` and `read`/`write` for socket and file I/O.
- Uses fixed-size buffers for struct fields.
- Handles buffer overflows and synchronization with care.
- Modular code for easy extension.

---

## License

This project is for educational purposes.

---

## Author

- [Your Name]
