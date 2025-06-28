#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>

#define PORT 8080
#define BUFFER_SIZE 10000

// Struct definitions
typedef struct {
    int id;
    char name[30];
    char password[30];
    int active; // 1 = active, 0 = inactive
    char email[50]; // Added email field
} student;

typedef struct {
    int id;
    char name[30];
    char password[30];
    char email[50]; // Added email field
} faculty;

typedef struct {
    int id;
    char name[30];
    int facultyId;
    int credits;
    int numEnroll;
} course;

typedef struct {
    int studentId;
    int courseId;
} enroll;

sem_t file_mutex;

// Admin credentials (stored internally)
const int adminId = 1234;
const char adminPassword[] = "admin123";

// File paths
const char *studentFile = "students.dat";
const char *facultyFile = "faculty.dat";
const char *courseFile = "courses.dat";
const char *enrollFile = "enrollments.dat";

#define MAX_STUDENTS 100
#define MAX_FACULTY 100
#define MAX_COURSES 100
#define MAX_ENROLLMENTS 100

student students[MAX_STUDENTS];
int student_count = 0;
faculty faculties[MAX_FACULTY];
int faculty_count = 0;
course courses[MAX_COURSES];
int course_count = 0;
enroll enrollments[MAX_ENROLLMENTS];
int enrollment_count = 0;

void removeCourseByIndex(int idx) {
    if (idx < 0 || idx >= course_count) return;
    for (int i = idx; i < course_count - 1; i++) {
        courses[i] = courses[i + 1];
    }
    course_count--;
}
void removeEnrollmentByIndex(int idx) {
    if (idx < 0 || idx >= enrollment_count) return;
    for (int i = idx; i < enrollment_count - 1; i++) {
        enrollments[i] = enrollments[i + 1];
    }
    enrollment_count--;
}
void removeStudentByIndex(int idx) {
    if (idx < 0 || idx >= student_count) return;
    for (int i = idx; i < student_count - 1; i++) {
        students[i] = students[i + 1];
    }
    student_count--;
}
void removeFacultyByIndex(int idx) {
    if (idx < 0 || idx >= faculty_count) return;
    for (int i = idx; i < faculty_count - 1; i++) {
        faculties[i] = faculties[i + 1];
    }
    faculty_count--;
}
void loadAllData() {
    // Load students
    FILE *fs = fopen(studentFile, "rb");
    student_count = 0;
    while (fread(&students[student_count], sizeof(student), 1, fs) == 1) {
        student_count++;
    }
    fclose(fs);
    // Repeat for faculty, courses, enrollments
    FILE *ff = fopen(facultyFile, "rb");
    faculty_count = 0;
    while (fread(&faculties[faculty_count], sizeof(faculty), 1, ff) == 1) {
        faculty_count++;
    }
    fclose(ff);
    FILE *fc = fopen(courseFile, "rb");
    course_count = 0;
    while (fread(&courses[course_count], sizeof(course), 1, fc) == 1) {
        course_count++;
    }
    fclose(fc);
    FILE *fe = fopen(enrollFile, "rb");
    enrollment_count = 0;
    while (fread(&enrollments[enrollment_count], sizeof(enroll), 1, fe) == 1) {
        enrollment_count++;
    }
    fclose(fe);
    write(STDOUT_FILENO, "All data loaded successfully.\n", 30);
}

void saveAllData() {
    FILE *fs = fopen(studentFile, "wb");
    fwrite(students, sizeof(student), student_count, fs);
    fclose(fs);
    // Repeat for faculty, courses, enrollments
    FILE *ff = fopen(facultyFile, "wb");
    fwrite(faculties, sizeof(faculty), faculty_count, ff);
    fclose(ff);
    FILE *fc = fopen(courseFile, "wb");
    fwrite(courses, sizeof(course), course_count, fc);
    fclose(fc);
    FILE *fe = fopen(enrollFile, "wb");
    fwrite(enrollments, sizeof(enroll), enrollment_count, fe);
    fclose(fe);
    write(STDOUT_FILENO, "All data saved successfully.\n", 29);
}

int findStudentIndexById(int id) {
    for (int i = 0; i < student_count; i++) {
        //print the student id and name
        char temp[BUFFER_SIZE];
        memset(temp, 0, sizeof(temp));
        sprintf(temp, "Student ID: %d, Name: %s\n", students[i].id, students[i].name);
        write(STDOUT_FILENO, temp, strlen(temp));
        if (students[i].id == id) return i;
    }
    return -1;
}

