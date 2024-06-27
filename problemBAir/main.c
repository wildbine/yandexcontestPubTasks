#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

typedef enum {
    left = 0,
    right = 1
} sideEnum;

typedef enum {
    aisle = 0,
    window = 1
} positionEnum;

typedef struct {
    char *data;
    size_t size;
    size_t capacity;
} Buffer;

int countFreeLeft(char *str);
int countFreeRight(char *str);
int isAisleFreeLeft(char *str);
int isAisleFreeRight(char *str);
int isWindowFreeLeft(char *str);
int isWindowFreeRight(char *str);
void fill (char *row, int pos, int count);
void print_seats(Buffer *buffer, char **row, int n, int rowGroup, int pos, int groupCount);
void printRowGroup(Buffer *buffer, char *row, int pos, int groupCount);
void printPassengerMessage(Buffer *buffer, int rowGroup, int pos, int groupCount);
int freeInARow(char *row, int pos, int groupCount);
void showSetas(char **row, int n);

int compareBufferWithFile(const char *buffer, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return -1; 
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);


    char *fileContent = (char *)malloc(fileSize + 1);
    if (fileContent == NULL) {
        fclose(file);
        perror("Ошибка выделения памяти");
        return -1; 
    }

    fread(fileContent, 1, fileSize, file);
    fileContent[fileSize] = '\0'; 
    fclose(file);

    int result = strcmp(buffer, fileContent);

    if (result != 0) {
        printf("Строки отличаются. Различия:\n");

        int i = 0;
        while (buffer[i] != '\0' && fileContent[i] != '\0' && buffer[i] == fileContent[i]) {
            i++;
        }

        printf("В буфере: %c (%d), в файле: %c (%d)\n", buffer[i], i, fileContent[i], i);
    }

    free(fileContent);

    return result;
}

void initBuffer(Buffer *buffer, size_t initialCapacity) {
    buffer->data = (char *)malloc(initialCapacity);
    if (!buffer->data) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    buffer->data[0] = '\0';
    buffer->size = 0;
    buffer->capacity = initialCapacity;
}

void addToBuffer(Buffer *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    int requiredSize = vsnprintf(NULL, 0, format, args) + 1; // +1 для нулевого символа
    va_end(args);
    
    if (buffer->size + requiredSize > buffer->capacity) {
        buffer->capacity = buffer->size + requiredSize;
        buffer->data = (char *)realloc(buffer->data, buffer->capacity);
        if (!buffer->data) {
            fprintf(stderr, "Memory reallocation failed\n");
            exit(1);
        }
    }
    
    va_start(args, format);
    vsprintf(buffer->data + buffer->size, format, args);
    va_end(args);
    
    buffer->size += requiredSize - 1; 
}

void freeBuffer(Buffer *buffer) {
    free(buffer->data);
    buffer->data = NULL;
    buffer->size = 0;
    buffer->capacity = 0;
}

