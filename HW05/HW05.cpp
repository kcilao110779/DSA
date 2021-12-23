#include <iostream>
#include <vector>
#include <stdlib.h>
using namespace std;

typedef unsigned long long ull;

int N, M;
int totalCount = 0;

int c[100][100], solved[100][100]; // init solved false

int C(int n, int m)
{
    if (solved[n][m] == true)
        return c[n][m];

    solved[n][m] = true;

    if (n == m || m == 0)
        return c[n][m] = 1;
    else
        return c[n][m] = (C(n - 1, m) + C(n - 1, m - 1)) % 1000000007;
}
ull find_canTear_count(vector<vector<char>> &paper, int L1, int R1, int L2, int R2, int K)
{
    vector<int> row_index;
    vector<int> col_index;
    int tmp = 0;
    bool tear = false; //表示該紙張是否還能撕下去
    if (K == 0)
        return 1;
    if (L1 == R1 && L2 == R2)
        return 0;

    if (L1 != R1)
    {
        for (int i = L1; i <= R1; i++)
        {                        //找出哪一列可以被撕
            bool canTear = true; //表示第i列可以被撕
            if (paper[i][L2] == 'O')
            {
                for (int j = L2; j <= R2; j++)
                {
                    if (paper[i][j] == 'X')
                        canTear = false;
                }
            }
            else
                canTear = false;
            if (canTear)
            { //紀錄可被撕的列的index
                row_index.push_back(i);
                tear = true;
            }
        }
    }
    if (L2 != R2)
    {
        for (int j = L2; j <= R2; j++)
        { //找出哪一行可以被撕
            bool canTear = true;
            if (paper[L1][j] == 'O')
            {
                for (int i = L1; i <= R1; i++)
                {
                    if (paper[i][j] == 'X')
                        canTear = false;
                }
            }
            else
                canTear = false;
            if (canTear)
            {
                col_index.push_back(j);
                tear = true;
            }
        }
    }
    if (!tear)
    {
        return 0;
    }
    else
    {
        for (int i = 0; i < row_index.size(); i++)
        { //撕列
            if (row_index[i] == L1)
            {
                tmp += find_canTear_count(paper, L1 + 1, R1, L2, R2, K - 1);
            }
            else if (row_index[i] == R1)
            {
                tmp += find_canTear_count(paper, L1, R1 - 1, L2, R2, K - 1);
            }
            else
            {
                for (int KA = 0; KA <= K - 1; K++)
                {
                    int Acount, Bcount;
                    Acount = find_canTear_count(paper, L1, row_index[i] - 1, L2, R2, KA);
                    Bcount = find_canTear_count(paper, row_index[i] + 1, R1, L2, R2, K - 1 - KA);
                    tmp = tmp + (C(K - 1, KA) * Acount * Bcount);
                }
            }
        }
        for (int i = 0; i < col_index.size(); i++)
        { //撕行
            if (col_index[i] == L2)
            {
                tmp += find_canTear_count(paper, L1, R1, L2 + 1, R2, K - 1);
            }
            else if (col_index[i] == R2)
            {
                tmp += find_canTear_count(paper, L1, R1, L2, R2 - 1, K - 1);
            }
            else
            {
                for (int KA = 0; KA <= K - 1; K++)
                {
                    int Acount, Bcount;
                    Acount = find_canTear_count(paper, L1, R1, L2, col_index[i] - 1, KA);
                    Bcount = find_canTear_count(paper, L1, R1, col_index[i] + 1, R2, K - 1 - KA);
                    tmp = tmp + (C(K - 1, KA) * Acount * Bcount);
                }
            }
        }
        return tmp;
    }
}
int main()
{
    cin >> N >> M;
    vector<vector<char>> paper(N, vector<char>(M));

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
            cin >> paper[i][j];
    }
    for (int K = 0; K <= 100; K++)
    {
        ull count = find_canTear_count(paper, 0, N - 1, 0, M - 1, K);
        cout << "K: " << K << endl;
        cout << count << endl;
        if (count == 0)
            break;
        totalCount += find_canTear_count(paper, 0, N - 1, 0, M - 1, K);
    }
    cout << totalCount;
    system("pause");
    return 0;
}