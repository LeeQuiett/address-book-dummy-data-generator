# address-book-dummy-data-generator

- 전화번호부의 더미 데이터를 생성하고 이를 파일에 저장하는 기능을 수행합니다.

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
