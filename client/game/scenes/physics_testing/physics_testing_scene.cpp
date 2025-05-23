//
// Created by ozzadar on 2025-02-12.
//

#include "physics_testing_scene.h"

#include "layers/physics_testing_layer.h"

namespace OZZ::game::scene {
    void PhysicsTestingScene::Init(std::shared_ptr<InputSubsystem> inInput) {
        Scene::Init(inInput);

        mainLayer = layerManager->LoadLayer<PhysicsTestingLayer>("MainLayer", GetWorld());
        mainLayer->SetInputSubsystem(inInput);
        layerManager->SetLayerActive("MainLayer", true);

        layerManager->Init();
    }
}
