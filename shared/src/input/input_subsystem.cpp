//
// Created by ozzadar on 2024-10-09.
//

#include <cassert>
#include "lights/input/input_subsystem.h"

#include <algorithm>
#include <spdlog/spdlog.h>

namespace OZZ {
    InputSubsystem::InputSubsystem() {
    }

    void InputSubsystem::Initialize() {
    }

    void InputSubsystem::Shutdown() {
    }

    void InputSubsystem::NotifyInputEvent(const InputEvent &Event) {
        // Notify all mappings
        for (auto &Mapping: Mappings) {
            // loops through all chords, if one of them is triggered, call the callback
            auto& chords = Mapping.Chords;
            for (auto &Chord: chords) {
                if (Chord.ReceiveEvent(Event.Key, Event.State)) {
                    switch (Chord.CurrentState) {
                        case EKeyState::KeyPressed:
                            if (Mapping.Callbacks.OnPressed)
                                Mapping.Callbacks.OnPressed();
                        break;
                        case EKeyState::KeyReleased:
                            if (Mapping.Callbacks.OnReleased)
                                Mapping.Callbacks.OnReleased();
                        break;
                        default:
                            break;
                    }
                }
            }
        }
    }

    void InputSubsystem::Tick(const KeyStateArrayType &keyStates, const ControllerStateMap& controllerStates) {
        // Update all the axis mappings
        for (auto &Mapping: AxisMappings) {
            Mapping.Value = 0.f;

            for (auto &[eKey, Weight] : Mapping.Keys) {
                if (Mapping.Value != 0.f) {
                    break;
                }

                if (eKey.DeviceID == -1) {
                    Mapping.Value = std::clamp(keyStates[+std::get<EKey>(eKey.Key)] * Weight, -1.f, 1.f);
                } else {
                    // this is a controller mapping
                    if (controllerStates.contains(eKey.DeviceID)) {
                        const auto key = std::get<EControllerButton>(eKey.Key);
                        auto value = controllerStates.at(eKey.DeviceID)[+key];

                        // TODO: Current deadzone is hardcoded. It might make sense to make this customizable
                        value = value < 0.1f && value > -0.1f ? 0.f : value;

                        Mapping.Value = std::clamp(value * Weight, -1.f, 1.f);
                    }
                }
            }
        }
    }

    float InputSubsystem::GetAxisValue(const std::string &Action) const {
        auto Mapping = std::ranges::find_if(AxisMappings, [&Action](const AxisMapping& Mapping) {
            return Mapping.Action == Action;
        });

        if (Mapping != AxisMappings.end()) {
            return Mapping->Value;
        }
        return 0.f;
    }

    void InputSubsystem::RegisterInputMapping(InputMapping &&Mapping) {
        std::string Action = Mapping.Action;
        bool bFound = false;
        for (auto &ExistingMapping: Mappings) {
            if (ExistingMapping.Action == Action) {
                ExistingMapping = Mapping;
                bFound = true;
                break;
            }
        }

        if (!bFound) {
            Mappings.push_back(Mapping);
        }
    }

    void InputSubsystem::UnregisterInputMapping(const std::string &Action) {
        std::erase_if(Mappings, [&Action](const InputMapping &Mapping) {
            return Mapping.Action == Action;
        });
    }

    void InputSubsystem::RegisterAxisMapping(AxisMapping &&Mapping) {
        const std::string &action = Mapping.Action;
        bool bFound = false;
        for (auto &ExistingMapping: AxisMappings) {
            if (ExistingMapping.Action == action) {
                ExistingMapping = Mapping;
                bFound = true;
                break;
            }
        }

        if (!bFound) {
            AxisMappings.push_back(Mapping);
        }
    }

    void InputSubsystem::UnregisterAxisMapping(const std::string &Action) {
        std::erase_if(AxisMappings, [&Action](const AxisMapping &Mapping) {
            return Mapping.Action == Action;
        });
    }

    bool InputChord::ReceiveEvent(InputKey Key, EKeyState State) {
        EnsureInitialized();

        bool bChangedState = false;
        for (size_t i = 0; i < Keys.size(); i++) {
            if (Keys[i] == Key) {
                States[i] = State;
                bChangedState = true;
            }
        }

        if (!bIsSequence) {
            // Calculate new state
            bool bAllPressed = true;
            for (const auto &KeyState: States) {
                if (KeyState == EKeyState::KeyReleased) {
                    bAllPressed = false;
                    break;
                }
            }

            EKeyState NewState{bAllPressed ? EKeyState::KeyPressed : EKeyState::KeyReleased};

            if (NewState != CurrentState) {
                CurrentState = NewState;
                return true;
            }
        } else {
            auto CurrentTime = std::chrono::high_resolution_clock::now();
            bool bProgressSequence =
                    (CurrentKeyIndex == 0 || CurrentTime - LastKeyTime < TimeBetweenKeys)
                    && bChangedState
                    && State == EKeyState::KeyPressed
                    && Keys[CurrentKeyIndex] == Key;

            // if you screw up, you want to be able to start over at any point.
            if (!bProgressSequence && State == EKeyState::KeyPressed && Key == Keys[0]) {
                CurrentKeyIndex = 0;
                bProgressSequence = true;
            }

            if (bProgressSequence) {
                if (CurrentKeyIndex == Keys.size() - 1) {
                    CurrentState = EKeyState::KeyPressed;
                    CurrentKeyIndex = 0;
                    LastKeyTime = CurrentTime;
                    return true;
                }

                CurrentKeyIndex++;
                LastKeyTime = CurrentTime;
            } else if (State == EKeyState::KeyPressed) {
                CurrentKeyIndex = 0;
            }

            CurrentState = EKeyState::KeyReleased;
        }

        return false;
    }

    void InputChord::EnsureInitialized() {
        if (!bInitialized) {
            States.resize(Keys.size());
            for (auto &State: States) {
                State = EKeyState::KeyReleased;
            }
            LastKeyTime = std::chrono::high_resolution_clock::now();
            bInitialized = true;
        }
    }
} // OZZ
