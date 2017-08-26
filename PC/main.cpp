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

    //rob.getPicture(0);
    /*rob.startVideo(320, 176);
    int i = 0;
    valarray<valarray<valarray<int>>> pic;
    while(i < 10){
        if (!rob.VideoBuffer.empty()){
            pic = *rob.VideoBuffer.begin();
            rob.VideoBuffer.pop_front();
            i++;
            cout << "Picture received" << endl;
        }
    }
    cout << "After video" << endl;
    rob.stopVideo();*/
    //valarray<valarray<valarray<int>>> pic;
    //pic = rob.getPicture();
    //cout << pic << endl;
    rob.closeConnection();
    return 0;
}
