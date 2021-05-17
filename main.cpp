#include <iostream>

int gcd (unsigned int n1, unsigned int n2) {
    return (n2 == 0) ? n1 : gcd (n2, n1 % n2);
}

void keyBuilder(int p, int q){
    int cofac[20];
    double totd = 1;
    int kp = 0;
    int kop = 0;
    int n = p * q;
    int tot = (p-1) * (q-1);
    for (int i = 1; i < n; i++){
        if(gcd(kp, i) == 1){
            kp = i;
        }
    }
    int d;

    std::cout << "Masukkan d = ";
    std::cin >> d;

    if (gcd(d,n) != 1){
        std::cout << "d dan n Tidak relatif prima atau terlalu kecil" << std::endl;
    } else {
        int k = d;
        int pem = 2;
        bool cc = false;
        
        while (k != 0 || k == pem){
            if (k == 1){
                break;
            }
            
            if(k % pem == 0){
                k = k / pem;
                cofac[kop] = pem;
                kop = kop + 1;
            } else {
                for (int i = pem+1; i < d; i++){
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
        while (cofac[i] != '\0')
        {
            totd = totd * (1-(1/cofac[i]));
            i++;
        }
        
        int ks = 1;
        while (gcd((ks*kp), totd) != 1){
            ks++;
        }
        std::cout << std::endl;
        std::cout << "kp = " << kp << ", ks = " << ks << std::endl;
    }
}

int main() {
    int p, q;
    std::cout << "Masukkan p = ";
    std::cin >> p;
    std::cout << "Masukkan q = ";
    std::cin >> q;
    keyBuilder(p,q);
}