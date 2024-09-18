#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_RECORDS 10000000
#define TEL_LENGTH 15
#define NAME_LENGTH 100
#define ADDR_LENGTH 200

typedef struct {
    char name[NAME_LENGTH];
    char tel[TEL_LENGTH];
    char addr[ADDR_LENGTH];
} Addr;

typedef struct TreeNode {
    char tel[TEL_LENGTH];
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// �ѱ��� �ּ� ���
const char* cities[] = {
    "����", "�λ�", "�뱸", "��õ", "����", "����", "���", "����", "���", "����", "���", "�泲", "����", "����", "���", "�泲", "����"
};

const char* districts[] = {
    "������", "������", "���ϱ�", "������", "���Ǳ�", "������", "���α�", "��õ��", "�����", "������", "���빮��", "���۱�", "������", "���빮��", "���ʱ�", "������", "���ϱ�", "���ı�", "��õ��", "��������", "��걸", "����", "���α�", "�߱�", "�߶���", "������", "������", "�絿", "��������", "�缾Ʈ��ϵ���", "��Ʈ����Ʈ", "��ư", "�ν���������"
};

// ������ ���ڿ� ���� �Լ�
void generate_random_string(char* str, size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (size_t i = 0; i < length - 1; i++) {
        str[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    str[length - 1] = '\0';
}

// ������ �ѱ��� �̸� ���� �Լ�
void generate_random_korean_name(char* name) {
    const char* family_names[] = { "��", "��", "��", "��", "��", "��", "��", "��", "��", "��" };
    const char* given_names[] = { "��", "��", "��", "��", "��", "��", "��", "��", "��", "��" };

    snprintf(name, NAME_LENGTH, "%s%s%s",
        family_names[rand() % (sizeof(family_names) / sizeof(family_names[0]))],
        given_names[rand() % (sizeof(given_names) / sizeof(given_names[0]))],
        given_names[rand() % (sizeof(given_names) / sizeof(given_names[0]))]);
}

// ������ ��ȭ��ȣ ���� �Լ�
void generate_random_tel(char* tel) {
    snprintf(tel, TEL_LENGTH, "010-%04d-%04d", rand() % 10000, rand() % 10000);
}

// ������ �ּ� ���� �Լ�
void generate_random_korean_address(char* addr) {
    snprintf(addr, ADDR_LENGTH, "%s %s %d�� %dȣ",
        cities[rand() % (sizeof(cities) / sizeof(cities[0]))],
        districts[rand() % (sizeof(districts) / sizeof(districts[0]))],
        rand() % 20 + 1, // ��
        rand() % 100 + 1); // ȣ
}

// �ߺ� ��ȭ��ȣ ���� �Լ� (BST)
TreeNode* create_node(const char* tel) {
    TreeNode* new_node = (TreeNode*)malloc(sizeof(TreeNode));
    if (new_node == NULL) {
        perror("Failed to allocate memory for new node");
        exit(EXIT_FAILURE);
    }
    strcpy(new_node->tel, tel);
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

TreeNode* insert_node(TreeNode* root, const char* tel) {
    if (root == NULL) {
        return create_node(tel);
    }
    int cmp = strcmp(tel, root->tel);
    if (cmp < 0) {
        root->left = insert_node(root->left, tel);
    }
    else if (cmp > 0) {
        root->right = insert_node(root->right, tel);
    }
    return root;
}

int search_node(TreeNode* root, const char* tel) {
    if (root == NULL) {
        return 0; // ��ȭ��ȣ�� �������� ����
    }
    int cmp = strcmp(tel, root->tel);
    if (cmp < 0) {
        return search_node(root->left, tel);
    }
    else if (cmp > 0) {
        return search_node(root->right, tel);
    }
    else {
        return 1; // ��ȭ��ȣ�� ������
    }
}

void free_tree(TreeNode* root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

// ���Ͽ� ������ ���� �Լ�
void save_data_to_file(const Addr* records, int num_records, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }
    fwrite(records, sizeof(Addr), num_records, file);
    fclose(file);
}

// �޸� ���� �Լ�
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

    TreeNode* bst_root = NULL;

    // �õ� �ʱ�ȭ
    srand(time(NULL));

    // ���� ������ ����
    for (int i = 0; i < NUM_RECORDS; i++) {
        char tel[TEL_LENGTH];
        int unique = 0;

        // �ߺ����� �ʴ� ��ȭ��ȣ ����
        while (!unique) {
            generate_random_tel(tel);
            unique = !search_node(bst_root, tel); // �ߺ����� ���� ��
        }

        // ��ȭ��ȣ BST�� ����
        bst_root = insert_node(bst_root, tel);

        // ����ü �ʵ忡 ������ ����
        generate_random_korean_name(records[i].name);
        strcpy(records[i].tel, tel);
        generate_random_korean_address(records[i].addr);
    }

    // ������ ���Ͽ� ����
    save_data_to_file(records, NUM_RECORDS, "Tree_test_addr_book.dat");

    // �޸� ����
    free_tree(bst_root);
    free(records);

    return 0;
}
