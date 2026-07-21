#pragma once
using namespace std;
#include <vector>
#include <iostream>
#include "../dynamics/RigidBody.h"

struct PhysicsWorld{
    // flatten every body position into a single 1D array of floats laid out as [x0, y0, z0, x1, y1, z1, ...] — 3 values per body
    // Used to pass position data across the wasm-js boundary each frame since embind handles flat vector<float> far more easily/efficiently

    int getBodyCount() {
    return bodies.size();
    }
    vector<float> getPositions()
    {
        vector<float>result;
        for(size_t a=0;a<bodies.size();a++)
        {
            result.push_back(bodies[a].position.x);
            result.push_back(bodies[a].position.y);
            result.push_back(bodies[a].position.z);
        }
        return result;
    }
    float gravity = -9.8f;
    vector<RigidBody> bodies; //this is a vector of rigidbody type which stores all the rigidbodies
    
    void addBody(RigidBody body){   
        bodies.emplace_back(body);
        cout<<"RigidBody of mass "<<body.mass<<" was added";
    }
    void setGravity(float g){
        gravity = g;
    }
    void step(float dt){
        for(RigidBody &body: bodies)
        {
            body.applyGravity(gravity);
            body.integrate(dt);
            //resetting acceleration else gravity will stack
            body.acceleration=Vector3(0,0,0);
        }
        for(size_t i=0;i<bodies.size();i++)
        {//traversing through all unique pairs of bodies to check for collision
            for(size_t j=i+1;j<bodies.size();j++)
            {
                if(bodies[i].checkCollision(bodies[j]))
                {
                    bodies[i].resolveCollision(bodies[j]);
                }
            }
        }
        //floor collision tackle (center - radius)
        //has dropped below the floor. if so, snap it back to rest exactly on floor
        //reverse the velocity and scale it w.r.t restitution
        float floorY=-8.0f;
        for (RigidBody &body: bodies)
        {
            if(body.position.y-body.radius<floorY){
                body.position.y=floorY+body.radius;
                body.velocity.y= -1*body.velocity.y * body.restitution;
            }
        }
        float boundary=50.0f;
        for (RigidBody &body: bodies)
        {
            if(body.position.x-body.radius < -boundary){
                body.position.x=-boundary+body.radius;
                body.velocity.x=-body.velocity.x * body.restitution;
            }
            if(body.position.x+body.radius>boundary){
                body.position.x=boundary-body.radius;
                body.velocity.x=-body.velocity.x*body.restitution;
            }
            if(body.position.z+body.radius>boundary){
                body.velocity.z=-body.velocity.z*body.restitution;
                body.position.z=boundary-body.radius;
            }
            if(body.position.z-body.radius<-boundary)
            {
                body.position.z=-boundary+body.radius;
                body.velocity.z=-body.velocity.z*body.restitution;
            }
        }
    }
};