int main(int argc, char **argw){
    Buffer buffer;
    initBuffer(&buffer, 128);
    int n;
    scanf("%d", &n);
    char **row = (char **)malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++){
        row[i] = (char *)malloc(7 * sizeof(char));
    }

    ///рассадка иначальная
    for (int i = 0; i < n; i++){
        scanf("%s", row[i]);
    }

    int m;
    int groupCount;
    int isFill = 0;
    char *side = (char *)malloc(6 * sizeof(char));
    char *position = (char *)malloc(7 * sizeof(char));
    scanf("%d", &m);
    for (int i = 0; i < m; i++){
        isFill = 0;
        int rowGroup = 0;
        scanf("%d", &groupCount);
        scanf("%s", side);
        scanf("%s", position);
        for (int j = 0; j < n; ++j){
            if (strcmp(side, "left") == 0 && countFreeLeft(row[j]) >= groupCount){
                if (strcmp(position, "aisle") == 0 && 
                isAisleFreeLeft(row[j]) && freeInARow(row[j], 3-groupCount, groupCount)){
                    fill(row[j], 3-groupCount, groupCount);
                    isFill = 1;
                    printPassengerMessage(&buffer, j, 3-groupCount, groupCount);
                    print_seats(&buffer, row, n, j, 3-groupCount, groupCount);
                    }
                else if (strcmp(position, "window") == 0 && 
                isWindowFreeLeft(row[j]) && freeInARow(row[j], 0, groupCount)){
                    fill(row[j], 0, groupCount);
                    isFill = 1;
                    printPassengerMessage(&buffer, j, 0, groupCount);
                    print_seats(&buffer, row, n, j, 0, groupCount);
                    }
            }
            else if (strcmp(side, "right") == 0 && countFreeRight(row[j]) >= groupCount){
                if (strcmp(position, "aisle") == 0 && 
                isAisleFreeRight(row[j]) && freeInARow(row[j], 4, groupCount)){
                    fill(row[j], 4, groupCount);
                    isFill = 1;
                    printPassengerMessage(&buffer, j, 4-1, groupCount);
                    print_seats(&buffer, row, n, j, 4, groupCount);
                    }
                else if (strcmp(position, "window") == 0 && 
                isWindowFreeRight(row[j]) && freeInARow(row[j], 7-groupCount, groupCount)){
                    fill(row[j], 7-groupCount, groupCount);
                    isFill = 1;
                    printPassengerMessage(&buffer, j, 7-groupCount-1, groupCount);
                    print_seats(&buffer, row, n, j, 7-groupCount, groupCount);
                    }
            }
            if (isFill) break;
        }
        if(!isFill) 
            addToBuffer(&buffer, "Cannot fulfill passengers requirements\n");
    }
    
    printf("%s", buffer.data);
    int comparisonResult = compareBufferWithFile(buffer.data, "text.txt");
    if (comparisonResult == 0) {
        printf("Содержимое buffer и текстового файла идентичны.\n");
    } else {
        printf("Содержимое buffer и текстового файла различается.\n");
    }
    free(side);
    free(position);

    freeBuffer(&buffer);

    for (int i = 0; i < n; i++) {
        free(row[i]); 
    }
    free(row);
}

int countFreeLeft(char *str){
    int count = 0;
    for (int i = 0; i < 3; i++)
        if (str[i] == '.')
            count++;
    return count;
}
int countFreeRight(char *str){
    int count = 0;
    for (int i = 4; i < 7; i++)
        if (str[i] == '.')
            count++;
    return count;
}
int isAisleFreeLeft(char *str){
    return str[2] == '.';
}
int isAisleFreeRight(char *str){
    return str[4] == '.';
}
int isWindowFreeLeft(char *str){
    return str[0] == '.';
}
int isWindowFreeRight(char *str){
    return str[6] == '.';
}

void fill (char *row, int pos, int count){
    for (int i = 0; i < count; i++)
        row[pos+i] = '#';
}

void print_seats(Buffer *buffer, char **row, int n, int rowGroup, int pos, int groupCount){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < 7; j++){
            if (i != rowGroup){
                addToBuffer(buffer, "%c", row[i][j]);
            }
            else{ 
                printRowGroup(buffer, row[i], pos, groupCount);
                break;
            }
        }
        addToBuffer(buffer, "\n");
    }
}

void printRowGroup(Buffer *buffer, char *row, int pos, int groupCount){
    for (int i = 0; i < 7; i++){
        if (i >= pos && i < pos+groupCount)
            addToBuffer(buffer, "X");
        else addToBuffer(buffer, "%c", row[i]);
    }
}

void printPassengerMessage(Buffer *buffer, int rowGroup, int pos, int groupCount){
    addToBuffer(buffer, "Passengers can take seats:");
    char str[3];
    str[2] = '\0';
    for(int i = 0; i < groupCount; i++)
        addToBuffer(buffer, " %d%c", rowGroup+1, pos+65+i);
    addToBuffer(buffer, "\n");
}

int freeInARow(char *row, int pos, int groupCount){
    int res = 0;
    for (int i = 0; i < groupCount; i++)
        if (row[pos+i] == '.')
            res++;
    return res == groupCount;
}

void showSetas(char **row, int n)
{
    for (int i = 0; i < n; i++){
        for(int j = 0; j < 7; j++)
            printf("%c", row[i][j]);
        printf("\n");
    }
}