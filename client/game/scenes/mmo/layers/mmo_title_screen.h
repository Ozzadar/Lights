//
// Created by ozzadar on 2025-04-06.
//

#pragma once

#include <lights/scene/scene.h>
#include <lights/text/font_loader.h>

namespace OZZ::game::objects {
	class Button;
	class TextInput;
	class TextLabel;
}

namespace OZZ::lights::library::layers {
	class UILayer;
}

class MMOTitleScreen : public OZZ::scene::SceneLayer {
public:
	explicit MMOTitleScreen(OZZ::GameWorld* inWorld, OZZ::lights::library::layers::UILayer* inUILayer);
	~MMOTitleScreen() override = default;

	void SetInputSubsystem(const std::shared_ptr<OZZ::InputSubsystem>& inInput);

	/// Override
	void Init() override;
	void PhysicsTick(float DeltaTime) override;
	void Tick(float DeltaTime) override;
    void RenderTargetResized(glm::ivec2 size) override;
    std::vector<OZZ::scene::SceneObject> GetSceneObjects() override;

private:
	void selectNextInputBox(int direction);

private:
	std::filesystem::path fontPath { "assets/fonts/MineMouseRegular.ttf" };
	OZZ::lights::library::layers::UILayer* uiLayer { nullptr };

	std::unique_ptr<OZZ::FontLoader> fontLoader { nullptr };

	// Keep the things under here
	OZZ::GameWorld* gameWorld;
	std::pair<uint64_t, OZZ::game::objects::TextLabel*> titleScreenText { UINT64_MAX, nullptr };
	std::pair<uint64_t, OZZ::game::objects::TextInput*> UsernameInputBox { UINT64_MAX, nullptr };

	std::shared_ptr<OZZ::InputSubsystem> input;

	std::vector<OZZ::GameObjectContainer<OZZ::game::objects::TextInput>> inputBoxes;
	OZZ::GameObjectContainer<OZZ::game::objects::TextLabel> titleLabel;
	OZZ::GameObjectContainer<OZZ::game::objects::TextLabel> usernameLabel;
	OZZ::GameObjectContainer<OZZ::game::objects::TextLabel> passwordLabel;

	// Login Button
	OZZ::GameObjectContainer<OZZ::game::objects::Button> loginButton;

	int focusedBox { INT_MAX };
	int width { 1920 };
	int height { 1080 };
	bool bShiftPressed { false };
};
