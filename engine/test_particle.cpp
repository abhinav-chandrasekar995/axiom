#include <iostream>
using namespace std;

#include "dynamics/Particle.h"

int main() {
    Particle p(Vector3(0,10,0)); //first and only one, matches to position
    float dt=0.1f;
    for(int i=0; i<20; i++)
    {
        p.applyGravity();
        p.integrate(dt);
        p.acceleration=Vector3(0,0,0); //acc=0 for each frame

        cout<<"t="<<i*dt<<" "
            <<"y="<<p.position.y<<" "
            <<"vy="<<p.velocity.y<<" "
            <<endl;
    }
    return 0;
}
