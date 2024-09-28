#include<stdio.h>

int memoryUnit(int vAddress){
    int codeStart = 32*1024;//0x8000
    int codeBound = 2*1024;//0x0800

    int heapStart = 34*1024;//0x8800
    int heapBound = 3*1024;//0x0c00

    int stackStart = 28*1024;//0x1000
    int stackBound = 2*1024;//0x0800

    unsigned int segment = (vAddress & 0xf000) >> 12;
    unsigned int offset = (vAddress & 0x0fff);

    /*
        segment = {0000, 1111} => {0,15}
        code = [0,5]
        heap = [6,10]
        stack = [11,15]

    */


    if(segment >= 0 && segment <= 5){
        if(offset<=codeBound){
            return codeStart+offset;
        }
        else{
            printf("Segmentation fault in Code Segment\n");
            return -1;
        }
    }
    else if(segment >= 6 && segment <= 10){
        if(offset<=heapBound){
            return heapStart+offset;
        }
        else{
            printf("Segmentation fault in Heap Segment\n");
            return -1;
        }
    }
    else if(segment >= 11 && segment <= 15){
        if(offset<=stackBound){
            return stackStart+offset;
        }
        else{
            printf("Segmentation fault in Stack Segment\n");
            return -1;
        }
    }
    else{
        printf("Invalid Input\n");
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