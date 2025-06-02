﻿// Copyright © Jodiah Jensen


#include "Widgets/Inventory/Spatial/INV_InventoryGrid.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryManagement/Components/INV_InventoryComponent.h"
#include "InventoryManagement/Utils/INV_InventoryStatics.h"
#include "Items/INV_InventoryItem.h"
#include "Items/Components/INV_ItemComponent.h"
#include "Items/Fragments/INV_FragmentTags.h"
#include "Items/Fragments/INV_ItemFragment.h"
#include "Widgets/Inventory/GridSlots/INV_GridSlot.h"
#include "Widgets/Inventory/SlottedItems/INV_SlottedItem.h"
#include "Widgets/Utils/INV_WidgetUtils.h"


void UINV_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ConstructGrid();

	InventoryComponent = UINV_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAdded.AddDynamic(this, &ThisClass::AddItem);
	InventoryComponent->OnStackChange.AddDynamic(this, &ThisClass::AddStacks);
}

FINV_SlotAvailabilityResult UINV_InventoryGrid::HasRoomForItem(const UINV_ItemComponent* ItemComponent) 
{
	return HasRoomForItem(ItemComponent->GetItemManifest());
	
}

FINV_SlotAvailabilityResult UINV_InventoryGrid::HasRoomForItem(const UINV_InventoryItem* Item) 
{
	return HasRoomForItem(Item->GetItemManifest());

}

FINV_SlotAvailabilityResult UINV_InventoryGrid::HasRoomForItem(const FINV_ItemManifest& Manifest) 
{
	FINV_SlotAvailabilityResult Result; 


	//determine if the item is stackable
	const FINV_StackableFragment* StackableFragment =
		Manifest.GetFragmentOfType<FINV_StackableFragment>();

	Result.bStackable = StackableFragment != nullptr;
	
	//determine how many stacks to add
    	const int32 MaxStackSize = Result.bStackable ? StackableFragment->GetMaxStackSize() : 1;
	int32 AmountToFill = Result.bStackable ? StackableFragment->GetStackCount() : 1 ;

	TSet<int32> CheckedIndices;
	//for each grid slot
	for (const auto& GridSlot : GridSlots )
	{
		if (AmountToFill == 0) break;

		//is this index claimed yet (the grid slot we are currently on)
		if ( IsIndexClaimed(CheckedIndices, GridSlot->GetIndex()) ) continue;

		//can the item fit here (within grid bounds)
		if (!IsInGridBounds(GridSlot->GetIndex(), GetItemDimensions(Manifest))) continue;
		
		
		TSet<int32> TentativelyClaimed;
		if ( !HasRoomAtIndex(GridSlot,
			GetItemDimensions(Manifest),
			CheckedIndices,
			TentativelyClaimed,
			Manifest.GetItemType(),
			MaxStackSize)
			)
		{
			continue;
		}
		
	// how much to fill
	const int32 AmountToFillInSlot = DetermineFillAmountForSlot(Result.bStackable, MaxStackSize, AmountToFill, GridSlot);
	if (AmountToFillInSlot == 0) continue;
		
	CheckedIndices.Append(TentativelyClaimed);


	//update amount left to fill
	//fill out the Result

		Result.TotalRoomToFill += AmountToFillInSlot;
		Result.SlotAvailabilities.Emplace(
			FINV_SlotAvailability{
				HasValidItem(GridSlot) ?  GridSlot->GetUpperLeftIndex() : GridSlot->GetIndex(),
				Result.bStackable ? AmountToFillInSlot : 0,
				HasValidItem(GridSlot)
			}
		);

		AmountToFill -= AmountToFillInSlot;
	// How much is the remainder
		Result.Remainder = AmountToFill;
		
		if (AmountToFill == 0) return Result;
		
	}
		
	return Result;
}

bool UINV_InventoryGrid::HasRoomAtIndex(const UINV_GridSlot* GridSlot,
		const FIntPoint& Dimensions,
		const TSet<int32>& CheckedIndices,
		TSet<int32>& OutTentativelyClaimed,
		const FGameplayTag& ItemType,
		const int32 MaxStackSize)
{
	//Is there room at this index? ( are there other items in the way?)
	bool bHasRoomAtIndex = true;
	
	UINV_InventoryStatics::ForEach2D(
		GridSlots,
		GridSlot->GetIndex(),
		Dimensions,
		Columns,
		[&](const UINV_GridSlot* SubGridSlot)
		{
			if ( CheckSlotConstraints(	GridSlot,
										SubGridSlot,
										CheckedIndices,
										OutTentativelyClaimed,
										ItemType,
										MaxStackSize) )
			{
				OutTentativelyClaimed.Add(SubGridSlot->GetIndex());
			}
			else
			{
				bHasRoomAtIndex = false;
			}
		});

	return bHasRoomAtIndex;
}

