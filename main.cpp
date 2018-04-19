#include <iostream>
#include <string>

#include "client_sock.h"

using namespace std;

int main(/*int _argc, char * _argv[]*/) {
    Client client("tr.contest.pizza", 10000);
    string s;

    s = client.read();
    cout << "take from server : " << s;
    client.write("team42");

    s = client.read();
    cout << "take from server : " << s;
    client.write("9e1b5878f4");

    s = client.read();
    cout << "take from server : " << s;

    return 0;
}
