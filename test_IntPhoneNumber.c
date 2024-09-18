#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_RECORDS 10000
#define TEL_LENGTH 12
#define NAME_LENGTH 12
#define ADDR_LENGTH 100

typedef struct {
    int tel;
    char name[NAME_LENGTH];
    char addr[ADDR_LENGTH];
} Addr;

typedef struct TreeNode {
    int tel;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// 한국의 주소 목록
const char* cities[] = {
    "서울", "부산", "대구", "인천", "광주", "대전", "울산", "세종", "경기", "강원", "충북", "충남", "전북", "전남", "경북", "경남", "제주"
};

const char* districts[] = {
    "강남구", "강동구", "강북구", "강서구", "관악구", "광진구", "구로구", "금천구", "노원구", "도봉구", "동대문구", "동작구", "마포구", "서대문구", "서초구", "성동구", "성북구", "송파구", "양천구", "영등포구", "용산구", "은평구", "종로구", "중구", "중랑구", "군포시", "수원시", "당동", "엑스포로", "루센트블록동네", "디트로이트", "컴튼", "로스엔젤레스"
};

// 무작위 문자열 생성 함수
void generate_random_string(char* str, size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (size_t i = 0; i < length - 1; i++) {
        str[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    str[length - 1] = '\0';
}

// 무작위 한국어 이름 생성 함수
void generate_random_korean_name(char* name) {
    const char* family_names[] = { "김", "이", "박", "정", "최", "강", "조", "윤", "장", "임" };
    const char* given_names[] = { "민", "서", "지", "우", "현", "준", "하", "영", "소", "은" };

    snprintf(name, NAME_LENGTH, "%s%s%s",
        family_names[rand() % (sizeof(family_names) / sizeof(family_names[0]))],
        given_names[rand() % (sizeof(given_names) / sizeof(given_names[0]))],
        given_names[rand() % (sizeof(given_names) / sizeof(given_names[0]))]);
}

// 무작위 전화번호 생성 함수
void generate_random_tel(int* tel) {
    *tel = rand() % 100000000;
    // snprintf(tel, TEL_LENGTH, "010-%04d-%04d", rand() % 10000, rand() % 10000);
}

// 무작위 주소 생성 함수
void generate_random_korean_address(char* addr) {
    snprintf(addr, ADDR_LENGTH, "%s %s %d동 %d호",
        cities[rand() % (sizeof(cities) / sizeof(cities[0]))],
        districts[rand() % (sizeof(districts) / sizeof(districts[0]))],
        rand() % 20 + 1, // 동
        rand() % 100 + 1); // 호
}

// 중복 전화번호 검증 함수 (BST)
TreeNode* create_node(int tel) {
    TreeNode* new_node = (TreeNode*)malloc(sizeof(TreeNode));
    if (new_node == NULL) {
        perror("Failed to allocate memory for new node");
        exit(EXIT_FAILURE);
    }
   // strcpy(new_node->tel, tel);
    new_node->tel = tel;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

TreeNode* insert_node(TreeNode* root, int tel) {
    if (root == NULL) {
        return create_node(tel);
    }
    if (tel < root->tel) {
        root->left = insert_node(root->left, tel);
    }
    else if (tel > root->tel) {
        root->right = insert_node(root->right, tel);
    }
    return root;
}

int search_node(TreeNode* root, int tel) {
    if (root == NULL) {
        return 0; // 전화번호가 존재하지 않음
    }
    if (tel < root->tel) {
        return search_node(root->left, tel);
    }
    else if (tel > root->tel) {
        return search_node(root->right, tel);
    }
    else {
        return 1; // 전화번호가 존재함
    }
}

void free_tree(TreeNode* root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
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

    TreeNode* bst_root = NULL;

    // 시드 초기화
    srand(time(NULL));

    // 더미 데이터 생성
    for (int i = 0; i < NUM_RECORDS; i++) {
        int tel;
        int unique = 0;

        // 중복되지 않는 전화번호 생성
        while (!unique) {
            generate_random_tel(&tel);
            unique = !search_node(bst_root, tel); // 중복되지 않을 때
        }

        // 전화번호 BST에 삽입
        bst_root = insert_node(bst_root, tel);

        // 구조체 필드에 데이터 저장
        generate_random_korean_name(records[i].name);
        //strcpy(records[i].tel, &tel);
        records[i].tel = tel;
        generate_random_korean_address(records[i].addr);
    }

    // 데이터 파일에 저장
    save_data_to_file(records, NUM_RECORDS, "Tree_test_addr_book.dat");

    // 메모리 해제
    free_tree(bst_root);
    free(records);

    return 0;
}
