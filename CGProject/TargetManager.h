#pragma once
#include "Target.h"
#include <vector>

class TargetManager
{
public:
	TargetManager(int _targetNum=1);
	~TargetManager();
	void render();

private:
	int targetNum;
	std::vector<Target*> targets;
	std::vector<glm::vec3> positions;
};

