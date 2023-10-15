#include <stdio.h>
#include <stdlib.h>

void add_element(char ***array, const char *element, int num_of_records, const int len_of_element) {
    if (num_of_records == 0) {
        *array = malloc(sizeof(char *) * (1));
    } else {
        *array = realloc(*array, sizeof(char *) * (num_of_records + 1));
    }

    if (*array == NULL) {
        printf("Memory allocation problem!\n");
        exit(1);
    }
    (*array)[num_of_records] = malloc(sizeof(char) * len_of_element);
    if ((*array)[num_of_records] == NULL) {
        printf("Memory allocation problem!\n");
        exit(1);
    }
    int i = 0;
    while (element[i] != '\0') {
        (*array)[num_of_records][i] = element[i];
        i++;
    }
    (*array)[num_of_records][i] = '\0';
}

void add_element_float(float **array, float element, int num_of_records) {
    if (num_of_records == 0) {
        *array = malloc( sizeof(float));

    } else {

        *array = realloc(*array, (num_of_records + 1) * sizeof(float));
    }
    if (*array == NULL) {
        printf("Memory allocation problem!\n");
        exit(1);
    }

    (*array)[num_of_records] = element;
}

void print_record(char *id, char *position, char *type, float value, char *time, char *date) {
    printf("ID. mer. modulu: %s\n", id);
    printf("Pozicia modulu: %s\n", position);
    printf("Typ mer. veliciny: %s\n", type);
    printf("Hodnota: %.2f\n", value);
    printf("Cas merania: %s\n", time);
    printf("Datum merania: %s\n\n", date);
}

/**
 * @brief Takes pointer to FILE* and change FILE*, should reassign the value after function call
 *
 * @param A pointer to the FILE* object to be operated on.
 *
 * @return void
 */

void case_v(FILE **p_main_file, const int num_of_records, char **ids, char **positions, char **types,
            float *values, char **times, char **dates) {
    if (*p_main_file == NULL) {
        *p_main_file = fopen("dataloger.txt", "r");
        if (*p_main_file == NULL) {
            printf("Neotvoreny subot\n");
            return;
        }
    }
    if (*p_main_file != NULL) {
        if (num_of_records == 0) {
            rewind(*p_main_file);
            char temp_id[5 + 1];
            char temp_position[14 + 1];
            char temp_type[2 + 1];
            float temp_value;
            char temp_time[4 + 1];
            char temp_date[8 + 1];
            while (fscanf(*p_main_file, "%s\n%s\n%s\n%f\n%s\n%s\n",
                          temp_id,
                          temp_position,
                          temp_type,
                          &temp_value,
                          temp_time,
                          temp_date) == 6) {
                print_record(temp_id, temp_position, temp_type, temp_value, temp_time, temp_date);
            }
        } else {
            for (int i = 0; i < num_of_records; ++i) {
                print_record(ids[i], positions[i], types[i], values[i], times[i], dates[i]);
            }
        }
    }
}

void
case_n(FILE *main_file, int *p_num_of_records, char ***p_ids, char ***p_positions, char ***p_types, float **p_values,
       char ***p_times, char ***p_dates) {
    if (main_file == NULL) {
        printf("Neotvoreny subor\n");
        return;
    }
    if (*p_num_of_records != 0) {
        for (int i = 0; i < *p_num_of_records; ++i) {
            free((*p_ids)[i]);
            free((*p_positions)[i]);
            free((*p_types)[i]);
            free((*p_times)[i]);
            free((*p_dates)[i]);
        }
        free(*p_ids);
        free(*p_positions);
        free(*p_types);
        free(*p_values);
        free(*p_times);
        free(*p_dates);
    }
    rewind(main_file);
    char temp_id[5 + 1];
    char temp_position[14 + 1];
    char temp_type[2 + 1];
    float temp_value;
    char temp_time[4 + 1];
    char temp_date[8 + 1];
    while (fscanf(main_file, "%s\n%s\n%s\n%f\n%s\n%s\n",
                  temp_id,
                  temp_position,
                  temp_type,
                  &temp_value,
                  temp_time,
                  temp_date) == 6) {
        add_element(p_ids, temp_id, *p_num_of_records, 6);
        add_element(p_positions, temp_position, *p_num_of_records, 15);
        add_element(p_types, temp_type, *p_num_of_records, 3);
        add_element_float(p_values, temp_value, *p_num_of_records);
        add_element(p_times, temp_time, *p_num_of_records, 5);
        add_element(p_dates, temp_date, *p_num_of_records, 9);
        *p_num_of_records += 1;
    }
}

void case_c() {

}

void case_s() {

}

void case_h() {

}

void case_z() {

}

int main() {
    char **ids;
    char **positions;
    char **types;
    float *values;
    char **times;
    char **dates;

    char ***p_ids = &ids;
    char ***p_positions = &positions;
    char ***p_types = &types;
    float **p_values = &values;
    char ***p_times = &times;
    char ***p_dates = &dates;

    FILE *main_file = NULL;
    FILE **p_main_file = &main_file;
    int num_of_records = 0;
    int *p_num_of_records = &num_of_records;

    char operation;
    while (scanf(" %c", &operation) == 1) {
        switch (operation) {
            case 'v':
                case_v(p_main_file, num_of_records, ids, positions, types, values, times, dates);
                main_file = *p_main_file;
                break;
            case 'n':
                case_n(main_file, p_num_of_records, p_ids, p_positions, p_types, p_values, p_times, p_dates);
                ids = *p_ids;
                positions = *p_positions;
                types = *p_types;
                values = *p_values;
                times = *p_times;
                dates = *p_dates;
                break;
            case 'c':
                case_c();
                break;
            case 's':
                case_s();
                break;
            case 'h':
                case_h();
                break;
            case 'z':
                case_z();
                break;
            case 'k':
                break;
            default:
                continue;
        }
    }
    return 0;
}
