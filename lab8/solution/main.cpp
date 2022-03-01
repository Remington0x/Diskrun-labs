#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

int main () {
    int n;
    std::cin >> n;
    long long int a = 0, b = 0, c = 0;
    long long int maxA = 0, maxB = 0, maxC = 0;
    double maxS = -1;
    std::vector <long long int> lines(n);
    for (int i = 0; i < n; i++) {
        std::cin >> lines[i];
    }
    if (n >= 3) {    
        std::sort(lines.begin(), lines.end());
        for (int i = n - 1; i >= 2; i--) {
            if (lines[i] < lines[i - 1] + lines[i - 2]) {
                a = lines[i];
                b = lines[i - 1];
                c = lines[i - 2];
            }
            if ((a != 0) && (b != 0) && (c != 0) && (a < b + c)) {
                double p = double(a + b + c) / 2;
                double s = double(sqrt(p * (p - a) * (p - b) * (p - c)));
                if (s > maxS) {
                    maxS = s;
                    maxA = a;
                    maxB = b;
                    maxC = c;
                }
            }
        }
    } else {
        std::cout << 0 << std::endl;
    } 
    if (maxS > 0) {
        printf ("%.3lf\n", maxS);
        std::cout << maxC << " " << maxB << " " << maxA << std::endl; 
    }
    else {
        std::cout << 0 << std::endl;
    }
    
    return 0;
}