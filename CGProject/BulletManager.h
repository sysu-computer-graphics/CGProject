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
	void render(Model &bulletModel);
	void deleteBullet(std::string bulletId);
private:
	std::vector<Bullet*> bulletLists;
};

