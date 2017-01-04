/*
 * Explosion.h
 *
 * Rudimentary particle system for rendering 2-D stylized explosions.
 *
 */

#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include "Entity.h"
#include "../Util/includes.h"
using std::vector;

class Particle : public Entity{
public:
	Particle(float, float, float);
	void update(float);
	void render(bool);
	void interact(Entity* e){}
	virtual ~Particle();
	short life;
	glm::vec2 vsize;
};
class Explosion : public Entity{
public:
	Explosion(float, float);
	void update(float);
	void render(bool);
	void interact(Entity* e){}
	virtual ~Explosion();
private:
	bool overloaded;
	static const int numParticles = 180;
	vector<Particle> particles;
};
#endif /* EXPLOSION_H_ */
