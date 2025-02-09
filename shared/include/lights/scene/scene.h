//
// Created by ozzadar on 2024-12-18.
//

#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <utility>
#include <vector>
#include "lights/rendering/buffer.h"
#include "lights/rendering/material.h"
#include "lights/input/input_subsystem.h"


namespace OZZ {
    class Camera {
    public:
        glm::mat4 ViewMatrix{1.f};
        glm::mat4 ProjectionMatrix{1.f};
    };

    class SceneObject {
    public:
        glm::mat4 Transform{1.f}; // Model transformation matrix
        std::shared_ptr<IndexVertexBuffer> Mesh{nullptr};
        std::shared_ptr<Material> Mat{nullptr}; // Shader/texture parameters
    };

    class LightSource {
    public:
        glm::vec3 Position;
        glm::vec3 Color;
        float Intensity;
    };

    class SceneLayer {
    public:
        virtual ~SceneLayer() = default;

        virtual void Init() {};
        virtual void Tick(float DeltaTime) {};
        virtual void RenderTargetResized(glm::ivec2 size) = 0;
        virtual std::vector<SceneObject>  GetSceneObjects() = 0;


        Camera LayerCamera {};
        std::vector<std::shared_ptr<LightSource>> Lights;
    };

    class Scene {
    public:
        virtual void Init() {};

        virtual void Tick(float DeltaTime) {
            for (auto &Layer: GetLayers()) {
                Layer->Tick(DeltaTime);
            }
        };

        // Marked virtual to allow derived Scenes with custom entities
        void RenderTargetResized(glm::ivec2 size) {
            for (auto &Layer: GetLayers()) {
                Layer->RenderTargetResized(size);
            }
        }

        virtual ~Scene() = default;

        virtual std::vector<std::shared_ptr<SceneLayer>>& GetLayers() = 0;
    public:
    };
}