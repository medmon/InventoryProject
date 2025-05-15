// Copyright © Jodiah Jensen


#include "Widgets/Inventory/SPatial/INV_SpatialInventory.h"

#include "JJ_Inventory.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Items/Components/INV_ItemComponent.h"
#include "Widgets/Inventory/Spatial/INV_InventoryGrid.h"

void UINV_SpatialInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_Equipment->OnClicked.AddDynamic(this, &ThisClass::ShowEquipment);
	Button_Consumables->OnClicked.AddDynamic(this, &ThisClass::ShowConsumables);
	Button_Crafting->OnClicked.AddDynamic(this, &ThisClass::ShowCrafting);

	ShowEquipment();
	
}

FINV_SlotAvailabilityResult UINV_SpatialInventory::HasRoomForItem(UINV_ItemComponent* ItemComponent) const
{
	switch (ItemComponent->GetItemManifest().GetItemCategory())
	{
		case EINV_ItemCategory::Consumable :
			return Grid_Consumables->HasRoomForItem(ItemComponent);
		case EINV_ItemCategory::Crafting :
			return Grid_Crafting->HasRoomForItem(ItemComponent);
		case EINV_ItemCategory::Equipment :
			return Grid_Equipment->HasRoomForItem(ItemComponent);
		default :
			UE_LOG(LogInventory, Error, TEXT("Item Category invalid for ItemComponent ( is it 'None'?)")) 
			return FINV_SlotAvailabilityResult();
	}
}

void UINV_SpatialInventory::ShowEquipment()
{
	SetActiveGrid(Grid_Equipment, Button_Equipment);
}

void UINV_SpatialInventory::ShowConsumables()
{
	SetActiveGrid(Grid_Consumables, Button_Consumables);
}

void UINV_SpatialInventory::ShowCrafting()
{
	SetActiveGrid(Grid_Crafting, Button_Crafting);
}

void UINV_SpatialInventory::SetActiveGrid(UINV_InventoryGrid* Grid, UButton* Button)
{
	Button_Equipment->SetIsEnabled(true);
	Button_Consumables->SetIsEnabled(true);
	Button_Crafting->SetIsEnabled(true);

	Button->SetIsEnabled(false);

	Switcher->SetActiveWidget(Grid);
}
