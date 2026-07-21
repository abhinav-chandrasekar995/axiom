#include<emscripten/bind.h> //to expose c++ classes/funcs to javaScript
#include "../world/PhysicsWorld.h"
using namespace emscripten;

//macro block for listing all class/functions internal label for this block — not visible to JS, can be any name, compiled to WebAssembly
EMSCRIPTEN_BINDINGS(physics_module){ 
    //embind syntax under emscripten
    class_<Vector3>("Vector3")
    .constructor<float,float,float>()
    .property("x", &Vector3::x)
    .property("y", &Vector3::y) //pointer to member
    .property("z", &Vector3::z);

    class_<Particle>("Particle")
    .constructor<Vector3,Vector3,float>()
    .property("position", &Particle::position)
    .property("velocity", &Particle::velocity)
    .property("mass", &Particle::mass);

    class_<RigidBody, base<Particle>>("RigidBody")
    .constructor<Vector3, Vector3, float, float, float>()
    .property("radius", &RigidBody::radius)
    .property("restitution", &RigidBody::restitution);

    register_vector<float>("FloatVector"); //tell embind how to convert vector<float> between c++ and js
    class_<PhysicsWorld>("PhysicsWorld")
    .constructor<>()
    .function("getPositions", &PhysicsWorld::getPositions)//mapping class to its constituent member fxn
    .function("addBody", &PhysicsWorld::addBody)
    .function("step", &PhysicsWorld::step)
    .function("setGravity", &PhysicsWorld::setGravity) // added this to the bindings
    .function("getBodyCount", &PhysicsWorld::getBodyCount);
}