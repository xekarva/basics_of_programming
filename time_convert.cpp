#include <iostream>

using namespace std;

void convert (int time, int* min, int* sec){
    *min = time/60;
    *sec = time%60;
}

int main()
{
    int total_time;
    int min=0;
    int sec=0;
    cout<<"Enter time in seconds ";
    cin>>total_time;
    convert(total_time, &min, &sec);
    cout<<total_time<<"sec = "<<min<<"min = "<<sec<<"sec = ";

    return 0;
}
