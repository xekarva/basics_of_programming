#include <iostream>
#include "5_3.h"

using namespace std;


int main()
{
    int a,b;
    cout << "Enter two numbers" << endl;
    cin>>a>>b;
    cout<<tru(a,b);
    cout<<"\nAnd now let make substraction: "<<endl;
    substract(a,b);
    return 0;
}
