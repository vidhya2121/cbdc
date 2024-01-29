#include <iostream>
using namespace std;
#include <string>

int main(){
    string to_id = "w+m";
    string w = to_id.substr(0, to_id.find("+"));
    string m = to_id.substr(2, to_id.find("+"));
    cout<<w<<" "<<m<<'\n';
    return 0;
}