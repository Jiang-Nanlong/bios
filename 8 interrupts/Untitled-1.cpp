#include <iostream>
#include <vector>
using namespace std;

bool findSeven(vector<int> &nums)
{
    for (int num : nums)
    {
        if (num == 7)
            return true;
    }
    return false;
}

int main()
{

    int nArray[] = {0, 1, 2, 3, 4, 5};
    vector<int> vecNum(nArray, nArray + 6);

    cout << findSeven(vecNum) << endl;
    return 0;
}
