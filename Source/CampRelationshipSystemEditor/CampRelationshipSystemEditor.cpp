// Copyright Epic Games, Inc. All Rights Reserved.

#include "CampRelationshipSystemEditor.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include <Interfaces/IPluginManager.h>
#include "Customization/CampInfoOperationPropertyTypeCustom.h"

#define LOCTEXT_NAMESPACE "FCampRelationshipSystemEditorModule"

void FCampRelationshipSystemEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.RegisterCustomPropertyTypeLayout(FName("CR_CampInfo")/*CampInfoOperation*/, FOnGetPropertyTypeCustomizationInstance::CreateStatic(&ICampInfoOperationPropertyTypeCustom::MakeInstance));
}

void FCampRelationshipSystemEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.UnregisterCustomClassLayout("FCR_CampInfoOperation");
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCampRelationshipSystemEditorModule, CampRelationshipSystemEditor)