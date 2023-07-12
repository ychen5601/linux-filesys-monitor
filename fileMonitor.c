#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <dirent.h>

// This function returns 1 if the process with pid is executed by current user, -1 otherwise.

int is_process_owned_by_user(pid_t pid) {
    char path[30];
    sprintf(path, "/proc/%ld/status", (long)pid);
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        return 0;
    }
    char field_name[100];
    for (int i = 0; i < 8; i++) {
        fgets(field_name, sizeof(field_name), fp);
    }
    long owner_id, num2, num3, num4;
    fscanf(fp, "%s    %ld        %ld        %ld        %ld", field_name, &owner_id, &num2, &num3, &num4);
    fclose(fp);
    if (owner_id == (long)getuid()) {
        return 1;
    } else {
        return -1;
    }
}


void print_per_process_header() {
    printf("         PID       FD\n");
    printf("        =================\n");
    return;
}


void print_per_process_end() {
    printf("        =================\n");
    return;
}


int print_per_process_table(pid_t pid) {
    char path[30];
    sprintf(path, "/proc/%ld/fdinfo", (long)pid);
    DIR *fdinfo = opendir(path);
    if (fdinfo == NULL) {
        return -1;
    }
    struct dirent *dp;

    while (dp = readdir(fdinfo)) {
        if (strncmp(dp -> d_name, ".", 1) != 0) {
            char pid_str[30];
            sprintf(pid_str, "%ld", (long)pid);
            int pid_len = strlen(pid_str);
            printf("         ");
            printf("%ld", (long) pid);
            int spaces_to_print = 10 - pid_len;
            for (int i = 0; i < spaces_to_print; i++) {
                printf(" ");
            }
            printf("%s\n", dp -> d_name);
        }
    }
    closedir(fdinfo);
    return 0;
}

void print_system_wide_header() {
    printf("         PID       FD      Filename\n");
    printf("        ========================================\n");
    return;
}


void print_system_wide_end() {
    printf("        ========================================\n");
    return;
}


int print_system_wide(pid_t pid) {
    char path[30];
    sprintf(path, "/proc/%ld/fdinfo", (long)pid);
    DIR *fdinfo = opendir(path);
    if (fdinfo == NULL) {
        return -1;
    }
    struct dirent *dp;
    while (dp = readdir(fdinfo)) {
        if (strncmp(dp -> d_name, ".", 1) != 0) {
            char pid_str[30];
            sprintf(pid_str, "%ld", (long)pid);
            int pid_len = strlen(pid_str);
            printf("         ");
            printf("%ld", (long) pid);
            int spaces_to_print = 10 - pid_len;
            for (int i = 0; i < spaces_to_print; i++) {
                printf(" ");
            }
            int d_name_len = strlen(dp -> d_name);
            printf("%s", dp -> d_name);
            spaces_to_print = 8 - d_name_len;
            for (int i = 0; i < spaces_to_print; i++) {
                printf(" ");
            }
            char link[100];
            char namepath[300];
            sprintf(namepath, "/proc/%ld/fd/%s", (long)pid, dp -> d_name);
            int k = readlink(namepath, link, sizeof(link));
            link[k] = '\0';
            printf("%s\n", link);
        }
    }
    closedir(fdinfo);
    return 0;
}


void print_vnodes_header() {
    printf("           FD      Inode\n");
    printf("        ========================================\n");
    return;
}


void print_vnodes_end() {
    printf("        ========================================\n");
    return;
}


int print_vnodes(pid_t pid) {
    char path[30];
    sprintf(path, "/proc/%ld/fdinfo", (long)pid);
    DIR *fdinfo = opendir(path);
    if (fdinfo == NULL) {
        return -1;
    }
    struct dirent *dp;
    while (dp = readdir(fdinfo)) {
        if (strncmp(dp -> d_name, ".", 1) != 0) {
            int d_name_len = strlen(dp -> d_name);
            printf("           ");
            printf("%s", dp -> d_name);
            int spaces_to_print = 8 - d_name_len;
            for (int i = 0; i < spaces_to_print; i++) {
                printf(" ");
            }
            printf("%ld\n", (long) dp -> d_ino);
        }
    }
    closedir(fdinfo);
    return 0;
}


void print_composite_header() {
    printf("         PID       FD      Inode          Filename\n");
    printf("        ===============================================\n");
    return;
}


void print_composite_end() {
    printf("        ===============================================\n");
    return;
}


