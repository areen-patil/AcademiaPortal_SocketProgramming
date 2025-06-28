#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 100000

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

int adminClient(int sock) {  
    // printf("\n--- Admin Menu ---\n");
    write(STDOUT_FILENO,"\n--- Admin Menu ---\n",strlen("\n--- Admin Menu ---\n"));
    while (1) {
        char request[BUFFER_SIZE], response[BUFFER_SIZE];
        char buffer[BUFFER_SIZE];
        char intBuffer[BUFFER_SIZE];
        int choice;

        // printf("1. Add Student\n2. Add Faculty\n3. Activate/Deactivate Student\n4. Update Details\n5.View Students\n6.View Faculty\n7. Exit\n");
        write(STDOUT_FILENO,"1. Add Student\n2. Add Faculty\n3. Activate/Deactivate Student\n4. Update Details\n5.View Students\n6.View Faculty\n7. Exit\n",strlen("1. Add Student\n2. Add Faculty\n3. Activate/Deactivate Student\n4. Update Details\n5.View Students\n6.View Faculty\n7. Exit\n"));
        // printf("Enter your choice: ");
        write(STDOUT_FILENO,"Enter your choice: ",strlen("Enter your choice: "));
        // scanf("%d", &choice);
        read(STDIN_FILENO,intBuffer,sizeof(intBuffer));
        choice = atoi(intBuffer);
        memset(response, 0, sizeof(response));
        memset(request, 0, sizeof(request));
        // if (choice == 5) {
        //     printf("Exiting Admin Menu...\n");
        //     break;
        // }

        if (choice == 1) { // Add Student
            student s;
            // printf("Enter Student ID: ");
            // write(STDOUT_FILENO,"Enter Student ID: ",strlen("Enter Student ID: "));
            // // scanf("%d", &s.id);
            // read(STDIN_FILENO,intBuffer,sizeof(intBuffer));
            // s.id=atoi(intBuffer);
            // printf("Enter Student Name: ");
            write(STDOUT_FILENO,"Enter Student Email: ",strlen("Enter Student Email: "));
            read(STDIN_FILENO,s.email,sizeof(s.email));
            s.email[strcspn(s.email, "\n")] = '\0'; // Remove newline
            write(STDOUT_FILENO,"Enter Student Name: ",strlen("Enter Student Name: "));
            // scanf("%s", s.name);
            read(STDIN_FILENO,s.name,sizeof(s.name));
            s.name[strcspn(s.name, "\n")] = '\0'; // Remove newline
            // printf("Enter Student Password: ");
            write(STDOUT_FILENO,"Enter Student Password: ",strlen("Enter Student Password: "));
            // scanf("%s", s.password);
            read(STDIN_FILENO,s.password,sizeof(s.password));
            s.password[strcspn(s.password, "\n")] = '\0'; // Remove newline
            s.active = 1; // By default, the student is active

            // Send the request to the server
            sprintf(request, "ADMIN|CHOICE|1");
            send(sock, request, strlen(request), 0);
            send(sock, &s, sizeof(student), 0); // Send the student struct

            // Wait for the server's response
            memset(response, 0, sizeof(response));
            read(sock, response, sizeof(response));
            // printf("Server Response: %s\n", response);
            write(STDOUT_FILENO,"Server Response:\n",strlen("Server Response:\n"));
            write(STDOUT_FILENO,response,strlen(response));
        } 
        else if(choice==2){
            faculty f;
            // printf("Enter Faculty ID: ");
            // write(STDOUT_FILENO,"Enter Faculty ID: ",strlen("Enter Faculty ID: "));
            // // scanf("%d", &f.id);
            // read(STDIN_FILENO,intBuffer,sizeof(intBuffer));
            // f.id=atoi(intBuffer);
            // printf("Enter Faculty Name: ");
            write(STDOUT_FILENO,"Enter Faculty Email: ",strlen("Enter Faculty Email: "));
            read(STDIN_FILENO,f.email,sizeof(f.email));
            f.email[strcspn(f.email, "\n")] = '\0'; // Remove newline
            write(STDOUT_FILENO,"Enter Faculty Name: ",strlen("Enter Faculty Name: "));
            // scanf("%s", f.name);
            read(STDIN_FILENO,f.name,sizeof(f.name));
            f.name[strcspn(f.name, "\n")] = '\0'; // Remove newline
            // printf("Enter Faculty Password: ");
            write(STDOUT_FILENO,"Enter Faculty Password: ",strlen("Enter Faculty Password: "));
            // scanf("%s", f.password);
            read(STDIN_FILENO,f.password,sizeof(f.password));
            f.password[strcspn(f.password, "\n")] = '\0'; // Remove newline

            // Send the request to the server
            sprintf(request, "ADMIN|CHOICE|2");
            send(sock, request, strlen(request), 0);
            send(sock, &f, sizeof(faculty), 0); // Send the faculty struct

            // Wait for the server's response
            memset(response, 0, sizeof(response));
            read(sock, response, sizeof(response));
            // printf("Server Response: %s\n", response);
            write(STDOUT_FILENO,"Server Response:\n",strlen("Server Response:\n"));
            write(STDOUT_FILENO,response,strlen(response));
        }
        else if (choice == 3) { // Activate/Deactivate Student
            int studentId, status;
            // printf("Enter Student ID: ");
            write(STDOUT_FILENO,"Enter Student ID: ",strlen("Enter Student ID: "));
            // scanf("%d", &studentId);
            read(STDIN_FILENO,intBuffer,sizeof(intBuffer));
            studentId=atoi(intBuffer);
            // printf("Enter Status (1 for active, 0 for inactive): ");
            write(STDOUT_FILENO,"Enter Status (1 for active, 0 for inactive): ",strlen("Enter Status (1 for active, 0 for inactive): "));
            // scanf("%d", &status);
            read(STDIN_FILENO,intBuffer,sizeof(intBuffer));
            status=atoi(intBuffer);


            // Send the request to the server
            sprintf(request, "ADMIN|CHOICE|3");
            send(sock, request, strlen(request), 0);
            send(sock,&studentId,sizeof(int),0);
            send(sock,&status,sizeof(int),0); // Send the studentId and status

            // Wait for the server's response
            memset(response, 0, sizeof(response));
            read(sock, response, sizeof(response));
            // printf("Server Response: %s\n", response);
            // sprintf(buffer, "The student with id %d is now %s\n", studentId, status == 1 ? "active" : "inactive");
            write(STDOUT_FILENO,response,strlen(response));
        }
        else if(choice==4){
            int id;
            char newName[30];
            
            // printf("Enter ID to update: ");
            write(STDOUT_FILENO,"Enter ID to update: ",strlen("Enter ID to update: "));
            // scanf("%d", &id);
            read(STDIN_FILENO,intBuffer,sizeof(intBuffer));
            id=atoi(intBuffer);
            // printf("Enter new name: ");
            write(STDOUT_FILENO,"Enter new name: ",strlen("Enter new name: "));
            // scanf("%s", newName);
            read(STDIN_FILENO,newName,sizeof(newName));
            newName[strcspn(newName, "\n")] = '\0'; // Remove newline
            // Send the request to the server
            sprintf(request, "ADMIN|CHOICE|4");
            send(sock, request, strlen(request), 0);
            send(sock,&id,sizeof(int),0);
            // printf("THE NEW NAME TO BE SENT IS: %s",newName);
            char temp[BUFFER_SIZE];
            sprintf(temp, "The new name to be sent is: %s\n", newName);
            write(STDOUT_FILENO,temp,strlen(temp));
            send(sock,newName,strlen(newName),0); // Send the new name
            // printf("info sent to the server\n");
            // Wait for the server's response
            memset(response, 0, sizeof(response));
            read(sock, response, sizeof(response));
            // printf("Server Response: %s\n", response);
            write(STDOUT_FILENO,"Server Response:\n",strlen("Server Response:\n"));
            write(STDOUT_FILENO,response,strlen(response));
        }
        else if(choice==5){ //View a List of all student with their student_id and name
            sprintf(request, "ADMIN|CHOICE|5");
            send(sock, request, strlen(request), 0);
            memset(response, 0, sizeof(response));
            read(sock, response, sizeof(response));
            // printf("List of all the students:\n %s\n", response);
            write(STDOUT_FILENO,"List of all the students:\n",strlen("List of all the students:\n"));
            write(STDOUT_FILENO,response,strlen(response));

        }
        else if(choice==6){  //View a List of all faculties with their student_id and name
            sprintf(request, "ADMIN|CHOICE|6");
            send(sock, request, strlen(request), 0);
            memset(response, 0, sizeof(response));
            read(sock, response, sizeof(response));
            // printf("List of all the faculty:\n %s\n", response);
            write(STDOUT_FILENO,"List of all the faculty:\n",strlen("List of all the faculty:\n"));
            write(STDOUT_FILENO,response,strlen(response));
        }
        else if(choice==7){
            // write(STDOUT_FILENO,"Exiting Admin Menu...\n",strlen("Exiting Admin Menu...\n"));
            return 1;
        }
        else {
            sprintf(request, "ADMIN|CHOICE|%d", choice);
            send(sock, request, strlen(request), 0);
            memset(response, 0, sizeof(response));
            read(sock, response, sizeof(response));
            // printf("Server Response: %s\n", response);
            write(STDOUT_FILENO,"Server Response:\n",strlen("Server Response:\n"));
            write(STDOUT_FILENO,response,strlen(response));
        }
    }
    return 0;
}

