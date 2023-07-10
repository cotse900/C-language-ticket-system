// ===================================================================================
//
//  Assignment:  2
//  Milestone:   4
//  Chungon Tse, 154928188, cotse@myseneca.ca
//  NFF
//  Description: commonHelpers.c
//
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include "commonHelpers.h"

int currentYear(void)
{
    time_t currentTime = time(NULL);
    return localtime(&currentTime)->tm_year + 1900;
}


void clearStandardInputBuffer(void)
{
    while (getchar() != '\n')
    {
    ; // On purpose: do nothing
    }
}
int getInteger(void){
    int num = 0;
    char newlineCheck = 'x';
    while(newlineCheck != '\n'){
        scanf(" %d%c", &num, &newlineCheck);
        if(newlineCheck != '\n'){
            clearStandardInputBuffer();
            printf("ERROR: Value must be an integer: ");
        }
    }
    return num;
}

int getPositiveInteger(void){
    int result = -1;
    while(result <= 0){
        result = getInteger();
        if(result <= 0){
            printf("ERROR: Value must be a positive integer greater than zero: ");
        }
    }
    return result;
}

int getZeroOrPositiveInteger(void){
    int result = -1;
    while(result < 0){
        result = getInteger();
        if(result < 0){
            printf("ERROR: Value must be positive or zero: ");
        }
    }
    return result;
}

double getDouble(void){
    double num = 0;
    char newlineCheck = 'x';
    while(newlineCheck != '\n'){
        scanf(" %lf%c", &num, &newlineCheck);
        if(newlineCheck != '\n'){
            clearStandardInputBuffer();
            printf("ERROR: Value must be a double floating-point number: ");
        }
    }
    return num;

}

double getPositiveDouble(void){
    double result = -1.0;
    while(result <= 0.0){
        result = getDouble();
        if(result <= 0.0){
            printf("ERROR: Value must be a positive double floating-point number: ");
        }
    }
    return result;
}

int getIntFromRange(int lower, int upper){
    int passed = 0;
    int result = 0;
    do
    {
        result = getInteger();
        if( result >= lower && result <= upper ){
            passed = 1;
        }else{
            printf("ERROR: Value must be between %d and %d inclusive: ", lower, upper);
        }
    } while (passed == 0);
    return result;
}

char getCharOption(char* validChars){

    char userChar;
    char newlineCheck = 'x';
    int isValid = 0;
    while(newlineCheck != '\n' || isValid == 0){
        scanf(" %c%c", &userChar, &newlineCheck);
        if(newlineCheck != '\n'){
            clearStandardInputBuffer();
        }else{
            //for loop check if userChar is in validChars
            char* t = "x";
            for (t = validChars; *t != '\0'; t++) {
                if(userChar == *t){
                    isValid = 1;
                }
            }
        }
        if(isValid == 0){
            printf("ERROR: Character must be one of [%s]: ", validChars);
        }
    }

    return userChar;
}

void getCString(char* stringPtr, int minLength, int maxLength){
    int size;
    int passed = 0;
    char tempString[100];
    while(passed == 0){
        //clean array
        int i;
        for(i=0; i<100; i++){
            tempString[i] = '\0'; //empty the char array
        }

        scanf("%[^\n]", tempString);
        clearStandardInputBuffer();

        size = 0;
        for ( i = 0; i < 100; i++) {
            if( tempString[i] != '\0' ){
                size++;
            }
        }

        if(size < minLength || size > maxLength){
            if (minLength == maxLength) {
                printf("ERROR: String length must be exactly %d chars: ", maxLength);
            }
            else if (size > maxLength) {
                printf("ERROR: String length must be no more than %d chars: ", maxLength);
            }
            else{
                printf("ERROR: String length must be between %d and %d chars: ", minLength, maxLength);
            }
        }else{
            passed = 1;
            for(i=0; i<maxLength; i++){
                stringPtr[i] = tempString[i];
            }
        }
    }
}

int haveWhitespace(char* stringPtr, int length){
    int i;
    for(i=0; i<length;i++)
    {
        if(stringPtr[i] == ' '){
            return 1;
        }
    }
    return 0;
}

int containsTwoOfDigitsUpLowSymbol(char* stringPtr, int length){
    int i;
    int digitsCount = 0;
    int upCount = 0;
    int lowCount = 0;
    int symbolCount = 0;

    for(i=0; i<length;i++)
    {
        if(stringPtr[i] >= '0' && stringPtr[i] <= '9'){
            digitsCount++;
        }
        if(stringPtr[i] >= 'A' && stringPtr[i] <= 'Z'){
            upCount++;
        }
        if(stringPtr[i] >= 'a' && stringPtr[i] <= 'z'){
            lowCount++;
        }
        if(stringPtr[i] == '!' || stringPtr[i] == '@' || stringPtr[i] == '#' || 
            stringPtr[i] == '$' || stringPtr[i] == '%' || stringPtr[i] == '^' || 
            stringPtr[i] == '&' || stringPtr[i] == '*' ){
            symbolCount++;
        }
    }
    
    if( digitsCount >= 2 && upCount >= 2 && lowCount >= 2 && symbolCount >= 2){
        return 1;
    }
    return 0;
}

void stringToUpper(char *convertString){
    int i=0;
    while(convertString[i]!='\0'){
        if(convertString[i] >= 'a' && convertString[i] <= 'z' ){
            convertString[i] = convertString[i] - ('a' - 'A');
        }
        ++i;
    }
}