int print_composite(pid_t pid) {
    char path[30];
    sprintf(path, "/proc/%ld/fdinfo", (long)pid);
    DIR *fdinfo = opendir(path);
    if (fdinfo == NULL) {
        return -1;
    }
    struct dirent *dp;
    while (dp = readdir(fdinfo)) {
        if (strncmp(dp -> d_name, ".", 1) != 0) {
            char pid_str[30];
            sprintf(pid_str, "%ld", (long)pid);
            int pid_len = strlen(pid_str);
            printf("         ");
            printf("%ld", (long) pid);
            int spaces_to_print = 10 - pid_len;
            for (int i = 0; i < spaces_to_print; i++) {
                printf(" ");
            }
            int d_name_len = strlen(dp -> d_name);
            printf("%s", dp -> d_name);
            spaces_to_print = 8 - d_name_len;
            for (int i = 0; i < spaces_to_print; i++) {
                printf(" ");
            }
            printf("%ld", (long) dp -> d_ino);
            char ino_str[30];
            sprintf(ino_str, "%ld", (long)dp -> d_ino);
            int ino_len = strlen(ino_str);
            spaces_to_print = 15 - ino_len;
            for (int i = 0; i < spaces_to_print; i++) {
                printf(" ");
            }
            char link[100];
            char namepath[300];
            sprintf(namepath, "/proc/%ld/fd/%s", (long)pid, dp -> d_name);
            int k = readlink(namepath, link, sizeof(link));
            link[k] = '\0';
            printf("%s\n", link);
        }
    }
    closedir(fdinfo);
    return 0;
}


int get_num_files_in_process(pid_t pid) {
    char path[30];
    sprintf(path, "/proc/%ld/fdinfo", (long)pid);
    int result = 0;
    DIR *fdinfo = opendir(path);
    if (fdinfo == NULL) {
        return -1;
    }
    struct dirent *dp;
    while (dp = readdir(fdinfo)) {
        if(strncmp(dp -> d_name, ".", 1) != 0) {
            result++;
        }
    }
    closedir(fdinfo);
    return result;
}


