#include "PhysicsApp.h"
#include "PhysicsScene.h"
#include "Circle.h"
#include "Line.h"
#include "Rect.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <glm/ext.hpp>
#include <Gizmos.h>

PhysicsApp::PhysicsApp() {

}

PhysicsApp::~PhysicsApp() {

}

bool PhysicsApp::startup() {
	
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetGravity(glm::vec2(0, -200.1));
	m_physicsScene->SetTimeStep(0.016667f);

	m_ball = new Circle(glm::vec2(0.0f, -99999.f), 0.0f, 10.0f, 1.0f, 1.f, 20.0f,  glm::vec4(1, 1, 0, 1));
	m_ball->OnCollision = [this](Rigidbody* self, Rigidbody* other, glm::vec2 point, glm::vec2 normal) {
		Rect* rectOther = dynamic_cast<Rect*>(other);
		aie::Input* input = aie::Input::getInstance();

		if (rectOther == m_paddleLeft) {
			if (input->isKeyDown(aie::INPUT_KEY_LEFT_ALT) && rectOther->GetRotation() < 0.7f) {
				rectOther->SetColour(glm::vec4(0.5f, 1, 0.7f, 1));
				self->ApplyForce(rectOther->GetLocalY() *1000.f, glm::vec2(0, 0));
			}
		}

		if (rectOther == m_paddleRight) {
			if (input->isKeyDown(aie::INPUT_KEY_RIGHT_ALT) && rectOther->GetRotation() > -0.7f) {
				rectOther->SetColour(glm::vec4(0.5f, 1, 0.7f, 1));
				self->ApplyForce(rectOther->GetLocalY() *1000.f, glm::vec2(0, 0));
			}
		}
	};
	//edges
	m_physicsScene->AddActor(new Line(glm::vec2(1.0f, 0.0f), -570.0f /2.0f, glm::vec4(1, 0, 0, 1)));
	m_physicsScene->AddActor(new Line(glm::vec2(-1.0f, 0.0f), -570.0f /2.0f, glm::vec4(1, 0, 0, 1)));

	m_physicsScene->AddActor(new Line(glm::vec2(0, 1), -300.0f, glm::vec4(1, 0, 0, 1)));
	m_physicsScene->AddActor(new Line(glm::vec2(0.0f, -1.0f), -510.0f, glm::vec4(1, 0, 0, 1)));
	m_physicsScene->AddActor(new Line(glm::normalize(glm::vec2(-1.0f, -2.0f)), -500.0f, glm::vec4(1, 0, 0, 1)));
	m_physicsScene->AddActor(new Line(glm::normalize(glm::vec2(-2.0f, -1.0f)), -400.0f, glm::vec4(1, 0, 0, 1)));

	m_physicsScene->AddActor(new Line(glm::normalize(glm::vec2(1.0f, -2.0f)), -500.0f, glm::vec4(1, 0, 0, 1)));
	m_physicsScene->AddActor(new Line(glm::normalize(glm::vec2(2.0f, -1.0f)), -400.0f, glm::vec4(1, 0, 0, 1)));

	m_physicsScene->AddActor(new Rect(glm::vec2(210.0f, -60.0f), 0.0f, glm::vec2(0.0f, 0.0f), glm::vec2(40.0f, 500.0f), 5.f, 1.0f, glm::vec4(1, 0, 0, 1), true));


	//pins
	m_physicsScene->AddActor(new Circle(glm::vec2(0, 420), glm::vec2(0, 0), 999999, 1, 25, glm::vec4(0, 1, 0, 1), true));
	m_physicsScene->AddActor(new Rect(glm::vec2(150, 350), 2.5f,glm::vec2(0, 0),glm::vec2(120, 40), 5.f, 1,glm::vec4(0, 1, 1, 1), true));
	m_physicsScene->AddActor(new Rect(glm::vec2(-150, 350), -2.5f, glm::vec2(0, 0), glm::vec2(120, 40), 5.f, 1, glm::vec4(0, 1, 1, 1), true));
	m_physicsScene->AddActor(new Rect(glm::vec2(50, 150), -2.5f, glm::vec2(0, 0), glm::vec2(120, 40), 5.f, 1, glm::vec4(0, 1, 1, 1), true));
	m_physicsScene->AddActor(new Rect(glm::vec2(-150, 150), 2.5f, glm::vec2(0, 0), glm::vec2(120, 40), 5.f, 1, glm::vec4(0, 1, 1, 1), true));
	
	m_physicsScene->AddActor(new Circle(glm::vec2(50, 300), glm::vec2(0, 0), 999999, 1, 25, glm::vec4(0, 1, 0, 1), true));
	m_physicsScene->AddActor(new Circle(glm::vec2(-70, 250), glm::vec2(0, 0), 999999, 1, 25, glm::vec4(0, 1, 0, 1), true));
	
	
	m_physicsScene->AddActor(new Circle(glm::vec2(110, -70), glm::vec2(0, 0), 999999, 1, 30, glm::vec4(0, 1, 0, 1), true));
	m_physicsScene->AddActor(new Circle(glm::vec2(-210, -70), glm::vec2(0, 0), 999999, 1, 30, glm::vec4(0, 1, 0, 1), true));
	m_physicsScene->AddActor(new Circle(glm::vec2(-50, 35), glm::vec2(0, 0), 999999, 1, 40, glm::vec4(0, 0.5f, 0.5f, 1), true));
	m_rotator = new Rect(glm::vec2(-50, 35), 0, glm::vec2(0,0), glm::vec2(175, 25), 999999, 1, glm::vec4(0, 0.5f, 0.5f, 1), true);
	m_physicsScene->AddActor(m_rotator);

	m_paddleRight = new Rect(glm::vec2(80.0f, -200.0f), -2.7f, glm::vec2(0.0f, 0.0f), glm::vec2(200.0f, 25.0f), 5.f, 1.0f, glm::vec4(1, 1, 1, 1), true);
	m_paddleLeft = new Rect(glm::vec2(-180.0f, -200.0f), 2.7f, glm::vec2(0.0f, 0.0f), glm::vec2(200.0f, 25.0f), 5.f, 1.0f, glm::vec4(1, 1, 1, 1), true);

	m_physicsScene->AddActor(m_ball);
	m_physicsScene->AddActor(m_paddleRight);
	m_physicsScene->AddActor(m_paddleLeft);
	m_physicsScene->AddActor(new Rect(glm::vec2(145.0f, -210.0f), -2.8f, glm::vec2(0.0f, 0.0f), glm::vec2(135.0f, 100.0f), 5.f, 1.0f, glm::vec4(1, 0, 0, 1), true));
	m_physicsScene->AddActor(new Rect(glm::vec2(-245.0f, -210.0f), 2.8f, glm::vec2(0.0f, 0.0f), glm::vec2(135.0f, 100.0f), 5.f, 1.0f, glm::vec4(1, 0, 0, 1), true));

	m_ball->SetPosition(glm::vec2(260, -275));
	m_ball->SetVelocity(glm::vec2(0, 0));

	return true;
}

void PhysicsApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
	delete m_physicsScene;
}

void PhysicsApp::update(float deltaTime) {

	// input example
	const float rotSpeed = 10;
	static float power = 0;

	aie::Input* input = aie::Input::getInstance();
	m_rotator->SetRotation(m_rotator->GetRotation() + (1 * deltaTime));

	if (m_ball->GetPosition().y < -275 || input->wasKeyPressed(aie::INPUT_KEY_BACKSPACE)) {
		m_ball->SetPosition(glm::vec2(260, -275));
		m_ball->SetVelocity(glm::vec2(0, 0));
	}

	if (input->isKeyDown(aie::INPUT_KEY_SPACE)) {
		power += deltaTime;
		if (power > 2) {
			power = 2;
		}
	}
	else if (power > 0) {
		m_ball->ApplyForce(glm::vec2(0, 1)*power*2000.0f, glm::vec2(0, 0));
		power = 0;
	}

	if (input->isKeyDown(aie::INPUT_KEY_RIGHT_ALT)) {
		m_paddleRight->SetRotation(m_paddleRight->GetRotation() - rotSpeed * deltaTime);
		if (m_paddleRight->GetRotation() < -0.7f)
			m_paddleRight->SetRotation(-0.7f);
	}
	else {
		m_paddleRight->SetRotation(m_paddleRight->GetRotation() + rotSpeed * deltaTime);
		if (m_paddleRight->GetRotation() > 0.3f)
			m_paddleRight->SetRotation(0.3f);
	}
	
	if (input->isKeyDown(aie::INPUT_KEY_LEFT_ALT)) {
		
		m_paddleLeft->SetRotation(m_paddleLeft->GetRotation() + rotSpeed * deltaTime);
		if (m_paddleLeft->GetRotation() > 0.7f)
			m_paddleLeft->SetRotation(0.7f);
		
	}
	else {
		m_paddleLeft->SetRotation(m_paddleLeft->GetRotation() - rotSpeed * deltaTime);
		if (m_paddleLeft->GetRotation() < -0.3f)
			m_paddleLeft->SetRotation(-0.3f);
	}

	aie::Gizmos::clear();
	m_physicsScene->Update(deltaTime);
	m_physicsScene->UpdateGizmos();
	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();


	
	
}

void PhysicsApp::draw() {

	// wipe the screen to the background colour
	
	clearScreen();
	

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 576.0f / 1024.0f;
	aie::Gizmos::draw2D(glm::ortho<float>(-576/2, 576 / 2, -576 /2 / aspectRatio, 576 /2 / aspectRatio, -1.0f, 1.0f));

	
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}