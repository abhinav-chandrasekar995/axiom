#include "../math/Vector3.h"
#pragma once

struct Particle{
        Vector3 position;
        Vector3 velocity;
        Vector3 acceleration;
        float mass;

        Particle(Vector3 position=Vector3(0,0,0), Vector3 velocity=Vector3(0,0,0), float mass=1.0f):
        position(position),velocity(velocity),acceleration(0,0,0),mass(mass) {}

        void integrate(float dt) //float dt is independent and is passed to show object change with small time slices. using void since it is a simulation/modification and isn't the final result
        {
            velocity = velocity + acceleration*dt;
            position = position + velocity*dt;  
        }

        void applyGravity(float g = -9.8f) //downward-pull
        {
            acceleration = acceleration + Vector3(0,g,0);
        }
};