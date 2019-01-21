#include "AISystem.h"
#include "extern.h"

//set initial state of AI system
void AISystem::init() {
}

//called once per frame
void AISystem::update(float dt) {
	auto& ais = ECS.getAllComponents<AI>();
	for (size_t i = 0; i < ais.size(); i++) {
		// Prepare all the variables
		AI& ai = ais[i];
		int owner = ai.owner;
		Transform& transform = ECS.getComponentFromEntity<Transform>(owner);
		Transform aux_transform_a = ECS.getComponentFromEntity<Transform>(owner);
		Transform aux_transform_b = ECS.getComponentFromEntity<Transform>(owner);
		Transform& transform_player = ECS.getComponentFromEntity<Transform>("PlayerFPS");
		float move_speed_dt = ai.speed * dt;
		lm::vec3 forward_dir;
		lm::vec3 forward_dir_to_go;
		float angle;
		float angle_a;
		float angle_b;
		// Detect if the player is over the patrolling area
		if (ECS.getComponentFromEntity<Collider>(ai.detection_collider_id).colliding && ai.state != 2) {
			ai.state = 2;
		} 
		// State machine
		switch (ai.state) {
		// Patrol from point a to point b
		case 0:
			// Calculate forward vector of were the ai wants to go
			forward_dir_to_go = lm::vec3(ai.patrol_points[ai.patrol_point].x - transform.position().x, 0, ai.patrol_points[ai.patrol_point].z - transform.position().z).normalize();
			angle = calculateAngle(transform.front(), forward_dir_to_go); 
			std::cout << angle << std::endl;

			// Calculate the direction of the rotation
			if (angle >= -1 && angle <= 1) {
				angle = acos(angle);
				aux_transform_a.rotateLocal(angle * 0.001, lm::vec3(0, 1, 0));
				angle_a = calculateAngle(aux_transform_a.front(), forward_dir_to_go);
				aux_transform_b.rotateLocal(angle * -0.001, lm::vec3(0, 1, 0));
				angle_b = calculateAngle(aux_transform_b.front(), forward_dir_to_go);
				if (angle_a < angle_b) {
					transform.rotateLocal(angle * 0.001, lm::vec3(0, 1, 0));
				}
				else {
					transform.rotateLocal(angle * -0.001, lm::vec3(0, 1, 0));
				}
			}

			// If it's over point b -> change state to rest and change target
			// Else -> move forward
			if (transform.position().x < ai.patrol_points[ai.patrol_point].x + 0.5 && transform.position().x > ai.patrol_points[ai.patrol_point].x - 0.5 &&
				transform.position().y < ai.patrol_points[ai.patrol_point].y + 0.5 && transform.position().y > ai.patrol_points[ai.patrol_point].y - 0.5 &&
				transform.position().z < ai.patrol_points[ai.patrol_point].z + 0.5 && transform.position().z > ai.patrol_points[ai.patrol_point].z - 0.5) {
				ai.patrol_point++;
				int size = ai.patrol_points.size();
				if (ai.patrol_point >= size) {
					ai.patrol_point = 0;
				}
				ai.rest_time = 0;
				ai.state = 1;
			}
			else {
				// Move forward
				forward_dir = transform.front() * move_speed_dt;
				transform.translate(forward_dir);
			}
			break;
		// Once arrive to point b -> rest
		case 1:
			ai.rest_time += dt * 10;
			if (ai.rest_time > 10) {
				ai.rest_time = 0;
				ai.state = 0;
			}
			break;
		// Follow player
		case 2:
			// If player out of the patrolling area -> go back to patrol
			// Else -> follow player
			if (!ECS.getComponentFromEntity<Collider>(ai.detection_collider_id).colliding) {
				ai.rest_time = 0;
				ai.state = 0;
			}
			else {
				forward_dir_to_go = lm::vec3(transform_player.position().x - transform.position().x, 0, transform_player.position().z - transform.position().z);
				angle = calculateAngle(transform.front(), forward_dir_to_go);
				std::cout << angle << std::endl;

				// Calculate the direction of the rotation
				if (angle >= -1 && angle <= 1) {
					angle = acos(angle);
					aux_transform_a.rotateLocal(angle * 0.001, lm::vec3(0, 1, 0));
					angle_a = calculateAngle(aux_transform_a.front(), forward_dir_to_go);
					aux_transform_b.rotateLocal(angle * -0.001, lm::vec3(0, 1, 0));
					angle_b = calculateAngle(aux_transform_b.front(), forward_dir_to_go);
					if (angle_a < angle_b) {
						transform.rotateLocal(angle * -0.001, lm::vec3(0, 1, 0));
					}
					else {
						transform.rotateLocal(angle * 0.001, lm::vec3(0, 1, 0));
					}
				}

				// Move forward
				forward_dir = transform.front() * move_speed_dt;
				transform.translate(forward_dir);
			}
			break;
		default:
			break;
		}
	}
}

float AISystem::calculateAngle(lm::vec3 u, lm::vec3 v) {
	float angle;
	float dot_product = u.dot(v);
	float u_length = u.length();
	float v_length = v.length();
	angle = dot_product / (u_length * v_length);
	//angle = acos(angle);
	return angle;
}