#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        *array = malloc(sizeof(float));

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

int date_difference(char *date1, char *date2) {
    int year, month, day;
    sscanf(date1, "%4d%2d%2d", &year, &month, &day);
    int total_months1 = year * 12 + month;
    sscanf(date2, "%4d%2d%2d", &year, &month, &day);
    int total_months2 = year * 12 + month;
    int diff = total_months1 - total_months2;

    if (diff < 0) {
        return -diff;
    }
    return diff;
}

int compare(char *date1, char *time1, char *date2, char *time2) {
    int dateA, dateB;
    sscanf(date1, "%d", &dateA);
    sscanf(date2, "%d", &dateB);
    int diff = dateA - dateB;
    if (diff == 0) {
        int timeA, timeB;
        sscanf(time1, "%d", &timeA);
        sscanf(time2, "%d", &timeB);
        diff = timeA - timeB;
    }
    return diff; // IF date1 is bigger => returns positive number
}

void sort(char ***p_dates_to_show, char ***p_times_to_show, float **p_values_to_show, char ***p_positions_to_show,
          int array_len) {
    for (int i = 0; i < array_len; ++i) {
        for (int j = 0; j < array_len - i - 1; ++j) {
            if (compare((*p_dates_to_show)[j], (*p_times_to_show)[j], (*p_dates_to_show)[j + 1],
                        (*p_times_to_show)[j + 1]) > 0) {
                char *temp_date = (*p_dates_to_show)[j];
                char *temp_time = (*p_times_to_show)[j];
                float temp_value = (*p_values_to_show)[j];
                char *temp_position = (*p_positions_to_show)[j];

                (*p_dates_to_show)[j] = (*p_dates_to_show)[j + 1];
                (*p_dates_to_show)[j + 1] = temp_date;

                (*p_times_to_show)[j] = (*p_times_to_show)[j + 1];
                (*p_times_to_show)[j + 1] = temp_time;

                (*p_values_to_show)[j] = (*p_values_to_show)[j + 1];
                (*p_values_to_show)[j + 1] = temp_value;

                (*p_positions_to_show)[j] = (*p_positions_to_show)[j + 1];
                (*p_positions_to_show)[j + 1] = temp_position;

            }
        }
    }
}

void delete_elements_from_all_arrays(char *id_to_delete, int *p_num_of_records, char ***p_ids, char ***p_positions, char ***p_types,
                                     float **p_values, char ***p_times, char ***p_dates) {
    int removedElements = 0;
    for (int i = 0; i < *p_num_of_records; ++i) {
        if ((strcmp(id_to_delete, (*p_ids)[i]) == 0)) {
            free((*p_ids)[i]);
            free((*p_positions)[i]);
            free((*p_types)[i]);
            free((*p_times)[i]);
            free((*p_dates)[i]);
            for (int j = i; j < *p_num_of_records; ++j) {
                (*p_ids)[j] = (*p_ids)[j + 1];
                (*p_positions)[j] = (*p_positions)[j + 1];
                (*p_types)[j] = (*p_types)[j + 1];
                (*p_values)[j] = (*p_values)[j + 1];
                (*p_times)[j] = (*p_times)[j + 1];
                (*p_dates)[j] = (*p_dates)[j + 1];
            }
            --i;
            ++removedElements;
        }
    }
    if (removedElements > 0) {
        *p_num_of_records -= removedElements;
        *p_ids = realloc(*p_ids, sizeof(char *) * (*p_num_of_records));
        *p_positions = realloc(*p_positions, sizeof(char *) * (*p_num_of_records));
        *p_types = realloc(*p_types, sizeof(char *) * (*p_num_of_records));
        *p_values = realloc(*p_values, sizeof(float) * (*p_num_of_records));
        *p_times = realloc(*p_times, sizeof(char *) * (*p_num_of_records));
        *p_dates = realloc(*p_dates, sizeof(char *) * (*p_num_of_records));
    }
    printf("Vymazalo sa: %d zaznamov!\n", removedElements);
}

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

