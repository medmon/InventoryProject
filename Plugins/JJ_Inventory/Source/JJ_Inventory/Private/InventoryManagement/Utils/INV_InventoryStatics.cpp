// Copyright © Jodiah Jensen


#include "INV_InventoryStatics.h"

#include "InventoryManagement/Components/INV_InventoryComponent.h"

UINV_InventoryComponent* UINV_InventoryStatics::GetInventoryComponent(const APlayerController* PlayerController)
{
	if (!IsValid(PlayerController)) return nullptr;

	UINV_InventoryComponent* InventoryComponent = PlayerController->FindComponentByClass<UINV_InventoryComponent>();
	return InventoryComponent;
}
