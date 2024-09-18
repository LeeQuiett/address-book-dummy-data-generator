#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_RECORDS 10000
#define NAME_LENGTH 12
#define ADDR_LENGTH 100

typedef struct {
    int tel;
    char name[NAME_LENGTH];
    char addr[ADDR_LENGTH];
} Addr;

// 파일에서 데이터 읽어오기 함수
void load_data_from_file(Addr* records, int num_records, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Failed to open file for reading");
        exit(EXIT_FAILURE);
    }
    fread(records, sizeof(Addr), num_records, file);
    fclose(file);
}

// 데이터 출력 함수
void print_records(const Addr* records, int num_records) {
    for (int i = 0; i < num_records; i++) {
        printf("전화번호: %d, 이름: %s, 주소: %s\n",
            records[i].tel, records[i].name, records[i].addr);
    }
}

int main() {
    Addr* records = (Addr*)malloc(NUM_RECORDS * sizeof(Addr));
    if (records == NULL) {
        perror("Failed to allocate memory for records");
        return EXIT_FAILURE;
    }

    // 데이터 파일에서 읽어오기
    load_data_from_file(records, NUM_RECORDS, "Tree_test_addr_book.dat");

    // 데이터 출력
    print_records(records, NUM_RECORDS);

    free(records);
    return 0;
}
