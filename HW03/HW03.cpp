// #include <stdio.h>
// #include <iostream>
// #include <string>
// #include <vector>
// using namespace std;
// void cardInput(vector<int> &card, int n, string input, int &total)
// {
//     for (int i = 0; i < n; i++)
//     {
//         cin >> input;
//         int suit = 0;
//         switch (input[0])
//         {
//         case 'C':
//             suit = 0;
//             break;
//         case 'D':
//             suit = 13;
//             break;
//         case 'H':
//             suit = 26;
//             break;
//         case 'S':
//             suit = 39;
//             break;
//         }
//         int num = suit;
//         switch (input[1])
//         {
//         case 'A':
//             total += 1;
//             num += 1;
//             break;
//         case 'T':
//             total += 10;
//             num += 10;
//             break;
//         case 'J':
//             total += 11;
//             num += 11;
//             break;
//         case 'Q':
//             total += 12;
//             num += 12;
//             break;
//         case 'K':
//             total += 13;
//             num += 13;
//             break;
//         default:
//             total += atoi(&input[1]);
//             num += atoi(&input[1]);
//         }
//         card[i] = num;
//     }
// }
// struct stateNode
// {
//     vector<int> Aremain;
//     int Acnt = 0; //A已經出的牌數
//     int Bcnt = 0; //B已經出的牌數
//     vector<int> Bremain;
//     bool turn; //0=A, 1=B;
//     int previousCard;
//     int score; // score>0: A win, score<0: B win
//     stateNode *nextState;
// };

// int DFS(stateNode &root, int n, int Atotal, int Btotal)
// {
//     if (root.Acnt == n || root.Bcnt == n)
//     {
//         if (root.Acnt == n)
//         {
//             return Btotal;
//         }
//         else
//             return Atotal;
//     }

//     if (!root.turn) // 輪到A出牌
//     {
//         vector<int> A_available;
//         root.Acnt++;

//         if (root.previousCard == 0)
//         { //root / pass
//             for (int i = 0; i < n; i++)
//             {
//                 A_available.push_back();
//             }
//         }
//         else
//         {
//             if (root.previousCard <= 13)
//             {
//                 for (int i = 0; i < n; i++)
//                 {
//                     A_available[i] = root.Aremain[i];
//                 }
//             }
//         }
//     }

// }
// int main()
// {
//     int n;
//     scanf("%d", &n);

//     string input;
//     vector<int> Acard(n, 0);
//     vector<int> Bcard(n, 0);
//     int Atotal = 0;
//     int Btotal = 0;
//     cardInput(Acard, n, input, Atotal);
//     cardInput(Bcard, n, input, Btotal);
//     stateNode root;
//     root.turn = 0;
//     root.previousCard = 0;
//     for (int i = 0; i < n; i++)
//     {
//         root.Aremain.push_back(Acard[i]);
//         root.Bremain.push_back(Bcard[i]);
//     }

//     // for (int i = 0; i < n; i++)
//     // {
//     //     cout << Acard[i] << endl;
//     // }
//     // cout << Atotal;
//     // for (int i = 0; i < n; i++)
//     // {
//     //     cout << Bcard[i] << endl;
//     // }
//     system("pause");
// }