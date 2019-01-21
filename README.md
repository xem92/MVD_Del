### La Salle - Universitat Ramon Llull : Master in Advanced Videogame Development
## Engine Programming - Delivery

## AI System
Simple AI system. It's a state machine with 3 states: Patrolling, Resting, Following.
The AI move from one patrolling point to another and resting when it arrives to that point. If the player collides with a selected collider, the AI, starts to follow the player.

## AI Component
New AI component created:
AIType ai_type: is the type of the AI. (at this moment there only one type "simpleAI")
vecto of vec3 patrol_points: all the target points where the ai moves when is patrolling.
int patrol_point: actual target point of the ai.
float speed: translation speed of the ai.
int state: is the actual state of the ai. (patrollin, resting, following)
float rest_time: rest time of the ai once arrive to its target.
string detection_collider_id: the id of the collider the ai use to detect the player.
