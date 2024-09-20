# address-book-dummy-data-generator

전화번호부의 더미 데이터를 생성하고 이를 파일에 저장하는 기능을 수행합니다.

<details>
<summary>ver 0: PK(전화번호) 중복검증을 선형적으로 루프를 돌며 수행하였습니다.</summary>
<div markdown="1">

## 코드 상세 설명

## 이름을 생성합니다.

### 1.  `generate_random_korean_name`

```c
void generate_random_korean_name(char* name) {
    const char* family_names[] = { "김", "이", "박", "정", "최", "강", "조", "윤", "장", "임" };
    const char* given_names[] = { "민", "서", "지", "우", "현", "준", "하", "영", "소", "은" };

    snprintf(name, NAME_LENGTH, "%s%s%s",
        family_names[rand() % (sizeof(family_names) / sizeof(family_names[0]))],
        given_names[rand() % (sizeof(given_names) / sizeof(given_names[0]))],
        given_names[rand() % (sizeof(given_names) / sizeof(given_names[0]))]);
}
```

- **성**과 **이름**의 조합으로 이름을 생성합니다.
- 성은 `family_names` 배열에서 무작위로 선택됩니다.
- 이름의 두 글자는 `given_names` 배열에서 무작위로 선택됩니다.
- `snprintf` 함수를 사용하여 생성된 이름을 `name` 변수에 저장합니다.

## 전화번호를 생성합니다.

### `generate_random_tel`

```c
void generate_random_tel(char* tel) {
    snprintf(tel, TEL_LENGTH, "010-%04d-%04d", rand() % 10000, rand() % 10000);
}
```

- `rand() % 10000`을 사용하여 4자리 숫자를 생성합니다.
- `snprintf` 함수를 사용하여 생성된 전화번호를 `tel` 변수에 저장합니다.

## 주소를 생성합니다.

### `generate_random_korean_address`

```c
void generate_random_korean_address(char* addr) {
    snprintf(addr, ADDR_LENGTH, "%s %s %d동 %d호",
        cities[rand() % (sizeof(cities) / sizeof(cities[0]))],
        districts[rand() % (sizeof(districts) / sizeof(districts[0]))],
        rand() % 20 + 1, // 동
        rand() % 100 + 1); // 호
}
```

```c
const char* cities[] = {
    "서울", "부산", "대구", "인천", "광주", "대전", "울산", "세종", 
    "경기", "강원", "충북", "충남", "전북", "전남", "경북", "경남", "제주"
};

const char* districts[] = {
    "강남구", "강동구", "강북구", "강서구", "관악구", "광진구", "구로구", 
    "금천구", "노원구", "도봉구", "동대문구", "동작구", "마포구", "서대문구", 
    "서초구", "성동구", "성북구", "송파구", "양천구", "영등포구", "용산구", 
    "은평구", "종로구", "중구", "중랑구", "군포시", "수원시", "당동", "엑스포로", 
    "루센트블록동네", "디트로이트", "컴튼", "로스엔젤레스"
};
```

```c
void generate_random_korean_name(char* name) {
    const char* family_names[] = { "김", "이", "박", "정", "최", "강", "조", 
    "윤", "장", "임" };
    const char* given_names[] = { "민", "서", "지", "우", "현", "준", "하", 
    "영", "소", "은" };

    snprintf(name, NAME_LENGTH, "%s%s%s",
        family_names[rand() % (sizeof(family_names) / sizeof(family_names[0]))],
        given_names[rand() % (sizeof(given_names) / sizeof(given_names[0]))],
        given_names[rand() % (sizeof(given_names) / sizeof(given_names[0]))]);
}
```

- `cities` 배열에서 무작위로 도시를 선택합니다.
- `districts` 배열에서 무작위로 구를 선택합니다.
- 동(아파트 동)을 `rand() % 20 + 1`을 사용하여 1에서 20 사이의 숫자로 생성합니다.
- 호(아파트 호)를 `rand() % 100 + 1`을 사용하여 1에서 100 사이의 숫자로 생성합니다.
- `snprintf` 함수를 사용하여 전체 주소를 `addr` 변수에 저장합니다.

---

## 전화번호의 중복을 검증합니다.

1. **중복 검증 함수**:
    
    ```c
    int is_tel_unique(const char* tel, const char* existing_tels[], int count)
    ```
    
    - **입력**:
        - `tel`: 검증할 전화번호.
        - `existing_tels`: 기존 전화번호 목록.
        - `count`: 현재 전화번호의 수.
    - **출력**:
        - 중복된 전화번호가 있으면 `0`, 그렇지 않으면 `1`을 반환합니다.
2. **중복 검사 과정**:
    
    ```c
    for (int i = 0; i < count; i++) {
        if (strcmp(tel, existing_tels[i]) == 0) {
            return 0; // 중복된 전화번호
        }
    }
    return 1; // 유일한 전화번호
    ```
    
    - 루프를 돌며  `existing_tels` 배열의 각 전화번호와 현재 생성한 전화번호(`tel`)를 비교합니다.
    - `strcmp(tel, existing_tels[i])`를 사용하여 전화번호가 일치하는지 검사합니다.
    - 중복된 전화번호가 발견되면 `0`을 반환하여 중복을 표시합니다.
    - `0`을 반환받으면 반복문을 탈출하지 못하고 다시 중복 검증 함수를 호출합니다.
    - 모든 전화번호와 비교한 후 중복이 없으면 `1`을 반환하며 반복문을 탈출해 전화번호를 저장합니다.
        
        ```c
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
        
             generate_random_korean_name(records[i].name);
             strcpy(records[i].tel, tel);
             generate_random_korean_address(records[i].addr);
         }
        ```

