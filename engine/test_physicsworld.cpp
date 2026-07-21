#include<iostream>
using namespace std;
#include "world/PhysicsWorld.h"

int main()
{
    PhysicsWorld world;
    world.addBody(RigidBody(Vector3(0,7,4), Vector3(1,3,-6), 1.5f, 0.8f, 0.4f));
    world.addBody(RigidBody(Vector3(1,3,4), Vector3(2,6,-4), 2.5f, 0.8f, 0.9f));
    world.addBody(RigidBody(Vector3(0,-3,-6), Vector3(2,4,-2), 0.9f, 1.0f, 0.2f));

    float dt=0.1f;

    for(int i=0;i<50; i++)
    {
        world.step(dt);

        cout<<"t= "<<i*dt<<" ";
        for(size_t a=0; a<world.bodies.size(); a++)
        {
            cout<<"Body "<<a<<"y = " << world.bodies[a].position.y<<" " <<"x = " << world.bodies[a].position.x << ") ";
        }
        cout<<endl;
    }
    return 0;
}