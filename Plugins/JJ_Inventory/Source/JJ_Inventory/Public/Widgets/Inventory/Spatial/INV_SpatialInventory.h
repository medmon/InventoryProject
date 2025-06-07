// Copyright © Jodiah Jensen

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Inventory/InventoryBase/INV_InventoryBase.h"
#include "INV_SpatialInventory.generated.h"

class UButton;
class UWidgetSwitcher;
class UINV_InventoryGrid;

/**
 * 
 */
UCLASS()
class JJ_INVENTORY_API UINV_SpatialInventory : public UINV_InventoryBase
{
	GENERATED_BODY()
public:

	virtual void NativeOnInitialized() override;

	virtual FINV_SlotAvailabilityResult HasRoomForItem(UINV_ItemComponent* ItemComponent) const override;
	

	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UINV_InventoryGrid> Grid_Equipment;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UINV_InventoryGrid> Grid_Consumables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UINV_InventoryGrid> Grid_Crafting;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Equipment;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Consumables;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Crafting;

	UFUNCTION()
	void ShowEquipment();

	UFUNCTION()
	void ShowConsumables();

	UFUNCTION()
	void ShowCrafting();

	void SetActiveGrid(UINV_InventoryGrid* Grid, UButton* Button);
	TWeakObjectPtr<UINV_InventoryGrid> ActiveGrid;
};
