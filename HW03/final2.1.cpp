#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <bitset>
#include <algorithm>
#include <map>

using namespace std;

#define make_key(turn, a, b, last_card, A_left, B_left) (turn + ((last_card & 63) << 1) + ((a & 1023) << 7) + ((b & 1023) << 17) + (A_left << 27) + (B_left << 44))
#define reset_bit(val, n) (val & ~(1ULL << n))
#define test_bit(val, n) (val & (1ULL << n))

const unsigned short cards_number = 52;
const unsigned short suit_number = 4;
const unsigned short card_value_number = 13;
const unsigned short pass_flag = 55;                           //use to denote the last round is pass
const unsigned short hash_key_size = 2 * cards_number + 1 + 6; //bit 0~51 : Alice's state ; bit 52~103 :Bob's state; bit 104 : Alice's turn ; bit 105~110 : last card
const long inf = 1023;                                         //the highest possible score is 193
short n;

typedef unsigned long cards_type; //64bit
typedef unsigned short us;
typedef unsigned long long ull;
typedef unsigned long long key_type;

map<ull, short> state_table;

vector<us> Alice_Cards_list;
vector<us> Bob_Cards_list;

long evaluate_score(cards_type cards, const vector<us> &cards_list, short n)
{
    long res = 0;
    for (short i = 0; i < n; i++)
    {
        if (cards & (1ULL << i))
        {
            res += (cards_list[i]) / suit_number + 1;
        }
    }
    return res;
}

inline long minmax(const cards_type Alice_left, const cards_type Bob_left, us last_card, short alpha, short beta, const bool turn_Alice)
{
    //case(1):terminal node => evaluate
    if (Alice_left == 0 || Bob_left == 0)
    {
        return Alice_left ? -(evaluate_score(Alice_left, Alice_Cards_list, n)) : evaluate_score(Bob_left, Bob_Cards_list, n);
    }
    //case(2):Alice's turn
    else if (turn_Alice == true)
    {
        key_type this_state = make_key(turn_Alice, alpha, beta, last_card, Alice_left, Bob_left);
        auto search_iter = state_table.find(this_state);
        if (search_iter != state_table.end())
        { //state in the table
            return search_iter->second;
        }
        //state not in table => need evaluated
        else if (last_card == pass_flag)
        { //Bob pass the previous turn
            long maxEval = -inf;
            long eval;
            //try every possible card
            for (us i = 0; i < n; i++)
            {
                //cout << "loop\n";
                if (test_bit(Alice_left, i))
                {
                    //this card can be thrown
                    //cout << "find card: " << Alice_Cards_list[i] << endl;
                    cards_type child_Alice_left = reset_bit(Alice_left, i);
                    eval = minmax(child_Alice_left, Bob_left, Alice_Cards_list[i], alpha, beta, false);
                    maxEval = (eval < maxEval) ? maxEval : eval;
                    alpha = (alpha < maxEval) ? maxEval : alpha;
                    //cout << "alpha: " << alpha << "beta: " << beta << endl;
                    if (alpha >= beta)
                    {
                        break;
                    }
                }
            }
            //store the state
            state_table.insert({this_state, maxEval});
            return maxEval;
        }
        else
        { //Bob didn't pass the previous turn
            long maxEval = -inf;
            long eval;
            //evaluate the child state
            const us last_card_suit = last_card % suit_number;
            const us last_card_value = last_card / suit_number;
            bool pass_this_turn = true;
            for (us i = 0; i < n; i++)
            {
                //cout << "i: " << i << endl;
                if (test_bit(Alice_left, i) && ((Alice_Cards_list[i] % suit_number == last_card_suit) || (Alice_Cards_list[i] / suit_number == last_card_value)))
                {
                    //this card can be thrown
                    pass_this_turn = false; //no need to pass
                    cards_type child_Alice_left = reset_bit(Alice_left, i);
                    //cout << "Alice didn't pass. She throw: " << Alice_Cards_list[i] << endl;
                    eval = minmax(child_Alice_left, Bob_left, Alice_Cards_list[i], alpha, beta, false);
                    maxEval = (eval < maxEval) ? maxEval : eval;
                    alpha = (alpha < eval) ? eval : alpha;
                    if (alpha >= beta)
                    {
                        break;
                    }
                }
            }
            if (pass_this_turn)
            {
                //cout << "Alice pass.\n";
                maxEval = minmax(Alice_left, Bob_left, pass_flag, alpha, beta, false);
                state_table.insert({this_state, maxEval});
                return maxEval;
            }
            //store the state
            state_table.insert({this_state, maxEval});
            return maxEval;
        }
    }
    //case(3) Bobs turn
    else
    {
        key_type this_state = make_key(turn_Alice, alpha, beta, last_card, Alice_left, Bob_left);
        auto search_iter = state_table.find(this_state);
        if (search_iter != state_table.end())
        { //state in the table
            return search_iter->second;
        }
        //state not in table => need evaluated
        else if (last_card == pass_flag)
        { //Alice pass the previous turn
            long minEval = inf;
            long eval;
            //try every possible card
            for (us i = 0; i < n; i++)
            {
                //cout << "loop\n";
                if (test_bit(Bob_left, i))
                {
                    //this card can be thrown
                    //cout << "find card: " << Bob_Cards_list[i] << endl;
                    cards_type child_Bob_left = reset_bit(Bob_left, i);
                    eval = minmax(Alice_left, child_Bob_left, Bob_Cards_list[i], alpha, beta, true);
                    minEval = (eval < minEval) ? eval : minEval;
                    beta = (beta < minEval) ? beta : minEval;
                    //cout << "alpha: " << alpha << "beta: " << beta << endl;
                    if (alpha >= beta)
                    {
                        break;
                    }
                }
            }
            //store the state
            state_table.insert({this_state, minEval});
            return minEval;
        }
        else
        { //Alice didn't pass the previous turn
            long minEval = inf;
            long eval;
            //evaluate the child state
            const us last_card_suit = last_card % suit_number;
            const us last_card_value = last_card / suit_number;
            bool pass_this_turn = true;
            for (us i = 0; i < n; i++)
            {
                //cout << "i: " << i << endl;
                if (test_bit(Bob_left, i) && ((Bob_Cards_list[i] % suit_number == last_card_suit) || (Bob_Cards_list[i] / suit_number == last_card_value)))
                {
                    //this card can be thrown
                    pass_this_turn = false; //no need to pass
                    cards_type child_Bob_left = reset_bit(Bob_left, i);
                    //cout << "Bob didn't pass. She throw: " << Alice_Cards_list[i] << endl;
                    eval = minmax(Alice_left, child_Bob_left, Bob_Cards_list[i], alpha, beta, true);
                    minEval = (eval < minEval) ? eval : minEval;
                    beta = (beta < eval) ? beta : eval;
                    if (alpha >= beta)
                    {
                        break;
                    }
                }
            }
            if (pass_this_turn)
            {
                //cout << "Alice pass.\n";
                minEval = minmax(Alice_left, Bob_left, pass_flag, alpha, beta, true);
                state_table.insert({this_state, minEval});
                return minEval;
            }
            //store the state
            state_table.insert({this_state, minEval});
            return minEval;
        }
    }
}

