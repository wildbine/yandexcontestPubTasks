#include <stdio.h>
#include <math.h>
#include <stdlib.h>


int symbolsUnique(char *str);
void work();

int main(int argc, char argw){
    printf("%d", symbolsUnique("helloworld"));
}

void work(){
    int candidateCount;
    scanf("%d", &candidateCount);
    char **array = (char **)malloc(candidateCount * sizeof(char *));
    for (int i = 0; i < candidateCount; i++){
        array[i] = (char *)malloc(15*6*sizeof(char));
    }
    

    for (int i = 0; i < candidateCount; i++){
        fgets(array[i], 15*6, stdin);
        char curr = array[i][0];
        int step;
        int charPos = 0;
        int fioCountUnique = 0;
        int dayPlusMonth = 0;
        int numberOfSecondName;
        while (curr != '\n'){
            if (curr == ',' && step == 0){
                step++;
                curr = array[i][charPos+1];

            }
            else 
                charPos++;
        }
    }
}

int symbolsUnique(char *str){
    char asciiArray[128];
    for (int i = 0; i < 128; i++)
        asciiArray[i] = ' ';
    int unique = 0;
    int pos = 0;
    while(str[pos] != '\0'){
        if (asciiArray[(int)str[pos]] == ' '){
            unique++;
            asciiArray[(int)str[pos]] = str[pos];
            printf("\n%d - %c", (int)str[pos], str[pos]);
        }
            pos++;
    }
    return unique;
}