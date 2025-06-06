//
// Created by ozzadar on 2025-02-12.
//

#include "physics_testing_layer.h"

#include <game/library/objects/player/player.h>

#include "lights/game/game_object.h"

namespace OZZ::game::scene {
    PhysicsTestingLayer::PhysicsTestingLayer(GameWorld *inWorld) : gameWorld(inWorld) {
    }

    PhysicsTestingLayer::~PhysicsTestingLayer() {
        if (gameWorld) {
            gameWorld->RemoveObject(pepe.first);
            gameWorld->RemoveObject(groundId);
            gameWorld->RemoveObject(rightWallId);
            gameWorld->RemoveObject(leftWallId);
            gameWorld->RemoveObject(topWallId);
        }
    }

    void PhysicsTestingLayer::Init() {
        LayerCamera.ViewMatrix = glm::lookAt(glm::vec3(0.f, 0.f, 3.f), // Camera position
                                             glm::vec3(0.f, 0.f, 0.f), // Target to look at
                                             glm::vec3(0.f, 1.f, 0.f)); // Up vector

        pepe = gameWorld->CreateGameObject<objects::Player>();
        pepe.second->SetupInput(input.get());

        auto [rId, inRightWall] = gameWorld->CreateGameObject<Sprite>("assets/textures/cone.png");
        rightWall = inRightWall;
        rightWallId = rId;
        rightWall->AddBody(
            BodyType::Static,
            OZZ::collision::shapes::OzzRectangle{
                .Position = glm::vec3{6.f * constants::PixelsPerMeter, 0.f, 1.f},
                .Size = glm::vec3{1.f * constants::PixelsPerMeter, 12.f * constants::PixelsPerMeter, 1.f},
            },
            glm::vec2{0, 0});


        auto [lId, inLeftWall] = gameWorld->CreateGameObject<Sprite>("assets/textures/container.jpg");
        leftWallId = lId;
        leftWall = inLeftWall;
        leftWall->AddBody(
            BodyType::Static,
            OZZ::collision::shapes::OzzRectangle{
                .Position = glm::vec3{-6.f * constants::PixelsPerMeter, 0.f, 1.f},
                .Size = glm::vec3{1.f * constants::PixelsPerMeter, 12.f * constants::PixelsPerMeter, 1.f}
            },
            glm::vec2{0, 0});

        auto [tId, inTopWall] = gameWorld->CreateGameObject<Sprite>("assets/textures/container.jpg");
        topWallId = tId;
        topWall = inTopWall;
        topWall->AddBody(
            BodyType::Static,
            OZZ::collision::shapes::OzzRectangle{
                .Position = glm::vec3{0.f, 10.f * constants::PixelsPerMeter, 1.f},
                .Size = glm::vec3{12.f * constants::PixelsPerMeter, 1.f * constants::PixelsPerMeter, 1.f}
            },
            glm::vec2{0, 0});

        auto [gId, inGround] = gameWorld->CreateGameObject<Sprite>("assets/textures/container.jpg");
        groundId = gId;
        ground = inGround;
        ground->AddBody(
            BodyType::Static,
            OZZ::collision::shapes::OzzRectangle
            {
                .Position = glm::vec3{0.f, -2.f * constants::PixelsPerMeter, 1.f},
                .Size = glm::vec3{12.f * constants::PixelsPerMeter, 1.f * constants::PixelsPerMeter, 1.f}
            },
            glm::vec2{0, 0});

        updateViewMatrix();
    }

    void PhysicsTestingLayer::PhysicsTick(float DeltaTime) {
        SceneLayer::PhysicsTick(DeltaTime);
    }

    void PhysicsTestingLayer::Tick(float DeltaTime) {
        SceneLayer::Tick(DeltaTime);

        // TODO: I feel like the gameworld doesn't need to be owned by the layer -- though I'm not entirely sure of the alternative.
        // I'll leave it here for now and see if it makes sense to move it somewhere else (like the scene) further down the line
        // Doing it here makes it easy to forget in the future if
        gameWorld->Tick(DeltaTime);

        updateViewMatrix();
    }

    void PhysicsTestingLayer::SetInputSubsystem(const std::shared_ptr<InputSubsystem> &inInput) {
        input = inInput;
    }

    void PhysicsTestingLayer::RenderTargetResized(glm::ivec2 size) {
        const auto width = size.x * 2;
        const auto height = size.y * 2;

        LayerCamera.ProjectionMatrix = glm::ortho(-width / 2.f, width / 2.f, -height / 2.f, height / 2.f, 0.001f,
                                                  1000.f);
    }

    std::vector<OZZ::scene::SceneObject> PhysicsTestingLayer::GetSceneObjects() {
        auto pepeSceneObject = pepe.second->GetSceneObjects();
        auto groundSceneObjects = ground->GetSceneObjects();
        auto rightWallSceneObjects = rightWall->GetSceneObjects();
        auto leftWallSceneObjects = leftWall->GetSceneObjects();
        auto topWallSceneObjects = topWall->GetSceneObjects();

        return
                groundSceneObjects +
                rightWallSceneObjects +
                leftWallSceneObjects +
                topWallSceneObjects +
                pepeSceneObject;
    }

    void PhysicsTestingLayer::updateViewMatrix() {
        const auto pepePosition = pepe.second->GetPosition();
        LayerCamera.ViewMatrix = glm::lookAt(glm::vec3{pepePosition.x, pepePosition.y, 3.f}, // Camera position
                                             glm::vec3{pepePosition.x, pepePosition.y, 0.f}, // Target to look at
                                             glm::vec3{0.f, 1.f, 0.f}); // Up vector
    }
}
