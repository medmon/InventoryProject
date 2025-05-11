// Copyright © Jodiah Jensen

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Inventory/InventoryBase/INV_InventoryBase.h"
#include "INV_SpatialInventory.generated.h"

class UWidgetSwitcher;
class UINV_InventoryGrid;

/**
 * 
 */
UCLASS()
class JJ_INVENTORY_API UINV_SpatialInventory : public UINV_InventoryBase
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UINV_InventoryGrid> Grid_Equipment;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UINV_InventoryGrid> Grid_Consumables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UINV_InventoryGrid> Grid_Crafting;

};
