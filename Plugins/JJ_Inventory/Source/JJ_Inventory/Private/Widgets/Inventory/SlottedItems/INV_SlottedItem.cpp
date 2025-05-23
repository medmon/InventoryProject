// Copyright © Jodiah Jensen


#include "Widgets/Inventory/SlottedItems/INV_SlottedItem.h"

#include "Components/TextBlock.h"

UINV_InventoryItem* UINV_SlottedItem::GetInventoryItem() const
{
	 return InventoryItem.Get(); 
}

void UINV_SlottedItem::UpdateStackCount(int32 StackCount)
{
	if (StackCount > 0)
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Visible);
		Text_StackCount->SetText(FText::AsNumber(StackCount));
	}
	else
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}

	
}


