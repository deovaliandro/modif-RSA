#include <iostream>
#include "sha1.h"
#include <string>
#include <cstring>

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

void sender(){
    uint e, d, n1, n2;

    std::cout << "Input message = ";
    std::string msg;
    std::getline(std::cin, msg);

    std::cout << "Input Kp receiver (e,n) = ";
    std::cin >> e >> n1;

    std::cout << "Input Ks sender (d,n) = ";
    std::cin >> d >> n2;

    uint c[msg.length()];
    uint i=0;
    while (msg[i] != '\0'){
        c[i] = powMod(msg.at(i), e, n1);
        i++;
    }

    SHA1 checksum;
    checksum.update(msg);
    std::string hash = checksum.final();

    uint chash[hash.length()];
    i=0;
    while (hash[i] != '\0'){
        chash[i] = powMod(hash.at(i), d, n2);
        i++;
    }

    int final[msg.length() + hash.length()];

    for (int j = 0; j < msg.length(); j++) {
        final[j] = c[j];
    }

    for (int j = msg.length(); j < msg.length()+hash.length(); j++) {
        final[j] = chash[j-msg.length()];
    }

    std::string ffinal[msg.length()+hash.length()];
    for (int j = 0; j < msg.length()+hash.length(); j++) {
        std::stringstream stream;
        stream << std::hex << final[j];
        std::string result(stream.str());

        uint len = 4 - result.length();
        std::string dumm;
        for (int k = 0; k < len; ++k) {
            dumm.push_back('0');
        }

        dumm.append(result);
        ffinal[j] = dumm;
    }


    for (int j = 0; j < msg.length()+hash.length(); j++) {
        std::cout << ffinal[j];
    }
}

void receiver(){
    uint e, d, n1, n2;
    std::cout << "Input cipher = ";
    std::string cipher;
    std::cin >> cipher;

    std::cout << "Input Ks receiver (e,n) = ";
    std::cin >> d >> n1;

    std::cout << "Input Kp sender (d,n) = ";
    std::cin >> e >> n2;

    uint c[(cipher.length() - 40*4)/4];
    uint hashc[40];

    for (int i = 0, j=0; i < cipher.length() - 40*4, j<(cipher.length()-40*4)/4; i=i+4, j++) {
        char app = cipher.at(i);
        char app2 = cipher.at(i+1);
        char app3 = cipher.at(i+2);
        char app4 = cipher.at(i+3);
        std::string append;
        append.push_back(app);
        append.push_back(app2);
        append.push_back(app3);
        append.push_back(app4);

        uint x;
        std::stringstream ss;
        ss << std::hex << append;
        ss >> x;

        c[j] = x;
    }

    for (int i = cipher.length() - 40*4, j=0; i < cipher.length(), j<40; i=i+4, j++) {
        char app = cipher.at(i);
        char app2 = cipher.at(i+1);
        char app3 = cipher.at(i+2);
        char app4 = cipher.at(i+3);
        std::string append;
        append.push_back(app);
        append.push_back(app2);
        append.push_back(app3);
        append.push_back(app4);

        uint x;
        std::stringstream ss;
        ss << std::hex << append;
        ss >> x;

        hashc[j] = x;
    }

    uint msg[(cipher.length() - 40*4)/4];
    uint i=0;
    while (c[i] != '\0'){
        msg[i] = powMod(c[i], d, n1);
        i++;
    }

    uint hash[40];
    std::string thash;
    i=0;
    while (i < 40){
        hash[i] = powMod(hashc[i], e, n2);
        thash.push_back(hash[i]);
        i++;
    }

    std::cout << "Message = ";
    std::string toHash;
    for (int j = 0; j < (cipher.length() - 40*4)/4; j++) {
        printf("%c", msg[j]);
        toHash.push_back(msg[j]);
    }

    std::cout << std::endl;

    SHA1 checksum;
    checksum.update(toHash);
    std::string fhash = checksum.final();

    std::cout << thash << " " << fhash << std::endl;
}

int main() {
//    sender();
    receiver();
//    printf("Pair key sender\n");
//    keyBuilder();
//
//    printf("Pair key receiver\n");
//    keyBuilder();
//
//    printf("Encryption RSA");
//    rsa();
//
//    printf("Input message");
//    std::string mytext;
//    std::cin >> mytext;
//
//    SHA1 checksum;
//    checksum.update(mytext);
//    std::string hash = checksum.final();
//    std::cout << "SHA-1 = " << hash << std::endl;
//    rsa();
}