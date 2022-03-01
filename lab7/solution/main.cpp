#include <iostream>
#include <vector>

int main() {
    int n;
    std::cin >> n;
    std::vector<long long> dp(n + 1, -1);
    std::vector<int> op(n + 1);
    dp[1] = 0;

    for (int i = 1; i < n; ++i) {
        if (dp[i + 1] == -1 || dp[i + 1] > dp[i] + i + 1) {
            dp[i + 1] = dp[i] + i + 1;
            op[i + 1] = 1;
        }
        if (i * 2 <= n && (dp[i * 2] == -1 || dp[i * 2] > dp[i] + i * 2)) {
            dp[i * 2] = dp[i] + i * 2;
            op[i * 2] = 2;
        }
        if (i * 3 <= n && (dp[i * 3] == -1 || dp[i * 3] > dp[i] + i * 3)) {
            dp[i * 3] = dp[i] + i * 3;
            op[i * 3] = 3;
        }
    }
    printf("%lld\n", dp[n]);
    int i = n;
    while(i > 1) {
        if (op[i] == 1) {
            i = i - 1;
            printf("-1 ");
        }

        if (op[i] == 2) {
            i = i / 2;
            printf("/2 ");
        }

        if (op[i] == 3) {
            i = i / 3;
            printf("/3 ");
        }
    }
    printf("\n");   
} 