int findFacultyIndexById(int id) {
    write(STDOUT_FILENO, "Entered the function that finds the index of the id faculty\n", strlen("Entered the function that finds the index of the id faculty\n"));
    for (int i = 0; i < faculty_count; i++) {
        if (faculties[i].id == id) return i;
        write(STDOUT_FILENO, "Checking faculty id: ", strlen("Checking faculty id: "));
        char temp[BUFFER_SIZE];
        memset(temp, 0, sizeof(temp));
        sprintf(temp, "%d\n", faculties[i].id);
        write(STDOUT_FILENO, temp, strlen(temp));
    }
    write(STDOUT_FILENO, "Returning from the function that finds the index of the id faculty\n", strlen("Returning from the function that finds the index of the id faculty\n"));
    return -1;
}

int findCourseIndexById(int id) {
    for (int i = 0; i < course_count; i++) {
        if (courses[i].id == id && courses[i].id != -1) return i;
    }
    return -1;
}

int findEnrollIndexByStudentCourse(int studentId, int courseId) {
    for (int i = 0; i < enrollment_count; i++) {
        if (enrollments[i].studentId == studentId && enrollments[i].courseId == courseId)
            return i;
    }
    return -1;
}

void trimWhitespace(char *str) {
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }
}



int verifyAdmin(int id, const char *password) {
    write(STDOUT_FILENO,"Entered the function to verify the admin\n",strlen("Entered the function to verify the admin\n"));
    write(STDOUT_FILENO,"The admin id is ",strlen("The admin id is "));
    char temp[BUFFER_SIZE];
    memset(temp, 0, sizeof(temp));
    //print the password using temp
    sprintf(temp, "%s\n", password);
    write(STDOUT_FILENO,temp,strlen(temp));
    return (id == adminId && strcmp(password, adminPassword) == 0);
}

// Function to verify Faculty credentials
int verifyFaculty(int id, const char *password) {
    sem_wait(&file_mutex);
    int fd = open(facultyFile, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open faculty file");
        sem_post(&file_mutex);
        return 0;
    }

    write(STDOUT_FILENO,"The faculty file is opened\n",strlen("The faculty file is opened\n"));
    faculty f;
    char temp[BUFFER_SIZE];
    // sprintf(temp,"The id is %d and the password is %s\n",id,password);
    // write(STDOUT_FILENO,temp,strlen(temp));
    // memset(temp,0,sizeof(temp));
    // sprintf(temp, "Length of password = %ld, Length of password = %ld\n", strlen(f.password), strlen(password));
    // write(STDOUT_FILENO, temp, strlen(temp));
    // memset(temp,0,sizeof(temp));
    while (read(fd, &f, sizeof(faculty)) > 0) {
        // sprintf("The faculty id is %d and the password is %s\n",f.id,f.password);
        trimWhitespace(f.password);
    //     sprintf(temp,"The faculty id is %d and the password is %s\n",f.id,f.password);
    //     write(STDOUT_FILENO,temp,strlen(temp));
    // memset(temp,0,sizeof(temp));
    //     sprintf(temp, "Length of f.password = %ld, Length of password = %ld\n", strlen(f.password), strlen(password));
    // write(STDOUT_FILENO, temp, strlen(temp));
        if (f.id == id && strcmp(f.password, password) == 0) {
            close(fd);
            sem_post(&file_mutex);
            write(STDOUT_FILENO,"The faculty is verified\n",strlen("The faculty is verified\n"));
            return 1; // Match found
        }
    }

    close(fd);
    sem_post(&file_mutex);
    return 0; // No match found
}

// Function to verify Student credentials
int verifyStudent(int id, const char *password) {
    sem_wait(&file_mutex);
    int fd = open(studentFile, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open student file");
        sem_post(&file_mutex);
        return 0;
    }

    student s;
    while (read(fd, &s, sizeof(student)) > 0) {
        trimWhitespace(s.password);
        if (s.id == id && strcmp(s.password, password) == 0 && s.active == 1) {
            close(fd);
            sem_post(&file_mutex);
            return 1; // Match found
        }
    }

    close(fd);
    sem_post(&file_mutex);
    return 0; // No match found
}

int verifyFacultyByEmail(const char *email, const char *password) {
    sem_wait(&file_mutex);
    for (int i = 0; i < faculty_count; i++) {
        if (strcmp(faculties[i].email, email) == 0 && strcmp(faculties[i].password, password) == 0) {
            sem_post(&file_mutex);
            return 1;
        }
    }
    sem_post(&file_mutex);
    return 0;
}

