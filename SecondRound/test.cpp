#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <unordered_map>

using namespace std;

const int count = 32;
int myarray[10];

int main()
{
    unordered_map<int, int> hash;
    hash[1] = 10;
    hash[2] = 20;
    
    system("pause");
    int ans[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int *ptr[1] = {*ans};
    int x = **ans;
    cout << x << endl;
    cout << *(ptr[0] + 1) << endl;
    ++*(ptr[0] + 1);
    cout << *(ptr[0] + 3) << endl;
    return 0;
}