int main()
{
    // process the input

    cin >> n;

    string temp;
    short number;
    short suit;
    for (int i = 0; i < n; i++)
    {
        cin >> temp;
        switch (temp[0])
        {
        case 'C':
            suit = 0;
            break;
        case 'D':
            suit = 1;
            break;
        case 'H':
            suit = 2;
            break;
        case 'S':
            suit = 3;
            break;
        default:
            break;
        }
        switch (temp[1])
        {
        case 'A':
            number = 0;
            break;
        case '2':
            number = 1;
            break;
        case '3':
            number = 2;
            break;
        case '4':
            number = 3;
            break;
        case '5':
            number = 4;
            break;
        case '6':
            number = 5;
            break;
        case '7':
            number = 6;
            break;
        case '8':
            number = 7;
            break;
        case '9':
            number = 8;
            break;
        case 'T':
            number = 9;
            break;
        case 'J':
            number = 10;
            break;
        case 'Q':
            number = 11;
            break;
        case 'K':
            number = 12;
            break;
        default:
            break;
        }
        Alice_Cards_list.push_back((number * suit_number) + suit);
    }
    for (int i = 0; i < n; i++)
    {
        cin >> temp;
        switch (temp[0])
        {
        case 'C':
            suit = 0;
            break;
        case 'D':
            suit = 1;
            break;
        case 'H':
            suit = 2;
            break;
        case 'S':
            suit = 3;
            break;
        default:
            break;
        }
        switch (temp[1])
        {
        case 'A':
            number = 0;
            break;
        case '2':
            number = 1;
            break;
        case '3':
            number = 2;
            break;
        case '4':
            number = 3;
            break;
        case '5':
            number = 4;
            break;
        case '6':
            number = 5;
            break;
        case '7':
            number = 6;
            break;
        case '8':
            number = 7;
            break;
        case '9':
            number = 8;
            break;
        case 'T':
            number = 9;
            break;
        case 'J':
            number = 10;
            break;
        case 'Q':
            number = 11;
            break;
        case 'K':
            number = 12;
            break;
        default:
            break;
        }
        Bob_Cards_list.push_back((number * suit_number) + suit);
    }

    /*
    cards_type r = (1UL << 5) - 1;
    r = reset_bit(r, 2);
    ull key = make_key(1, -inf, 7, 0, 4, r);
    //state_table.insert({key, 5});
    cout << minmax(5, r, 6, -inf, 28, true);
    cin >> r;
    */

    //output

    unsigned long initial_cards = (1UL << n) - 1;

    long result = minmax(initial_cards, initial_cards, pass_flag, -inf, inf, true);
    if (result > 0)
    {
        cout << "Alice" << endl
             << result;
    }
    else if (result < 0)
    {
        cout << "Bob" << endl
             << -result;
    }
    //system("pause");

    return 0;
}