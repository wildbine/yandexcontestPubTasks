#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char **argw){
    int n;
    scanf("%d", &n);
    long int* barrel = (long int*)malloc(n * sizeof(long int));

    int noway = 0;
    int maxvalue = 0;
    for(int i = 0; i < n; i++){
        scanf("%ld", &barrel[i]);
        if (barrel[i] < maxvalue)
            noway = 1;
        else
            maxvalue = barrel[i];
    }
    
    int minCountIterations = maxvalue - barrel[0];

    free(barrel);

    printf("%d", noway == 1? -1 : minCountIterations);
}

