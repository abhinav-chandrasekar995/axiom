#pragma once 
//run this file only once for no overlaps
#include <cmath>
struct Vector3
{
    float x,y,z; //three elements x y and z co-ordinates
    Vector3(float x=0, float y=0, float z=0): //take 0 if no input
    x(x),y(y),z(z){} //x->x y->y z->z

    Vector3 operator+(const Vector3& other) const{ //Vector3 return type
        return Vector3(x+other.x, y+other.y, z+other.z); 
    }

    Vector3 operator-(const Vector3& other) const{
        return Vector3(x-other.x, y-other.y, z-other.z);
    }

    Vector3 operator*(const float scalar) const{ //const to handle consistency
        return Vector3(x*scalar, y*scalar, z*scalar);
    }

    float length() const{
        return pow((x*x + y*y + z*z), 0.5);
    }

    float lengthSquared() const{
        return (x*x + y*y + z*z);
    }

    Vector3 normalize() const{ //unit vector calculation and handling null vector as an edge-case
        float len=length();
        if (length() == 0) return Vector3(0,0,0);
        else{
            return Vector3(x/len,y/len,z/len);
        }
    }

    float dot(const Vector3& other) const{
        return(x*other.x+ y*other.y+ z*other.z);
    }

    Vector3 cross(const Vector3& other) const{ //relevance pertaining to rotational mechanics
        return Vector3(y*other.z-z*other.y, -(x*other.z-z*other.x), x*other.y-y*other.x);
    }

    Vector3 operator/(const float scalar) const{
        if(scalar==0) return Vector3(0,0,0); //handling zero-edge case
        return Vector3(x/scalar, y/scalar, z/scalar);
    }
};