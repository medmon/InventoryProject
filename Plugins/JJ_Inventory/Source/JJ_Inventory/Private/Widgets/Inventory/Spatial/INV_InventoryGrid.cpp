// Copyright © Jodiah Jensen


#include "Widgets/Inventory/Spatial/INV_InventoryGrid.h"

#include "JJ_Inventory.h"
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
#include "Widgets/Inventory/HoverItem/INV_HoverItem.h"
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

void UINV_InventoryGrid::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const FVector2d CanvasPosition = UINV_WidgetUtils::GetWidgetPosition(CanvasPanel);
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());

	if (CursorExitedCanvas(CanvasPosition, UINV_WidgetUtils::GetWidgetSize(CanvasPanel), MousePosition))
	{
		return;
	}
	
	UpdateTileParameters(CanvasPosition, MousePosition);
	
}

void UINV_InventoryGrid::UpdateTileParameters(const FVector2D& CanvasPosition, const FVector2D& MousePosition)
{
	// if mouse not in canvas panel, early return

	if (!bMouseWithinCanvas) return;
	
	// Calculate the tile quadrant, tile index and coordinates

	const FIntPoint HoveredTileCoordinates = CalculateHoveredCoordinates(CanvasPosition, MousePosition);

	LastTileParameters = TileParameters;
	TileParameters.TileCoordinates = HoveredTileCoordinates;
	TileParameters.TileIndex = UINV_WidgetUtils::GetIndexFromPosition(HoveredTileCoordinates, Columns);
	TileParameters.TileQuadrant = CalculateTileQuadrant(CanvasPosition, MousePosition);
	
	// handle highligh/unhighlight of the grid slots
	OnTileParametersUpdated(TileParameters);
	
}

void UINV_InventoryGrid::OnTileParametersUpdated(const FINV_TileParameters Parameters)
{
	if (!IsValid(HoverItem)) return;
	
	// Get Hover Item's Dimensions
	const FIntPoint Dimensions = HoverItem->GetGridDimensions();
	
	// Calculate the starting coords for highlighting
	const FIntPoint StartingCoordinate = CalculateStartingCoordinate(Parameters.TileCoordinates, Dimensions, Parameters.TileQuadrant);
	ItemDropIndex = UINV_WidgetUtils::GetIndexFromPosition(StartingCoordinate, Columns);
	
	// Check Hover Position
	CurrentQueryResult = CheckHoverPosition(StartingCoordinate, Dimensions);

	if (CurrentQueryResult.bHasSpace)
	{
		HighlightSlots(ItemDropIndex, Dimensions);
		return;
	}
	UnHighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);

	if (CurrentQueryResult.ValidItem.IsValid() && GridSlots.IsValidIndex(CurrentQueryResult.UpperLeftIndex))
	{
		//TODO:  there is a single item in this slot, we can swap or add stacks
		const FINV_GridFragment* GridFragment = GetFragment<FINV_GridFragment>(CurrentQueryResult.ValidItem.Get(), FragmentTags::GridFragment);
		if (!GridFragment) return;

		ChangeHoverType(CurrentQueryResult.UpperLeftIndex, GridFragment->GetGridSize(), EINV_GridSlotState::GrayedOut);
	}
	
}

FINV_SpaceQueryResult UINV_InventoryGrid::CheckHoverPosition(const FIntPoint& Position,
	const FIntPoint& Dimensions) 
{
	FINV_SpaceQueryResult Result;
	
	// are the dimensions in grid bounds
	if (!IsInGridBounds(UINV_WidgetUtils::GetIndexFromPosition(Position, Columns), Dimensions)) return Result;

	Result.bHasSpace = true;
	
	// are there item(s) in the way (check all tiles in the dimensions)
	TSet<int32> OccupiedUpperLeftIndices;
	UINV_InventoryStatics::ForEach2D(GridSlots, UINV_WidgetUtils::GetIndexFromPosition(Position, Columns), Dimensions, Columns,[&](const UINV_GridSlot* GridSlot)
	{
		if (GridSlot->GetInventoryItem().IsValid())
		{
			OccupiedUpperLeftIndices.Add(GridSlot->GetUpperLeftIndex());
			Result.bHasSpace = false;
		}
		
	});
		
	// is there only 1 item in the way (can we swap hover item)
	if (OccupiedUpperLeftIndices.Num() == 1)   // single item at position, is valid for swapping/combining
	{
		const int32 Index = *OccupiedUpperLeftIndices.CreateConstIterator();
		Result.ValidItem = GridSlots[Index]->GetInventoryItem();
		Result.UpperLeftIndex = GridSlots[Index]->GetUpperLeftIndex();
		
	}
		
	return Result;
}

