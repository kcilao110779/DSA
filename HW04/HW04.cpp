#include <iostream>
// #include <stdio.h>
#include <string>
#include <unordered_map>
// #include <map>
#include <vector>

using namespace std;

unsigned long long power(int x, int pow)
{
    unsigned long long result = x;
    for (int i = 1; i < pow; i++)
        result *= x;
    return result;
}
int x = 131;
unsigned long long prime_mod = 1034269013;
const int max_size = 100001;

string ans_longestsubstr[max_size];
string ans_prefixsuffix[max_size];

int count = 0;
void output()
{
    // cout << "----------------------------------" << endl;
    // cout << "problem size: " << ans_problem1.size() << endl;
    for (int i = 1; i <= count; i++)
    {
        // cout << "case: " << i + 1 << endl;

        cout << ans_longestsubstr[i] << endl;
        cout << ans_prefixsuffix[i] << endl;
    }
}
//aaaaaaaaaaaaaaaaaaaa:20
unordered_map<unsigned long long, int>::iterator iter;

int main()
{

    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int m;
    string str;

    bool end = false;

    while (1)
    {
        cin >> m;

        if (m == 0)
            break;
        cin >> str;
        count++;
        // cout << "-----------case: " << count << "-----------------" << endl;
        vector<unsigned long long> rolling_hash(str.length());
        rolling_hash[0] = str[0];
        for (int i = 1; i < str.length(); i++)
        {
            rolling_hash[i] = (rolling_hash[i - 1] * x + str[i]);
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
        string pro1_substr;

        while (len >= 1)
        { //problem 1
            //len = 搜尋的字串長度
            // unordered_map<unsigned long long, int> frequency_table;
            cout << "binary search len = " << len << endl;
            int rightmost_starting = 0;
            unordered_map<unsigned long long, int> frequency_table;
            for (int start = 0; start + len <= str.length(); start++)
            { //start=搜尋的起始位置
                string sub = str.substr(start, len);
                // cout << "start: " << start << endl;
                // cout << "str: " << sub << endl;
                unsigned long long sub2int;

                sub2int = (start == 0) ? rolling_hash[len - 1] % prime_mod : (rolling_hash[start + len - 1] - rolling_hash[start - 1] * power(x, len)) % prime_mod;
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
                        pro1_substr = sub;
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
                // printf("%d %d", maxlen, rightmost_starting);
                // cout << maxlen << " " << rightmost_starting << endl;
                ans_longestsubstr[count] = to_string(maxlen) + " " + to_string(rightmost_starting);
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
                    string sub = str.substr(start, len + 1);
                    // cout << "str2: " << sub << endl;
                    unsigned long long sub2int;

                    sub2int = (start == 0) ? rolling_hash[len + 1 - 1] % prime_mod : (rolling_hash[start + len + 1 - 1] - rolling_hash[start - 1] * power(x, len + 1)) % prime_mod;
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
                    ans_longestsubstr[count] = to_string(maxlen) + " " + to_string(rightmost_starting);
                    cout << ans_longestsubstr[count] << endl;
                    break;
                }
                else //若len和len+1皆可成為答案，則二分搜往上跳
                    len = (len + prefail_len) / 2;
            }
        }
        if (no_answer)
        {
            // printf("none\n");
            // cout << "none\n";
            ans_longestsubstr[count] = "none";
            // cout << ans_longestsubstr[count] << endl;
        }

        //------------------problem 1 finish------------------------

        // frequency_table.clear();

        //------------------problem 2 start--------------------------

        vector<int> possible_len;
        vector<string> ans_str;
        for (int len2 = 1; len2 <= str.length() - 2; len2++)
        { //先找出prefix = suffix的子字串長度，將可能長度存進possible_len中
            // cout << len << endl;

            string prefix = str.substr(0, len2);
            string suffix = str.substr(str.length() - len2, len2);
            unsigned long long prefix2int = rolling_hash[len2 - 1] % prime_mod;
            unsigned long long suffix2int = (rolling_hash[str.length() - 1] - rolling_hash[str.length() - len2 - 1] * power(x, len2)) % prime_mod;

            // cout << "prefix: " << str.substr(0, len2) << " " << prefix2int << endl;
            // cout << "suffix: " << str.substr(str.length() - len2, len2) << " " << suffix2int << endl;
            if (prefix2int == suffix2int)
            {
                possible_len.push_back(len2);
                ans_str.push_back(prefix);
                // cout << "push: " << prefix << endl;
            }
        }
        cout << "----------problem2-----------" << endl;
        for (int i = 0; i < possible_len.size(); i++)
            cout << i << " " << possible_len[i] << endl;
        string longest_prefixsuffix;
        bool have_answer = false;
        int i = possible_len.size() - 1;
        int prefail_i = i;
        int presuccess_i = 0;
        while (i >= 0)
        { //用二分搜，去看有沒有長度為possible_len[i]的子字串在中綴也有出現
            int len2 = possible_len[i];

            cout << "i: " << i << endl;
            cout << "len2: " << len2 << endl;
            // unordered_map<unsigned long long, int> frequency_table;
            unsigned long long possible_int = rolling_hash[len2 - 1] % prime_mod;
            unsigned long long sub2int;

            for (int start = 1; start + len2 < str.length(); start++)
            {
                sub2int = (rolling_hash[start + len2 - 1] - rolling_hash[start - 1] * power(x, len2)) % prime_mod;
                if (sub2int == possible_int)
                { //該子字串有在中綴中出現
                    // cout << len2 << " can be answer" << endl;
                    longest_prefixsuffix = ans_str[i];
                    have_answer = true;
                    break;
                }
            }
            //abcd
            // if ((have_answer && (i == possible_len.size() - 1 || (i == prefail_i && i != possible_len.size() - 1))) || i == 0)
            // {   //
            //     break;
            // }
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
        if (have_answer)
        {
            // cout << longest_prefixsuffix << endl;
            ans_prefixsuffix[count] = longest_prefixsuffix;
            // cout << ans_prefixsuffix[count] << endl;
            // ans_prefixsuffix.push_back(longest_prefixsuffix);
        }
        else
        {
            // printf("Just a legend\n");
            ans_prefixsuffix[count] = "Just a legend";
            // cout << ans_prefixsuffix[count] << endl;
            // ans_prefixsuffix.push_back("Just a legend");
            // cout << "Just a legend" << endl;
        }
        cout << ans_prefixsuffix[count] << endl;
    }
    output();

    system("pause");
    return 0;
}