int facultyClient(int sock) {
    // printf("\n--- Faculty Menu ---\n");
    write(STDOUT_FILENO,"\n--- Faculty Menu ---\n",strlen("\n--- Faculty Menu ---\n"));
    while (1) {
        char request[BUFFER_SIZE], response[BUFFER_SIZE];
        int choice;
        char intBuffer[BUFFER_SIZE];

        // printf("1. Add Course\n2. Remove Course\n3. View Enrollments\n4. Change Password\n5.View Students\n6. Exit\n");
        write(STDOUT_FILENO,"1. Add Course\n2. Remove Course\n3. View Your Courses\n4. Change Password\n5.View Students\n6. View All Courses\n7.Exit\n",strlen("1. Add Course\n2. Remove Course\n3. View Your Courses\n4. Change Password\n5.View Students\n6. View All Courses\n7.Exit\n"));
        // printf("Enter your choice: ");
        write(STDOUT_FILENO,"Enter your choice: ",strlen("Enter your choice: "));
        // scanf("%d", &choice);
        read(STDIN_FILENO,intBuffer,sizeof(intBuffer));
        choice=atoi(intBuffer);

        // if (choice == 5) {
        //     printf("Exiting Faculty Menu...\n");
        //     break;
        // }


        if(choice==1){
            course c;
            // printf("Enter Course ID: ");
            int course_id;
            write(STDOUT_FILENO,"Enter Course ID: ",strlen("Enter Course ID: "));
            // scanf("%d", &c.id);
            read(STDIN_FILENO,intBuffer,sizeof(intBuffer));
            c.id=atoi(intBuffer);
            // printf("Enter Course Name: ");
            write(STDOUT_FILENO,"Enter Course Name: ",strlen("Enter Course Name: "));
            // scanf("%s", c.name);
            read(STDIN_FILENO,c.name,sizeof(c.name));
            c.name[strcspn(c.name, "\n")] = '\0'; // Remove newline
            // printf("Enter Faculty ID: ");
            write(STDOUT_FILENO,"Enter Faculty ID: ",strlen("Enter Faculty ID: "));
            // scanf("%d", &c.facultyId);
            read(STDIN_FILENO,intBuffer,sizeof(intBuffer));
            c.facultyId=atoi(intBuffer);
            // printf("Enter Credits: ");
            write(STDOUT_FILENO,"Enter Credits: ",strlen("Enter Credits: "));
            // scanf("%d", &c.credits);
            read(STDIN_FILENO,intBuffer,sizeof(intBuffer));
            c.credits=atoi(intBuffer);
            c.numEnroll = 0; // By default, no enrollments

            // Send the request to the server
            sprintf(request, "FACULTY|CHOICE|1");
            send(sock, request, strlen(request), 0);
            send(sock, &c, sizeof(course), 0); // Send the course struct

            // Wait for the server's response
            memset(response, 0, sizeof(response));
            read(sock, response, sizeof(response));
            // printf("Server Response: %s\n", response);
            write(STDOUT_FILENO,"Server Response:\n",strlen("Server Response:\n"));
            write(STDOUT_FILENO,response,strlen(response));
        }
        else if(choice==2){
            int courseId;
            // printf("Enter Course ID to remove: ");
            write(STDOUT_FILENO,"Enter Course ID to remove: ",strlen("Enter Course ID to remove: "));
            // scanf("%d", &courseId);
            read(STDIN_FILENO,&courseId,sizeof(int));

            // Send the request to the server
            sprintf(request, "FACULTY|CHOICE|2" );
            send(sock, request, strlen(request), 0);
            send(sock,&courseId,sizeof(int),0); // Send the courseId
            // Wait for the server's response
            memset(response, 0, sizeof(response));
            read(sock, response, sizeof(response));
            // printf("Server Response: %s\n", response);
            write(STDOUT_FILENO,"Server Response:\n",strlen("Server Response:\n"));
            write(STDOUT_FILENO,response,strlen(response));
        }
        else if (choice == 3) { // View Enrollments
            int facultyId;
            // printf("Enter Faculty ID: ");
            write(STDOUT_FILENO,"Enter Faculty ID: ",strlen("Enter Faculty ID: "));
            // scanf("%d", &facultyId);
            read(STDIN_FILENO,intBuffer,sizeof(intBuffer));
            facultyId=atoi(intBuffer);

            // Send the request to the server
            sprintf(request, "FACULTY|CHOICE|3");
            send(sock, request, strlen(request), 0);
            send(sock,&facultyId,sizeof(int),0); // Send the facultyId

            // Wait for the server's response
            memset(response, 0, sizeof(response));
            read(sock, response, sizeof(response));
            // printf("Server Response: %s\n", response);
            write(STDOUT_FILENO,"Server Response:\n",strlen("Server Response:\n"));
            write(STDOUT_FILENO,response,strlen(response));
        }
        else if(choice==4){
            int id;
            char newPassword[30];
            // printf("Enter Faculty ID to update password: ");
            write(STDOUT_FILENO,"Enter Faculty ID to update password: ",strlen("Enter Faculty ID to update password: "));
            // scanf("%d", &id);
            read(STDIN_FILENO,intBuffer,sizeof(intBuffer));
            id=atoi(intBuffer);
            // printf("Enter new password: ");
            write(STDOUT_FILENO,"Enter new password: ",strlen("Enter new password: "));
            // scanf("%s", newPassword);
            read(STDIN_FILENO,newPassword,sizeof(newPassword));
            newPassword[strcspn(newPassword, "\n")] = '\0'; // Remove newline
            // Send the request to the server
            sprintf(request, "FACULTY|CHOICE|4");
            send(sock, request, strlen(request), 0);
            // Wait for the server's response
            write(STDOUT_FILENO,newPassword,strlen(newPassword));
            send(sock, &id, sizeof(int), 0); // Send the facultyId  
            send(sock, newPassword, strlen(newPassword), 0); // Send the new password
            write(STDOUT_FILENO,"info sent to the server\n",strlen("info sent to the server\n"));
            memset(response, 0, sizeof(response));
            read(sock, response, sizeof(response));
            // printf("Server Response: %s\n", response);
            write(STDOUT_FILENO,"Server Response:\n",strlen("Server Response:\n"));
            write(STDOUT_FILENO,response,strlen(response));
        }
        else if(choice==5){
            sprintf(request, "FACULTY|CHOICE|5");
            send(sock, request, strlen(request), 0);
            memset(response, 0, sizeof(response));
            read(sock, response, sizeof(response));
            // printf("List of all the students:\n %s\n", response);
            write(STDOUT_FILENO,"List of all the students:\n",strlen("List of all the students:\n"));
            write(STDOUT_FILENO,response,strlen(response));
        }
        else if(choice==6){
            sprintf(request, "FACULTY|CHOICE|6");
            send(sock, request, strlen(request), 0);
            memset(response, 0, sizeof(response));
            read(sock, response, sizeof(response));
            // printf("List of all the faculty:\n %s\n", response);
            write(STDOUT_FILENO,"List of all the courses:\n",strlen("List of all the courses:\n"));
            write(STDOUT_FILENO,response,strlen(response));
        }
        else if(choice==7){
            // printf("Exiting Faculty Menu...\n");
            write(STDOUT_FILENO,"Exiting Faculty Menu...\n",strlen("Exiting Faculty Menu...\n"));
            return 1;
        }
        else {
            sprintf(request, "FACULTY|CHOICE|%d", choice);
            send(sock, request, strlen(request), 0);
            memset(response, 0, sizeof(response));
            read(sock, response, sizeof(response)); 
            // printf("Server Response: %s\n", response);
            write(STDOUT_FILENO,"Server Response:\n",strlen("Server Response:\n"));
            write(STDOUT_FILENO,response,strlen(response));
        }

        // sprintf(request, "FACULTY|CHOICE|%d", choice);
        // send(sock, request, strlen(request), 0);
        // read(sock, response, sizeof(response));
        // printf("Server Response: %s\n", response);
    }
    return 0;
}

