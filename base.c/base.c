#include <stdio.h>
#include <stdlib.h>

// 1. 64KB 물리 메모리 배열 생성 (요구사항 반영)
#define MEMORY_SIZE (64 * 1024)
unsigned char physical_memory[MEMORY_SIZE];

typedef struct {
    unsigned int base;
    unsigned int bounds;
} TZASC;

// 2. 가상 주소 -> 물리 주소 변환 함수
int translate(TZASC config, unsigned int v_addr) {
    // 요구사항: 범위 초과 시 로그 출력 및 프로그램 즉시 종료
    if (v_addr >= config.bounds) {
        printf("[SECURITY FAULT] Virtual Address %u is Out of Bounds! (Limit: %u)\n", v_addr, config.bounds);
        exit(1); 
    }

    // 요구사항: 정상 주소 접근 시 물리 주소 정확히 계산
    unsigned int p_addr = config.base + v_addr;

    // 추가 검사: 계산된 물리 주소가 실제 메모리 크기(64KB)를 넘는지 확인
    if (p_addr >= MEMORY_SIZE) {
        printf("[SYSTEM ERROR] Physical Address %u exceeds System Memory %d!\n", p_addr, MEMORY_SIZE);
        exit(1);
    }

    return p_addr;
}

int main() {
    // 설정: 32KB 지점부터 16KB 크기의 보안 구역 할당
    TZASC secure_partition = {32768, 16384}; 

    printf("=== MMU Address Translation Test ===\n");

    // 테스트 1: 정상 주소 접근 (가상 주소 100)
    unsigned int v_addr1 = 100;
    unsigned int p_addr1 = translate(secure_partition, v_addr1);
    printf("[SUCCESS] Virtual: %u -> Physical: %u\n", v_addr1, p_addr1);

    // 테스트 2: 범위 초과 접근 (가상 주소 20000)
    // 16384(bounds)를 넘으므로 여기서 종료됨
    unsigned int v_addr2 = 20000;
    printf("[ATTEMPT] Accessing Virtual Address %u...\n", v_addr2);
    unsigned int p_addr2 = translate(secure_partition, v_addr2); 

    return 0;
}