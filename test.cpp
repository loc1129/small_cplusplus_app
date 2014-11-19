//LocCase 1: email regex checking
#include <iostream>
#include <regex>
#include <string>

using namespace std;
using namespace std::tr1;
using std::tr1::regex;
using std::tr1::smatch;
int main()
{
        std::string email = "loc@153.com";
        regex pattern("([0-9A-Za-z\\-_\\.]+)@([0-9a-z]+\\.[a-z]{2,4}(\\.[a-z]{2,4})?)");
        if(regex_match(email, pattern))
        {
                cout<<"email right"<<endl;
        }
        else
        {
                cout<<"email wrong"<<endl;
        }
        return 0;
}

