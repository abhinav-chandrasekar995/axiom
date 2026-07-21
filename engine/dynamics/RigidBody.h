#pragma once
#include "Particle.h"
#include <cmath>
using namespace std;
#include<algorithm>

struct RigidBody : public Particle
{
    float radius; //handling only spherical objects
    float restitution; //bounciness 0-absorbs all energy and 1-perfectly elastic

    RigidBody(Vector3 position=Vector3(0,0,0), Vector3 velocity=Vector3(0,0,0), float mass=1.0f, float radius=1.0f, float restitution=0.5):
    Particle(position,velocity,mass), radius(radius), restitution(restitution) {}

    bool checkCollision(const RigidBody& other) const{
        Vector3 position_vector_difference = Vector3(position-other.position);
        float absolute_positon_distance = position_vector_difference.lengthSquared();
        float radius_distance = radius + other.radius;
       return absolute_positon_distance <= (radius_distance * radius_distance);
    }

    void resolveCollision(RigidBody& other){
        Vector3 normal = (other.position-position).normalize();
        Vector3 relative_velocity = other.velocity-velocity; //this->other hence other-this
        float speed = relative_velocity.dot(normal);
        if (speed>0) return; //objects are separating

        float e = min(restitution,other.restitution);
        float j = -(1+e)*speed/(1/mass + 1/other.mass); //instantaneous change
        Vector3 impulse = normal * j;

        velocity=velocity-impulse*(1/mass);
        other.velocity=other.velocity+impulse*(1/other.mass);

        float distance_between_centres=(other.position-position).length();
        float depth_of_contact=(radius+other.radius)-distance_between_centres;

        Vector3 correction = normal*(depth_of_contact/2);
        position = position - correction;
        other.position = other.position + correction;
    }
};