bool UINV_InventoryGrid::CursorExitedCanvas(const FVector2D BoundaryPos, const FVector2D BoundarySize,
	const FVector2D Location)
{
	bLastMouseWithinCanvas = bMouseWithinCanvas;
	bMouseWithinCanvas = UINV_WidgetUtils::IsWithinBounds(BoundaryPos, BoundarySize, Location);
	if (!bMouseWithinCanvas && bLastMouseWithinCanvas)
	{
		UnHighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);
		return true;
	}

	return false;
	
}

void UINV_InventoryGrid::HighlightSlots(const int32 Index, const FIntPoint& Dimensions)
{
	if (!bMouseWithinCanvas) return;

	UnHighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);
	
	UINV_InventoryStatics::ForEach2D(GridSlots, Index, Dimensions, Columns, [&](UINV_GridSlot* GridSlot)
	{
		GridSlot->SetOccupiedTexture();
	});

	LastHighlightedDimensions = Dimensions;
	LastHighlightedIndex = Index;
	
	
}
void UINV_InventoryGrid::UnHighlightSlots(const int32 Index, const FIntPoint& Dimensions)
{
	UINV_InventoryStatics::ForEach2D(GridSlots, Index, Dimensions, Columns, [&](UINV_GridSlot* GridSlot)
	{
		if (GridSlot->IsAvailable())
		{
			GridSlot->SetUnoccupiedTexture();				
		}
		else
		{
			GridSlot->SetOccupiedTexture();
		}
	});

}

void UINV_InventoryGrid::ChangeHoverType(const int32 Index, const FIntPoint& Dimensions,
	EINV_GridSlotState GridSlotState)
{
	UnHighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);
	UINV_InventoryStatics::ForEach2D(GridSlots,Index,Dimensions,Columns,[State = GridSlotState](UINV_GridSlot* GridSlot)
	{
		switch  (State)
		{
		case EINV_GridSlotState::Occupied :
			GridSlot->SetOccupiedTexture();
			break;
		case EINV_GridSlotState::Unoccupied :
			GridSlot->SetUnoccupiedTexture();
			break;
		case EINV_GridSlotState::Selected :
			GridSlot->SetSelectedTexture();
			break;
		case EINV_GridSlotState::GrayedOut :
			GridSlot->SetGrayedOutTexture();
			break;
			
		}
		
	});

	LastHighlightedIndex = Index;
	LastHighlightedDimensions = Dimensions;
	
}


FIntPoint UINV_InventoryGrid::CalculateStartingCoordinate(const FIntPoint& Coordinate, const FIntPoint& Dimensions,
                                                          const EINV_TileQuadrant Quadrant) const
{
	const int32 HasEvenWidth = Dimensions.X % 2 == 0 ? 1 : 0;
	const int32 HasEvenHeight = Dimensions.Y % 2 == 0 ? 1 : 0;

	FIntPoint StartingCoord;
	switch (Quadrant)
	{
		case EINV_TileQuadrant::TopLeft:
			StartingCoord.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimensions.X);
			StartingCoord.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimensions.Y);
		break;
		
		case EINV_TileQuadrant::TopRight:
			StartingCoord.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimensions.X) + HasEvenWidth;
			StartingCoord.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimensions.Y);
			
		break;

		case EINV_TileQuadrant::BottomLeft:
			StartingCoord.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimensions.X);
			StartingCoord.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimensions.Y) + HasEvenHeight;

		break;

		case EINV_TileQuadrant::BottomRight:
			StartingCoord.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimensions.X) + HasEvenWidth;
			StartingCoord.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimensions.Y) + HasEvenHeight;

		break;
	default:
		UE_LOG(LogInventory, Error, TEXT("Invalid Quadrant in UINV_InventoryGrid::CalculateStartingCoordinates()."));
		return FIntPoint(-1,-1);
	}

	return StartingCoord;
	
}


