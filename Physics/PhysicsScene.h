#pragma once
#include <glm/vec2.hpp>
#include <vector>

class PhysicsObject;

class PhysicsScene{
protected:

	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;

public:
	PhysicsScene();
	~PhysicsScene();

	void AddActor(PhysicsObject* actor);
	void RemovedActor(PhysicsObject* actor);
	void Update(float deltatime);
	void UpdateGizmos();

	void SetGravity(const glm::vec2 gravity) { m_gravity = gravity; };
	glm::vec2 GetGravity() const { return m_gravity; };

	void SetTimeStep(const float timeStep) { m_timeStep = timeStep; };
	float GetTimeStep() const { return m_timeStep; };

	void CheckForCollisions();

	static bool Line2Line(PhysicsObject*, PhysicsObject*);
	static bool Line2Circle(PhysicsObject*, PhysicsObject*);
	static bool Line2Rect(PhysicsObject*, PhysicsObject*);
	static bool Circle2Line(PhysicsObject*, PhysicsObject*);
	static bool Circle2Circle(PhysicsObject*, PhysicsObject*);
	static bool Circle2Rect(PhysicsObject*, PhysicsObject*);
	static bool Rect2Line(PhysicsObject*, PhysicsObject*);
	static bool Rect2Circle(PhysicsObject*, PhysicsObject*);
	static bool Rect2Rect(PhysicsObject*, PhysicsObject*);



};
