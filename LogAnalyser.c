#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 512

typedef struct {
    char message[256];
    int count;
} ErrorEntry;

typedef struct {
    int total_line;
    int info_count;
    int warn_count;
    int error_count_unique;
    int error_capacity;
    ErrorEntry *error_entries;
} AnalysisResult;

AnalysisResult* init_analyser() {
    AnalysisResult *result = malloc(sizeof(AnalysisResult));
    if (!result) return NULL;

    result->total_line = 0;
    result->info_count = 0;
    result->warn_count = 0;
    result->error_count_unique = 0;
    result->error_capacity = 100;
    result->error_entries = calloc(result->error_capacity, sizeof(ErrorEntry));

    if (!result->error_entries) {
        free(result);
        return NULL;
    }

    return result;
}

void free_analyser(AnalysisResult *result) {
    if (!result) return;
    free(result->error_entries);
    free(result);
}

void add_error_message(AnalysisResult *result, const char *message) {
    if (!result || !message) return;

    for (int i = 0; i < result->error_count_unique; i++) {
        if (strcmp(result->error_entries[i].message, message) == 0) {
            result->error_entries[i].count++;
            return;
        }
    }

    if (result->error_count_unique >= result->error_capacity) {
        int new_capacity = result->error_capacity * 2;
        ErrorEntry *new_entries = realloc(result->error_entries, new_capacity * sizeof(ErrorEntry));
        if (!new_entries) return;
        result->error_entries = new_entries;
        result->error_capacity = new_capacity;
    }

    strncpy(result->error_entries[result->error_count_unique].message, message, 255);
    result->error_entries[result->error_count_unique].message[255] = '\0';
    result->error_entries[result->error_count_unique].count = 1;
    result->error_count_unique++;
}

void analyse_file(AnalysisResult *analyser, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        analyser->total_line++;

        if (strstr(line, "INFO")) {
            analyser->info_count++;
        } else if (strstr(line, "WARN")) {
            analyser->warn_count++;
        } else if (strstr(line, "ERROR")) {
            add_error_message(analyser, line);
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <log-file-path>\n", argv[0]);
        return 1;
    }

    AnalysisResult *analyser = init_analyser();
    if (!analyser) {
        printf("Failed to allocate memory!\n");
        return 1;
    }

    analyse_file(analyser, argv[1]);

    printf("Total lines: %d\n", analyser->total_line);
    printf("INFO: %d\n", analyser->info_count);
    printf("WARN: %d\n", analyser->warn_count);
    printf("Unique ERRORs: %d\n", analyser->error_count_unique);

    for (int i = 0; i < analyser->error_count_unique; i++) {
        printf("%s: %d times\n", analyser->error_entries[i].message, analyser->error_entries[i].count);
    }

    free_analyser(analyser);
    return 0;
}
