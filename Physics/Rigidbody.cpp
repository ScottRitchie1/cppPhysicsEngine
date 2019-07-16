#include "Rigidbody.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/mat3x3.hpp>
#include <iostream>
#include <Gizmos.h>
#define MIN_LINEAR_THRESHOLD 0.1f
#define MIN_ROTATION_THRESHOLD 0.01f



Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float elasticity, bool isKinematic) : PhysicsObject(shapeID){
	m_position = position;
	m_velocity = velocity;
	m_angularVelocity = 0;
	m_rotation = rotation;
	m_isKinematic = isKinematic;
	SetMass(mass);
	SetDrag(0.1f, 1.1f);
	SetElasticity(elasticity);
}

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, float angle, float speed, float rotation, float mass, float elasticity, bool isKinematic) : PhysicsObject(shapeID) {
	m_position = position;
	m_velocity.x = speed * glm::cos(angle);
	m_velocity.y = speed * glm::sin(angle);
	m_angularVelocity = 0;

	m_rotation = rotation;
	m_isKinematic = isKinematic;
	SetMass(mass);
	SetDrag(0.1f, 1.1f);
	SetElasticity(elasticity);
}

Rigidbody::~Rigidbody() {

}

void Rigidbody::FixedUpdate(glm::vec2 gravity, float timeStep) {
	if (m_isKinematic) {
		m_angularVelocity = 0;
		m_velocity = glm::vec2(0,0);
		return;
	}

	
	m_velocity += gravity * timeStep;
	m_position += m_velocity * timeStep;


	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_rotation += m_angularVelocity * timeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;


	if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD) {
		m_velocity = glm::vec2(0, 0);
	}

	if (abs(m_angularVelocity) < MIN_ROTATION_THRESHOLD) {
		m_angularVelocity = 0;
	}
}

void Rigidbody::CalculateKinematic(glm::vec2 gravity, float timeStep, float time)
{
	float t = 0;
	float angle = atan2f(m_velocity.y, m_velocity.x);
	float speed = glm::length(m_velocity);
	while (t <= time) {


		aie::Gizmos::add2DCircle(
			glm::vec2(
				m_position.x + (speed * t * cosf(angle)) - (0.5f * gravity.x * pow(t, 2)),
				m_position.y + (speed * t * sinf(angle)) - (0.5f * -gravity.y * pow(t, 2))
			), 
			0.5f, 6, glm::vec4(1, 1, 0, 1));
		t += timeStep;
	}
}

void Rigidbody::ApplyForce(glm::vec2 force, glm::vec2 pos) {
	m_velocity += force / m_mass;
	m_angularVelocity += ((force.y) * pos.x - (force.x) * pos.y) / (m_moment);
}



void Rigidbody::ResolvedCollision(Rigidbody * actor, glm::vec2 contact, glm::vec2* collisionNormal)
{
	const glm::vec2 normal = collisionNormal ? *collisionNormal : glm::normalize(actor->m_position - m_position);
	const glm::vec2 perp(normal.y, -normal.x);
	
	const float r1 = glm::dot(contact - m_position, -perp);
	const float r2 = glm::dot(contact - actor->m_position, perp);
	const float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	const float v2 = glm::dot(actor->m_velocity, normal) + r2 * actor->m_angularVelocity;
	
	if (v1 > v2)
	{
		const float mass1 = 1.0f / (1.0f / m_mass + (r1*r1) / m_moment);
		const float mass2 = 1.0f / (1.0f / actor->m_mass + (r2*r2) / actor->m_moment);
		const float elasticity = (m_elasticity + actor->GetElasticity()) * 0.5f;
	
		const glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * normal;
	
		ApplyForce(-force, contact - m_position);
		actor->ApplyForce(force, contact - actor->m_position);
	}
}	

float Rigidbody::GetKineticEnergy()
{
	return 0.5f * (m_mass * glm::dot(m_velocity, m_velocity) + m_moment * m_angularVelocity * m_angularVelocity);
}
