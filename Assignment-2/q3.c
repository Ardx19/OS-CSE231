#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>

//MARCOS
#define PAGE_SIZE 4*1024    // 4KB page size
#define MEMORY_SIZE 64*1024 // 64KB total memory
#define TLB_SIZE 4 

int pageTable[16];
int tlbVpn[TLB_SIZE]={-1,-1,-1,-1};
int tlbPfn[TLB_SIZE]={-1,-1,-1,-1};
int tlbId =0;
int currSize=0;

int mapping(int vpn){ //TLB-->PFN
    return  ((vpn ^ 0xA) + (vpn << 2) + 13) % 64;
}

void pageTablemaker(){
    for(int i=0;i<16;i++){
        pageTable[i]=mapping(i);
    }
    return;
}



int TLB(int vpn,int offset){
    int i;
    bool isPresent=false;
    currSize=0;
    for(i=0;i<TLB_SIZE;i++){
        if(tlbVpn[i]!=-1){
            currSize++;
        }
        else{
            break;
        }
        if(vpn==tlbVpn[i]){
            isPresent=true;
            break;
        }
    }
    if(isPresent){
        printf("TLB hit\n");
        printf("Physical Address: %x\n",(tlbPfn[i]*1024)+offset);
        return i;
    }
    printf("TLB miss\n");
    return -1;
}

void updateTLB(int pfn,int vpn,int idx){
    if(idx==-1){
        tlbVpn[tlbId]=vpn;
        tlbPfn[tlbId]=pfn;
        tlbId++;
        tlbId%=4;
    }
    else{
        if(tlbId==idx){
            tlbId++;
            tlbId%=4;
            return;
        }
    }
}

void MMU(int vAddress){
    unsigned int vpn = (vAddress&0xf000)>>12;
    unsigned int offset = (vAddress & 0x0fff);
    if(offset>16*1024){
        printf("Memory Access out of Bounds\n");
        return;
    }
    int pfn;
    int idx=TLB(vpn,offset);
    if(idx==-1){
        pfn = pageTable[vpn];
        printf("Physical Address: %x\n",(pfn*1024)+offset);
    }
    updateTLB(pfn,vpn,idx);
    return;
}

int main(){
    pageTablemaker();
    while(1){
        int vAddress;
        printf("Enter Virtual Address(0x0000 - 0xffff): ");
        if(scanf("%x",&vAddress)<1){
            printf("Invalid input\n");
            sleep(1);
            printf("Terminating Program ....\n");
            break;
        }
        if(vAddress==-1 || vAddress>0xffff || vAddress<0x000){
            break;
        }
        MMU(vAddress);
    }
    return 0;
}