FIntPoint UINV_InventoryGrid::CalculateHoveredCoordinates(const FVector2D& CanvasPosition,
                                                          const FVector2D& MousePosition) const
{
	return FIntPoint {	static_cast<int32>(FMath::FloorToInt((MousePosition.X - CanvasPosition.X) / TileSize)),
						static_cast<int32>(FMath::FloorToInt((MousePosition.Y - CanvasPosition.Y) / TileSize))
						};

}

EINV_TileQuadrant UINV_InventoryGrid::CalculateTileQuadrant(const FVector2D& CanvasPosition,
	const FVector2D& MousePosition) const
{
	// Calculate relative position within the current tile
	const float TileLocalX = FMath::Fmod(MousePosition.X - CanvasPosition.X, TileSize);
	const float TileLocalY = FMath::Fmod(MousePosition.Y - CanvasPosition.Y, TileSize);

	// Determine which quadrant the mouse is in

	const bool bIsTop = TileLocalY < TileSize / 2;
	const bool bIsLeft = TileLocalX < TileSize / 2;

	EINV_TileQuadrant HoveredTileQuadrant{EINV_TileQuadrant::None};
	if (bIsTop && bIsLeft) HoveredTileQuadrant = EINV_TileQuadrant::TopLeft;
	else if (bIsTop && !bIsLeft) HoveredTileQuadrant = EINV_TileQuadrant::TopRight;
	else if (!bIsTop && bIsLeft) HoveredTileQuadrant = EINV_TileQuadrant::BottomLeft;
	else if (!bIsTop && !bIsLeft) HoveredTileQuadrant = EINV_TileQuadrant::BottomRight;

	return HoveredTileQuadrant;
	
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

bool UINV_InventoryGrid::IsRightClick(const FPointerEvent& MouseEvent) const
{
	return MouseEvent.GetEffectingButton() == EKeys::RightMouseButton;
}

bool UINV_InventoryGrid::IsLeftClick(const FPointerEvent& MouseEvent) const
{
	return MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
}

void UINV_InventoryGrid::PickUp(UINV_InventoryItem* ClickedInventoryItem, const int32 GridIndex)
{
	//assign the hover item
	AssignHoverItem(ClickedInventoryItem, GridIndex, GridIndex);
	
	// remove clicked item from grid
	RemoveItemFromGrid(ClickedInventoryItem, GridIndex);
	
}

void UINV_InventoryGrid::AssignHoverItem(UINV_InventoryItem* InventoryItem)
{
	if (!IsValid(HoverItem))
	{
		HoverItem = CreateWidget<UINV_HoverItem>(GetOwningPlayer(), HoverItemClass);
	}

	const FINV_GridFragment* GridFragment = GetFragment<FINV_GridFragment>(InventoryItem, FragmentTags::GridFragment);
	const FINV_ImageFragment* ImageFragment = GetFragment<FINV_ImageFragment>(InventoryItem, FragmentTags::IconFragment);

	if (!GridFragment || !ImageFragment) return;

	const FVector2D DrawSize = GetDrawSize(GridFragment);
	
	FSlateBrush IconBrush;
	IconBrush.SetResourceObject(ImageFragment->GetIcon());
	IconBrush.DrawAs = ESlateBrushDrawType::Image;
	IconBrush.ImageSize = DrawSize * UWidgetLayoutLibrary::GetViewportScale(this);

	HoverItem->SetImageBrush(IconBrush);
	HoverItem->SetGridDimensions(GridFragment->GetGridSize());
	HoverItem->SetInventoryItem(InventoryItem);
	HoverItem->SetIsStackable(InventoryItem->IsStackable());

	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, HoverItem);
	
}

