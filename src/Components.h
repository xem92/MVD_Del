//
//  Copyright � 2018 Alun Evans. All rights reserved.
//
//  This file contains the definitions of an entity, and all the different types of component
//  At the end is a struct called the EntityComponentManager (ECM) which contains an std::vector for
//  each of the different component types, stored in an std::tuple. The advantage of this system is that
//  if a system wishes to interact/use/update all components of a certain type (e.g. draw all meshes),
//  then these components are stored in contiguous memory, which the various levels of caching can use
//  to improve performance
//
//    TO ADD A NEW COMPONENT TYPE:
//    - define it as a sub-class of Component
//    - add it to the ComponentArrays tuple
//    - add it as a subtemplate of typetoint() and increment 'result' variable
//    - increment NUM_TYPE_COMPONENTS
//
#pragma once
#include "includes.h"
#include <vector>

/**** COMPONENTS ****/

//Component (base class)
// - owner: id of Entity which owns the instance of the component
struct Component {
    int owner;
};

// Transform Component
// - inherits a mat4 which represents a model matrix
// - all_transform - reference to vector of all transforms
struct Transform : public Component, public lm::mat4 {
    int parent = -1;
    lm::mat4 getGlobalMatrix(std::vector<Transform>& transforms) {
        if (parent != - 1){
            return transforms.at(parent).getGlobalMatrix(transforms) * *this;
        }
        else return *this;
    }
};

// Mesh Component
// - geometry - name of geometry resource
// - material - name of material resource
struct Mesh : public Component {
    int geometry;
    int material;
};


//Camera component
// - position, forward and up vectors for rapid access
// - view and projection matrices for camera
// - is_main bool so that ECM can quickly find main camera
struct Camera : public Component {
	lm::vec3 position;
	lm::vec3 forward;
	lm::vec3 up;
	lm::mat4 view_matrix;
	lm::mat4 projection_matrix;
	lm::mat4 view_projection;

	//constructor that sets placeholder matrices
	Camera() {
		position = lm::vec3(0.0f, 0.0f, 1.0f); forward = lm::vec3(0.0f, 0.0f, -1.0f); up = lm::vec3(0.0f, 1.0f, 0.0f);
		lm::vec3 target = position + forward;
		view_matrix.lookAt(position, target, up);
		projection_matrix.perspective(60.0f*DEG2RAD, 1, 0.01f, 100.0f);
	}

	//sets view and projection matrices based on current position, view direction and up vector
	void updateViewMatrix() {
		forward.normalize();
		lm::vec3 target = position + forward;
		view_matrix.lookAt(position, target, up);
	}

	//wraps orthographic projection matrix
	void setOrthographic(float left, float right, float bottom, float top, float near, float far) {
		projection_matrix.orthographic(left, right, bottom, top, near, far);
	}

	//wraps perspective projection matrix
	void setPerspective(float fov_rad, float aspect, float near, float far) {
		projection_matrix.perspective(fov_rad, aspect, near, far);
	}

	void update() {
		updateViewMatrix();
		view_projection = projection_matrix * view_matrix;
	}
};

//Light Component
// - id of transform in ECS array
// - color of light
//Later will be developed extensively
struct Light : public Component {
    lm::vec3 color;
};

enum ColliderType {
    ColliderTypeBox,
    ColliderTypeRay
};

//ColliderComponent. Only specifies size - collider location is given by any
//associated TransformComponent
// - collider_type is the type according to enum above
// - local_center is offset from transform
// - local_halfwidth is used for box,
// - direction is used for ray
// - max_distance is used to convert ray to segment
struct Collider: public Component {
    ColliderType collider_type;
    lm::vec3 local_center; //offset from transform component
    lm::vec3 local_halfwidth; // for box
    lm::vec3 direction; // for ray
    float max_distance; // for segment
    
    //collision state
    bool colliding;
    int other;
    lm::vec3 collision_point;
    float collision_distance;
    
    Collider() {
        local_halfwidth = lm::vec3(0.5, 0.5, 0.5); //default dimensions = 1 in each axis
        max_distance = 10000000.0f; //infinite ray by default
        colliding = false; // not colliding
        other = -1; //no other collider
    }
    
};

// Types of AI
enum AIType {
	simpleAI
};

//AIComponent.
// ai_type is the type of the AI
// patrol_points all the target points were the ai moves
// patrol_point actual target point of the ai
// speed translation speed of the ai
// state is the actual state of the ai
// rest_time time that the ai rest once arrive to its target
// detection_collider_id the id of the collider the ai use to detect the player
struct AI : public Component {
	AIType ai_type;
	std::vector <lm::vec3> patrol_points;
	int patrol_point;
	float speed;
	int state;
	float rest_time;
	std::string detection_collider_id;
};

/**** COMPONENT STORAGE ****/

//add new component type vectors here to store them in *ECS*
typedef std::tuple<
std::vector<Transform>,
std::vector<Mesh>,
std::vector<Camera>,
std::vector<Light>,
std::vector<Collider>,
std::vector<AI>
> ComponentArrays;

//way of mapping different types to an integer value i.e.
//the index within ComponentArrays
template< typename T >
struct type2int{};
template<> struct type2int<Transform> { enum { result = 0 }; };
template<> struct type2int<Mesh> { enum { result = 1 }; };
template<> struct type2int<Camera> { enum { result = 2 }; };
template<> struct type2int<Light> { enum { result = 3 }; };
template<> struct type2int<Collider> { enum { result = 4 }; };
template<> struct type2int<AI> { enum { result = 5 }; };
//UPDATE THIS!
const int NUM_TYPE_COMPONENTS = 6;

/**** ENTITY ****/

struct Entity {
    //name is used to store entity
    std::string name;
    //array of handles into ECM component arrays
    int components[NUM_TYPE_COMPONENTS];
    //sets active or not
    bool active = true;
    
    Entity() {
        for (int i = 0; i < NUM_TYPE_COMPONENTS; i++) { components[i] = -1;}
    }
    Entity(std::string a_name) : name(a_name) {
        for (int i = 0; i < NUM_TYPE_COMPONENTS; i++) { components[i] = -1;}
    }
};

