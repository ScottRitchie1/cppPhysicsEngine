#pragma once
#include "PhysicsObject.h"
#include <functional>

class Rigidbody : public PhysicsObject {
protected:
	glm::vec2 m_position;
	float m_rotation;

	glm::vec2 m_velocity;
	float m_angularVelocity;

	float m_mass;
	float m_moment;
	float m_elasticity;

	bool m_isKinematic = false;
	
	float m_linearDrag;
	float m_angularDrag;
public:
	Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float elasticity, bool isKinematic = false);
	Rigidbody(ShapeType shapeID, glm::vec2 position, float angle, float speed, float rotation, float mass, float elasticity, bool isKinematic = false);
	~Rigidbody();

	std::function<void(Rigidbody*, Rigidbody*, const glm::vec2&, const glm::vec2&) > OnCollision = nullptr;
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	void CalculateKinematic(glm::vec2 gravity, float timeStep, float time);
	void ApplyForce(glm::vec2 force, glm::vec2 pos);

	virtual void ResolvedCollision(Rigidbody* actor, glm::vec2 contact, glm::vec2* collisionNormal=nullptr);

	bool isKinematic() { return m_isKinematic; };

	void SetKinematic(bool state) { m_isKinematic = state; SetMass(m_mass);};

	void SetPosition(glm::vec2 pos) { m_position = pos; };
	void MovePosition(glm::vec2 pos) { if(m_isKinematic == false) m_position = pos; };
	void SetRotation(float rot) { m_rotation = rot; };
	void SetVelocity(glm::vec2 vel) { m_velocity = vel; };
	void SetVelocityAngular(float vel) { m_angularVelocity = vel; }

	void SetMass(float mass) { m_mass = (m_isKinematic ? 99999 : mass); };
	void SetDrag(float linear, float angular) { m_linearDrag = linear; m_angularDrag = angular; };
	void SetElasticity(float elasticity) { m_elasticity = elasticity; };

	glm::vec2 GetPosition() const { return m_position; }
	float GetRotation() { return m_rotation; }
	glm::vec2 GetVelocity() { return m_velocity; }
	float GetVelocityAngular() { return m_angularVelocity; }
	float GetMass() { return m_mass; }
	float GetDragLinear() { return m_linearDrag; }
	float GetDragAngular() { return m_angularDrag; }
	float GetElasticity() { return m_elasticity; }
	float GetMomentum() { return m_moment; }
	float GetKineticEnergy();


	
};