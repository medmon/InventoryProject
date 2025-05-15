// Copyright © Jodiah Jensen


#include "Widgets/Inventory/Spatial/INV_InventoryGrid.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryManagement/Components/INV_InventoryComponent.h"
#include "InventoryManagement/Utils/INV_InventoryStatics.h"
#include "Items/INV_InventoryItem.h"
#include "Items/Components/INV_ItemComponent.h"
#include "Widgets/Inventory/GridSlots/INV_GridSlot.h"
#include "Widgets/Utils/INV_WidgetUtils.h"


void UINV_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ConstructGrid();

	InventoryComponent = UINV_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAdded.AddDynamic(this, &ThisClass::AddItem);
}

FINV_SlotAvailabilityResult UINV_InventoryGrid::HasRoomForItem(const UINV_ItemComponent* ItemComponent) const
{
	return HasRoomForItem(ItemComponent->GetItemManifest());
	
}

FINV_SlotAvailabilityResult UINV_InventoryGrid::HasRoomForItem(const UINV_InventoryItem* Item) const
{
	return HasRoomForItem(Item->GetItemManifest());

}

FINV_SlotAvailabilityResult UINV_InventoryGrid::HasRoomForItem(const FINV_ItemManifest& Manifest) const
{
	return FINV_SlotAvailabilityResult();
}

void UINV_InventoryGrid::AddItem(UINV_InventoryItem* Item)
{
	if (!MatchesCategory(Item)) return;

	FINV_SlotAvailabilityResult Result = HasRoomForItem(Item);

	//Create a widget to show the item icon and add it to the correct spot on the grid
	
	FString GridName = this->GetName();	
	UE_LOG(LogTemp, Warning, TEXT("InventoryGrid::AddItem - %s"), *GridName);
}

void UINV_InventoryGrid::ConstructGrid()
{
	GridSlots.Reserve(Rows * Columns);

	for (int32 j=0; j < Rows; ++j)
	{
		for (int32 i=0; i < Columns; ++i)
		{
			UINV_GridSlot* GridSlot = CreateWidget<UINV_GridSlot>(this, GridSlotClass);
			CanvasPanel->AddChild(GridSlot);

			const FIntPoint TilePosition( i,j);
			
			GridSlot->SetTileIndex(UINV_WidgetUtils::GetIndexFromPosition(TilePosition, Columns));
			
			UCanvasPanelSlot* GridCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			GridCPS->SetSize(FVector2d(TileSize));
			GridCPS->SetPosition(TilePosition * TileSize);

			GridSlots.Add(GridSlot);
		}
	}
}

bool UINV_InventoryGrid::MatchesCategory(const UINV_InventoryItem* Item) const
{
	return Item->GetItemManifest().GetItemCategory() == ItemCategory;
}
