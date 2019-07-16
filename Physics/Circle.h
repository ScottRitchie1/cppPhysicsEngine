#pragma once

#include "Rigidbody.h"

#include <glm/vec4.hpp>

class Circle : public Rigidbody {
public:
	Circle(glm::vec2 position, glm::vec2 velocity, float mass, float elasticity, float radius, glm::vec4 colour, bool isKinematic = false);
	Circle(glm::vec2 position, float angle, float speed, float mass, float elasticity, float radius, glm::vec4 colour, bool isKinematic = false);
	~Circle();

	virtual void MakeGizmo();

	float GetRadius() { return m_radius; }
	glm::vec4 GetColour() { return m_colour; }

protected:
	float m_radius;
	glm::vec4 m_colour;
};