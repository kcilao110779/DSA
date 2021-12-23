#include <algorithm>
#include <deque>
#include <cstdio>
//#include <iostream>
//
//using namespace std;

unsigned int binarySearch(std::deque<unsigned int>&win, unsigned int item, int low, int high) //回傳新元素item該插入之位置
{
    if (high <= low)
        return (item > win[low]) ? (low + 1) : low;

    int mid = (low + high) / 2;

    if (item == win[mid])
        return mid;

    if (item > win[mid])
        return binarySearch(win, item, mid + 1, high);
    return binarySearch(win, item, low, mid - 1);
}
int main()
{
    unsigned int counts, win_size, k;
    scanf("%d%d%d", &counts, &win_size, &k);
    std::deque<unsigned int> input(counts, 0);
    std::deque<unsigned int> win(win_size, 0);
    unsigned int num, i, round;
    for (i = 0; i < counts; i++)
    {
        scanf("%d", &input[i]);
        if (i < win_size)
            win[i] = input[i];
    }
    // for (i = 0; i < counts; i++)
    // 	cout << &win+i << " " << *(win+i)<< endl;

    std::sort(win.begin(), win.begin() + win_size);

    for (round = 0; round < counts - win_size + 1; round++)
    {
        printf("%d\n", win[k - 1]);                                                                                         //輸出第k小的數字
        win.erase(win.begin() + binarySearch(win, input[round], 0, win_size - 1));                                          //刪除最老的元素
        win.insert(win.begin() + binarySearch(win, input[round + win_size], 0, win_size - 1 - 1), input[round + win_size]); //插入下一個元素，因為此時deque只剩下k-1個元素，所以-1再-1
    }
}