void UINV_InventoryGrid::AssignHoverItem(UINV_InventoryItem* InventoryItem, const int32 GridIndex,
	const int32 PreviousGridIndex)
{
	AssignHoverItem(InventoryItem);

	HoverItem->SetPreviousGridIndex(PreviousGridIndex);
	HoverItem->UpdateStackCount(InventoryItem->IsStackable() ? GridSlots[GridIndex]->GetStackCount() : 0);
}

void UINV_InventoryGrid::RemoveItemFromGrid(UINV_InventoryItem* InventoryItem, const int32 GridIndex)
{
	const FINV_GridFragment* GridFragment = GetFragment<FINV_GridFragment>(InventoryItem, FragmentTags::GridFragment);
	if (!GridFragment) return;

	UINV_InventoryStatics::ForEach2D(GridSlots, GridIndex, GridFragment->GetGridSize(), Columns, [&](UINV_GridSlot* GridSlot)
	{
		GridSlot->SetInventoryItem(nullptr);
		GridSlot->SetUpperLeftIndex(INDEX_NONE);
		GridSlot->SetAvailable(true);
		GridSlot->SetUnoccupiedTexture();
		GridSlot->SetStackCount(0);
		
	});

	if (SlottedItems.Contains(GridIndex))
	{
		TObjectPtr<UINV_SlottedItem> FoundSlottedItem;

		//remove from slottedItems array
		SlottedItems.RemoveAndCopyValue(GridIndex, FoundSlottedItem);

		//remove from viewport
		FoundSlottedItem->RemoveFromParent();

	}
	
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

void UINV_InventoryGrid::OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	check(GridSlots.IsValidIndex(GridIndex));
	UINV_InventoryItem* ClickedInventoryItem = GridSlots[GridIndex]->GetInventoryItem().Get();

	if (!IsValid(HoverItem) && IsLeftClick(MouseEvent))
	{
		// take slotted item here - assign the hover item and remove the slotted item from the grid
		PickUp(ClickedInventoryItem, GridIndex);
		return;
	}
	
	// Do the hover item and the clicked inventory item share an item type, and are they stackable
	if (IsSameStackable(ClickedInventoryItem))
	{
		// should we swap their stack counts?
		// Should we consume the hover item?
		// Should we fill in the stacks of the clicked item (and not consume the hover item)
		// is there no room in the clicked slot
		return;
	}
	
	// swap with the hover item
	SwapWithHoverItem(ClickedInventoryItem, GridIndex);

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

	SlottedItem->OnSlottedItemClicked.AddDynamic(this, &ThisClass::OnSlottedItemClicked);
	
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

			//bind callbacks to delegates
			GridSlot->GridSlotClicked.AddDynamic(this, &ThisClass::OnGridSlotClicked);
			GridSlot->GridSlotHovered.AddDynamic(this, &ThisClass::OnGridSlotHovered);
			GridSlot->GridSlotUnhovered.AddDynamic(this, &ThisClass::OnGridSlotUnhovered);
			
			
		}
	}
}

void UINV_InventoryGrid::OnGridSlotClicked(int32 GridIndex,const FPointerEvent& MouseEvent)
{
	if (!IsValid(HoverItem)) return;
	if (!GridSlots.IsValidIndex(ItemDropIndex)) return;

	if (CurrentQueryResult.ValidItem.IsValid() && GridSlots.IsValidIndex(CurrentQueryResult.UpperLeftIndex))
	{
		OnSlottedItemClicked(CurrentQueryResult.UpperLeftIndex, MouseEvent);
		return;
	}

	auto GridSlot = GridSlots[ItemDropIndex];

	if (!GridSlot->GetInventoryItem().IsValid())
	{
		// put item down at this index
		PutDownOnIndex(ItemDropIndex);
	}
	
	
}

