#include <iostream>
#include <string>
using namespace std;

int main()
{
    string x;
    cout<<"Enter your password ";
    cin>>x;
    cout<<"Hello, ";
    for(int i = x.length() - 1; i >= 0; i--)
    {
        cout<<x[i];
    }
    cout<<endl;
    return 0;
}