int studentClient(int sock) {
    // printf("\n--- Student Menu ---\n");
    write(STDOUT_FILENO,"\n--- Student Menu ---\n",strlen("\n--- Student Menu ---\n"));
    // memset(request, 0, sizeof(request));
    while (1) {
        char request[BUFFER_SIZE], response[BUFFER_SIZE];
        int choice;
        char intBuffer[BUFFER_SIZE];

        // printf("1. Enroll in Course\n2. Unenroll from Course\n3. View Enrolled Courses\n4. Change Password\n5. Exit\n");
        write(STDOUT_FILENO,"1. Enroll in Course\n2. Unenroll from Course\n3. View Enrolled Courses\n4. Change Password\n5. Exit\n",strlen("1. Enroll in Course\n2. Unenroll from Course\n3. View Enrolled Courses\n4. Change Password\n5. Exit\n"));
        // printf("Enter your choice: ");
        write(STDOUT_FILENO,"Enter your choice: ",strlen("Enter your choice: "));
        // scanf("%d", &choice);
        read(STDIN_FILENO,intBuffer,sizeof(intBuffer));
        choice = atoi(intBuffer);

        // if (choice == 5) {
        //     printf("Exiting Student Menu...\n");
        //     break;
        // }
        memset(response, 0, sizeof(response));

        if(choice==1){
            enroll e;
            // printf("Enter Student ID: ");
            write(STDOUT_FILENO,"Enter Student ID: ",strlen("Enter Student ID: "));
            // scanf("%d", &e.studentId);
            read(STDIN_FILENO,intBuffer,sizeof(intBuffer));
            e.studentId = atoi(intBuffer);
            // printf("Enter Course ID: ");
            write(STDOUT_FILENO,"Enter Course ID: ",strlen("Enter Course ID: "));
            // scanf("%d", &e.courseId);
            read(STDIN_FILENO,intBuffer,sizeof(intBuffer));
            e.courseId = atoi(intBuffer);

            // Send the request to the server
            sprintf(request, "STUDENT|CHOICE|1");
            send(sock, request, strlen(request), 0);
            send(sock, &e, sizeof(enroll), 0); // Send the enroll struct

            // Wait for the server's response

            memset(response, 0, sizeof(response));
            read(sock, response,sizeof(response));
            // printf("Server Response: %s\n", response);
            write(STDOUT_FILENO,"Server Response:\n",strlen("Server Response:\n"));
            write(STDOUT_FILENO,response,strlen(response));
        }
        else if(choice==2){
            int studentId, courseId;
            // printf("Enter Student ID: ");
            write(STDOUT_FILENO,"Enter Student ID: ",strlen("Enter Student ID: "));
            // scanf("%d", &studentId);
            // read(STDIN_FILENO,&studentId,sizeof(int));
            read(STDIN_FILENO,intBuffer,sizeof(intBuffer));
            studentId = atoi(intBuffer);
            // printf("Enter Course ID to unenroll from: ");
            write(STDOUT_FILENO,"Enter Course ID to unenroll from: ",strlen("Enter Course ID to unenroll from: "));
            // scanf("%d", &courseId);
            // read(STDIN_FILENO,&courseId,sizeof(int));
            read(STDIN_FILENO,intBuffer,sizeof(intBuffer));
            courseId = atoi(intBuffer);

            // Send the request to the server
            sprintf(request, "STUDENT|CHOICE|2");
            send(sock, request, strlen(request), 0);
            send(sock,&studentId,sizeof(int),0); // Send the studentId
            send(sock,&courseId,sizeof(int),0); // Send the courseId

            // Wait for the server's response
            memset(response, 0, sizeof(response));
            read(sock, response, sizeof(response));
            // printf("Server Response: %s\n", response);
            write(STDOUT_FILENO,"Server Response:\n",strlen("Server Response:\n"));
            write(STDOUT_FILENO,response,strlen(response));
        }
        else if (choice == 3) { // View Enrolled Courses
            int studentId;
            // printf("Enter Student ID: ");
            write(STDOUT_FILENO,"Enter Student ID: ",strlen("Enter Student ID: "));
            // scanf("%d", &studentId);
            // read(STDIN_FILENO,&studentId,sizeof(int));
            read(STDIN_FILENO,intBuffer,sizeof(intBuffer));
            studentId = atoi(intBuffer);
            // Send the request to the server
            sprintf(request, "STUDENT|CHOICE|3");
            send(sock, request, strlen(request), 0);
            send(sock,&studentId,sizeof(int),0);
            // Wait for the server's response
            memset(response, 0, sizeof(response));
            read(sock, response, sizeof(response));
            // printf("Server Response: %s\n", response);
            write(STDOUT_FILENO,"Server Response:\n",strlen("Server Response:\n"));
            write(STDOUT_FILENO,response,strlen(response));
        }
        else if(choice==4){
            int id;
            char newPassword[30];
            // printf("Enter Student ID to update password: ");
            write(STDOUT_FILENO,"Enter Student ID to update password: ",strlen("Enter Student ID to update password: "));
            // scanf("%d", &id);
            read(STDIN_FILENO,intBuffer,sizeof(intBuffer));
            id=atoi(intBuffer);
            // printf("Enter new password: ");
            write(STDOUT_FILENO,"Enter new password: ",strlen("Enter new password: "));
            // scanf("%s", newPassword);
            read(STDIN_FILENO,newPassword,sizeof(newPassword));
            // Send the request to the server
            sprintf(request, "STUDENT|CHOICE|4");
            send(sock, request, strlen(request), 0);
            send(sock,&id,sizeof(int),0); // Send the studentId
            send(sock,newPassword,strlen(newPassword),0);
            // Wait for the server's response
            memset(response, 0, sizeof(response));
            read(sock, response, sizeof(response));
            // printf("Server Response: %s\n", response);
            write(STDOUT_FILENO,"Server Response:\n",strlen("Server Response:\n"));
            write(STDOUT_FILENO,response,strlen(response));
        }
        else if(choice==5){
            // printf("Exiting Student Menu...\n");
            write(STDOUT_FILENO,"Exiting Student Menu...\n",strlen("Exiting Student Menu...\n"));
            return 1;
        }
        else {
            sprintf(request, "STUDENT|CHOICE|%d", choice);
            send(sock, request, strlen(request), 0);
            memset(response, 0, sizeof(response));
            read(sock, response, sizeof(response));     
            // printf("Server Response: %s\n", response);
            write(STDOUT_FILENO,"Server Response:\n",strlen("Server Response:\n"));
            write(STDOUT_FILENO,response,strlen(response));
        }
        // sprintf(request, "STUDENT|CHOICE|%d", choice);
        // send(sock, request, strlen(request), 0);
        // read(sock, response, sizeof(response));
        // printf("Server Response: %s\n", response);
    }
    return 0;
}

