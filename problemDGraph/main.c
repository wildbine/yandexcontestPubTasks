#include <stdio.h>

unsigned long long power(unsigned long long base, unsigned long long exponent, unsigned long long mod) {
    unsigned long long result = 1;
    base = base % mod;
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exponent = exponent / 2;
    }
    return result;
}

unsigned long long inv(unsigned long long num, unsigned long long mod){
    return power(num, mod - 2, mod);
}

unsigned long long f(unsigned long long n, unsigned long long mod){
    if (n == 1 || n == 0)
        return 1;
    unsigned long long prod = 1;
    for (unsigned long long i = 2; i <= n; i++){
        prod = ((prod % mod) * (i % mod)) % mod;
    }
    return prod;
}

unsigned long long C(unsigned long long n, unsigned long long m, unsigned long long mod){
    return (f(n, mod) * (inv(f(m, mod) * f(n-m, mod), mod) % mod)) % mod;
}

int main(){
    unsigned long long mod = 1e9+7;
    long long n;
    unsigned long long res ;
    scanf("%llu", &n);
    if (n == 1)
        res = 1;
    else if (n == 2)
        res = 0;
    else {
        long long m = n-3;
        unsigned long long sum = 0;

        do{
            sum = (sum + C(n-1, m, mod))%mod;
            m--;
        }while(m >= 0);

        res = ((n % mod) * (sum % mod)) % mod;
    }
    printf("%llu", res);
}