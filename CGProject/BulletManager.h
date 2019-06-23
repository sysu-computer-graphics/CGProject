#pragma once
#include <vector>

#include "Bullet.h"
#include "Model.h"

class BulletManager
{
public:
	BulletManager();
	~BulletManager();
	void newBullet(glm::vec3 front, glm::vec3 position);
	void render(Model &bulletModel, const glm::vec3 &_lightPos);
	void deleteBullet(std::string bulletId);
	std::vector<Bullet*> &getBulletLists() { return this->bulletLists; }
private:
	std::vector<Bullet*> bulletLists;
};

