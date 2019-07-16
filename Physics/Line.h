#pragma once
#include "Rigidbody.h"
#include <glm/vec4.hpp>

class Line : public PhysicsObject {
public:
	Line(glm::vec2 normal, float distToOrigin, glm::vec4 colour);
	~Line();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) {};
	virtual void MakeGizmo();

	void ResetPosition();
	glm::vec2 GetNormal() { return m_normal; }
	float GetDistToOrigin() { return m_distToOrigin; }

	void ResolvedCollision(Rigidbody* actor, glm::vec2 contact);

protected:
	glm::vec2 m_normal;
	float m_distToOrigin;
	glm::vec4 m_colour;
};
