// Copyright © Jodiah Jensen


#include "InventoryManagement/Utils/INV_InventoryStatics.h"

#include "InventoryManagement/Components/INV_InventoryComponent.h"
#include "Items/Components/INV_ItemComponent.h"
#include "Types/INV_GridTypes.h"

UINV_InventoryComponent* UINV_InventoryStatics::GetInventoryComponent(const APlayerController* PlayerController)
{
	if (!IsValid(PlayerController)) return nullptr;

	UINV_InventoryComponent* InventoryComponent = PlayerController->FindComponentByClass<UINV_InventoryComponent>();
	return InventoryComponent;
}

EINV_ItemCategory UINV_InventoryStatics::GetItemCategoryFromItemComp(UINV_ItemComponent* ItemComp)
{
	if (!IsValid(ItemComp)) return EINV_ItemCategory::None;

	return ItemComp->GetItemManifest().GetItemCategory();
}
