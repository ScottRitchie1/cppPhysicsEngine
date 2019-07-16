#pragma once
#include "Rigidbody.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <Gizmos.h>
enum Side
{
	TOP,
	BOT,
	LEFT,
	RIGHT,
	NONE
};
class Rect : public Rigidbody {
public:
	Rect(glm::vec2 position, float rotation, glm::vec2 velocity, glm::vec2 size, float mass, float elasticity, glm::vec4 colour, bool isKinematic = false);
	~Rect() {};

	virtual void MakeGizmo();
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);

	glm::vec2 GetExtents() const { return m_size / 2.f; };
	float GetWidth() const { return m_size.x; }
	float GetHeight() const { return m_size.y; }
	glm::vec4 GetColour()  const { return m_colour; }
	void SetColour(glm::vec4 colour) { m_colour = colour; }

	glm::vec2 GetLocalX() { return m_localX; };
	glm::vec2 GetLocalY() { return m_localY; };

	Side FindClosestSide(glm::vec2 point) const;
	bool CheckRectCorners(const Rect& rect, glm::vec2& contact, int& numContacts, glm::vec2& edgeNormal, glm::vec2& contactForce);

protected:
	glm::vec2 m_size;
	glm::vec4 m_colour;

	glm::vec2 m_localX;
	glm::vec2 m_localY;
};
