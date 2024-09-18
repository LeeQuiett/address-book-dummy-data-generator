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

// ���Ͽ��� ������ �о���� �Լ�
void load_data_from_file(Addr* records, int num_records, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Failed to open file for reading");
        exit(EXIT_FAILURE);
    }
    fread(records, sizeof(Addr), num_records, file);
    fclose(file);
}

// ������ ��� �Լ�
void print_records(const Addr* records, int num_records) {
    for (int i = 0; i < num_records; i++) {
        printf("��ȭ��ȣ: %d, �̸�: %s, �ּ�: %s\n",
            records[i].tel, records[i].name, records[i].addr);
    }
}

int main() {
    Addr* records = (Addr*)malloc(NUM_RECORDS * sizeof(Addr));
    if (records == NULL) {
        perror("Failed to allocate memory for records");
        return EXIT_FAILURE;
    }

    // ������ ���Ͽ��� �о����
    load_data_from_file(records, NUM_RECORDS, "Tree_test_addr_book.dat");

    // ������ ���
    print_records(records, NUM_RECORDS);

    free(records);
    return 0;
}
