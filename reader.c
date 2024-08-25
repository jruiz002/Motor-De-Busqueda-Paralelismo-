#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include "reader.h"

#define MAX_LINE_LENGTH 2048
#define MAX_TITLE_LENGTH 500
#define MAX_DATE_LENGTH 40

typedef struct {
    char title[MAX_TITLE_LENGTH];
    float vote_average;
    char release_date[MAX_DATE_LENGTH];
    long long revenue;
} Movie;

FILE *output_file;
omp_lock_t writelock;

int parseCSVLine(char *line, Movie *movie) {
    char *token;
    int field = 0;

    char temp[MAX_LINE_LENGTH];
    strcpy(temp, line);
    for (char *p = temp; *p; ++p) {
        if (*p == '\"') *p = ' ';
    }

    token = strtok(temp, ",");
    while (token != NULL) {
        char *start = token;
        while (*start == ' ') start++;
        char *end = start + strlen(start) - 1;
        while (end > start && *end == ' ') end--;
        *(end + 1) = '\0';

        switch (field) {
            case 1:  // "title"
                strncpy(movie->title, start, sizeof(movie->title));
                movie->title[sizeof(movie->title) - 1] = '\0';
                break;
            case 2:  // "vote_average"
                movie->vote_average = strtof(start, NULL);
                break;
            case 5:  // "release_date"
                strncpy(movie->release_date, start, sizeof(movie->release_date));
                movie->release_date[sizeof(movie->release_date) - 1] = '\0';
                break;
            case 6:  // "revenue"
                movie->revenue = atoll(start);
                break;
            default:
                break;
        }
        token = strtok(NULL, ",");
        field++;
    }

    if (field < 7) {
        return 0;
    }

    return 1;
}

void searchMovies(const char *filename, const char *search_param, const char *search_value, int num_threads) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error al abrir el archivo");
        return;
    }

    output_file = fopen("busqueda.csv", "w");
    if (!output_file) {
        perror("Error al abrir el archivo de salida");
        fclose(file);
        return;
    }

    fprintf(output_file, "\"title\",\"vote_average\",\"release_date\",\"revenue\"\n");

    omp_init_lock(&writelock);
    omp_set_num_threads(num_threads);

    char line[MAX_LINE_LENGTH];
    int line_count = 0;

    if (fgets(line, sizeof(line), file) == NULL) {
        perror("Error al leer la cabecera");
        fclose(file);
        fclose(output_file);
        return;
    }

    char **lines = NULL;
    size_t lines_size = 0;
    while (fgets(line, sizeof(line), file)) {
        char **temp = (char **)realloc(lines, (lines_size + 1) * sizeof(char *));
        if (temp == NULL) {
            fprintf(stderr, "Error al asignar memoria\n");
            free(lines);
            fclose(file);
            fclose(output_file);
            return;
        }
        lines = temp;
        lines[lines_size] = strdup(line);
        lines_size++;
    }
    fclose(file);

    // Empieza a medir el tiempo
    double start_time = omp_get_wtime();

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < lines_size; i++) {
        Movie movie;
        if (parseCSVLine(lines[i], &movie)) {
            int match = 0;
            if (strcmp(search_param, "title") == 0 && strstr(movie.title, search_value)) {
                match = 1;
            } else if (strcmp(search_param, "vote_average") == 0 && movie.vote_average == atof(search_value)) {
                match = 1;
            } else if (strcmp(search_param, "release_date") == 0 && strcmp(movie.release_date, search_value) == 0) {
                match = 1;
            } else if (strcmp(search_param, "revenue") == 0 && movie.revenue == atoll(search_value)) {
                match = 1;
            }

            if (match) {
                omp_set_lock(&writelock);
                fprintf(output_file, "\"%s\",%.2f,\"%s\",\"%lld\"\n", movie.title, movie.vote_average, movie.release_date, movie.revenue);
                omp_unset_lock(&writelock);
            }
        }
        free(lines[i]);
    }

    // Termina de medir el tiempo
    double end_time = omp_get_wtime();

    double time_taken = end_time - start_time;

    free(lines);
    omp_destroy_lock(&writelock);
    fclose(output_file);

    // Imprime el tiempo tomado
    printf("Tiempo tomado para la búsqueda con %d hilos: %.6f segundos\n", num_threads, time_taken);
}