//
// Created by ozzadar on 2025-04-06.
//

#pragma once
#include <lights/scene/scene.h>

class MMOScene : public OZZ::scene::Scene {
public:
	~MMOScene() override = default;

	void Init(std::shared_ptr<OZZ::InputSubsystem> inInput) override;
	void WindowResized(glm::ivec2 size) override;
};