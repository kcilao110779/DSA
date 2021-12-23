#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

int x = 197; //(131,251,283,197)
int x2 = 251;
// unsigned long long prime_mod = 1034269013;
// unsigned long long prime_mod = 68718952447; //(90,80,95)分
// unsigned long long prime_mod = 68720001023; //(85,95,95)分
unsigned long long prime_mod = 63018038201; //(95,90,95,100)分
// unsigned long long prime_mod = 51539607551; //(90,90,90,85)分
// unsigned long long prime_mod = 51539607599; //(80,90,80,85)
// unsigned long long prime_mod2 = 1005230747;
const int max_size = 100001;

unsigned long long x_power[max_size];
// unsigned long long x2_power[max_size];
// string ans_longestsubstr[max_size];
// string ans_prefixsuffix[max_size];

vector<string> ans_longestsubstr;
vector<string> ans_prefixsuffix;
int count = 0;

unordered_map<unsigned long long, int>::iterator iter;

void calpower()
{
    x_power[0] = 1;
    // x2_power[0] = 1;
    for (int i = 1; i < 100001; i++)
    {
        x_power[i] = x_power[i - 1] * x;
        // x2_power[i] = x_power[i - 1] * x2;
    }
}

// unsigned long long power(int x, int pow)
// {
//     unsigned long long result = x;
//     for (int i = 1; i < pow; i++)
//         result *= x;
//     return result;
// }
void output()
{
    for (int i = 0; i < count; i++)
    {
        cout << ans_longestsubstr[i] << endl;
        cout << ans_prefixsuffix[i] << endl;
    }
}

