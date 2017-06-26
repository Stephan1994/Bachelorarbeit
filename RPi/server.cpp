#include <iostream>
#include "Netzwerk/ProzessPi.h"

using std::cout;
using std::endl;

using namespace std;
int main() {
	char recvdValue[10000];
	while(true)
	{
		EmpfangeRobotKommando(recvdValue);
		cout << "Value received: " << recvdValue << endl;
	}
	
    return 0;
}