void UINV_InventoryGrid::PutDownOnIndex(const int32 Index)
{
	AddItemAtIndex(HoverItem->GetInventoryItem(), Index, HoverItem->IsStackable(), HoverItem->GetStackCount() );
	UpdateGridSlots(HoverItem->GetInventoryItem(), Index, HoverItem->IsStackable(), HoverItem->GetStackCount());
	ClearHoverItem();
}

void UINV_InventoryGrid::ClearHoverItem()
{
	if (!IsValid(HoverItem)) return;

	HoverItem->SetInventoryItem(nullptr);
	//HoverItem->SetIsStackable(false);
	HoverItem->SetPreviousGridIndex(INDEX_NONE);
	HoverItem->UpdateStackCount(0);
	HoverItem->SetImageBrush(FSlateNoResource());

	HoverItem->RemoveFromParent();
	HoverItem = nullptr;

	// Show Mouse Cursor
	ShowCursor();
	
	
}

UUserWidget* UINV_InventoryGrid::GetVisibleCursorWidget()
{
	if (!IsValid(GetOwningPlayer())) return nullptr;

	if (!IsValid(VisibleCursorWidget))
	{
		VisibleCursorWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), VisibleCursorWidgetClass);
	}

	return VisibleCursorWidget;
}

UUserWidget* UINV_InventoryGrid::GetHiddenCursorWidget()
{
	if (!IsValid(GetOwningPlayer())) return nullptr;

	if (!IsValid(HiddenCursorWidget))
	{
		HiddenCursorWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), HiddenCursorWidgetClass);
	}

	return HiddenCursorWidget;

}

bool UINV_InventoryGrid::IsSameStackable(const UINV_InventoryItem* ClickedInventoryItem) const
{
	const bool bIsSameItem = ClickedInventoryItem == HoverItem->GetInventoryItem();
	const bool bIsStackable = ClickedInventoryItem->IsStackable();
	return bIsSameItem && bIsStackable && HoverItem->GetItemType().MatchesTagExact(ClickedInventoryItem->GetItemManifest().GetItemType());
	
}

void UINV_InventoryGrid::SwapWithHoverItem(UINV_InventoryItem* ClickedInventoryItem, const int32 GridIndex)
{
	if (!IsValid(HoverItem)) return;

	UINV_InventoryItem* TempInventoryItem = HoverItem->GetInventoryItem();
	const int32 TempStackCount = HoverItem->GetStackCount();
	const bool bTempIsStackable = HoverItem->IsStackable();

	// Keep the same previou grid index
	AssignHoverItem(ClickedInventoryItem, GridIndex, HoverItem->GetPreviousGridIndex());
	RemoveItemFromGrid(ClickedInventoryItem, GridIndex);
	AddItemAtIndex(TempInventoryItem, ItemDropIndex, bTempIsStackable, TempStackCount);
	UpdateGridSlots(TempInventoryItem, ItemDropIndex, bTempIsStackable, TempStackCount);
	
	
}

void UINV_InventoryGrid::ShowCursor()
{
	if (!IsValid(GetOwningPlayer())) return;
	
	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, GetVisibleCursorWidget());
}

void UINV_InventoryGrid::HideCursor()
{
	if (!IsValid(GetOwningPlayer())) return;
	
	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, GetHiddenCursorWidget());

}


void UINV_InventoryGrid::OnGridSlotHovered(int32 GridIndex,const FPointerEvent& MouseEvent)
{
	if (IsValid(HoverItem)) return;

	UINV_GridSlot* GridSlot = GridSlots[GridIndex];
	if (GridSlot->IsAvailable())
	{
		GridSlot->SetOccupiedTexture();
	}
	
}

void UINV_InventoryGrid::OnGridSlotUnhovered(int32 GridIndex,const FPointerEvent& MouseEvent)
{
	if (IsValid(HoverItem)) return;

	UINV_GridSlot* GridSlot = GridSlots[GridIndex];
	if (GridSlot->IsAvailable())
	{
		GridSlot->SetUnoccupiedTexture();
	}

}



bool UINV_InventoryGrid::MatchesCategory(const UINV_InventoryItem* Item) const
{
	return Item->GetItemManifest().GetItemCategory() == ItemCategory;
}

