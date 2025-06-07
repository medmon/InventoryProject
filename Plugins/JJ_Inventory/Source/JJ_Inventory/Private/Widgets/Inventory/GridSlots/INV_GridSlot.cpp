// Copyright © Jodiah Jensen


#include "Widgets/Inventory/GridSlots/INV_GridSlot.h"
#include "Items/INV_InventoryItem.h"

#include "Components/Image.h"

void UINV_GridSlot::NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	Super::NativeOnMouseEnter(MyGeometry, MouseEvent);

	GridSlotHovered.Broadcast(TileIndex, MouseEvent);
}

void UINV_GridSlot::NativeOnMouseLeave(const FPointerEvent& MouseEvent)
{
	Super::NativeOnMouseLeave(MouseEvent);

	GridSlotUnhovered.Broadcast(TileIndex, MouseEvent);

	
}

FReply UINV_GridSlot::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	GridSlotClicked.Broadcast(TileIndex, MouseEvent);	
	return FReply::Handled();
}





void UINV_GridSlot::SetInventoryItem(TWeakObjectPtr<UINV_InventoryItem> Item)
{
	InventoryItem = Item;
}

void UINV_GridSlot::SetUnoccupiedTexture()
{
	GridSlotState = EINV_GridSlotState::Unoccupied;
	Image_GridSlot->SetBrush(Brush_Unoccupied);
}

void UINV_GridSlot::SetOccupiedTexture()
{
	GridSlotState = EINV_GridSlotState::Occupied;
	Image_GridSlot->SetBrush(Brush_Occupied);
}

void UINV_GridSlot::SetSelectedTexture()
{
	GridSlotState = EINV_GridSlotState::Selected;
	Image_GridSlot->SetBrush(Brush_Selected);
}

void UINV_GridSlot::SetGrayedOutTexture()
{
	GridSlotState = EINV_GridSlotState::GrayedOut;
	Image_GridSlot->SetBrush(Brush_GrayedOut);
}





