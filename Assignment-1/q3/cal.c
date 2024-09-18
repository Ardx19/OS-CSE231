#include<stdio.h>
int Zeller(int month , int year){
    if(month==1){
        month=13;
        year--; 
    }
    if(month==2){
        month=14;
        year--;
    }
    return (1 + (13*(month+1)/5) + year%100 + (year%100)/4 + year/400 - 2*(year/100)) % 7;
}


int func(int month) {
    switch (month) {
        case 1: return 31;
        case 2: return 28;  
        case 3: return 31;
        case 4: return 30;
        case 5: return 31;
        case 6: return 30;
        case 7: return 31;
        case 8: return 31;
        case 9: return 30;
        case 10: return 31;
        case 11: return 30;
        case 12: return 31;
        default: return 0;  
    }
}

int main() {
    int month, year;
    printf("Enter Month (1-12): ");
    scanf("%d", &month);
    printf("Enter Year: ");
    scanf("%d", &year);
    int day = Zeller(month, year)-1;
    if (day<0) day += 7;  // Ensure positive day of the week
    switch (month) {
        case 1: printf("\n   January "); break;
        case 2: printf("\n   February "); break;
        case 3: printf("\n   March "); break;
        case 4: printf("\n   April "); break;
        case 5: printf("\n   May "); break;
        case 6: printf("\n   June "); break;
        case 7: printf("\n   July "); break;
        case 8: printf("\n   August "); break;
        case 9: printf("\n   September "); break;
        case 10: printf("\n   October "); break;
        case 11: printf("\n   November "); break;
        case 12: printf("\n   December "); break;
        default: printf("Invalid Month"); return 0;
    }
    printf("%d\n", year);
    printf("Su Mo Tu We Th Fr Sa\n");
    int totalDays = func(month);
    for (int i = 0; i < day; i++) {
        printf("   ");  
    }
    for (int date = 1; date <= totalDays; date++) {
        printf("%2d ", date);
        if ((day + date) % 7 == 0) {
            printf("\n");
        }
    }
    printf("\n");
    return 0;
}