int main() {
    int sock;
    struct sockaddr_in serv_addr;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // printf("Connected to the server.\n");
    write(STDOUT_FILENO,"Connected to the server.\n",strlen("Connected to the server.\n"));


    int WheterLoggedIn=0;
    int LoggenInRole=0;
    int flag=0;
    while (1) {
        // Login menu
        int role;
        char intBuffer[BUFFER_SIZE];
        // printf("\nLogin as:\n1. Admin\n2. Faculty\n3. Student\n 4. Quit \nEnter your choice: ");
        write(STDOUT_FILENO,"\nLogin as:\n1. Admin\n2. Faculty\n3. Student\n 4. Quit \nEnter your choice: ",strlen("\nLogin as:\n1. Admin\n2. Faculty\n3. Student\n 4. Quit \nEnter your choice: "));
        // scanf("%d", &role);
        memset(intBuffer, 0, sizeof(intBuffer));
        read(STDIN_FILENO,intBuffer,sizeof(intBuffer));
        role=atoi(intBuffer);
        if (role == 1) {
            // Admin Login
            if(WheterLoggedIn!=1 || LoggenInRole!=1){
                // printf("You are already logged in as Admin.\n");
                // adminClient(sock);
                // break;
                int adminId;
                char adminPassword[BUFFER_SIZE];
                // printf("Enter Admin ID: ");
                write(STDOUT_FILENO,"Enter Admin ID: ",strlen("Enter Admin ID: "));
                // scanf("%d", &adminId);
                read(STDIN_FILENO,intBuffer,sizeof(intBuffer));
                adminId=atoi(intBuffer);
                // printf("Enter Admin Password: ");
                write(STDOUT_FILENO,"Enter Admin Password: ",strlen("Enter Admin Password: "));
                // scanf("%s", adminPassword);
                read(STDIN_FILENO,adminPassword,sizeof(adminPassword));

                write(STDOUT_FILENO,adminPassword,strlen(adminPassword));
                // Verify admin credentials (stored internally on the server)
                char request[BUFFER_SIZE], response[BUFFER_SIZE];
                sprintf(request, "LOGIN|ADMIN|%d|%s", adminId, adminPassword);
                send(sock, request, strlen(request), 0);
                read(sock, response, sizeof(response));
                if(strcmp(response, "SUCCESS") == 0){
                    // printf("Admin login successful.\n");
                    write(STDOUT_FILENO,"Admin login successful.\n",strlen("Admin login successful.\n"));
                    WheterLoggedIn=1;
                    LoggenInRole=1;
                    flag=adminClient(sock);
                    if(flag==1){
                        WheterLoggedIn=0;
                        LoggenInRole=0;
                        // printf("Exiting Admin Menu...\n");
                        write(STDOUT_FILENO,"Exiting Admin Menu...\n",strlen("Exiting Admin Menu...\n"));
                    }
                }
                else{
                    // printf("Invalid Admin credentials. Try again.\n");
                    write(STDOUT_FILENO,"Invalid Admin credentials. Try again.\n",strlen("Invalid Admin credentials. Try again.\n"));
                }
            }

            if(WheterLoggedIn==1 && LoggenInRole==1){
                flag=adminClient(sock);
                if(flag==1){
                        WheterLoggedIn=0;
                        LoggenInRole=0;
                        // printf("Exiting Admin Menu...After finding the flag as 1 \n");
                        write(STDOUT_FILENO,"Exiting Admin Menu...After finding the flag as 1 \n",strlen("Exiting Admin Menu...After finding the flag as 1 \n"));
                    }
                break;
            } else {
                // printf("Not Logged In. Try again.\n");
                write(STDOUT_FILENO,"Not Logged In. Try again.\n",strlen("Not Logged In. Try again.\n"));
            }
        } else if (role == 2) {
            // Faculty Login
            if(WheterLoggedIn!=1 || LoggenInRole!=2){
                int facultyId;
                char facultyPassword[BUFFER_SIZE];
                char facultyEmail[50];
                // printf("Enter Faculty ID: ");
                write(STDOUT_FILENO,"Enter Faculty Email: ",strlen("Enter Faculty Email: "));
                read(STDIN_FILENO,facultyEmail,sizeof(facultyEmail));
                facultyEmail[strcspn(facultyEmail, "\n")] = '\0';
                facultyId=atoi(intBuffer);
                // printf("Enter Faculty Password: ");
                write(STDOUT_FILENO,"Enter Faculty Password: ",strlen("Enter Faculty Password: "));
                // scanf("%s", facultyPassword);
                memset(facultyPassword, 0, sizeof(facultyPassword));
                read(STDIN_FILENO,facultyPassword,sizeof(facultyPassword));

                // write(STDOUT_FILENO,intBuffer,strlen(intBuffer));
                // write(STDOUT_FILENO,facultyPassword,strlen(facultyPassword));
                // Verify faculty credentials
                char request[BUFFER_SIZE], response[BUFFER_SIZE];
                sprintf(request, "LOGIN|FACULTY|%s|%s", facultyEmail, facultyPassword);
                send(sock, request, strlen(request), 0);
                read(sock, response, sizeof(response));
                if (strcmp(response, "SUCCESS") == 0) {
                    // printf("Faculty login successful.\n");
                    write(STDOUT_FILENO,"Faculty login successful.\n",strlen("Faculty login successful.\n"));
                    WheterLoggedIn=1;
                    LoggenInRole=2;
                    flag=facultyClient(sock);
                    if(flag==1){
                        WheterLoggedIn=0;
                        LoggenInRole=0;
                        // printf("Exiting Admin Menu...\n");
                        write(STDOUT_FILENO,"Exiting Admin Menu...\n",strlen("Exiting Admin Menu...\n"));
                    }
                } else {
                    // printf("Invalid Faculty credentials. Try again.\n");
                    write(STDOUT_FILENO,"Invalid Faculty credentials. Try again.\n",strlen("Invalid Faculty credentials. Try again.\n"));
                    continue;
                }
            }

            if(WheterLoggedIn==1 && LoggenInRole==2){
                flag=facultyClient(sock);
                if(flag==1){
                        WheterLoggedIn=0;
                        LoggenInRole=0;
                        // printf("Exiting Admin Menu...\n");
                        write(STDOUT_FILENO,"Exiting Admin Menu...\n",strlen("Exiting Admin Menu...\n"));
                    }
                break;
            } else {
                // printf("Not Logged In. Try again.\n");
                write(STDOUT_FILENO,"Not Logged In. Try again.\n",strlen("Not Logged In. Try again.\n"));

            }
        } else if (role == 3) {
            // Student Login
            if(WheterLoggedIn!=1 || LoggenInRole!=3){
                int studentId;
                char studentPassword[BUFFER_SIZE];
                char studentEmail[50];
                // printf("Enter Student ID: ");
                write(STDOUT_FILENO,"Enter Student Email: ",strlen("Enter Student Email: "));
                read(STDIN_FILENO,studentEmail,sizeof(studentEmail));
                studentEmail[strcspn(studentEmail, "\n")] = '\0';
                // printf("Enter Student Password: ");
                write(STDOUT_FILENO,"Enter Student Password: ",strlen("Enter Student Password: "));
                // scanf("%s", studentPassword);
                read(STDIN_FILENO,studentPassword,sizeof(studentPassword));
    
                // Verify student credentials
                char request[BUFFER_SIZE], response[BUFFER_SIZE];
                sprintf(request, "LOGIN|STUDENT|%s|%s", studentEmail, studentPassword);
                send(sock, request, strlen(request), 0);
                read(sock, response, sizeof(response));

                if (strcmp(response, "SUCCESS") == 0) {
                    // printf("Student login successful.\n");
                    write(STDOUT_FILENO,"Student login successful.\n",strlen("Student login successful.\n"));
                    WheterLoggedIn=1;
                    LoggenInRole=3;
                    // Call the student client function
                    flag=studentClient(sock);
                    if(flag==1){
                        WheterLoggedIn=0;
                        LoggenInRole=0;
                        // printf("Exiting Admin Menu...\n");
                        write(STDOUT_FILENO,"Exiting Admin Menu...\n",strlen("Exiting Admin Menu...\n"));
                    }
                    // break;
                } else {
                    // printf("Invalid Student credentials. Try again.\n");
                    write(STDOUT_FILENO,"Invalid Student credentials. Try again.\n",strlen("Invalid Student credentials. Try again.\n"));
                }
            }

            if(WheterLoggedIn==1 && LoggenInRole==3){
                flag=studentClient(sock);
                if(flag==1){
                        WheterLoggedIn=0;
                        LoggenInRole=0;
                        // printf("Exiting Admin Menu...\n");
                        write(STDOUT_FILENO,"Exiting Admin Menu...\n",strlen("Exiting Admin Menu...\n"));
                }
                // break;
            } else {
                // printf("Not Logged In. Try again.\n");
                write(STDOUT_FILENO,"Not Logged In. Try again.\n",strlen("Not Logged In. Try again.\n"));
            }
        } 
        else if(role==4){
            // printf("Exiting...\n");
            write(STDOUT_FILENO,"Exiting...\n",strlen("Exiting...\n"));
            break;
        }
        else {
            // printf("Invalid choice. Please select a valid role.\n");
            write(STDOUT_FILENO,"Invalid choice. Please select a valid role.\n",strlen("Invalid choice. Please select a valid role.\n"));
        }
    }

    close(sock);
    // printf("Disconnected from the server.\n");
    write(STDOUT_FILENO,"Disconnected from the server.\n",strlen("Disconnected from the server.\n"));
    return 0;
}