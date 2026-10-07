#include <iostream>

using namespace std;

int tru(int a, int b){
    return a*b;}

int main()
{
    int a,b;
    cout << "Enter two char" << endl;
    cin>>a>>b;
    cout<<tru(a,b);
    return 0;
}