bool UINV_InventoryGrid::CheckSlotConstraints(	const UINV_GridSlot* GridSlot,
												const UINV_GridSlot* SubGridSlot,
												const TSet<int32>& CheckedIndices,
												TSet<int32>& OutTentativelyClaimed,
												const FGameplayTag& ItemType,
												const int32 MaxStackSize) const
{
			//Check any other important conditions

	// Index Claimed?
	if ( IsIndexClaimed(CheckedIndices, SubGridSlot->GetIndex()) ) return false;
	
	//Has Valid Item?
   if ( !HasValidItem(SubGridSlot) )
   {
	   OutTentativelyClaimed.Add(SubGridSlot->GetIndex());
   		return true;
   	
   }
	
	// is this grid slot an upper-left slot
   if (!IsUpperLeftSlot(GridSlot, SubGridSlot)) return false;

	//is this a stackable item ?
	const UINV_InventoryItem* SubItem = SubGridSlot->GetInventoryItem().Get();
	if (!SubItem->IsStackable()) return false;
	
	//is the item in this slot the same type as the item we're trying to add?
	if (!DoesItemTypeMatch(SubItem, ItemType)) return false;
	
	//if stackable is this slot at max stack size already?
	if (GridSlot->GetStackCount()>= MaxStackSize) return false;
	
	
	return true;
}

FIntPoint UINV_InventoryGrid::GetItemDimensions(const FINV_ItemManifest& Manifest) const
{
	const FINV_GridFragment* GridFragment = Manifest.GetFragmentOfType<FINV_GridFragment>();
	return GridFragment ? GridFragment->GetGridSize() : FIntPoint(1,1);
}

bool UINV_InventoryGrid::HasValidItem(const UINV_GridSlot* GridSlot) const
{
	return GridSlot->GetInventoryItem().IsValid();
}

bool UINV_InventoryGrid::IsUpperLeftSlot(const UINV_GridSlot* GridSlot, const UINV_GridSlot* SubGridSlot) const

{
	
	return SubGridSlot->GetUpperLeftIndex() == GridSlot->GetIndex();
}

bool UINV_InventoryGrid::DoesItemTypeMatch(const UINV_InventoryItem* SubItem, const FGameplayTag& ItemType) const
{
	return SubItem->GetItemManifest().GetItemType().MatchesTagExact(ItemType);
}

bool UINV_InventoryGrid::IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimensions) const
{
	if (StartIndex < 0 || StartIndex >= GridSlots.Num()) return false;

	const int32 EndColumn = (StartIndex % Columns) + ItemDimensions.X;
	const int32 EndRow = (StartIndex / Columns) + ItemDimensions.Y;
	

	return EndColumn <= Columns && EndRow <= Rows;
}

int32 UINV_InventoryGrid::DetermineFillAmountForSlot(const bool bStackable, const int32 MaxStackSize,
	const int32 AmountToFill, UINV_GridSlot* GridSlot) const
{
	//calculate room in the slot
	const int32 RoomInSlot = MaxStackSize - GetStackAmount(GridSlot);
	//if stackable, need the minimum between AmountToFill and RoomInSlot
	return bStackable ? FMath::Min(AmountToFill, RoomInSlot) : 1;
	
}

int32 UINV_InventoryGrid::GetStackAmount(const UINV_GridSlot* GridSlot) const
{
	int32 CurrentSlotStackCount = GridSlot->GetStackCount();
	//if we are at aq slot that doesn't hold the stack count, we must get the actual stack count
	if (const int32 UpperLeftIndex = GridSlot->GetUpperLeftIndex(); UpperLeftIndex != INDEX_NONE)
	{
		UINV_GridSlot* UpperLeftGridSlot = GridSlots[UpperLeftIndex];
		CurrentSlotStackCount = UpperLeftGridSlot->GetStackCount();
	}

	return CurrentSlotStackCount;
}

void UINV_InventoryGrid::AddStacks(const FINV_SlotAvailabilityResult& Result)
{
	if (!MatchesCategory(Result.Item.Get())) return;

	for (const auto& Availability : Result.SlotAvailabilities)
	{
		if (Availability.bItemAtIndex)
		{
			const auto& GridSlot = GridSlots[Availability.Index];
			const auto& SlottedItem = SlottedItems.FindChecked(Availability.Index);
			SlottedItem->UpdateStackCount(GridSlot->GetStackCount() + Availability.AmountToFill);
			GridSlot->SetStackCount(GridSlot->GetStackCount() + Availability.AmountToFill);
			
		}
		else
		{
			AddItemAtIndex(Result.Item.Get(), Availability.Index, Result.bStackable, Availability.AmountToFill);
			UpdateGridSlots(Result.Item.Get(), Availability.Index, Result.bStackable, Availability.AmountToFill);
		}
	}
}

