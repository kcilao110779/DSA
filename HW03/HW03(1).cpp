#include <stdio.h>
#include <iostream>

#include <vector>

#include <unordered_map>

using namespace std;
void cardInput(vector<int> &card, int n, string input)
{
    for (int i = 0; i < n; i++)
    {
        cin >> input;
        int suit = 0;
        switch (input[0])
        {
        case 'C':
            suit = 0;
            break;
        case 'D':
            suit = 13;
            break;
        case 'H':
            suit = 26;
            break;
        case 'S':
            suit = 39;
            break;
        }
        int num = suit;
        switch (input[1])
        {
        case 'A':
            num += 1;

            break;
        case 'T':
            num += 10;
            break;
        case 'J':
            num += 11;
            break;
        case 'Q':
            num += 12;
            break;
        case 'K':
            num += 13;
            break;
        default:
            num += atoi(&input[1]);
        }
        card.push_back(num);
    }
}

typedef unsigned long long ull;
typedef unsigned long ul;

unordered_map<ull, int> stateScore;
unordered_map<ull, int>::iterator iter;
vector<int> Acard;
vector<int> Bcard;
int highest = 300;
int lowest = -300;
bool turn = false; // false = A, true = B;
ul check_bit(ul remain, int ith_bit)
{
    return (remain & (1UL << ith_bit));
}
ul play_bit(ul remain, int ith_bit)
{
    return (remain & ~(1UL << ith_bit));
}
ull state_to_ull(bool turn, ul Aremain, ul Bremain, int previousCard, int alpha, int beta)
{
    return ((turn << 60) + (Aremain << 43) + (Bremain << 26) + ((previousCard & 63) << 20) + ((alpha & 1023) << 10) + (beta & 1023));
}
int calculateScore(ul remain, vector<int> card, int n)
{
    int score = 0;
    for (int i = 0; i < n; i++)
    { //if ith bit is 1, than count into score
        if (check_bit(remain, i))
        {
            if (card[i] / 13 == 0)
                score += 13;
            else
            {
                score += (card[i] % 13);
            }
            // cout << "card: " << card[i] << "    "
            //      << "score: " << score << endl;
        }
    }
    return score;
}

