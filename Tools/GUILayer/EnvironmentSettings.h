// Copyright 2015 XLGAMES Inc.
//
// Distributed under the MIT License (See
// accompanying file "LICENSE" or the website
// http://www.opensource.org/licenses/mit-license.php)

#pragma once

#include "FlexGobInterface.h"
#include "CLIXAutoPtr.h"

namespace PlatformRig { class EnvironmentSettings; }
using namespace System;
using namespace System::Collections::Generic;

namespace GUILayer
{
    PlatformRig::EnvironmentSettings
        BuildEnvironmentSettings(
            const EditorDynamicInterface::FlexObjectType& flexGobInterface,
            const EditorDynamicInterface::FlexObjectType::Object& obj);

    using EnvSettingsVector = std::vector<std::pair<std::string, PlatformRig::EnvironmentSettings>>;
    EnvSettingsVector BuildEnvironmentSettings(
        const EditorDynamicInterface::FlexObjectType& flexGobInterface);

    ref class EditorSceneManager;

    public ref class EnvironmentSettingsSet
    {
    public:
        clix::auto_ptr<EnvSettingsVector> _settings;
        property IEnumerable<String^>^ Names { IEnumerable<String^>^ get(); }

        void AddDefault();
        const PlatformRig::EnvironmentSettings& GetSettings(String^ name);

        EnvironmentSettingsSet(EditorSceneManager^ scene);
        ~EnvironmentSettingsSet();
    };
}
