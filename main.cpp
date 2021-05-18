#include <iostream>
#include "sha1.h"
#include <string>

#define uint unsigned int

uint gcd (uint n1, uint n2) {
    return (n2 == 0) ? n1 : gcd (n2, n1 % n2);
}

uint powMod(uint msg, uint ed, uint n) {
    uint h = msg;
    for (uint i = 1; i < ed; i++){
        h = (h*msg) % n;
    }
    return h;
}

void keyBuilder(){
    uint p, q, d, n, e = 0, kop = 0;
    int cofactor[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    double totd = 1;

    printf("Input p, q = ");
    scanf("%d %d", &p, &q);

    n = p * q;
    uint totn = (p-1) * (q-1);

    printf("Masukkan d = ");
    scanf("%d", &d);

    if (gcd(d,n) != 1){
        std::cout << "d dan n Tidak relatif prima atau terlalu kecil" << std::endl;
    } else {
        uint k = d;
        int pem = 2;
        bool cc = false;
        
        while (k != 0 || k == pem){
            if (k == 1){
                break;
            }
            
            if(k % pem == 0 && pem == k){
                k = k / pem;
                cofactor[kop] = pem;
            }else if(k % pem == 0){
                k = k / pem;
                cofactor[kop] = pem;
                kop = kop + 1;
            } else {
                for (int i = pem+1; i <= d; i++){
                    for (int j=2; j < i; j++){
                        if(i % j != 0){
                            cc = false;
                        } else {
                            cc = true;
                            break;
                        }
                    }

                    if (cc == false){
                        pem = i;
                        break;
                    }
                }
            }            
        }

        int i = 0;
        totd = totd*d; 
        while (cofactor[i] != 0){
            totd = totd * (1-(1/cofactor[i]));
            i++;
        }

        for (uint i = 2; i < n; i++){
            if(gcd(totn, i) == 1){
                e = i;
                break;
            }
        }
        
        int ks = 1;
        while (gcd((ks * e), (int) totd) != 1){
            ks++;
        }

        printf("e = %d, d = %d, n = %d\n", e, d, n);
    }
}

std::string encryption(){
    std::string msg;
    std::cin >> msg;

    

    return msg;
}

void rsa(){
    uint e, d, n, c[100];
    char msg[100];

    scanf("%s", msg);

    printf("Input e, d, n = ");
    scanf("%d %d %d", &e, &d, &n);

    printf("Encryption\n");
    uint i=0;
    while (msg[i] != '\0'){
        c[i] = powMod(msg[i], e, n);
        printf("%c = %d => %d\n",msg[i], msg[i], c[i]);
        i++;
    }

    printf("Decryption\n");

    i = 0;
    while (c[i] != '\0'){
        msg[i] = powMod(c[i], d, n);
        printf("%d => %d = %c\n", c[i], msg[i], msg[i]);
        i++;
    }
}

int main() {
    printf("Pair key sender\n");
    keyBuilder();

    printf("Pair key receiver\n");
    keyBuilder();

    printf("Encryption RSA");
    rsa();

    printf("Input message");
    std::string mytext;
    std::cin >> mytext;

    SHA1 checksum;
    checksum.update(mytext);
    std::string hash = checksum.final();
    std::cout << "SHA-1 = " << hash << std::endl;
    rsa();
}