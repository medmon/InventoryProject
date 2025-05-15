// Copyright © Jodiah Jensen

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/INV_GridTypes.h"

#include "INV_InventoryGrid.generated.h"


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

	FINV_SlotAvailabilityResult HasRoomForItem(const UINV_ItemComponent* ItemComponent) const;
	FINV_SlotAvailabilityResult HasRoomForItem(const UINV_InventoryItem* Item) const;
	FINV_SlotAvailabilityResult HasRoomForItem(const FINV_ItemManifest& Manifest) const;

	
	UFUNCTION()
	void AddItem(UINV_InventoryItem* Item);

private:

	TWeakObjectPtr<UINV_InventoryComponent> InventoryComponent;

	void ConstructGrid();

	void AddItemToIndices(const FINV_SlotAvailabilityResult& Result, UINV_InventoryItem* NewItem);
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category = "Inventory")
	EINV_ItemCategory ItemCategory;

	UPROPERTY()
	TArray<TObjectPtr<UINV_GridSlot>> GridSlots;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UINV_GridSlot> GridSlotClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Rows;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Columns;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float TileSize;

	bool MatchesCategory(const UINV_InventoryItem* Item) const;
	
		
	
};
