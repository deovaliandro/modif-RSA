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

#define unsigned int unsigned int

std::uint64_t gcd(std::uint64_t n1, std::uint64_t n2) {
    return (n2 == 0) ? n1 : gcd(n2, n1 % n2);
}

std::uint64_t powMod(std::uint64_t msg, std::uint64_t ed, std::uint64_t n) {
    std::uint64_t h = msg;

    for (int i = 1; i < ed; i++) {
        h = (h * msg) % n;
    }

    return h;
}

void keyBuilder() {
    std::uint64_t p = 0, q = 0, d = 0, e = 0, kop = 0;
    std::vector<std::uint64_t> cofactor;
    double totD = 0.0;

    std::cout << "Input p, q = ";
    std::cin >> p >> q;

    std::uint64_t n = p * q;
    std::uint64_t totN = (p - 1) * (q - 1);

    std::cout << "Input d = ";
    std::cin >> d;

    if (gcd(d, n) != 1 && n - std::max(p, q) && d < n) {
        std::cout << "d dan n tidak relatif prima atau terlalu kecil" << std::endl;
    } else {
        int k = (int) d;
        int pem = 2;
        bool cc = false;

        while (k != 0 || k == pem) {
            if (k == 1) {
                break;
            }

            if (k % pem == 0 && pem == k) {
                k = k / pem;
                cofactor.push_back(pem);
            } else if (k % pem == 0) {
                k = k / pem;
                cofactor.push_back(pem);
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

        totD = totD * (int) d;

        for (uint j : cofactor) {
            totD = totD * (1 - (1.0 / j));
        }

        for (int j = 7; j < n; j++) {
            if (gcd(totN, j) == 1) {
                e = j;
                break;
            }
        }

        int ks = 1;
        while (gcd((ks * e), (int) totD) != 1) {
            ks++;
        }

        std::cout << "Pk = (" << e << ", " << n << "), PrK = (" << d << ", " << n << ")" << std::endl;
    }
}

void sender() {
    std::uint64_t e = 0, d = 0, n1 = 0, n2 = 0;

    std::cout << "Input message = ";
    std::string msg;
    std::cin >> msg;

    std::cout << "Input Kp receiver (e,n) = ";
    std::cin >> e >> n1;

    std::cout << "Input Ks sender (d,n) = ";
    std::cin >> d >> n2;

    std::vector<std::uint64_t> c(msg.length());
    std::uint64_t i = 0;
    while (msg[i] != '\0') {
        c.push_back(powMod(msg.at(i), e, n1));
        i++;
    }

    SHA1 checksum;
    checksum.update(msg);
    std::string hash = checksum.final();

    std::vector<std::uint64_t> chash(msg.length());
    i = 0;
    while (hash[i] != '\0') {
        chash.push_back(powMod(hash.at(i), d, n2));
        i++;
    }

    uint flength = msg.length() + hash.length();
    std::vector<std::uint64_t> final(flength);
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
    std::uint64_t e, d, n1, n2;
    std::cout << "Input cipher = ";
    std::string cipher;
    std::cin >> cipher;

    std::cout << "Input Ks receiver (e,n) = ";
    std::cin >> d >> n1;

    std::cout << "Input Kp sender (d,n) = ";
    std::cin >> e >> n2;

    std::uint64_t c[(cipher.length() - 40 * 4) / 4];
    std::vector<std::uint64_t> hashC;

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

        hashC.push_back(x);
    }

    std::uint64_t msg[(cipher.length() - 40 * 4) / 4];
    int i = 0;
    while (c[i] != '\0') {
        msg[i] = powMod(c[i], d, n1);
        i++;
    }

    std::vector<std::uint64_t> hash;
    std::string tHash;
    i = 0;
    while (i < 40) {
        hash.push_back(powMod(hashC.at(i), e, n2));
        tHash.push_back((char) hash.at(i));
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
    std::string fHash = checksum.final();

    if (tHash == fHash) {
        std::cout << "Verified" << std::endl;
        std::cout << "First hash from cipher = " << tHash << " , and second hash from message = "
        << fHash << std::endl;
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