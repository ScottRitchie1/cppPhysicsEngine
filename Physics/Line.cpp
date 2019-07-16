#include "line.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <Gizmos.h>
Line::Line(glm::vec2 normal, float distToOrigin, glm::vec4 colour) : PhysicsObject(ShapeType::PLANE) {
	m_normal = glm::normalize(normal);
	m_distToOrigin = distToOrigin;
	m_colour = colour;
}
Line::~Line() {

}

void Line::MakeGizmo() {
	glm::vec2 centerPoint = m_normal * m_distToOrigin;

	glm::vec2 parrallel(m_normal.y, -m_normal.x);

	glm::vec2 start = centerPoint + (parrallel * (600.0f));
	glm::vec2 end = centerPoint - (parrallel * (600.0f));
	aie::Gizmos::add2DLine(start, end, m_colour);
}

void Line::ResetPosition()
{
	m_distToOrigin = 0;
	m_normal = glm::vec2(0, 1);
}

void Line::ResolvedCollision(Rigidbody * actor, glm::vec2 contact)
{
	glm::vec2 rVel = actor->GetVelocity();
	float elasticity = actor->GetElasticity();

	glm::vec2 vp = actor->GetVelocity() - Cross(actor->GetVelocityAngular(), contact - actor->GetPosition());

   	float vp_p = glm::dot(vp, m_normal);
	float j = -(1+elasticity) * vp_p / (1 / actor->GetMass() + powf(Cross(contact - actor->GetPosition(), m_normal), 2) / actor->GetMomentum());
		
	glm::vec2 force = m_normal * j;

	actor->ApplyForce(force, contact - actor->GetPosition());
}