void UINV_InventoryGrid::AddItem(UINV_InventoryItem* Item)
{
	if (!MatchesCategory(Item)) return;

	FINV_SlotAvailabilityResult Result = HasRoomForItem(Item);

	AddItemToIndices(Result, Item);
	
	FString GridName = this->GetName();	
	UE_LOG(LogTemp, Warning, TEXT("InventoryGrid::AddItem - %s"), *GridName);
}

void UINV_InventoryGrid::AddItemToIndices(const FINV_SlotAvailabilityResult& Result, UINV_InventoryItem* NewItem)
{
	for (const auto& Availability : Result.SlotAvailabilities)
	{
		AddItemAtIndex(
			NewItem,
			Availability.Index,
			Result.bStackable,
			Availability.AmountToFill);

		UpdateGridSlots(NewItem, Availability.Index, Result.bStackable, Availability.AmountToFill);
		
	}
	
}

void UINV_InventoryGrid::AddItemAtIndex(UINV_InventoryItem* Item, const int32 Index, const bool bStackable,
	const int32 StackAmount)
{
	const FINV_GridFragment* GridFragment = GetFragment<FINV_GridFragment>(Item, FragmentTags::GridFragment);
	const FINV_ImageFragment* ImageFragment = GetFragment<FINV_ImageFragment>(Item, FragmentTags::IconFragment);

	if (!GridFragment || !ImageFragment) return;

	UINV_SlottedItem* SlottedItem = CreateSlottedItem(Item, bStackable, StackAmount, GridFragment, ImageFragment, Index);
	AddSlottedItemToCanvas(Index, GridFragment, SlottedItem);


	SlottedItems.Add(Index, SlottedItem);
	

}

UINV_SlottedItem* UINV_InventoryGrid::CreateSlottedItem(
	UINV_InventoryItem* Item,
	const bool bStackable,
	const int32 StackAmount,
	const FINV_GridFragment* GridFragment,
	const FINV_ImageFragment* ImageFragment,
	const int32 Index)
{
	UINV_SlottedItem* SlottedItem = CreateWidget<UINV_SlottedItem>(GetOwningPlayer(), SlottedItemClass);
	SlottedItem->SetInventoryItem(Item);
	SetSlottedItemImage(SlottedItem, GridFragment, ImageFragment);
	SlottedItem->SetGridIndex(Index);

	SlottedItem->SetIsStackable(bStackable);
	const int32 StackUpdateAmount = bStackable ? StackAmount : 0;
	SlottedItem->UpdateStackCount(StackUpdateAmount);
		
	
	return SlottedItem;
	
}

void UINV_InventoryGrid::AddSlottedItemToCanvas(const int32 Index, const FINV_GridFragment* GridFragment,
	UINV_SlottedItem* SlottedItem) const
{
	CanvasPanel->AddChild(SlottedItem);
	UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SlottedItem);
	CanvasSlot->SetSize(GetDrawSize(GridFragment));

	const FVector2D DrawPos = UINV_WidgetUtils::GetPositionFromIndex(Index, Columns) * TileSize;
	const FVector2D DrawPosWithPadding = DrawPos + FVector2D( GridFragment->GetGridPadding());

	CanvasSlot->SetPosition(DrawPosWithPadding);
	
}

void UINV_InventoryGrid::UpdateGridSlots(UINV_InventoryItem* NewItem, const int32 Index, bool bStackableItem, int32 StackAmount)
{
	check(GridSlots.IsValidIndex(Index));

	if (bStackableItem)
	{
		GridSlots[Index]->SetStackCount(StackAmount);
		
	}

	const FINV_GridFragment* GridFragment = GetFragment<FINV_GridFragment>(NewItem, FragmentTags::GridFragment);

	const FIntPoint Dimensions = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1,1) ;

	UINV_InventoryStatics::ForEach2D(GridSlots, Index, Dimensions, Columns, [&](UINV_GridSlot* GridSlot)
	{
		GridSlot->SetInventoryItem(NewItem);
		GridSlot->SetUpperLeftIndex(Index);
		GridSlot->SetOccupiedTexture();
		GridSlot->SetAvailable(false);
	});
	
	UINV_GridSlot* GridSlot = GridSlots[Index];
	GridSlot->SetOccupiedTexture();
}

bool UINV_InventoryGrid::IsIndexClaimed(const TSet<int32>& CheckedIndices, const int32 Index) const
{
	return CheckedIndices.Contains(Index);
}


FVector2D UINV_InventoryGrid::GetDrawSize(const FINV_GridFragment* GridFragment) const
{
	const float IconTileWidth = TileSize - GridFragment->GetGridPadding() * 2;
	FVector2D IconSize = GridFragment->GetGridSize() * IconTileWidth;

	return IconSize;

}

void UINV_InventoryGrid::SetSlottedItemImage(const UINV_SlottedItem* SlottedItem, const FINV_GridFragment* GridFragment,
	const FINV_ImageFragment* ImageFragment) const
{
	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;

	Brush.ImageSize = GetDrawSize(GridFragment); 

	SlottedItem->SetImageBrush(Brush);

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

