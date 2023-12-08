#include <stdio.h>     // printf, puts, perror
#include <string.h>    // memcpy
#include <unistd.h>    // getcwd
#include <dirent.h>    // opendir, readdir, closedir
#include <stdlib.h>    // exit, EXIT_FAILURE, EXIT_SUCCESS
#include <errno.h>     // errno

#define STRINGV_IMPLEMENTATION
#include "stringv.h"

#define MERGE_SORT_IMPLEMENTATION
#include "merge_sort.h"

#define array_len(x) sizeof((x))/sizeof(*(x))

#define MAX_N_PATHS  100
#define MAX_PATH_LEN 100 // in number of chars

struct Paths {
    char **paths;
    size_t path_len;
    size_t count;
} Paths;

void get_cwd_paths(struct Paths *paths, const char *file_type)
{
    char dir[MAX_PATH_LEN] = { 0 };
    getcwd(dir, sizeof(dir)/sizeof(*dir));

    // copy cwd into paths
    size_t dir_sz = strlen(dir);
    memcpy(*paths->paths, dir, dir_sz);
    (*paths->paths)[dir_sz] = '\0';
    paths->count++;

    DIR *dir_stream;
    if ((dir_stream = opendir(dir)) == NULL) {
        perror("Something went bad opening directory: ");
        exit(EXIT_FAILURE);
    }

    // loop through the paths and save the ones with specific 'file_type'
    struct dirent *dir_struct = { 0 };
    errno = 0;
    while ((dir_struct = readdir(dir_stream)) != NULL) {
        StringV name = stringv_create(dir_struct->d_name);
        // filter paths by file_type
        if (stringv_ends_with(name, file_type, strlen(file_type))) {
            // check if enough space available for path + '\0'
            if (MAX_PATH_LEN < name.count) {
                puts("Not enought space in paths->paths[paths->count] to memcpy name.sv\n");
                if (paths->paths != NULL) free(paths->paths);
                return;
            }

            // save path
            char *next_path_pos = *paths->paths + paths->count * paths->path_len;
            size_t next_path_sz  = name.count * sizeof(*name.sv);
            memcpy(next_path_pos, name.sv, next_path_sz);
            next_path_pos[next_path_sz] = '\0';
            paths->count++;
        }
        stringv_destroy(&name);
    }
    if (errno != 0) {
        perror("Something went bad reading directory structure: ");
        closedir(dir_stream);
        exit(EXIT_FAILURE);
    }

    // sort paths
    ms_merge_sort_string(&paths->paths[1] , paths->count - 1);
}

void print_usage(void)
{
    printf("Show files and paths in current working directory\n\n"
           "USAGE: ./paths [OPTIONS | file_type]\n"
           "OPTIONS:\n"
           "  -h, --help    Print this help\n\n"
           "EXAMPLES:\n"
           "  ./paths       List all files in cwd\n"
           "  ./paths '.c'  List all C source code files in cwd\n");
}

int main(int argc, char **argv)
{
    const char *file_type;
    if (argc == 1) {
        file_type = "";
    }
    else if (argc > 1) {
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            print_usage();
            exit(EXIT_SUCCESS);
        }

        file_type = argv[1];
    }
    else {
        print_usage();
        exit(EXIT_FAILURE);
    }

    // initialise paths struct
    char *paths_ptr = (char *) calloc(MAX_N_PATHS * MAX_PATH_LEN, sizeof(char));
    struct Paths paths = {
        .paths    = (char **) calloc(MAX_N_PATHS, sizeof(char *)),
        .path_len = MAX_PATH_LEN,
        .count    = 0
    };
    for (size_t i = 0; i < MAX_N_PATHS; ++i) {
        paths.paths[i] = paths_ptr + paths.path_len * i;
    }

    // get paths and print them
    get_cwd_paths(&paths, file_type);
    if (paths.paths == NULL) {
        free(paths_ptr);
        fprintf(stderr, "Error getting cwd paths");
        exit(EXIT_FAILURE);
    }
    printf("CWD: %s\n", paths.paths[0]);
    for (size_t i = 0; i < paths.count - 1; ++i) {
        printf("  % 4zu  %s\n", i, paths.paths[i + 1]);
    }

    free(paths.paths);
    free(paths_ptr);
    
    return 0;
}
