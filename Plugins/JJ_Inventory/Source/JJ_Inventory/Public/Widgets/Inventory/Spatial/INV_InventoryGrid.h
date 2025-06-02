// Copyright © Jodiah Jensen

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/Fragments/INV_ItemFragment.h"
#include "Types/INV_GridTypes.h"

#include "INV_InventoryGrid.generated.h"


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

	UFUNCTION()
	void AddStacks(const FINV_SlotAvailabilityResult& Result);


	UFUNCTION()
	void OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent);
		
	
};
