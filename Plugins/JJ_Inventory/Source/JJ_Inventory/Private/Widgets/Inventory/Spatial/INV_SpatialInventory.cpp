// Copyright © Jodiah Jensen


#include "Widgets/Inventory/SPatial/INV_SpatialInventory.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Widgets/Inventory/Spatial/INV_InventoryGrid.h"

void UINV_SpatialInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_Equipment->OnClicked.AddDynamic(this, &ThisClass::ShowEquipment);
	Button_Consumables->OnClicked.AddDynamic(this, &ThisClass::ShowConsumables);
	Button_Crafting->OnClicked.AddDynamic(this, &ThisClass::ShowCrafting);

	ShowEquipment();
	
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