int main(int argc, char *argv[]) {

    // Parsing command line arguments

    int visited_flag = 0;
    int is_threshold = 0;
    int is_pid = 0;
    int per_process = 0;
    int system_wide = 0;
    int vnodes = 0;
    int composite = 0;
    int threshold;
    long pid;
    int i = 1;
    char *token;

    if ((argc > 1) && (atoi(argv[1]) != 0)) {
        pid = atoi(argv[1]);
        is_pid = 1;
        i = 2;
    }

    for (; i < argc; i++) {
        if (strncmp(argv[i], "--per-process", 13) == 0) {
            visited_flag = 1;
            per_process = 1;
        }
        else if (strncmp(argv[i], "--systemWide", 12) == 0) {
            visited_flag = 1;
            system_wide = 1;
        }
        else if (strncmp(argv[i], "--Vnodes", 8) == 0) {
            visited_flag = 1;
            vnodes = 1;
        }
        else if (strncmp(argv[i], "--composite", 11) == 0) {
            visited_flag = 1;
            composite = 1;
        }
        else if (strncmp(argv[i], "--threshold=", 12) == 0) {
            if (is_threshold == 1) {
                fprintf(stderr, "Detected multiple calls to --threshold in one call to program - please only flag once!\n");
                return 0;
            }
            char *token = strtok(argv[i], "=");
            token = strtok(NULL, "=");
            if (token == NULL) {
                fprintf(stderr, "Unable to read number after --threshold= call. \n");
                return 0;
            }
            threshold = atoi(token);
            is_threshold = 1;
        }
        else {
            fprintf(stderr, "Unable to parse argument. Available arguments are: --per-process --systemWide --Vnodes --composite --threshold=N\n");
            return 0;
        }
    }

    // General variable to store pid to get information
    
    pid_t pid_to_get;

    // Case where PID is given

    if (is_pid == 1) {
        pid_to_get = (pid_t) pid;

        // Checking if proc with id pid is owned by caller of program
        int is_proc_owned_by_user = is_process_owned_by_user(pid_to_get);
        if (is_proc_owned_by_user == -1) {
            fprintf(stderr, "Unable to access process with pid %ld\n", pid);
            return -1;
        } else if (is_proc_owned_by_user == 0) {
            fprintf(stderr, "Error when opening file.\n");
            return -1;
        }

        // Printing per process
        if (visited_flag == 0 || per_process == 1) {
            print_per_process_header();
            if (print_per_process_table(pid_to_get) == -1) {
                fprintf(stderr, "Error occurred when printing per process - returning.\n");
                return -1;
            }
            print_per_process_end();
        }

        // Printing system wide
        if (visited_flag == 0 || system_wide == 1) {
            print_system_wide_header();
            if (print_system_wide(pid_to_get) == -1) {
                fprintf(stderr, "Error occurred when printing per process - returning.\n");
                return -1;
            }
            print_system_wide_end();
        }

        // Printing Vnodes
        if (visited_flag == 0 || vnodes == 1) {
            print_vnodes_header();
            if (print_vnodes(pid_to_get) == -1) {
                fprintf(stderr, "Error occurred when printing vnodes - Returning.\n");
                return -1;
            }
            print_vnodes_end();
        }

        // Printing composite
        if (visited_flag == 0 || composite == 1) {
            print_composite_header();
            if (print_composite(pid_to_get) == -1) {
                fprintf(stderr, "Error occurred when printing composite - Returning.\n");
                return -1;
            }
            print_composite_end();
        }
    }

    // Case where PID is not given - iterate through entire /proc file

    if (is_pid == 0) {
        char path[30];
        sprintf(path, "/proc");
        DIR *proc_info = opendir(path);
        struct dirent *dp;
        if (proc_info == NULL) {
            perror("opendir");
            return -1;
        }
        // Print per process
        if (per_process == 1) {
            rewinddir(proc_info);
            print_per_process_header();
            while (dp = readdir(proc_info)) {
                pid = atol(dp -> d_name);
                if (pid == 0) {continue;}
                pid_to_get = (pid_t) pid;
                if (is_process_owned_by_user(pid_to_get) == 1) {
                    print_per_process_table(pid_to_get);
                }
            }
            print_per_process_end();
        }
        // Print system wide
        if (system_wide == 1) {
            rewinddir(proc_info);
            print_system_wide_header();
            while (dp = readdir(proc_info)) {
                pid = atol(dp -> d_name);
                if (pid == 0) {continue;}
                pid_to_get = (pid_t) pid;
                if (is_process_owned_by_user(pid_to_get) == 1) {
                    print_system_wide(pid_to_get);
                }
            }
            print_per_process_end();
        }
        // Print vnodes
        if (vnodes == 1) {
            rewinddir(proc_info);
            print_vnodes_header();
            while (dp = readdir(proc_info)) {
                pid = atol(dp -> d_name);
                if (pid == 0) {continue;}
                pid_to_get = (pid_t) pid;
                if (is_process_owned_by_user(pid_to_get) == 1) {
                    print_vnodes(pid_to_get);
                }
            }
            print_per_process_end();
        }
        // Printing composite
        if (visited_flag == 0 || composite == 1) {
            rewinddir(proc_info);
            print_composite_header();
            while (dp = readdir(proc_info)) {
                pid = atol(dp -> d_name);
                if (pid == 0) {continue;}
                pid_to_get = (pid_t) pid;
                if (is_process_owned_by_user(pid_to_get) == 1) {
                    print_composite(pid_to_get);
                }
            }
            print_composite_end();
        }
        closedir(proc_info);
    }

    if (is_threshold == 1) {
        int num_files;
        if (is_pid == 1) {
            pid_to_get = (pid_t) pid;
            int is_proc_owned_by_user = is_process_owned_by_user(pid_to_get);
            if (is_proc_owned_by_user == -1) {
                fprintf(stderr, "Unable to access process with pid %ld\n", pid);
                return -1;
            } else if (is_proc_owned_by_user == 0) {
                fprintf(stderr, "Error when opening file.\n");
                return -1;
            }
            if ((num_files = get_num_files_in_process(pid_to_get)) > threshold) {
                printf("## Offending processes:\n");
                printf("%ld(%d)\n", pid, num_files);
            }
        }
        if (is_pid == 0) {
            char path[30];
            sprintf(path, "/proc");
            DIR *proc_info = opendir(path);
            struct dirent *dp;
            if (proc_info == NULL) {
                return -1;
            }
            printf("## Offending processes:\n");
            while (dp = readdir(proc_info)) {
                pid = atol(dp -> d_name);
                if (pid == 0) {continue;}
                pid_to_get = (pid_t) pid;
                if (is_process_owned_by_user(pid_to_get) == 1) {
                    if ((num_files = get_num_files_in_process(pid_to_get)) > threshold) {
                        printf("%ld(%d) ", pid, num_files);
                    }
                }
            }
            printf("\n");
            closedir(proc_info);
        }
    }
    return 0;
}
