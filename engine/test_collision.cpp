#include<iostream>
using namespace std;

#include "dynamics/RigidBody.h"

int main()
{
    RigidBody a(Vector3(7,1,1), Vector3(-3,0,0), 2.0f, 1.0f, 0.4f); //position velocity mass radius and restitution
    RigidBody b(Vector3(-7,1,1), Vector3(3,0,0), 1.8f, 0.9f, 0.2f);

    float dt=0.1f; //time slice

    for(int i=0;i<40;i++)
    {
        a.integrate(dt); //using equations of motion
        b.integrate(dt);

        if(a.checkCollision(b)) {
            cout<<"Collision at time= "<<i*dt<<endl;
            a.resolveCollision(b);
        }//collision check happens

        cout<<"t= "<<i*dt
             << "a.x=" << a.position.x << " "
             << "b.x=" << b.position.x << " "
             << "a.vx=" << a.velocity.x << " "
             << "b.vx=" << b.velocity.x << " "
             <<endl; //checking only x-coordinate
    }
    return 0;
}