#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_RECORDS 10000
#define TEL_LENGTH 15

typedef struct {
    char name[30];
    char tel[TEL_LENGTH];
    char addr[100];
} Addr;

// 무작위 문자열 생성 함수
void generate_random_string(char* str, size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (size_t i = 0; i < length - 1; i++) {
        str[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    str[length - 1] = '\0';
}

// 무작위 전화번호 생성 함수
void generate_random_tel(char* tel) {
    snprintf(tel, TEL_LENGTH, "010-%04d-%04d", rand() % 10000, rand() % 10000);
}

// 중복 전화번호 검증 함수
int is_tel_unique(const char* tel, const char* existing_tels[], int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(tel, existing_tels[i]) == 0) {
            return 0; // 중복된 전화번호
        }
    }
    return 1; // 유일한 전화번호
}

// 전화번호 복사 함수 (strdup 대체)
char* my_strdup(const char* s) {
    size_t len = strlen(s) + 1;
    char* copy = (char*)malloc(len);
    if (copy == NULL) {
        return NULL; // 메모리 할당 실패
    }
    strcpy(copy, s);
    return copy;
}

// 파일에 데이터 저장 함수
void save_data_to_file(const Addr* records, int num_records, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }
    fwrite(records, sizeof(Addr), num_records, file);
    fclose(file);
}

// 메모리 해제 함수
void free_string_array(char* array[], int count) {
    for (int i = 0; i < count; i++) {
        free(array[i]);
    }
}

int main() {
    Addr* records = (Addr*)malloc(NUM_RECORDS * sizeof(Addr));
    if (records == NULL) {
        perror("Failed to allocate memory for records");
        return EXIT_FAILURE;
    }

    char** existing_tels = (char**)malloc(NUM_RECORDS * sizeof(char*));
    if (existing_tels == NULL) {
        perror("Failed to allocate memory for existing_tels");
        free(records);
        return EXIT_FAILURE;
    }

    int tel_count = 0;

    // 시드 초기화
    srand(time(NULL));

    // 더미 데이터 생성
    for (int i = 0; i < NUM_RECORDS; i++) {
        char tel[TEL_LENGTH];
        int unique = 0;

        // 중복되지 않는 전화번호 생성
        while (!unique) {
            generate_random_tel(tel);
            unique = is_tel_unique(tel, existing_tels, tel_count);
        }

        // 전화번호 저장
        existing_tels[tel_count] = my_strdup(tel);
        if (existing_tels[tel_count] == NULL) {
            perror("Failed to duplicate phone number");
            free_string_array(existing_tels, tel_count);
            free(records);
            return EXIT_FAILURE;
        }
        tel_count++;

        // 구조체 필드에 데이터 저장
        generate_random_string(records[i].name, sizeof(records[i].name));
        strcpy(records[i].tel, tel);
        generate_random_string(records[i].addr, sizeof(records[i].addr));
    }

    // 데이터 파일에 저장
    save_data_to_file(records, NUM_RECORDS, "addr_book.dat");

    // 메모리 해제
    free_string_array(existing_tels, tel_count);
    free(existing_tels);
    free(records);

    return 0;
}
