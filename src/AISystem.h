#pragma once
#include "includes.h"
#include "Components.h"
#include <map>

class AISystem {
public:
	void init();
	void update(float dt);

private:

	float calculateAngle(lm::vec3 u, lm::vec3 v);
};