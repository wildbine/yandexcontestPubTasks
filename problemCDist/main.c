#include <stdio.h>

typedef struct{
    unsigned long long value;
    unsigned long int index;
    unsigned long long sum;
    int used;
} element;

void findAndChangeSum(unsigned long long value, unsigned long long sum, element elem[], unsigned long long n);

unsigned long long dist(unsigned long long a, unsigned long long b){
    return (a > b) ? (a - b) : (b - a);
}

unsigned long int min(unsigned long long a, unsigned long long b){
    if (a < b)
        return a;
    else return b;
}

unsigned long long sumDist(unsigned long long arr[], long int n, long int k, long int index);

void sort(unsigned long long arr[], unsigned long long n) {
    for (unsigned long long i = 0; i < n - 1; i++) {
        for (unsigned long long j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                unsigned long long temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main(){
    unsigned long long n;
    unsigned long long k;
    scanf("%llu", &n);
    scanf("%llu", &k);
    unsigned long long arr[n];
    element elem[n];
    for (unsigned long long i = 0; i < n; i++){
        scanf("%llu", &arr[i]);
        elem[i].value = arr[i];
        elem[i].index = i;
        elem[i].used = 0;
    }

    sort(arr, n);
    for (unsigned long long i = 0; i < n; i++){
        findAndChangeSum(arr[i], sumDist(arr, n, k, i), elem, n);
    }

    for (unsigned long long i = 0; i < n; i++)
        i != n-1 ? printf("%llu ", elem[i].sum) : printf("%llu", elem[i].sum);

}

void findAndChangeSum(unsigned long long value, unsigned long long sum, element elem[], unsigned long long n){
    for(unsigned long long i = 0; i < n; i++)
        if (value == elem[i].value && !elem[i].used){
            elem[i].sum = sum;
            elem[i].used = 1;
            break;
        }
}

unsigned long long sumDist(unsigned long long arr[], long int n, long int k, long int index)
{
    unsigned long long sum = 0;
    long int left = index - 1 >= 0 ? index - 1 : - 1;
    long int right = index + 1 <= n-1 ? index + 1 : -1;
    while(k != 0){
        if (left == -1){
            sum+=dist(arr[index], arr[right]);
            right = (right+1 < n-1) ? right+1 : -1;
        }
        else if (right == -1){
            sum+=dist(arr[index], arr[left]);
            left = (left-1 > 0) ? left-1 : -1;
        }
        else {
            sum+=min(dist(arr[index], arr[right]), dist(arr[index], arr[left]));
            if (dist(arr[index], arr[right]) < dist(arr[index], arr[left]))
                right = right+1 <= n-1 ? right+1 : -1;
            else 
                left = (left-1 >= 0) ? left-1 : -1;
        }
        k--;
    }
    return sum;
}