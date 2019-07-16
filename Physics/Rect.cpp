#include "Rect.h"
#include <glm/ext.hpp>
Rect::Rect(glm::vec2 position, float rotation, glm::vec2 velocity, glm::vec2 size, float mass, float elasticity, glm::vec4 colour, bool isKinematic) : Rigidbody(ShapeType::BOX, position, velocity, rotation, mass, elasticity, isKinematic)
{
	m_localX = glm::vec2(0, 0);
	m_localY = glm::vec2(0, 0);
	m_size = size;
	m_colour = colour;
	m_moment = 1.0f / 12.0f * m_mass * m_size.x * m_size.y;
}

void Rect::MakeGizmo()
{
	glm::vec2 extents = GetExtents();
	glm::vec2 p1 = m_position - m_localX * extents.x - m_localY * extents.y;
	glm::vec2 p2 = m_position + m_localX * extents.x - m_localY * extents.y;
	glm::vec2 p3 = m_position - m_localX * extents.x + m_localY * extents.y;
	glm::vec2 p4 = m_position + m_localX * extents.x + m_localY * extents.y;

	aie::Gizmos::add2DTri(p1, p2, p4, m_colour);
	aie::Gizmos::add2DTri(p1, p4, p3, m_colour);
	if (lastCollisionPoint != glm::vec2()) {
		aie::Gizmos::add2DCircle(lastCollisionPoint, 5.f, 12, glm::vec4(1, 1, 0, 1));
		if (lastCollisionNormal != glm::vec2()) {
			aie::Gizmos::add2DLine(lastCollisionPoint, lastCollisionPoint + lastCollisionNormal * 1000.f, glm::vec4(1, 0, 1, 1));

		}
	}
}

void Rect::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	Rigidbody::FixedUpdate(gravity, timeStep);

	float cs = cosf(m_rotation);
	float sn = sinf(m_rotation);
	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));
}

bool Rect::CheckRectCorners(const Rect & rect, glm::vec2 & contact, int & numContacts, glm::vec2 & edgeNormal, glm::vec2& contactForce)
{
	float rectW = rect.GetExtents().x * 2;
	float rectH = rect.GetExtents().y * 2;
	float penetration = 0;
	int localNumContacts = 0;
	float w2 = m_size.x / 2.0f, h2 = m_size.y / 2.0f;
	for (float x = -rect.GetExtents().x; x < rectW; x += rectW)
	{
		for (float y = -rect.GetExtents().y; y < rectH; y += rectH) {
			glm::vec2 p = rect.GetPosition() + x * rect.m_localX + y * rect.m_localY;
			glm::vec2 p0(glm::dot(p - m_position, m_localX), glm::dot(p - m_position, m_localY));
			
			if (p0.y <= h2 && p0.y >= -h2 && p0.x <= w2 && p0.x > -w2) { //corner in rect

				const float penTop = h2 - p0.y;
				const float penBot = h2 + p0.y;
				const float penRight = w2 - p0.x;
				const float penLeft = w2 + p0.x;

				float pen = fminf(fminf(penTop, penBot), fminf(penRight, penLeft));

				const Side closestSide = FindClosestSide(rect.GetPosition());
				localNumContacts++;

				if (closestSide == LEFT || (closestSide == NONE && pen == penRight))
				{
					// Right
					contact += m_position + w2 * m_localX + p0.y * m_localY;
					edgeNormal = -m_localX;

					if (penRight > penetration)
						penetration = penRight;
				}
				else if (closestSide == TOP || (closestSide == NONE && pen == penTop))
				{
					// Top
					contact += m_position + p0.x * m_localX + h2 * m_localY;

					if (penTop > penetration)
						penetration = penTop;

					edgeNormal = -m_localY;
				}
				else if (closestSide == RIGHT || (closestSide == NONE && pen == penLeft))
				{
					// Left
					contact += m_position + -w2 * m_localX + p0.y * m_localY;

					if (penLeft > penetration)
						penetration = penLeft;

					edgeNormal = m_localX;
				}
				else if (closestSide == BOT || (closestSide == NONE && pen == penBot))
				{
					// Bottom
					contact += m_position + p0.x * m_localX + -h2 * m_localY;

					if (penBot > penetration)
						penetration = penBot;

					edgeNormal = m_localY;
				}
			}
		}
	}
	contact /= (float)localNumContacts;
	numContacts += localNumContacts;
	contactForce = penetration * -edgeNormal;
	return (penetration != 0);
}

Side Rect::FindClosestSide(glm::vec2 point) const
{
	const glm::vec2 dirToPoint = point - m_position;

	float distanceX = glm::dot(dirToPoint, m_localX);

	if (distanceX > m_size.x/2)
	{
		distanceX = m_size.x / 2;
	}
	else if (distanceX < -m_size.x / 2)
	{
		distanceX = -m_size.x / 2;
	}

	float distanceY = glm::dot(dirToPoint, m_localY);

	if (distanceY > m_size.y / 2)
	{
		distanceY = m_size.y / 2;
	}
	else if (distanceY < -m_size.y / 2)
	{
		distanceY = -m_size.y / 2;
	}

	const glm::vec2 closestPoint = m_position + distanceX * m_localX + distanceY * m_localY;
	const glm::vec2 direction = glm::normalize(point - closestPoint);

	const float top = glm::dot(direction, m_localY);
	const float bot = glm::dot(direction, -m_localY);
	const float left = glm::dot(direction, m_localX);
	const float right = glm::dot(direction, -m_localX);

	const float side = fmaxf(fmaxf(top, bot), fmaxf(left, right));

	if (side == 1.0f)
	{
		if (side == right)
		{
			return RIGHT;
		}
		else if (side == top)
		{
			return TOP;
		}
		else if (side == left)
		{
			return LEFT;
		}
		else
		{
			return BOT;
		}
	}

	return NONE;
}
