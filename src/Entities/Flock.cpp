/*
 * Flock.cpp
 *
 *  Created on: Jan 23, 2011
 *      Author: Nathan
 */

#include "Flock.h"

float decay = 0.998;
Vec3f orbit;
GLuint vertexBufferObject;

const float vertexData[] = {
		-0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f,  0.0f, 0.0f, 1.0f,
		 0.0f,  0.0f, 0.4f, 1.0f,

		 0.5f,  0.0f, 0.0f, 1.0f,
		 0.0f,  0.0f, 0.4f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f,

		-0.5f,  0.5f, 0.0f, 1.0f,
		 0.0f,  0.0f, 0.4f, 1.0f,
		-0.3f,  0.0f, 0.0f, 1.0f,

		-0.3f,  0.0f, 0.0f, 1.0f,
		 0.0f,  0.0f, 0.4f, 1.0f,
		-0.5f, -0.5f, 0.0f, 1.0f,
};

Flock::Flock(){}

void Flock::addEntity(Entity* e){
	members.push_back(e);
}

void Flock::removeEntity(Entity* e){
}

bool Flock::update(float dt){
	for(unsigned i = 0; i < members.size(); i++){
		members[i]->acc -= (members[i]->pos - orbit)*(rand()%10)/10;
	}
	return members.size();
}

void Flock::setObjective(Vec3f o){
	orbit = o;
}

Boid::Boid(Renderer * r, Vec3f _pos, int _team)
    : Entity(r, _pos) {
    if(team == PLAYER)
        color = Vec3f(129, 204, 60);
    else {
        color = Vec3f(181, 40, 65);
    }
    team = _team;
    type = BOID;
    explodes = true;
    minSpeed = 5;
    maxSpeed = rand()%45 + 30;
    minSq = minSpeed*minSpeed;
    maxSq = maxSpeed*maxSpeed;
    pos = _pos;
    vel = Vec3f(rand()%(int)maxSpeed-maxSpeed/2, rand()%(int)maxSpeed-maxSpeed/2, 0.0f);
    dead = false;
    dir = Vec3f(0, 1, 0);
    value = 5;
    damage = 30;
    health = maxHealth = 90;
    fear = 0;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Boid::render(){
	renderer->pushMatrix();
	//renderer->setColor(r, g, b, 1.0f);
//	renderer->scale(0.5f, 0.5f, 0.5f);
	renderer->rotate(dir.theta(), 0.0f, 0.0f, 1.0f);
	renderer->applyCamera();
	renderer->translate(pos.x, pos.y, 0.0f);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glUniformMatrix4fv(renderer->uniforms.modelMat, 1, GL_FALSE, (GLfloat*)renderer->currentMatrix().m);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertexData)/4);
	glDisableVertexAttribArray(0);
	renderer->popMatrix();
}

void Boid::update(float dt){
	if(health < 0)
		die();
	maxSpeed = maxSpeed + fear;
	vel += (acc/(30.0/1000))*dt;

	if(vel.lengthSq() < minSq){
		vel = vel.normalized()*minSpeed;
	}
	else if(vel.lengthSq() > maxSq){
		vel = vel.normalized()*maxSpeed;
	}
	pos += vel * dt;
	vel *= decay;
	acc.zero();
	dir = vel.normalized();
}

//Thresholds
float zone = 15;
float interactZone = zone*zone;
float repelThresh = 0.25f;
float alignThresh = 0.5f;
float repelWeight = 5.f;
float coheseWeight = 8;
float alignWeight = 1;

void Boid::interact(Entity* e){
	Vec3f dist = pos - e->pos;
	float dsq = dist.lengthSq();
	dist = dist.normalized();
	if(dsq <= interactZone)
	{
		float ratio = dsq/interactZone;
		if(ratio < repelThresh){
			//Repel
			float force = (repelThresh/ratio - 1.0f)*repelWeight;
			acc += dist*force;
		}
		else if(ratio < alignThresh){
			float weight = (ratio - alignThresh)/(1.0f - alignThresh);
			double force = (1.0f - (cos(weight*PI*2) * -0.5f + 0.5f))*alignWeight;
			acc += e->vel.normalized()*force;
		}
		else {
			//Attract
			float weight = (ratio - repelThresh)/(1.0f - repelThresh);
			double force = (1.0f - (cos(weight*PI*2) * -0.5f + 0.5f))*coheseWeight;
			acc -= dist*force;
		}
	}

}

void Boid::die(){
	dead = true;
}

Boid::~Boid(){

}

