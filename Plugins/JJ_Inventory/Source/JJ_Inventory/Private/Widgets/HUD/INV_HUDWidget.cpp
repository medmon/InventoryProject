// Copyright © Jodiah Jensen


#include "Widgets/HUD/INV_HUDWidget.h"

#include "InventoryManagement/Components/INV_InventoryComponent.h"
#include "InventoryManagement/Utils/INV_InventoryStatics.h"
#include "Widgets/HUD/INV_InfoMessage.h"

void UINV_HUDWidget::NativeOnInitialized()
{
	 UINV_InventoryComponent* InventoryComponent = UINV_InventoryStatics::GetInventoryComponent(GetOwningPlayer());

	if(IsValid(InventoryComponent))
	{
		InventoryComponent->NoRoomInInventory.AddDynamic(this,&ThisClass::OnNoRoom);
	}
}

void UINV_HUDWidget::OnNoRoom()
{
	if (!IsValid(InfoMessage)) return;

	InfoMessage->SetMessage(FText::FromString("No Room In Inventory"));
	
}
