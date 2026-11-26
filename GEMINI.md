# Ascendencia — Unreal Engine 5.8 Project Guide

## Project & Engine Overview
- **Engine**: Unreal Engine 5.8 (`C:\Program Files\Epic Games\UE_5.8`)
- **Build Settings**: `BuildSettingsVersion.V7`, `IncludeOrderVersion.Unreal5_8`, C++20
- **Primary Module**: `Ascendencia` (`ASCENDENCIA_API`), `Source/Ascendencia/`
- **Key Enabled Modules & Plugins**:
  - `GameplayAbilities`, `GameplayTags`, `GameplayTasks` (GAS)
  - `EnhancedInput`
  - `AIModule`, `NavigationSystem`, `StateTreeModule`, `GameplayStateTreeModule`
  - `Niagara`, `UMG`, `Slate`

## Code Structure & Naming Conventions
- Public headers live under `Source/Ascendencia/Public/` and implementations under `Source/Ascendencia/Private/`, grouped by domain:
  - `AbilitySystem/` (`UAscAbilitySystemComponent`, `UAscAttributeSet`)
  - `Actor/` (`AAscEffectActor`)
  - `Character/` (`AAscBaseCharacter`, `AAscPlayerCharacter`, `AAscCompanion`, `AAscEnemy`)
  - `Interaction/` (`ISelectableInterface`)
  - `Player/` (`AAscPlayerCharacterController`, `AAscPlayerState`, `AAscCRPGCameraRig`)
  - `UI/` (`HUD/`, `Widget/`, `WidgetController/`)
- Follow Unreal naming prefixes (`AAsc*`, `UAsc*`, `IAsc*` / `ISelectableInterface`, `FAsc*` / `F*`) and use `TObjectPtr<T>` for UObject pointer `UPROPERTY()` fields.
- Keep `#include "*.generated.h"` as the last include in UHT headers.

## Build, IntelliSense & File Generation Commands
- **Build Editor (Development)**:
  `"C:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat" AscendenciaEditor Win64 Development "C:\Users\mateu\Documents\Unreal Projects\Ascendencia\Ascendencia.uproject" -waitmutex`
- **After adding/removing `.h` or `.cpp` files**:
  Run UBT to refresh `.vscode/` and update `.vscode/compile_commands.json` so `clangd` indexes the new files:
  `"C:\Program Files\Epic Games\UE_5.8\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -ProjectFiles -VSCode -Project="C:\Users\mateu\Documents\Unreal Projects\Ascendencia\Ascendencia.uproject" -Game -Engine`
  followed by copying `.vscode\compileCommands_Ascendencia.json` to `.vscode\compile_commands.json`.
- **Important**:
  - Never manually edit generated files under `Intermediate/`, `Binaries/`, `DerivedDataCache/`, or `Saved/`.
  - Note that `.vscode/compile_commands.json` relies on `.clangd` at the project root for C++20 and MSVC preprocessor flags (`/D_MSVC_TRADITIONAL=0`).