int miniMAX(ul Aremain, ul Bremain, int n, int previousCard, int alpha, int beta, bool turn)
{
    if (Aremain == 0 || Bremain == 0)
    {                     //leaf node(A finish or B finish) return score
        if (Aremain == 0) // A lose
        {
            // cout << "A win." << endl;
            // cout << "B remainCard: ";
            // for (int i = 0; i < n; i++)
            // {
            //     if (check_bit(Bremain, i))
            //     {
            //         cout << Bcard[i] << " ";
            //     }
            // }
            // cout << endl;
            // cout << "score: " << calculateScore(Bremain, Bcard, n) << endl;
            return calculateScore(Bremain, Bcard, n);
        }

        else
        {
            // cout << "B win." << endl;
            // cout << "A remainCard: ";
            // for (int i = 0; i < n; i++)
            // {
            //     if (check_bit(Aremain, i))
            //     {
            //         cout << Acard[i] << " ";
            //     }
            // }
            // cout << endl;
            // cout << "score: " << (-1) * calculateScore(Aremain, Acard, n) << endl;
            return -calculateScore(Aremain, Acard, n);
        }
    }
    else if (!turn)
    { //Alice's turn
        ull thisState = state_to_ull(turn, Aremain, Bremain, previousCard, alpha, beta);
        iter = stateScore.find(thisState);
        if (iter != stateScore.end())
        { //find thisState in the hash table
            // cout << "Find" << endl;
            return iter->second;
        }
        else
        {
            int max = lowest;
            if (previousCard == 0)
            { //initial state or Bob pass the previous round;
                for (int i = 0; i < n; i++)
                {
                    if (check_bit(Aremain, i))
                    { //A plays ith card
                        ul next_Aremain = play_bit(Aremain, i);
                        int thisCard = Acard[i];
                        // cout << "A plays: " << thisCard << endl;
                        turn = true; //B's turn for next state
                        int tmp = miniMAX(next_Aremain, Bremain, n, thisCard, alpha, beta, turn);
                        if (max <= tmp)
                            max = tmp;
                        if (alpha < max)
                            alpha = max;
                        if (alpha >= beta)
                        { //pruning
                            break;
                        }
                    }
                }
                //update the hashTable by thisState
                stateScore.insert({thisState, max});
                return max;
            }
            else
            { //previous != 0;
                bool pass = true;
                for (int i = 0; i < n; i++)
                { //A chooses the available next card(B didn't pass);
                    if (check_bit(Aremain, i) && ((Acard[i] % 13 == previousCard % 13) || ((Acard[i] - 1) / 13 == (previousCard - 1) / 13)))
                    {
                        pass = false;
                        ul next_Aremain = play_bit(Aremain, i);
                        int thisCard = Acard[i];
                        // cout << "A plays: " << thisCard << endl;
                        turn = true; //B's turn for next state
                        int tmp = miniMAX(next_Aremain, Bremain, n, thisCard, alpha, beta, turn);
                        if (max <= tmp)
                            max = tmp;
                        if (alpha < tmp)
                            alpha = tmp;
                        if (alpha >= beta)
                        { //pruning
                            break;
                        }
                    }
                }
                if (pass)
                {
                    // cout << "A pass!" << endl;
                    turn = true;
                    max = miniMAX(Aremain, Bremain, n, 0, alpha, beta, turn);
                    stateScore.insert({thisState, max});
                    return max;
                }

                stateScore.insert({thisState, max});
                return max;
            }
        }
    }
    else //B's turn
    {
        ull thisState = state_to_ull(turn, Aremain, Bremain, previousCard, alpha, beta);
        iter = stateScore.find(thisState);
        if (iter != stateScore.end())
        { //find thisState in the hash table
            // cout << "Find" << endl;
            return iter->second;
        }
        else
        {
            int min = highest;
            if (previousCard == 0)
            { //Alice pass the previous round;
                for (int i = 0; i < n; i++)
                {
                    if (check_bit(Bremain, i))
                    { //B plays ith card
                        ul next_Bremain = play_bit(Bremain, i);
                        int thisCard = Bcard[i];
                        // cout << "B plays: " << thisCard << endl;
                        turn = false; //A's turn for next state
                        int tmp = miniMAX(Aremain, next_Bremain, n, thisCard, alpha, beta, turn);
                        if (min > tmp)
                            min = tmp;
                        if (beta >= min)
                            beta = min;
                        if (alpha >= beta)
                        { //pruning
                            break;
                        }
                    }
                }
                //update the hashTable by thisState
                stateScore.insert({thisState, min});
                return min;
            }
            else
            { //previous != 0;
                bool pass = true;
                for (int i = 0; i < n; i++)
                { //B chooses the available next card(A didn't pass);
                    if (check_bit(Bremain, i) && ((Bcard[i] % 13 == previousCard % 13) || ((Bcard[i] - 1) / 13 == (previousCard - 1) / 13)))
                    {
                        pass = false;
                        ul next_Bremain = play_bit(Bremain, i);
                        int thisCard = Bcard[i];
                        // cout << "B plays: " << thisCard << endl;
                        turn = false; //A's turn for next state
                        int tmp = miniMAX(Aremain, next_Bremain, n, thisCard, alpha, beta, turn);
                        if (min > tmp)
                            min = tmp;
                        if (beta > tmp)
                            beta = tmp;
                        if (alpha >= beta)
                        { //pruning
                            break;
                        }
                    }
                }
                if (pass)
                {
                    // cout << "B pass!" << endl;
                    turn = false;
                    min = miniMAX(Aremain, Bremain, n, 0, alpha, beta, turn);
                    stateScore.insert({thisState, min});
                    return min;
                }

                stateScore.insert({thisState, min});
                return min;
            }
        }
    }
}

int main()
{
    int n;
    scanf("%d", &n);

    string input;

    cardInput(Acard, n, input);
    cardInput(Bcard, n, input);

    ul AinitialCard = (1UL << n) - 1; //n 1s present the initial condition
    ul BinitialCard = (1UL << n) - 1;

    int inital_previous = 0;
    int ans = miniMAX(AinitialCard, AinitialCard, n, inital_previous, lowest, highest, turn);
    // cout << "ans:  " << ans << endl;
    if (ans > 0)
    {
        cout << "Alice\n";
        cout << ans;
    }

    else
    {
        cout << "Bob\n";
        cout << -ans;
    }
    system("pause");
}
