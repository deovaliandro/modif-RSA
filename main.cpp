/*
    main.cpp
    sha1.h from
    an implementation of RSA modification
    original code by @deovaliandro
*/

#include <iostream>
#include <string>
#include <vector>

#include "sha1.h"

#define uint unsigned int

uint gcd(uint n1, uint n2) {
    return (n2 == 0) ? n1 : gcd(n2, n1 % n2);
}

int powMod(int msg, int ed, int n) {
    int h = msg;

    for (uint i = 1; i < ed; i++) {
        h = (h * msg) % n;
    }

    return h;
}

void keyBuilder() {
    uint p = 0, q = 0, d = 0, e = 0, kop = 0;
    int cofactor[20] = {0};
    double totd = 1.0;

    std::cout << "Input p, q = ";
    std::cin >> p >> q;

    uint n = p * q;
    uint totn = (p - 1) * (q - 1);

    std::cout << "Input d = ";
    std::cin >> d;

    if (gcd(d, n) != 1 && n - std::max(p, q) && d < n) {
        std::cout << "d dan n tidak relatif prima atau terlalu kecil" << std::endl;
    } else {
        uint k = d;
        int pem = 2;
        bool cc = false;

        while (k != 0 || k == pem) {
            if (k == 1) {
                break;
            }

            if (k % pem == 0 && pem == k) {
                k = k / pem;
                cofactor[kop] = pem;
            } else if (k % pem == 0) {
                k = k / pem;
                cofactor[kop] = pem;
                kop = kop + 1;
            } else {
                for (int i = pem + 1; i <= d; i++) {
                    for (int j = 2; j < i; j++) {
                        if (i % j != 0) {
                            cc = false;
                        } else {
                            cc = true;
                            break;
                        }
                    }

                    if (!cc) {
                        pem = i;
                        break;
                    }
                }
            }
        }

        int i = 0;
        totd = totd * d;
        while (cofactor[i] != 0) {
            totd = totd * (1 - (1.0 / cofactor[i]));
            i++;
        }

        for (uint j = 7; j < n; j++) {
            if (gcd(totn, j) == 1) {
                e = j;
                break;
            }
        }

        int ks = 1;
        while (gcd((ks * e), (int) totd) != 1) {
            ks++;
        }

        std::cout << "Pk = (" << e << ", " << n << "), PrK = (" << d << ", " << n << ")" << std::endl;
    }
}

void sender() {
    int e = 0, d = 0, n1 = 0, n2 = 0;

    std::cout << "Input message = ";
    std::string msg;
    std::cin >> msg;

    std::cout << "Input Kp receiver (e,n) = ";
    std::cin >> e >> n1;

    std::cout << "Input Ks sender (d,n) = ";
    std::cin >> d >> n2;

    std::vector<int> c(msg.length());
    uint i = 0;
    while (msg[i] != '\0') {
        c.push_back(powMod(msg.at(i), e, n1));
        i++;
    }

    SHA1 checksum;
    checksum.update(msg);
    std::string hash = checksum.final();

    std::vector<int> chash(msg.length());
    i = 0;
    while (hash[i] != '\0') {
        chash.push_back(powMod(hash.at(i), d, n2));
        i++;
    }

    uint flength = msg.length() + hash.length();
    std::vector<int> final(flength);
    for (int j = 0; j < msg.length(); j++) {
        final.push_back(c[j]);
    }

    for (int j = (int) msg.length(); j < flength; j++) {
        final.push_back(chash[j - msg.length()]);
    }

    std::string ffinal[flength];
    for (int j = 0; j < flength; j++) {
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


    for (int j = 0; j < flength; j++) {
        std::cout << ffinal[j];
    }
    std::cout << std::endl;
}

void receiver() {
    int e, d, n1, n2;
    std::cout << "Input cipher = ";
    std::string cipher;
    std::cin >> cipher;

    std::cout << "Input Ks receiver (e,n) = ";
    std::cin >> d >> n1;

    std::cout << "Input Kp sender (d,n) = ";
    std::cin >> e >> n2;

    int c[(cipher.length() - 40 * 4) / 4];
    int hashc[40] = {0};

    for (int i = 0, j = 0; j < (cipher.length() - 40 * 4) / 4; i = i + 4, j++) {
        char app = cipher.at(i);
        char app2 = cipher.at(i + 1);
        char app3 = cipher.at(i + 2);
        char app4 = cipher.at(i + 3);
        std::string append;
        append.push_back(app);
        append.push_back(app2);
        append.push_back(app3);
        append.push_back(app4);

        int x = 0;
        std::stringstream ss;
        ss << std::hex << append;
        ss >> x;

        c[j] = x;
    }

    for (int i = (int) cipher.length() - 40 * 4, j = 0; j < 40; i = i + 4, j++) {
        char app = cipher.at(i);
        char app2 = cipher.at(i + 1);
        char app3 = cipher.at(i + 2);
        char app4 = cipher.at(i + 3);
        std::string append;
        append.push_back(app);
        append.push_back(app2);
        append.push_back(app3);
        append.push_back(app4);

        int x;
        std::stringstream ss;
        ss << std::hex << append;
        ss >> x;

        hashc[j] = x;
    }

    int msg[(cipher.length() - 40 * 4) / 4];
    int i = 0;
    while (c[i] != '\0') {
        msg[i] = powMod(c[i], d, n1);
        i++;
    }

    uint hash[40] = {0};
    std::string thash;
    i = 0;
    while (i < 40) {
        hash[i] = powMod(hashc[i], e, n2);
        thash.push_back((char) hash[i]);
        i++;
    }

    std::cout << "Message = ";
    std::string toHash;
    for (int j = 0; j < (cipher.length() - 40 * 4) / 4; j++) {
        toHash.push_back((char) msg[j]);
    }

    std::cout << std::endl;

    SHA1 checksum;
    checksum.update(toHash);
    std::string fhash = checksum.final();

    if (thash == fhash) {
        std::cout << "Verified" << std::endl;
        std::cout << "First hash from cipher = " << thash << " , and second hash from message = " << fhash << std::endl;
    }
}

int main() {
    printf("Pair key sender\n");
    keyBuilder();

    printf("Pair key receiver\n");
    keyBuilder();

    sender();
    receiver();
}