int main()
{

    ios_base::sync_with_stdio(0);
    cin.tie(0);
    calpower();

    int m;
    string str;

    while (1)
    {
        // cin >> m;
        cin >> m;
        if (m == 0)
            break;
        cin >> str;

        count++;
        // cout << "-----------case: " << count << "-----------------" << endl;

        vector<unsigned long long> rolling_hash(str.length());
        vector<unsigned long long> rolling_hash2(str.length());
        rolling_hash[0] = str[0];
        rolling_hash2[0] = str[0];
        for (int i = 1; i < str.length(); i++)
        {
            rolling_hash[i] = rolling_hash[i - 1] * x + str[i];
            rolling_hash2[i] = rolling_hash2[i - 1] * x2 + str[i];
            // // i = 字元長度
        }

        // for (int i = 0; i < str.length(); i++)
        // {
        //     cout << i << " " << rolling_hash[i] << endl;
        //     // // i = 字元長度
        // }
        int len = str.length();

        vector<bool> be_ans(str.length() + 1, false);
        // be_ans[i] 表示長度為i的子字串能否為答案
        bool no_answer = true;
        // cout << "problem1: " << endl;
        int prefail_len = len; //紀錄二分搜時前一個搜尋長度，方便往上跳index
        int presuccess_len = 0;
        int maxlen = 0;
        // string pro1_substr;

        while (len >= 1)
        { //problem 1
            //len = 搜尋的子字串長度
            // unordered_map<unsigned long long, int> frequency_table;
            // cout << "binary search len = " << len << endl;
            unordered_map<unsigned long long, int> frequency_table;
            int rightmost_starting = 0;
            for (int start = 0; start + len <= str.length(); start++)
            { //start=子字串搜尋的起始位置
                // string sub = str.substr(start, len);
                // cout << "start: " << start << endl;
                // cout << "str: " << sub << endl;
                unsigned long long sub2int;

                sub2int = (start == 0) ? rolling_hash[len - 1] % prime_mod : (rolling_hash[start + len - 1] - rolling_hash[start - 1] * x_power[len]) % prime_mod;
                iter = frequency_table.find(sub2int);
                if (iter == frequency_table.end())
                { //此子字串不在hashtable中
                    frequency_table.insert({sub2int, 1});

                    // cout << "insert: " << sub << endl;
                    if (m == 1)
                    {
                        no_answer = false;
                        be_ans[len] = true;
                        rightmost_starting = start;
                        maxlen = len;
                        // pro1_substr = sub;
                    }
                }
                else
                { //此子字串已在hashtable
                    iter->second++;
                    // cout << "find " << sub << " count: " << frequency_table.find(sub2int)->second << endl;
                    if (iter->second >= m)
                    {
                        no_answer = false;
                        be_ans[len] = true;
                        rightmost_starting = start;
                        maxlen = len;
                    }
                }
            }

            if (be_ans[len] && len == str.length())
            {
                ans_longestsubstr.push_back(to_string(maxlen) + " " + to_string(rightmost_starting));
                // ans_longestsubstr[count] = to_string(maxlen) + " " + to_string(rightmost_starting);
                // cout << ans_longestsubstr[count] << endl;
                break;
            }
            else if (!be_ans[len])
            { //二分搜往下跳
                prefail_len = len;
                len = (len + presuccess_len) / 2;
            }

            else if (be_ans[len] && len != str.length())
            { //去搜尋子字串長度為len+1時，能否成為答案
                //若len和len+1皆可成為答案，則二分搜往上跳，若len可成為答案但len+1不可成為答案，則len即是所求的轉折點
                // frequency_table.clear();
                unordered_map<unsigned long long, int> frequency_table2;
                presuccess_len = len;
                // no_answer = true;

                for (int start = 0; start + len + 1 <= str.length(); start++)
                { //start=搜尋的起始位置
                    // string sub = str.substr(start, len + 1);
                    // cout << "str2: " << sub << endl;
                    unsigned long long sub2int;

                    sub2int = (start == 0) ? rolling_hash[len + 1 - 1] % prime_mod : (rolling_hash[start + len + 1 - 1] - rolling_hash[start - 1] * x_power[len + 1]) % prime_mod;
                    iter = frequency_table2.find(sub2int);
                    if (iter == frequency_table2.end())
                    { //此子字串不在hashtable中
                        frequency_table2.insert({sub2int, 1});

                        // cout << "insert2 " << sub2int << endl;
                        if (m == 1)
                        {
                            be_ans[len + 1] = true;
                        }
                    }
                    else
                    { //此子字串已在hashtable
                        iter->second++;
                        // cout << "find2 " << sub2int << "count: " << frequency_table2.find(sub2int)->second << endl;
                        if (iter->second >= m)
                        {
                            be_ans[len + 1] = true;
                        }
                    }
                }
                // frequency_table.clear();
                if (!be_ans[len + 1])
                { //若len可成為答案但len+1不可成為答案，則len即是所求的轉折點
                    // ans_longestsubstr[count] = to_string(maxlen) + " " + to_string(rightmost_starting);
                    // cout << ans_longestsubstr[count] << endl;
                    ans_longestsubstr.push_back(to_string(maxlen) + " " + to_string(rightmost_starting));

                    break;
                }
                else //若len和len+1皆可成為答案，則二分搜往上跳
                    len = (len + prefail_len) / 2;
            }
        }
        if (no_answer)
        {
            ans_longestsubstr.push_back("none");

            // ans_longestsubstr[count] = "none";
            // cout << ans_longestsubstr[count] << endl;
        }

        //------------------problem 1 finish------------------------

        // frequency_table.clear();

        //------------------problem 2 start--------------------------
        // cout << "prob2" << endl;
        vector<int> possible_len;
        // vector<string> ans_str;

        bool have_answer = false;
        string longest_prefixsuffix;
        int longest_len = 0;

        if (str.length() > 2)
        { //若長度小於2，因為str.length()是unsigned，所以str.length()-2還是正數不會跳出迴圈
            for (int len2 = 1; len2 <= str.length() - 2; len2++)
            { //先找出prefix = suffix的子字串長度，將可能長度存進possible_len中
                // cout << len << endl;

                // string prefix = str.substr(0, len2);
                // string suffix = str.substr(str.length() - len2, len2);
                unsigned long long prefix2int = rolling_hash[len2 - 1] % prime_mod;
                unsigned long long suffix2int = (rolling_hash[str.length() - 1] - rolling_hash[str.length() - len2 - 1] * x_power[len2]) % prime_mod;

                // cout << "prefix: " << str.substr(0, len2) << " " << prefix2int << endl;
                // cout << "suffix: " << str.substr(str.length() - len2, len2) << " " << suffix2int << endl;
                if (prefix2int == suffix2int)
                {
                    possible_len.push_back(len2);
                    // ans_str.push_back(prefix);
                    // cout << "push: " << prefix << endl;
                }
            }

            int i = possible_len.size() - 1;
            int prefail_i = i;
            int presuccess_i = 0;
            while (i >= 0)
            { //用二分搜，去看有沒有長度為possible_len[i]的子字串在中綴也有出現
                int len2 = possible_len[i];

                // cout << "i: " << i << endl;
                // cout << "len2: " << len2 << endl;
                // unordered_map<unsigned long long, int> frequency_table;
                unsigned long long possible_int = rolling_hash[len2 - 1] % prime_mod;
                unsigned long long sub2int;

                for (int start = 1; start + len2 < str.length(); start++)
                {
                    sub2int = (rolling_hash[start + len2 - 1] - rolling_hash[start - 1] * x_power[len2]) % prime_mod;
                    if (sub2int == possible_int)
                    { //該子字串有在中綴中出現
                        // cout << len2 << " can be answer" << endl;
                        // longest_prefixsuffix = ans_str[i];
                        longest_len = len2;
                        have_answer = true;
                        break;
                    }
                }

                if (!have_answer)
                { //二分搜index往下跳
                    prefail_i = i;
                    i = (i + presuccess_i) / 2;
                    if (i == 0)
                        break;
                }
                else
                {
                    //二分搜index往上跳
                    presuccess_i = i;
                    int tmp = i;
                    i = (i + prefail_i) / 2;
                    if (i == tmp)
                        break;
                }
                // frequency_table.clear();
            }
        }

        // cout << "----------problem2-----------" << endl;
        // for (int i = 0; i < possible_len.size(); i++)
        //     cout << i << " " << possible_len[i] << endl;

        string ans = (have_answer) ? str.substr(0, longest_len) : "Just a legend";
        ans_prefixsuffix.push_back(ans);
        // cout << ans_prefixsuffix[count] << endl;
    }
    output();

    system("pause");
    return 0;
}