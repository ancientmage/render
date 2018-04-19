#include <iostream>
#include <string>

#include "client_sock.h"

using namespace std;

int main(int _argc, char * _argv[]) {
    Client client("127.0.0.1", 8080);
    string s = client.read();
    client.write("hello from client\n");
    cout << "take from server : " << s << endl;
    getchar();
}