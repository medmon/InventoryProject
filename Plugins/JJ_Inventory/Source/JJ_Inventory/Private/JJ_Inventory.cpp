// Copyright Epic Games, Inc. All Rights Reserved.

#include "JJ_Inventory.h"

#define LOCTEXT_NAMESPACE "FJJ_InventoryModule"

void FJJ_InventoryModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FJJ_InventoryModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FJJ_InventoryModule, JJ_Inventory)