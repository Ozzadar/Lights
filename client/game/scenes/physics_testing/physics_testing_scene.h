//
// Created by ozzadar on 2025-02-12.
//

#pragma once
#include <lights/scene/scene.h>

namespace OZZ::game::scene {
    class PhysicsTestingLayer;
}

namespace OZZ::game::scene {
    class PhysicsTestingScene : public OZZ::scene::Scene {
    public:
        void Init(std::shared_ptr<InputSubsystem> inInput) override;

    private:
        PhysicsTestingLayer* mainLayer { nullptr };
    };
}