void case_n(FILE *main_file, int *p_num_of_records, char ***p_ids, char ***p_positions, char ***p_types,
            float **p_values, char ***p_times, char ***p_dates) {
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
        *p_num_of_records = 0;
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

void case_c(int num_of_records, char **ids, char **dates) {
    if (num_of_records == 0) {
        printf("Polia nie su vytvorene\n");
        return;
    }

    int month_n;
    printf("Napiste pocet mesiacov: ");
    scanf_s("%d", &month_n);
    printf("\n");

    FILE *calibration_file;
    calibration_file = fopen("ciachovania.txt", "r");
    if (calibration_file == NULL) {
        printf("Neotvoreny subor ciachovania.txt\n");
        return;
    }

    char temp_id[5 + 1];
    char temp_date[8 + 1];
    for (int i = 0; i < num_of_records; ++i) {
        int found = 0;
        while (fscanf(calibration_file, "%s\n%s\n", temp_id, temp_date) == 2) {
            if (strcmp(ids[i], temp_id) == 0) {
                found = 1;
                int diff = date_difference(dates[i], temp_date);
                if (diff > month_n) {
                    printf("%s ma %d mesiacov od ciachovania\n", ids[i], diff);
                }
            }
        }
        if (found == 0) {
            printf("ID %s nie je ciachovany\n", ids[i]);
        }
        rewind(calibration_file);
    }
}

void case_s(int num_of_records, char **ids, char **types, char **dates, char **times, float *values, char **positions) {
    if (num_of_records == 0) {
        printf("Polia nie su vytvorene\n");
        return;
    }
    char id_to_show[5 + 1];
    char type_to_show[2 + 1];
    printf("Napiste mer. modul a typ:\n");
    scanf("%s %s", id_to_show, type_to_show);
    printf("\n");

    char **dates_to_show;
    char **times_to_show;
    float *values_to_show;
    char **positions_to_show;

    char ***p_dates_to_show = &dates_to_show;
    char ***p_times_to_show = &times_to_show;
    float **p_values_to_show = &values_to_show;
    char ***p_positions_to_show = &positions_to_show;

    int new_arrays_len = 0;

    for (int i = 0; i < num_of_records; ++i) {
        if ((strcmp(ids[i], id_to_show) == 0) && (strcmp(types[i], type_to_show) == 0)) {
            add_element(p_dates_to_show, dates[i], new_arrays_len, 9);
            add_element(p_times_to_show, times[i], new_arrays_len, 5);
            add_element_float(p_values_to_show, values[i], new_arrays_len);
            add_element(p_positions_to_show, positions[i], new_arrays_len, 15);
            dates_to_show = *p_dates_to_show;
            times_to_show = *p_times_to_show;
            values_to_show = *p_values_to_show;
            positions_to_show = *p_positions_to_show;
            new_arrays_len += 1;
        }
    }

    if (new_arrays_len == 0) {
        printf("Pre dany vstup neexistuju zaznamy\n");
        return;
    }
    sort(p_dates_to_show, p_times_to_show, p_values_to_show, p_positions_to_show, new_arrays_len);

    FILE *output_file = fopen("vystup_S.txt.", "w");
    if (output_file == NULL) {
        printf("Pre dany vstup nie je vytvoreny txt subor\n");
        for (int i = 0; i < new_arrays_len; ++i) {
            free((*p_positions_to_show)[i]);
            free((*p_times_to_show)[i]);
            free((*p_dates_to_show)[i]);
        }
        free(*p_positions_to_show);
        free(*p_values_to_show);
        free(*p_times_to_show);
        free(*p_dates_to_show);
        return;
    }
    for (int i = 0; i < new_arrays_len; ++i) {
        char *temp_pos = (positions_to_show)[i];
        char op1, op2;
        int lat1, lat2, lon1, lon2;
        sscanf(temp_pos, "%c%2d%4d%c%2d%4d", &op1, &lat1, &lat2, &op2, &lon1, &lon2);
        fprintf(output_file, "%s%s\t%.5f\t%c%d.%d\t%c%d.%d\n",
                (dates_to_show)[i], (times_to_show)[i], (values_to_show)[i], op1, lat1, lat2, op2, lon1, lon2);
    }
    if (fclose(output_file) != 0) {
        printf("Pre dany vstup nie je vytvoreny txt subor\n");
        for (int i = 0; i < new_arrays_len; ++i) {
            free((*p_positions_to_show)[i]);
            free((*p_times_to_show)[i]);
            free((*p_dates_to_show)[i]);
        }
        free(*p_positions_to_show);
        free(*p_values_to_show);
        free(*p_times_to_show);
        free(*p_dates_to_show);

        return;
    }
    for (int i = 0; i < new_arrays_len; ++i) {
        free((*p_positions_to_show)[i]);
        free((*p_times_to_show)[i]);
        free((*p_dates_to_show)[i]);
    }
    free(*p_positions_to_show);
    free(*p_values_to_show);
    free(*p_times_to_show);
    free(*p_dates_to_show);
    printf("Pre dany vstup je vytvoreny txt subor\n");
    rewind(output_file);
}

void case_h(int num_of_records, char **types, const float *values) {
    if (num_of_records == 0) {
        printf("Polia nie su vytvorene\n");
        return;
    }
    //{RD, RM, RO, PI, PE, PA}
    int RD_n = 0, RM_n = 0, RO_n = 0, PI_n = 0, PE_n = 0, PA_n = 0;
    float RD_max, RM_max, RO_max, PI_max, PE_max, PA_max;
    float RD_min, RM_min, RO_min, PI_min, PE_min, PA_min;
    for (int i = 0; i < num_of_records; ++i) {
        char *current_type = types[i];
        if (strcmp(current_type, "RD") == 0) {
            RD_n += 1;
            if (RD_n == 1) {
                RD_max = values[i];
                RD_min = values[i];
            } else {
                RD_max = values[i] > RD_max ? values[i] : RD_max;
                RD_min = values[i] < RD_min ? values[i] : RD_min;
            }
        }
        if (strcmp(current_type, "RM") == 0) {
            RM_n += 1;
            if (RM_n == 1) {
                RM_max = values[i];
                RM_min = values[i];
            } else {
                RM_max = values[i] > RM_max ? values[i] : RM_max;
                RM_min = values[i] < RM_min ? values[i] : RM_min;
            }
        }
        if (strcmp(current_type, "RO") == 0) {
            RO_n += 1;
            if (RO_n == 1) {
                RO_max = values[i];
                RO_min = values[i];
            } else {
                RO_max = values[i] > RO_max ? values[i] : RO_max;
                RO_min = values[i] < RO_min ? values[i] : RO_min;
            }
        }

        if (strcmp(current_type, "PI") == 0) {
            PI_n += 1;
            if (PI_n == 1) {
                PI_max = values[i];
                PI_min = values[i];
            } else {
                PI_max = values[i] > PI_max ? values[i] : PI_max;
                PI_min = values[i] < PI_min ? values[i] : PI_min;
            }
        }

        if (strcmp(current_type, "PE") == 0) {
            PE_n += 1;
            if (PE_n == 1) {
                PE_max = values[i];
                PE_min = values[i];
            } else {
                PE_max = values[i] > PE_max ? values[i] : PE_max;
                PE_min = values[i] < PE_min ? values[i] : PE_min;
            }
        }
        if (strcmp(current_type, "PA") == 0) {
            PA_n += 1;
            if (PA_n == 1) {
                PA_max = values[i];
                PA_min = values[i];
            } else {
                PA_max = values[i] > PA_max ? values[i] : PA_max;
                PA_min = values[i] < PA_min ? values[i] : PA_min;
            }
        }
    }
    if (RD_n + RM_n + RO_n + PI_n + PE_n + PA_n == 0) {
        return;
    }
    printf("Typ mer. vel.\tPocetnost\tMinimum\tMaximum\n");
    if (RD_n > 0) {
        printf("    %-16s%-12d%-8.2f%-8.2f\n", "RD", RD_n, RD_min, RD_max);
    }
    if (RM_n > 0) {
        printf("    %-16s%-12d%-8.2f%-8.2f\n", "RM", RM_n, RM_min, RM_max);
    }
    if (RO_n > 0) {
        printf("    %-16s%-12d%-8.2f%-8.2f\n", "RO", RO_n, RO_min, RO_max);
    }
    if (PI_n > 0) {
        printf("    %-16s%-12d%-8.2f%-8.2f\n", "PI", PI_n, PI_min, PI_max);
    }
    if (PE_n > 0) {
        printf("    %-16s%-12d%-8.2f%-8.2f\n", "PE", PE_n, PE_min, PE_max);
    }
    if (PA_n > 0) {
        printf("    %-16s%-12d%-8.2f%-8.2f\n", "PA", PA_n, PA_min, PA_max);
    }
}

void case_z(int *p_num_of_records, char ***p_ids, char ***p_positions, char ***p_types,
            float **p_values, char ***p_times, char ***p_dates) {
    if (*p_num_of_records == 0) {
        printf("Polia nie su vytvorene\n");
        return;
    }
    char id_to_delete[5 + 1];
    printf("Id: ");
    scanf("%s", id_to_delete);
    printf("\n");

    delete_elements_from_all_arrays(id_to_delete, p_num_of_records, p_ids, p_positions, p_types,
                                    p_values, p_times, p_dates);
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
                case_c(num_of_records, ids, dates);
                break;
            case 's':
                case_s(num_of_records, ids, types, dates, times, values, positions);
                break;
            case 'h':
                case_h(num_of_records, types, values);
                break;
            case 'z':
                case_z(p_num_of_records, p_ids, p_positions, p_types, p_values, p_times, p_dates);
                ids = *p_ids;
                positions = *p_positions;
                types = *p_types;
                values = *p_values;
                times = *p_times;
                dates = *p_dates;
                break;
            case 'k':
                fclose(main_file);
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
                    *p_num_of_records = 0;
                }
                return 0;
            default:
                continue;
        }
    }
    return 0;
}
