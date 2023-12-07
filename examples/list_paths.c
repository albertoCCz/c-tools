#include <stdio.h>     // printf, puts, perror
#include <string.h>    // memcpy
#include <unistd.h>    // getcwd
#include <dirent.h>    // opendir, readdir, closedir
#include <stdlib.h>    // exit, EXIT_FAILURE
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
                return; // return so allocated mem can be freed in calling function
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

int main(int argc, char **argv)
{
    const char *file_type = "";
    if (argc > 1) file_type = argv[1];

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
    printf("CWD: %s\n", paths.paths[0]);
    for (size_t i = 0; i < paths.count - 1; ++i) {
        printf("  % 4zu  %s\n", i, paths.paths[i + 1]);
    }

    if (paths.paths != NULL) free(paths.paths);
    if (paths_ptr != NULL) free(paths_ptr);
    
    return 0;
}