---

### 🌟 기여 방법

오류를 발견하거나 개선 사항이 있을 경우, 이슈를 열거나 풀 리퀘스트를 제출해 주세요. 모든 기여는 환영합니다!

### 📧 연락처

질문이나 문의 사항이 있으시면 vkdnjpo1512@naver.com으로 연락 주세요.

### 🗝️LICENSE

이 프로젝트는 MIT 라이선스에 따라 배포됩니다.

</div>
</details>

<!--# address-book-dummy-data-generator-->

## ver1에서 개선한 내용

### 전화번호 중복 검증
더미 데이터 생성 시 실행 시간 **3시간 이상 -> 30초 미만**으로 단축

본 프로그램은 전화번호의 중복을 효율적으로 검증하기 위해 이진 탐색 트리(BST)를 사용합니다. 기존의 선형 검색 방식은 100만 개 이상의 더미 데이터를 생성할 경우 성능이 크게 저하되어, 3시간짜리 영화를 보고 와도 끝나지 않는 문제가 있었습니다.

**개선된 방법:**

- **이진 탐색 트리 사용:** 전화번호를 BST에 저장함으로써 중복 검증 시 탐색의 평균 시간 복잡도를 O(log n)으로 줄일 수 있습니다. 이진 탐색 트리는 데이터가 정렬되어 있으며, 각 노드의 왼쪽 서브트리에는 더 작은 값, 오른쪽 서브트리에는 더 큰 값을 저장합니다.

- **중복 검증 과정:** 새로운 전화번호를 생성할 때마다 BST에서 해당 전화번호의 존재 여부를 확인합니다. 중복되지 않는 경우에만 BST에 삽입하고, 구조체에 데이터를 저장합니다.

이러한 방식으로 전화번호 중복 검증의 성능을 대폭 개선하여 1000만 개 이상의 더미 데이터 생성에 30초 미만의 시간이 소요되도록 하였으며, 프로그램의 실행 시간을 크게 단축할 수 있었습니다.

## 코드 상세 설명

### 이름 생성

`generate_random_korean_name` 함수는 무작위로 성과 이름을 조합하여 한국어 이름을 생성합니다.

```c
void generate_random_korean_name(char* name) {
    const char* family_names[] = { "김", "이", "박", "정", "최", "강", "조", "윤", "장", "임" };
    const char* given_names[] = { "민", "서", "지", "우", "현", "준", "하", "영", "소", "은" };

    snprintf(name, NAME_LENGTH, "%s%s%s",
        family_names[rand() % (sizeof(family_names) / sizeof(family_names[0]))],
        given_names[rand() % (sizeof(given_names) / sizeof(given_names[0]))],
        given_names[rand() % (sizeof(given_names) / sizeof(given_names[0]))]);
}
```

- 성은 `family_names` 배열에서 무작위로 선택됩니다.
- 이름의 두 글자는 `given_names` 배열에서 무작위로 선택됩니다.
- `snprintf` 함수를 사용하여 생성된 이름을 `name` 변수에 저장합니다.

### 전화번호 생성

`generate_random_tel` 함수는 무작위로 전화번호를 생성합니다.
앞자리, 뒷자리 부분을 따로 생성하여 중복 가능성을 줄임으로 실행 시간을 단축시켰습니다.

```c
void generate_random_tel(int* tel) {
    int part1 = rand() % 10000; // 앞자리 부분 생성
    int part2 = rand() % 10000; // 뒷자리 부분 생성
    *tel = part1 * 10000 + part2;
}
```

- `rand() % 10000`을 사용하여 4자리 숫자를 생성합니다.
- 생성된 전화번호는 `tel` 변수에 저장됩니다.

### 주소 생성

`generate_random_korean_address` 함수는 무작위로 한국어 주소를 생성합니다.

```c
void generate_random_korean_address(char* addr) {
    snprintf(addr, ADDR_LENGTH, "%s %s %d동 %d호",
        cities[rand() % (sizeof(cities) / sizeof(cities[0]))],
        districts[rand() % (sizeof(districts) / sizeof(districts[0]))],
        rand() % 20 + 1, // 동
        rand() % 100 + 1); // 호
}
```

- `cities` 배열에서 무작위로 도시를 선택합니다.
- `districts` 배열에서 무작위로 구를 선택합니다.
- 동과 호는 각각 1에서 20 사이, 1에서 100 사이의 숫자로 생성됩니다.
- `snprintf` 함수를 사용하여 전체 주소를 `addr` 변수에 저장합니다.

### 전화번호 중복 검증

중복 검증 함수는 BST를 사용하여 전화번호의 유일성을 확인합니다.

```c
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
```

- 전화번호가 존재하지 않으면 0을 반환하고, 존재하면 1을 반환합니다.
- BST를 활용하여 중복 검증을 수행함으로써 성능을 개선하였습니다.

### 기여 방법

오류를 발견하거나 개선 사항이 있을 경우, 이슈를 열거나 풀 리퀘스트를 제출해 주세요. 모든 기여는 환영합니다!

📧 **연락처**  
질문이나 문의 사항이 있으시면 vkdnjpo1512@naver.com으로 연락 주세요.

🗝️ **LICENSE**  
이 프로젝트는 MIT 라이선스에 따라 배포됩니다.
