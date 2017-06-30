#include <iostream>
#include "Robot.h"

using namespace std;
int main() {
    std::cout << "Hello, World!" << std::endl;

    Robot rob;
    rob.connect("192.168.1.1");
    cout << "connected" << endl;
	rob.forward(100);
	cout << "forwarded" << endl;
    rob.closeConnection();
    return 0;
}