int verifyStudentByEmail(const char *email, const char *password) {
    sem_wait(&file_mutex);
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].email, email) == 0 && strcmp(students[i].password, password) == 0 && students[i].active == 1) {
            sem_post(&file_mutex);
            return 1;
        }
    }
    sem_post(&file_mutex);
    return 0;
}
// Function to handle login requests
void handleLogin(int client_sock, char *request) {
    char response[BUFFER_SIZE];
    char role[10],email[50], password[30];        
    int id;

    // Parse the login request
    if (strncmp(request, "LOGIN|ADMIN", 11) == 0) {
        sscanf(request, "LOGIN|ADMIN|%d|%s", &id, password);
        printf("Admin login request received for ID: %d\n", id);
        //printing the request
        write(STDOUT_FILENO, "Admin login request received is :\n", strlen("Admin login request received is :\n")); 
        write(STDOUT_FILENO,request,strlen(request));
        //print the password:
        write(STDOUT_FILENO,"The password is: ",strlen("The password is: "));
        write(STDOUT_FILENO,password,strlen(password));
        if (verifyAdmin(id, password)) {
            strcpy(response, "SUCCESS");
        } else {
            strcpy(response, "FAILURE");
        }
        write(STDOUT_FILENO, "Admin login request processed.\n", strlen("Admin login request processed.\n"));
    } else if (strncmp(request, "LOGIN|FACULTY", 13) == 0) {
        sscanf(request, "LOGIN|FACULTY|%[^|]|%s", email, password);
        if (verifyFacultyByEmail(email, password)) {
            strcpy(response, "SUCCESS");
        } else {
            strcpy(response, "FAILURE");
        }
        write(STDOUT_FILENO, "Faculty login request processed.\n", strlen("Faculty login request processed.\n"));
    } else if (strncmp(request, "LOGIN|STUDENT", 13) == 0) {
        sscanf(request, "LOGIN|STUDENT|%[^|]|%s", email, password);
        if (verifyStudentByEmail(email, password)) {
            strcpy(response, "SUCCESS");
        } else {
            strcpy(response, "FAILURE");
        }
        write(STDOUT_FILENO, "Student login request processed.\n", strlen("Student login request processed.\n"));
    } else {
        strcpy(response, "FAILURE");
        write(STDOUT_FILENO, "Login request processed with failure.\n", strlen("Login request processed with failure.\n"));
    }

    // Send the response back to the client
    send(client_sock, response, strlen(response), 0);
}


