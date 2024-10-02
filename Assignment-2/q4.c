#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define oofset 12 
#define pdbits 10


#define pagesize 4096
#define pdentry 1024
#define ptentry 1024 


#define ptbits 10 
typedef struct {
    uint32_t frameno;
    uint8_t valid;
} table_component;


typedef struct {
    table_component entries[ptentry];
} pagetable;


typedef struct {
    pagetable* pt[pdentry];
} pagedirec;


pagedirec pd;
uint8_t memory[pagesize * ptentry];
uint32_t tohits = 0;
uint32_t tomisses = 0;

uint8_t load(uint32_t va) {
    uint32_t pdi = (va >> (oofset + ptbits)) & ((1 << pdbits) - 1);
    uint32_t pti = (va >> oofset) & ((1 << ptbits) - 1);
    uint32_t offset = va & ((1 << oofset) - 1);

    if (pd.pt[pdi] == NULL) {
        printf("Page fault at PD index %u\n", pdi);
        pd.pt[pdi] = (pagetable*)calloc(1, sizeof(pagetable));
        tomisses++;
    }

    if (pd.pt[pdi]->entries[pti].valid == 0) {
        printf("Page fault at PT index %u\n", pti);
        pd.pt[pdi]->entries[pti].frameno = (pti * pagesize);
        pd.pt[pdi]->entries[pti].valid = 1;
        tomisses++;
    } else {
        tohits++;
    }

    uint32_t phys = pd.pt[pdi]->entries[pti].frameno + offset;
    return memory[phys];
}

void store(uint32_t va, uint8_t value) {
    uint32_t pdi = (va >> (oofset + ptbits)) & ((1 << pdbits) - 1);
    uint32_t pti = (va >> oofset) & ((1 << ptbits) - 1);
    uint32_t offset =  ((1 << oofset) - 1)  & va;

    if (pd.pt[pdi] == NULL) {
        printf("Page fault at PD index %u\n", pdi);
        pd.pt[pdi] = (pagetable*)calloc(1, sizeof(pagetable));
        tomisses++;
    }

    if (pd.pt[pdi]->entries[pti].valid == 0) {
        printf("Page fault at PT index %u\n", pti);
        pd.pt[pdi]->entries[pti].frameno = (pti * pagesize);
        pd.pt[pdi]->entries[pti].valid = 1;
        tomisses++;
    } else {
        tohits++;
    }

    uint32_t phys = pd.pt[pdi]->entries[pti].frameno + offset;
    memory[phys] = value;
}

int main() {
    for (int i = 0; i < pdentry; i++) {
        pd.pt[i] = NULL;
    }

    store(0x12345678, 85);
    printf("Value at 0x12345678: %u\n", load(0x12345678));

    printf("Summary:\n");
    printf("Total page hits: %u\n", tohits);
    printf("Total page misses: %u\n", tomisses);
    printf("Hit/Miss Ratio: %.2f\n", (float)tohits / (tomisses + tohits));

    return 0;
}