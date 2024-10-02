#include<stdio.h>

int memoryUnit(int vAddress){
    int codeStart = 32*1024;//0x8000
    int codeBound = 2*1024;//0x0800

    int heapStart = 34*1024;//0x8800
    int heapBound = 3*1024;//0x0c00

    int stackStart = 28*1024;//0x1000
    int stackBound = 2*1024;//0x0800

    unsigned int segment = (vAddress & 0xc000) >> 14;
    unsigned int offset = (vAddress & 0x3fff);


    if(segment == 0){
        if(offset<=codeBound){
            return codeStart+offset;
        }
        else{
            printf("Segmentation fault in Code Segment\n");
            return -1;
        }
    }
    else if(segment==1){
        if(offset<=heapBound){
            return heapStart+offset;
        }
        else{
            printf("Segmentation fault in Heap Segment\n");
            return -1;
        }
    }
    else if(segment==2){
        if(offset<=stackBound){
            return stackStart-offset;
        }
        else{
            printf("Segmentation fault in Stack Segment\n");
            return -1;
        }
    }
    else{
        printf("Segmentation fault\n");
        return -1;
    }

}

int main(){
    int vAddress;
    printf("Enter virtual address in hex form(0000-FFFF): ");
    scanf("%x",&vAddress);
    int pAddress = memoryUnit(vAddress);
    if(pAddress!=-1){
       printf("Physical Memory Address: %x\n",pAddress); 
    }
    return 0;
}