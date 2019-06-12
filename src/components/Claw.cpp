#include "Claw.h"
#include "Sprite.h"
#include "Collider.h"
#include "Easing.h"

Claw::Claw(GameObject &associated, float angle, float speed, int damage, weak_ptr<GameObject> center, float radius, float width, float high, bool targetsPlayer) : Component(associated)
{
	Collider *cl = new Collider(associated);
	associated.AddComponent(cl);

	this->targetsPlayer = targetsPlayer;
	this->speed = speed;
	this->damage = damage;
	this->radius = radius;
	this->angleLeft = 90;
	this->atAngle = angle;
	this->center = center;
	associated.angleDeg = atAngle;

	shared_ptr<GameObject> shared_Center = center.lock();
	Vec2 circRadius = Vec2(radius, 0);

	circRadius.Rotate(atAngle * (180 / PI)); // Rotates minion around the Alien

	circRadius += shared_Center->box.Center();
	circRadius.x -= associated.box.w / 2;
	circRadius.y -= associated.box.h / 2;

	associated.box.x = circRadius.x; // Moves minion to circRadius in x axis calculated
	associated.box.y = circRadius.y;

	duration.Restart();
}

void Claw::Update(float dt)
{
	shared_ptr<GameObject> shared_Center = center.lock();
	Vec2 circRadius = Vec2(radius, 0);

	duration.Update(dt);

	associated.angleDeg = atAngle;
	circRadius.Rotate(atAngle * (180 / PI));

	circRadius += shared_Center->box.Center();
	circRadius.x -= associated.box.w / 2;
	circRadius.y -= associated.box.h / 2;

	associated.box.x = circRadius.x;
	associated.box.y = circRadius.y;

	atAngle -= speed * dt; // Updates arc
	angleLeft -= speed * dt;

	// cout << angleLeft << endl;

	// atAngle += 5;
	// speed.Rotate(atAngle * (180 / PI));
	// associated.box.x -= speed.x * dt;
	// associated.box.y -= speed.y * dt;
	// associated.angleDeg = atAngle;
	if (duration.Get() > 0.5)
	{
		associated.RequestDelete();
		// std::cout << "destroyed" << std::endl;
	}
}

void Claw::Render()
{
}

bool Claw::Is(std::string type)
{
	return !strcmp(type.c_str(), "Claw");
}

int Claw::GetDamage()
{
	return damage;
}

void Claw::NotifyCollision(GameObject &other)
{
	if (other.GetComponent("Capelobo") && !targetsPlayer)
		associated.RequestDelete();

	if (other.GetComponent("PenguinBody") && targetsPlayer)
		associated.RequestDelete();
}