void handleClient(int client_sock) {
    char buffer[BUFFER_SIZE];
    int bytes_read;
    loadAllData(); // Load all data from files at the start
    while ((bytes_read = read(client_sock, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0'; // Null-terminate the received string
        // printf("Received: %s\n", buffer);

        // Check if the request is a login request
        if (strncmp(buffer, "LOGIN", 5) == 0) {
            handleLogin(client_sock, buffer); // Call the login handler
        } else if (strncmp(buffer, "ADMIN", 5) == 0) {
            handleAdmin(client_sock, buffer); // Handle admin operations
        } else if (strncmp(buffer, "FACULTY", 7) == 0) {
            handleFaculty(client_sock, buffer); // Handle faculty operations
        } else if (strncmp(buffer, "STUDENT", 7) == 0) {
            handleStudent(client_sock, buffer); // Handle student operations
        } else {
            char response[] = "Invalid request format";
            send(client_sock, response, strlen(response), 0);
        }
        memset(buffer, 0, sizeof(buffer));
    }

    if (bytes_read == 0) {
        // printf("Client disconnected.\n");
        write(STDOUT_FILENO, "Client disconnected.\n", 20);
    } else if (bytes_read < 0) {
        perror("Read error");
    }

    close(client_sock);
}
// Utility functions for file operations
int getNextFacultyId() {
    int maxId = 0;
    for (int i = 0; i < faculty_count; i++) {
        if (faculties[i].id > maxId) maxId = faculties[i].id;
    }
    return maxId + 1;
}

int getNextStudentId() {
    int maxId = 0;
    for (int i = 0; i < student_count; i++) {
        if (students[i].id > maxId) maxId = students[i].id;
    }
    return maxId + 1;
}


void appendToFile(const char *filename, void *data, size_t size) {
    sem_wait(&file_mutex);
    // Identify which array to append to based on filename
    if (strcmp(filename, studentFile) == 0) {
        if (student_count < MAX_STUDENTS) {
            students[student_count++] = *(student*)data;
        }
    } else if (strcmp(filename, facultyFile) == 0) {
        if (faculty_count < MAX_FACULTY) {
            faculties[faculty_count++] = *(faculty*)data;
        }
    } else if (strcmp(filename, courseFile) == 0) {
        if (course_count < MAX_COURSES) {
            courses[course_count++] = *(course*)data;
        }
    } else if (strcmp(filename, enrollFile) == 0) {
        if (enrollment_count < MAX_ENROLLMENTS) {
            enrollments[enrollment_count++] = *(enroll*)data;
        }
    }
    // After updating the array, save all data back to files
    saveAllData();
    sem_post(&file_mutex);
}

void readFromFile(const char *filename, void *buffer, size_t size, int index) {
    sem_wait(&file_mutex);
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("File open failed");
        sem_post(&file_mutex);
        return;
    }
    lseek(fd, index * size, SEEK_SET);
    read(fd, buffer, size);
    close(fd);
    sem_post(&file_mutex);
}

void updateFile(const char *filename, void *data, size_t size, int index) {
    sem_wait(&file_mutex);

    if (strcmp(filename, studentFile) == 0) {
        if (index >= 0 && index < student_count) {
            students[index] = *(student*)data;
        }
    } else if (strcmp(filename, facultyFile) == 0) {
        if (index >= 0 && index < faculty_count) {
            faculties[index] = *(faculty*)data;
        }
    } else if (strcmp(filename, courseFile) == 0) {
        if (index >= 0 && index < course_count) {
            courses[index] = *(course*)data;
        }
    } else if (strcmp(filename, enrollFile) == 0) {
        if (index >= 0 && index < enrollment_count) {
            enrollments[index] = *(enroll*)data;
        }
    }

    saveAllData(); // Write all arrays back to files
    sem_post(&file_mutex);
}

// Admin operations
void handleAdmin(int client_sock, char *request) {
    char response[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    int choice;

    memset(response, 0, sizeof(response)); // Clear response buffer
    sscanf(request, "ADMIN|CHOICE|%d", &choice);
    switch (choice) {
        case 1: { // Add Student
            student s;
            read(client_sock, &s, sizeof(student));
            s.id = getNextStudentId();
            appendToFile(studentFile, &s, sizeof(student));
            sprintf(response, "Student added successfully: %s\n", s.name);
            break;
        }
        case 2: { // Add Faculty
            faculty f;
            read(client_sock, &f, sizeof(faculty));
            f.id = getNextFacultyId();
            appendToFile(facultyFile, &f, sizeof(faculty));
            sprintf(response, "Faculty added successfully: %s\n", f.name);
            break;
        }
        case 3: { // Activate/Deactivate Student
            int studentId, status;
            read(client_sock, &studentId, sizeof(int));
            read(client_sock, &status, sizeof(int));
            student s;
            int idx = findStudentIndexById(studentId);
            if (idx != -1) {
                students[idx].active = status;
                saveAllData();
                sprintf(response, "Student %d status updated to %d\n", studentId, status);
            } else {
                sprintf(response, "Student not found\n");
            }
            break;
        }
        case 4:{ //Update Student Name
            // printf("Entered the function to update the student name\n");
            sem_wait(&file_mutex);
            write(STDOUT_FILENO,"Entered the function to update the student name\n",strlen("Entered the function to update the student name\n"));
            int id;
            char newName[30];
            read(client_sock, &id, sizeof(int));
            read(client_sock, newName, sizeof(newName));
            student s;
            //print the count of the total number of students
            char temp[BUFFER_SIZE];
            memset(temp, 0, sizeof(temp));
            sprintf(temp, "Total number of students: %d\n", student_count);
            write(STDOUT_FILENO, temp, strlen(temp));
            int idx = findStudentIndexById(id);
            if (idx != -1) {
                strcpy(students[idx].name, newName);
                saveAllData();
                write(STDOUT_FILENO,students[idx].name,strlen(students[idx].name));
                sprintf(response, "Student %d name updated to %s\n", id, students[idx].name);
            } else {
                sprintf(response, "Student not found\n");
            }
            sem_post(&file_mutex);  
            // readFromFile(studentFile, &s, sizeof(student), id - 1);
            // sprintf(response, "Student %d name updated to %s okay so what the prob", id, s.name);
            break;
        }
        case 5:{ // View All Students
            write(STDOUT_FILENO,"Entered the function to view all the students\n",strlen("Entered the function to view all the students\n"));
            //here write me the code to get all the students is and name in a string that I can return to the client 
            sem_wait(&file_mutex);
            int fd = open(studentFile, O_RDONLY);
            if (fd < 0) {
                sprintf(response, "Error opening students file\n");
                break;
            }
            student s;
            // char response[BUFFER_SIZE] = "";
            response[0] = '\0'; // Initialize response string
            while (read(fd, &s, sizeof(student)) == sizeof(student)) {
                if (s.active == 1) {
                    char line[128];
                    sprintf(line, "Student ID: %d | Name: %s\n", s.id, s.name);
                    strncat(response, line, sizeof(response) - strlen(response) - 1);
                }
            }
            close(fd);
            sem_post(&file_mutex);
            if (strlen(response) == 0)
                strcpy(response, "No students found.\n");
            
            break;
        }
        case 6: { // View All faculty
            //now we do teh same as above to print all the faculty
            sem_wait(&file_mutex);
            int fd = open(facultyFile, O_RDONLY);
            if (fd < 0) {
                sprintf(response, "Error opening faculty file\n");
                break;
            }
            faculty f;
            response[0] = '\0'; // Initialize response string
            while (read(fd, &f, sizeof(faculty)) == sizeof(faculty)) {
                char line[128];
                sprintf(line, "Faculty ID: %d | Name: %s\n", f.id, f.name);
                strncat(response, line, sizeof(response) - strlen(response) - 1);
            }
            close(fd);
            sem_post(&file_mutex);
            if (strlen(response) == 0)
                strcpy(response, "No faculty found.\n");
            break;
        }
        case 7: { // Remove Student
            int studentId;
            read(client_sock, &studentId, sizeof(int));
            int idx = findStudentIndexById(studentId);
            if (idx != -1) {
                removeStudentByIndex(idx); // Remove from array
                saveAllData();              // Save updated array to file
                sprintf(response, "Student %d removed successfully\n", studentId);
            } else {
                sprintf(response, "Student not found\n");
            }
            break;
        }
        case 8: { // Remove Faculty
            int facultyId;
            read(client_sock, &facultyId, sizeof(int));
            int idx = findFacultyIndexById(facultyId);
            if (idx != -1) {
                removeFacultyByIndex(idx); // Mark as inactive
                saveAllData();              // Save updated array to file
                sprintf(response, "Faculty %d removed successfully\n", facultyId);
            } else {
                sprintf(response, "Faculty not found\n");
            }
            break;
        }
        case 9: { // View All Courses
            sem_wait(&file_mutex);
            int fd = open(courseFile, O_RDONLY);
            if (fd < 0) {
                sprintf(response, "Error opening courses file\n");
                break;
            }
            course c;
            response[0] = '\0'; // Initialize response string
            while (read(fd, &c, sizeof(course)) == sizeof(course)) {
                char line[128];
                sprintf(line, "Course ID: %d | Name: %s | Credits: %d | Enrolled: %d\n", c.id, c.name, c.credits, c.numEnroll);
                strncat(response, line, sizeof(response) - strlen(response) - 1);
            }
            close(fd);
            sem_post(&file_mutex);
            if (strlen(response) == 0)
                strcpy(response, "No courses found.\n");
            break;
        }
        case 10: { // Remove Course
            int courseId;
            read(client_sock, &courseId, sizeof(int));
            int idx = findCourseIndexById(courseId);    
            if (idx != -1) {
                removeCourseByIndex(idx);   // Remove from array
                saveAllData();              // Save updated array to file
                sprintf(response, "Course %d removed successfully\n", courseId);
            } else {
                sprintf(response, "Course not found\n");
            }   
            break;            
        }
        case 11: { // View All Enrollments
            sem_wait(&file_mutex);
            int fd = open(enrollFile, O_RDONLY);
            if (fd < 0) {
                sprintf(response, "Error opening enrollments file\n");
                break;
            }
            enroll e;
            response[0] = '\0'; // Initialize response string
            while (read(fd, &e, sizeof(enroll)) == sizeof(enroll)) {
                char line[128];
                sprintf(line, "Student ID: %d | Course ID: %d\n", e.studentId, e.courseId);
                strncat(response, line, sizeof(response) - strlen(response) - 1);
            }
            close(fd);
            sem_post(&file_mutex);
            if (strlen(response) == 0)
                strcpy(response, "No enrollments found.\n");
            break;
        }
        case 12: { // Change Admin Password
            char newPassword[30];
            read(client_sock, newPassword, sizeof(newPassword));
            // Here we would update the admin password in a secure way
            // For simplicity, we just print it
            sprintf(response, "Admin password changed successfully to: %s\n", newPassword);
            break;
        }

        default:
            sprintf(response, "Invalid admin choice\n");
    }
    send(client_sock, response, strlen(response), 0);
    return 0;
}

// Faculty operations
void handleFaculty(int client_sock, char *request) {
    char response[BUFFER_SIZE];
    memset(response, 0, sizeof(response)); // Clear response buffer
    int choice;
    int intBuffer[BUFFER_SIZE];

    sscanf(request, "FACULTY|CHOICE|%d", &choice);

    switch (choice) {
        case 1: { // Add Course
            course c;
            read(client_sock, &c, sizeof(course));
            appendToFile(courseFile, &c, sizeof(course));
            sprintf(response, "Course added successfully: %s\n", c.name);
            break;
        }
        case 2: { // Remove Course
            int courseId;
            read(client_sock, intBuffer, sizeof(intBuffer));
            courseId=atoi(intBuffer);
            course c;
            int idx = findCourseIndexById(courseId);
            if (idx != -1) {
                removeCourseByIndex(idx);   // Remove from array
                saveAllData();              // Save updated array to file
                sprintf(response, "Course %d removed successfully\n", courseId);
            } else {
                sprintf(response, "Course not found\n");
            }
            break;
        }
        case 3: { // View Enrollments
            int facultyId;
            // char 
            read(client_sock, &facultyId, sizeof(int));
            // facultyId=atoi(intBuffer);
            course c;
            sprintf(response,"");
            sem_wait(&file_mutex);
            int fd = open(courseFile, O_RDONLY);
            if (fd < 0) {
                sprintf(response, "Error opening courses file\n");
                break;
            }
            char temp[BUFFER_SIZE];
            write(STDOUT_FILENO,"entered the function\n",strlen("entered the function\n"));
            while (read(fd, &c, sizeof(course)) == sizeof(course)) {
                memset(temp, 0, sizeof(temp));
                sprintf(temp,"c.facultyId: %d facultyId: %d\n",c.facultyId,facultyId);
                write(STDOUT_FILENO,temp,strlen(temp));
                if (c.facultyId == facultyId) {
                    char line[128];
                    sprintf(line, "Course ID: %d | Name: %s\n", c.id, c.name);
                    strncat(response, line, sizeof(response) - strlen(response) - 1);
                }
            }
            close(fd);
            sem_post(&file_mutex);
            if (strlen(response) == 0){
                strcpy(response, "No courses found for this faculty.\n");
                char buffer[32];
                sprintf(buffer, "Length of response: %ld\n", strlen(response));
                write(STDOUT_FILENO, buffer, strlen(buffer));
                // write(STDOUT_FILENO,strlen(response),sizeof(int));
                // char buffer[32];
                memset(buffer, 0, sizeof(buffer));
                sprintf(buffer, "Length of response: %ld\n", strlen("No courses found for this faculty.\n"));
                write(STDOUT_FILENO, buffer, strlen(buffer));
                
                // write(STDOUT_FILENO,strlen("No courses found for this faculty.\n"),sizeof(int));
            }
            
            break;
        }
        case 4: { // Change Password
            write(STDOUT_FILENO,"Entered the function to change the password\n",strlen("Entered the function to change the password\n"));
            int facultyId;
            char newPassword[30];
            read(client_sock, &facultyId, sizeof(int));
            // facultyId=atoi(intBuffer);
            char temp[BUFFER_SIZE];         
            memset(temp, 0, sizeof(temp));
            sprintf(temp, "%d\n", facultyId);
            write(STDOUT_FILENO, temp, strlen(temp));
            int idx = findFacultyIndexById(facultyId);
            read(client_sock, newPassword, sizeof(newPassword));
            write(STDOUT_FILENO,"The faculty id is ",strlen("The faculty id is "));
            write(STDOUT_FILENO,"The new password is ",strlen("The new password is "));
            memset(temp, 0, sizeof(temp));
            sprintf(temp, "%s\n", newPassword);
            write(STDOUT_FILENO, temp, strlen(temp));
            // print the faculty id and password
            if (idx != -1) {
                write(STDOUT_FILENO,"The faculty id is found\n",strlen("The faculty id is found\n"));
                strcpy(faculties[idx].password, newPassword);
                write(STDOUT_FILENO,"The faculty password is changed\n",strlen("The faculty password is changed\n"));
                saveAllData();
                sprintf(response, "Faculty %d password updated successfully\n", facultyId);
            } else {
                sprintf(response, "Faculty not found\n");
            }
            break;
        }
        case 5:{ // View All Students
            //here write me the code to get all the students is and name in a string that I can return to the client 
            sem_wait(&file_mutex);
            int fd = open(studentFile, O_RDONLY);
            if (fd < 0) {
                sprintf(response, "Error opening students file\n");
                break;
            }
            student s;
            // char response[BUFFER_SIZE] = "";
            memset(response, 0, sizeof(response));
            response[0] = '\0'; // Initialize response string
            while (read(fd, &s, sizeof(student)) == sizeof(student)) {
                if (s.active == 1) {
                    char line[128];
                    memset(line, 0, sizeof(line));
                    sprintf(line, "Student ID: %d | Name: %s\n", s.id, s.name);
                    strncat(response, line, sizeof(response) - strlen(response) - 1);
                }
            }
            close(fd);
            sem_post(&file_mutex);
            if (strlen(response) == 0)
                strcpy(response, "No students found.\n");
            
            break;
        }
        case 6:{
            //view all the courses
            sem_wait(&file_mutex);
            int fd = open(courseFile, O_RDONLY);
            if (fd < 0) {
                sprintf(response, "Error opening courses file\n");
                break;
            }
            course c;
            memset(response, 0, sizeof(response));
            response[0] = '\0'; // Initialize response string
            while (read(fd, &c, sizeof(course)) == sizeof(course)) {
                char line[128];
                memset(line, 0, sizeof(line));
                sprintf(line, "Course ID: %d | Name: %s | Credits: %d | Enrolled: %d\n", c.id, c.name, c.credits, c.numEnroll);
                strncat(response, line, sizeof(response) - strlen(response) - 1);
            }
            close(fd);
            sem_post(&file_mutex);
            if (strlen(response) == 0)
                strcpy(response, "No courses found.\n");
            break;
        }
        default:
            sprintf(response, "Invalid faculty choice\n");
    }

    send(client_sock, response, strlen(response), 0);
}

// Student operations
void handleStudent(int client_sock, char *request) {
    char response[BUFFER_SIZE];
    int choice;
    // int intBuffer[BUFFER_SIZE];

    sscanf(request, "STUDENT|CHOICE|%d", &choice);

    switch (choice) {
        case 1: { // Enroll in Course
            enroll e;
            read(client_sock, &e, sizeof(enroll));
            appendToFile(enrollFile, &e, sizeof(enroll));
            sprintf(response, "Enrolled in course %d successfully\n", e.courseId);
            break;
        }
        case 2: { // Unenroll from Course
            int studentId, courseId;
            read(client_sock, &studentId, sizeof(int));
            read(client_sock, &courseId, sizeof(int));
            enroll e;
            int idx = findEnrollIndexByStudentCourse(studentId, courseId);
            if (idx != -1) {
                removeEnrollmentByIndex(idx); // Remove from array
                saveAllData();
                sprintf(response, "Unenrolled from course %d successfully\n", courseId);
            } else {
                sprintf(response, "Enrollment not found\n");
            }
            break;
        }
        case 3: { // View Enrolled Courses
            int studentId;
            read(client_sock, &studentId, sizeof(int));
            enroll e;
            char response[BUFFER_SIZE] = "";
            sem_wait(&file_mutex);
            int fd = open(enrollFile, O_RDONLY);
            if (fd < 0) {
                sprintf(response, "Error opening enrollments file\n");
                break;
            }
            while (read(fd, &e, sizeof(enroll)) == sizeof(enroll)) {
                if (e.studentId == studentId) {
                    char line[128];
                    sprintf(line, "Enrolled in course ID: %d\n", e.courseId);
                    strncat(response, line, sizeof(response) - strlen(response) - 1);
                }
            }
            close(fd);
            sem_post(&file_mutex);
            if (strlen(response) == 0)
                strcpy(response, "No courses found for this student.\n");
            break;
        }
        case 4: { // Change Password
            // int studentId;
            // char newPassword[30];
            // read(client_sock, &studentId, sizeof(int));
            // read(client_sock, newPassword, sizeof(newPassword));
            // student s;
            // readFromFile(studentFile, &s, sizeof(student), studentId - 1);
            // strcpy(s.password, newPassword);
            // updateFile(studentFile, &s, sizeof(student), studentId - 1);
            // sprintf(response, "Student %d password updated successfully", studentId);
            // break;
            // printf("Entered the function to update the student name\n");
            // int id;
            // char newName[30];
            // read(client_sock, &id, sizeof(int));
            // read(client_sock, newName, sizeof(newName));

            // sem_wait(&file_mutex);
            // int fd = open(studentFile, O_RDWR); // Open the file for reading and writing
            // if (fd < 0) {
            //     perror("Error opening students file");
            //     write(STDERR_FILENO, "Error opening students file\n", 29);
            //     sprintf(response, "Error opening students file\n");
            //     break;
            // }

            // student s;
            // int found = 0;
            // off_t offset = 0;

            // // Search for the student by ID
            // while (read(fd, &s, sizeof(student)) == sizeof(student)) {
            //     if (s.id == id) {
            //         found = 1;
            //         break;
            //     }
            //     offset += sizeof(student); // Keep track of the current offset
            // }

            // if (found) {
            //     // printf("The old name of the student is %s\n", s.name);
            //     strcpy(s.name, newName); // Update the name
            //     // printf("The new name of the student is %s\n", s.name);

            //     // Write the updated record back to the file
            //     lseek(fd, offset, SEEK_SET); // Move to the correct position
            //     write(fd, &s, sizeof(student));

            //     sprintf(response, "Student %d name updated to %s\n", id, s.name);
            // } else {
            //     sprintf(response, "Student with ID %d not found\n", id);
            // }

            // close(fd);
            // sem_post(&file_mutex);
            // break;
                int id;
                char newPassword[30];
                read(client_sock, &id, sizeof(int));
                read(client_sock, newPassword, sizeof(newPassword));
                newPassword[sizeof(newPassword)-1] = '\0';
                trimWhitespace(newPassword);
                sem_wait(&file_mutex);
                int idx = findStudentIndexById(id);
                if (idx != -1) {
                    strcpy(students[idx].password, newPassword);
                    write(STDOUT_FILENO,students[idx].password,strlen(students[idx].password));
                    saveAllData();
                    //print the value of idx
                    char temp[BUFFER_SIZE];
                    memset(temp, 0, sizeof(temp));
                    sprintf(temp, "The idx is %d\n", idx);
                    write(STDOUT_FILENO, temp, strlen(temp));
                    write(STDOUT_FILENO,newPassword,strlen(newPassword));
                    sprintf(response, "Student %d password updated successfully\n", id);
                } else {
                    sprintf(response, "Student with ID %d not found\n", id);
                }
                sem_post(&file_mutex);
                break;
        }
        default:
            sprintf(response, "Invalid student choice\n");
    }

    send(client_sock, response, strlen(response), 0);
}

// Client handler
// void handleClient(int client_sock) {
//     char buffer[BUFFER_SIZE];
//     int bytes_read;
//     while ((bytes_read = read(client_sock, buffer, sizeof(buffer) - 1)) > 0) {
//         buffer[bytes_read] = '\0'; // Null-terminate the received string
//         printf("Received: %s\n", buffer);
//         if (strncmp(buffer, "ADMIN", 5) == 0) {
//             handleAdmin(client_sock, buffer);
//         } else if (strncmp(buffer, "FACULTY", 7) == 0) {
//             handleFaculty(client_sock, buffer);
//         } else if (strncmp(buffer, "STUDENT", 7) == 0) {
//             handleStudent(client_sock, buffer);
//         } else {
//             char response[] = "Invalid request format";
//             send(client_sock, response, strlen(response), 0);
//         }
//     }
//     if (bytes_read == 0) {
//         printf("Client disconnected.\n");
//     } else if (bytes_read < 0) {
//         perror("Read error");
//     }
//     close(client_sock);
// }

void* handleClientThread(void *arg) {
    int client_sock = *(int*)arg;
    free(arg);

    handleClient(client_sock);

    return NULL;
}

// Main function
int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    sem_init(&file_mutex, 0, 1); // 1 means unlocked (binary semaphore)
    // Create socket
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        // perror("Bind failed");
        write(STDERR_FILENO, "Bind failed\n", 12);
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_sock, 5) < 0) {
        // perror("Listen failed");
        write(STDERR_FILENO, "Listen failed\n", 14);
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Accept and handle clients
    while (1) {
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_len);
        if (client_sock < 0) {
            write(STDERR_FILENO, "Accept failed\n", 14);
            continue;
        }

        printf("Client connected.\n");

        int *pclient = malloc(sizeof(int));
        *pclient = client_sock;
        pthread_t tid;
        pthread_create(&tid, NULL, handleClientThread, pclient);
        pthread_detach(tid); // Auto-cleanup
    }

    close(server_sock);
    sem_destroy(&file_mutex);
    return 0;
}