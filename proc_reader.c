#include "proc_reader.h"

int list_process_directories(void) {
    
    DIR *proc_dir = opendir("/proc");

    if (!proc_dir) {
        perror("Error opening proc directory");
    }

    struct dirent *entry;
    int counter = 0;

    printf("Process directories in /proc:\n");
    printf("%-8s %-20s\n", "PID", "Type");
    printf("%-8s %-20s\n", "---", "----");


    while ((entry = readdir(proc_dir)) != NULL) {
            if (is_number(entry->d_name)) {
                printf("%-8s %-20s\n", entry->d_name, "Process");
                counter++;
            }
    }

    if (closedir(proc_dir) == -1 ) {
        perror("Error closing /proc directory");
        return -1;
    }

    printf("Total processes found: %d\n", counter);

    return 0; // Replace with proper error handling
}

int read_process_info(const char* pid) {

    char filepath[256];

    snprintf(filepath, sizeof(filepath), "/proc/%s/status", pid);

    printf("\n--- Process Information for PID %s ---\n", pid);


    if (read_file_with_syscalls(filepath) == -1) {
        printf("Failed to read %s\n", filepath);
        return -1;
    }

    snprintf(filepath, sizeof(filepath), "/proc/%s/cmdline", pid);

    printf("\n--- Command Line ---\n");

    if (read_file_with_syscalls(filepath) == -1) {
        printf("Failed to read %s\n", filepath);
        return -1;
    }

    printf("\n"); // Add extra newline for readability

    return 0; // Replace with proper error handling
}

int show_system_info(void) {
    int line_count = 0;
    const int MAX_LINES = 10;

    printf("\n--- CPU Information (first %d lines) ---\n", MAX_LINES);

    FILE *fp = fopen("/proc/cpuinfo", "r");

    if (!fp) {
        perror("Failed to open cpuinfo");
        return -1;
    }
  
    char lines[256];

    while (line_count < MAX_LINES && fgets(lines, sizeof(lines), fp)) {
        printf("%s", lines);
        line_count++;
    }
    fclose(fp);

    line_count = 0;


    printf("\n--- Memory Information (first %d lines) ---\n", MAX_LINES);

    fp = fopen("/proc/meminfo", "r");

    if (!fp) {
        perror("Failed to open memory info");
        return -1;
    }

    while (line_count < MAX_LINES && fgets(lines, sizeof(lines), fp)) {
        printf("%s", lines);
        line_count++;
    }
    fclose(fp);

    return 0; // Replace with proper error handling
}

void compare_file_methods(void) {
    const char* test_file = "/proc/version";

    printf("Comparing file reading methods for: %s\n\n", test_file);

    printf("=== Method 1: Using System Calls ===\n");
    read_file_with_syscalls(test_file);

    printf("\n=== Method 2: Using Library Functions ===\n");
    read_file_with_library(test_file);

    printf("\nNOTE: Run this program with strace to see the difference!\n");
    printf("Example: strace -e trace=openat,read,write,close ./lab2\n");
}

int read_file_with_syscalls(const char* filename) {

    int fd;
    char buffer[256];
    ssize_t bytes_read;

    fd = open(filename, O_RDONLY);

    if (fd == -1) {
        perror("Failed to open filename");
        return -1;
    }

    while ((bytes_read = (fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        printf("%s", buffer);
    }

    if (bytes_read == -1) {
        perror("Failed to read bytes");
        close(fd);
        return -1;
    }

    return 0; // Replace with proper error handling
}

int read_file_with_library(const char* filename) {
    // TODO: Declare variables: FILE pointer, buffer (char array)

    FILE *fp;
    char buffer[256];

    fp = fopen(filename, "r");

    if (!fp) {
        perror("Failed to open file");
        return -1;
    }


    while (fgets(buffer, sizeof(buffer), fp)) {
        printf("%s", buffer);
    }

    if (fclose(fp) == -1) {
        perror("Error closing file");
        return -1;
    }

    return 0; // Replace with proper error handling
}

int is_number(const char* str) {
    if (!str || *str == '\0') {
        return 0;
    }

    for (int i = 0; str[i]; i++) {
        if (!isdigit(str[i])) return 0;  
    }


    return 1; // Replace with actual implementation
}