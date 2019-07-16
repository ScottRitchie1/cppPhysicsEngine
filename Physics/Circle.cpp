#include "Circle.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <Gizmos.h>
Circle::Circle(glm::vec2 position, glm::vec2 velocity, float mass, float elasticity, float radius, glm::vec4 colour, bool isKinematic) : Rigidbody(ShapeType::SPHERE, position, velocity, 0, mass, elasticity, isKinematic) {
	m_radius = radius;
	m_colour = colour;
	SetMass(mass);
	m_moment = 0.5f * m_mass * m_radius * m_radius;
}
Circle::Circle(glm::vec2 position, float angle, float speed, float mass, float elasticity, float radius, glm::vec4 colour, bool isKinematic) : Rigidbody(ShapeType::SPHERE, position, glm::vec2(speed * glm::cos(angle), speed * glm::sin(angle)), 0, mass, elasticity, isKinematic) {
	m_radius = radius;
	m_colour = colour;
	SetMass(mass);
	m_moment = 0.5f * m_mass * m_radius * m_radius;
}
Circle::~Circle() {

}

void Circle::MakeGizmo() {
	aie::Gizmos::add2DCircle(m_position, m_radius, 12, m_colour);
}