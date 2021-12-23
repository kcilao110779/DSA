#include <iostream>
#include <math.h>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class hw4
{
public:
    int k;
    long long mod;
    int m;
    string s;
    vector<unsigned long long> pow;
    vector<unsigned long long> s_hash;
    unordered_map<unsigned long long,int>::iterator it;
    hw4(int num, vector<int>& answer1, vector<string>& answer2)
    {
        m = num;
        k = 3;
        mod = 68718952477;
        pow.push_back(1);
        cin>>s;
        vector<vector<unsigned long long> > s_table(s.size()+1);
        for (unsigned int i = 1; i <= s.size(); ++i)
        {
            pow.push_back(pow[i - 1]*k);
        }
        //------------------------------
/*
        cout<<"pow: "<<endl;
        for (int i = 0; i < pow.size(); ++i)
        {
            cout<<pow[i]<<" ";
        }
        cout<<endl;
        
        */

        //-------------------------------
        s_hash.push_back(0);
        for (unsigned int i = 0; i < s.size(); ++i)
        {
            s_hash.push_back(calhash(i));
        }

        //------------------------------------
        /*
        cout<<"hash: "<<endl;
        for (int i = 0; i < s_hash.size(); ++i)
        {
            cout<<s_hash[i]<<" ";
        }
        cout<<endl;
        */
        //-------------------------------------
        caltable(s_table);
        //------------------------------------
        /*
        cout<<"table: "<<endl;
        for (int i = 0; i <= s.size(); ++i)
        {
            for (int j = 0; j < s_table[i].size(); ++j)
            {
                cout<<s_table[i][j]<<" ";
            }
            cout<<endl;
            cout<<endl;
        }
        */
        //-------------------------------------
        problem1(0, s.size(), answer1, s_table);
        problem2(answer2, s_table);
        //cout<<s.size()<<endl;
    }

    unsigned long long calhash(int index)
    {
        unsigned long long hash = 0;
        for (int i = 0; i <= index; ++i)
        {
            //hash = hash + ((s[index-i]-96) * pow[i]) % mod;
            hash = hash + (s[index-i]-96) * pow[i];
        }
        //return hash % mod;
        return hash;
    }

    void caltable(vector<vector<unsigned long long> >& s_table)
    {
        for (int l = 0; l <= s.size(); ++l)
        {
            for (unsigned int i = 0; i <= s.size()-l; ++i)
            {
                s_table[l].push_back((s_hash[i+l] - s_hash[i]*pow[l]) % mod);
            }
        }
    }
    unsigned long long modpow(unsigned long long h, int l)
    {
        for (int i = 0; i < l; ++i)
        {
            h = (h * k) % mod;
        }
        return h;
    }
    void problem1(int left, int right, vector<int>& answer1, vector<vector<unsigned long long> >& s_table)
    {
        if ((m == 1)&&(s.size() == 1))
        {
            answer1.push_back(1);
            answer1.push_back(0);
        }
        else
        {
            
            int mid= (left + right)/2;
            /*
            cout<<"("<<left<<" "<<right<<")"<<endl;
            cout<<mid<<endl;*/
            
            vector<int> indexmap(s.size()+2,-1);
            while(left <= right)
            {
                indexmap[mid] = goodlength(mid, s_table);
                if (indexmap[mid] >= 0)
                {
                    left = mid + 1;
                }
                else
                {
                    right = mid - 1;
                }
                mid= (left + right)/2;
                /*
                cout<<"("<<left<<" "<<right<<")"<<endl;
                cout<<mid<<endl;*/
                
            }
            /*
            cout<<"indexmap: "<<endl;
            for (int i = 0; i < indexmap.size(); ++i)
            {
                cout<<indexmap[i]<<" ";
            }
            cout<<endl;
            */
            if (mid == 0)
            {
                answer1.push_back(0);
                answer1.push_back(0);
            }
            
            else if(indexmap[mid+1] >= 0)
            {
                answer1.push_back(mid+1);
                answer1.push_back(indexmap[mid+1]);
            }
            
            else
            {
                answer1.push_back(mid);
                answer1.push_back(indexmap[mid]);
            }
        }
    }
    int goodlength(int l, vector<vector<unsigned long long> >& s_table)
    {
        if (l == 0)
        {
            return -1;
        }
        unordered_map<unsigned long long, int> count;
        caltmap(l, s_table, count);
        /*
        //-------------------------------
        
        cout<<endl;
        cout<<"row "<<l<<endl;
        for (int i = 0; i < s_table[l].size(); ++i)
        {
            cout<<s_table[l][i]<<" ";
        }
        cout<<endl;
        for (it = count.begin(); it != count.end(); ++it)
        {
            cout<<"( "<<it->first<<" "<<it->second<<" )"<<endl;
        } */
        //-------------------------------
        
        int temp = -1;
        for (it = count.begin(); it != count.end(); ++it)
        {
            if (it->second >= m)
            {
                for (int i = 0; i < s_table[l].size(); ++i)
                {
                    if ((it->first == s_table[l][i]) && (i > temp))
                    {
                        temp = i;
                    }
                }
            }
        }
        //cout<<"temp = "<<temp<<endl;
        return temp;
    }
    void caltmap(int l, vector<vector<unsigned long long> >& s_table, unordered_map<unsigned long long, int>& count)
    {
        for (unsigned int i = 0; i <= s.size()-l; ++i)
        {
            it = count.find(s_table[l][i]);
            if (it != count.end()) //有找到
            {
                int temp = it->second;
                temp++;
                count[s_table[l][i]] = temp;
            }
            else
            {
                count.insert(pair<unsigned long long, int>(s_table[l][i],1));
            }
        }
    }
    void problem2(vector<string>& answer2, vector<vector<unsigned long long> >& s_table)
    {
        vector<int> length;
        for (unsigned int i = 1; i < s.size(); ++i)
        {
            if (s_table[i][0] == s_table[i].back())
            {
                length.push_back(i);
            }
        }
        /*
        cout<<"length: "<<endl;
        for (int i = 0; i < length.size(); ++i)
        {
            cout<<length[i]<<" ";
        }
        cout<<endl;
        */
        if (length.size() == 1)
        {
            if (checklength(length[0], s_table))
            {
                answer2.push_back(s.substr(0,length[0]));
                length.clear();
            }
            else
            {
                answer2.push_back("Just a legend");
            }
            
        }
        else if (length.size() == 0)
        {
            answer2.push_back("Just a legend");
        }
        else
        {
            int left = 0;
            int right = length.size();
            int mid = (left + right) / 2;
            
            while(left <= right)
            {
                //cout<<mid<<endl;
                if (checklength(length[mid], s_table))
                {
                    
                    if (mid == length.size()-1)
                    {
                        break;
                    }

                    left = mid + 1;
                }
                else
                {
                    right = mid - 1;
                }
                mid = (left + right)/2;
            }

            
            if (checklength(length[mid], s_table))
            {
                answer2.push_back(s.substr(0,length[mid]));
            }
            else
            {
                answer2.push_back("Just a legend");
            }
        }
    }
    /*
    void caltable2(int l)
    {
        for (unsigned int i = 0; i <= s.size()-l; ++i)
        {
            s_table.push_back(s_hash[i+l] - s_hash[i]*pow[l]);
        }
    }
    */
    bool checklength(int l, vector<vector<unsigned long long> >& s_table)
    {
        for (unsigned int i = 1; i < s_table[l].size()-1; ++i)
        {
            if (s_table[l][i] == s_table[l][0])
            {
                return 1;
                break;
            }
        }
        return 0;
    }
};

int main()
{
    vector<int> answer1;
    vector<string> answer2;
    while(1)
    {
        int m;
        cin>>m;
        if (m == 0) break;
        hw4 problem(m, answer1, answer2);
    }
    for (unsigned int i = 0; i < answer1.size(); i = i+2)
    {
        if (answer1[i] != 0)
        {
            cout<<answer1[i]<<" "<<answer1[i+1]<<endl;
        }
        else
        {
            cout<<"none"<<endl;
        }
        cout<<answer2[i/2]<<endl;
    }
    return 0;
}