// Copyright © Jodiah Jensen

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/Fragments/INV_ItemFragment.h"
#include "Types/INV_GridTypes.h"

#include "INV_InventoryGrid.generated.h"


enum class EINV_GridSlotState : uint8;
class UINV_HoverItem;
class UINV_SlottedItem;
class UINV_ItemComponent;
struct FINV_ItemManifest;
class UINV_InventoryComponent;
class UCanvasPanel;
class UINV_GridSlot;
/**
 * 
 */
UCLASS()
class JJ_INVENTORY_API UINV_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	EINV_ItemCategory GetItemCategory() const { return ItemCategory; }

	FINV_SlotAvailabilityResult HasRoomForItem(const UINV_ItemComponent* ItemComponent) ;
	FINV_SlotAvailabilityResult HasRoomForItem(const UINV_InventoryItem* Item) ;
	FINV_SlotAvailabilityResult HasRoomForItem(const FINV_ItemManifest& Manifest) ;

	
	UFUNCTION()
	void AddItem(UINV_InventoryItem* Item);

private:

	TWeakObjectPtr<UINV_InventoryComponent> InventoryComponent;

	void ConstructGrid();

	void AddItemToIndices(const FINV_SlotAvailabilityResult& Result, UINV_InventoryItem* NewItem);
	bool MatchesCategory(const UINV_InventoryItem* Item) const;
	FVector2D GetDrawSize(const FINV_GridFragment* GridFragment) const;
	void SetSlottedItemImage(
		const  UINV_SlottedItem* SlottedItem,
		const FINV_GridFragment* GridFragment,
		const FINV_ImageFragment* ImageFragment) const;
	void AddItemAtIndex(UINV_InventoryItem* Item, const int32 Index, const bool bStackable, const int32 StackAmount);
	UINV_SlottedItem* CreateSlottedItem(UINV_InventoryItem* Item,
		const bool bStackable,
		const int32 StackAmount,
		const FINV_GridFragment* GridFragment,
		const FINV_ImageFragment* ImageFragment,
		const int32 Index);
	void AddSlottedItemToCanvas(const int32 Index, const FINV_GridFragment* GridFragment, UINV_SlottedItem* SlottedItem) const;
	void UpdateGridSlots(UINV_InventoryItem* NewItem, const int32 Index, bool bStackableItem, int32 StackAmount);
	bool IsIndexClaimed(const TSet<int32>& CheckedIndices, const int32 Index) const;
	bool HasRoomAtIndex(const UINV_GridSlot* GridSlot,
		const FIntPoint& Dimensions,
		const TSet<int32>& CheckedIndices,
		TSet<int32>& OutTentativelyClaimed,
		const FGameplayTag& ItemType,
		const int32 MaxStackSize);

	bool CheckSlotConstraints(	const UINV_GridSlot* GridSlot,
								const UINV_GridSlot* SubGridSlot,
								const TSet<int32>& CheckedIndices,
								TSet<int32>& OutTentativelyClaimed,
								const FGameplayTag& ItemType,
								const int32 MaxStackSize) const;
	
	FIntPoint GetItemDimensions(const FINV_ItemManifest& Manifest) const;
	bool HasValidItem( const  UINV_GridSlot* GridSlot) const;
	bool IsUpperLeftSlot(const UINV_GridSlot* GridSlot, const UINV_GridSlot* SubGridSlot) const;
	bool DoesItemTypeMatch(const UINV_InventoryItem* SubItem, const FGameplayTag& ItemType) const;
	bool IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimensions) const;
	int32 DetermineFillAmountForSlot(	const bool bStackable,
										const int32 MaxStackSize,
										const int32 AmountToFill,
										UINV_GridSlot* GridSlot) const;
	int32 GetStackAmount(const UINV_GridSlot* GridSlot) const;
	bool IsRightClick(const FPointerEvent& MouseEvent) const;
	bool IsLeftClick(const FPointerEvent& MouseEvent) const;

	void PickUp(UINV_InventoryItem* ClickedInventoryItem, const int32 GridIndex);
	void AssignHoverItem(UINV_InventoryItem* InventoryItem);
	void AssignHoverItem(UINV_InventoryItem* InventoryItem, const int32 GridIndex, const int32 PreviousGridIndex);
	void RemoveItemFromGrid(UINV_InventoryItem* InventoryItem, const int32 GridIndex);
	void UpdateTileParameters(const FVector2D& CanvasPosition, const FVector2D& MousePosition);
	FIntPoint CalculateHoveredCoordinates(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const;
	EINV_TileQuadrant CalculateTileQuadrant(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const;
	void OnTileParametersUpdated(const FINV_TileParameters Parameters);
	FIntPoint CalculateStartingCoordinate(const FIntPoint& Coordinate, const FIntPoint& Dimensions, const EINV_TileQuadrant Quadrant) const;
	FINV_SpaceQueryResult CheckHoverPosition(const FIntPoint& Position, const FIntPoint& Dimensions) ;
	bool CursorExitedCanvas(const FVector2D BoundaryPos, const FVector2D BoundarySize, const FVector2D Location);
	void HighlightSlots (const int32 Index, const FIntPoint& Dimensions);
	void UnHighlightSlots(const int32 Index, const FIntPoint& Dimensions);
	void ChangeHoverType( const int32 Index, const FIntPoint& Dimensions, EINV_GridSlotState GridSlotState);
	
	UFUNCTION()
	void AddStacks(const FINV_SlotAvailabilityResult& Result);

	UFUNCTION()
	void OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void OnGridSlotClicked(int32 GridIndex, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void OnGridSlotHovered(int32 GridIndex, const FPointerEvent& MouseEvent);
	
	UFUNCTION()
	void OnGridSlotUnhovered(int32 GridIndex, const FPointerEvent& MouseEvent);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category = "Inventory")
	EINV_ItemCategory ItemCategory;

	UPROPERTY()
	TArray<TObjectPtr<UINV_GridSlot>> GridSlots;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UINV_GridSlot> GridSlotClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UINV_SlottedItem> SlottedItemClass;

	UPROPERTY()
	TMap<int32, TObjectPtr<UINV_SlottedItem>> SlottedItems;
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Rows;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Columns;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float TileSize;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UINV_HoverItem> HoverItemClass;

	UPROPERTY()
	TObjectPtr<UINV_HoverItem> HoverItem;

	FINV_TileParameters TileParameters;
	FINV_TileParameters LastTileParameters;

	// index where an item would be placed if we click on the grid at a valid location
	int32 ItemDropIndex{INDEX_NONE};	

	FINV_SpaceQueryResult CurrentQueryResult;
	bool bMouseWithinCanvas;
	bool bLastMouseWithinCanvas;

	int32 LastHighlightedIndex;
	FIntPoint LastHighlightedDimensions;
};
