// Copyright © Jodiah Jensen

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "INV_GridSlot.generated.h"

class UImage;
class UINV_InventoryItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGridSlotEvent, int32, GridIndex, const FPointerEvent&, MouseEvent);


UENUM(BlueprintType)
enum class EINV_GridSlotState : uint8
{
	Unoccupied,
	Occupied,
	Selected,
	GrayedOut
};

UCLASS()
class JJ_INVENTORY_API UINV_GridSlot : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

		
	void SetTileIndex(int32 Index) { TileIndex = Index; }
	int32 GetTileIndex() const { return TileIndex; }
	EINV_GridSlotState GetGridSlotState() const { return GridSlotState; }

	TWeakObjectPtr<UINV_InventoryItem> GetInventoryItem() const { return InventoryItem; }
	void SetInventoryItem(TWeakObjectPtr<UINV_InventoryItem> Item);

	int32 GetUpperLeftIndex() const { return UpperLeftIndex; }
	void SetUpperLeftIndex(int32 Index) { UpperLeftIndex = Index; }
	int32 GetStackCount() const { return StackCount; }
	void SetStackCount(int32 Count) { StackCount = Count; }	

	void SetIndex(int32 Index) { TileIndex = Index; }
	int32 GetIndex() const { return TileIndex; }

	bool IsAvailable() const { return bAvailable; }
	void SetAvailable(bool bIsAvailable) { bAvailable = bIsAvailable; }
	
	
	void SetUnoccupiedTexture();
	void SetOccupiedTexture();
	void SetSelectedTexture();
	void SetGrayedOutTexture();

	FGridSlotEvent GridSlotClicked;
	FGridSlotEvent GridSlotHovered;
	FGridSlotEvent GridSlotUnhovered;
	
	
	
private:

	int32 TileIndex{INDEX_NONE};

	int32 StackCount{0};
	int32 UpperLeftIndex{INDEX_NONE};
	TWeakObjectPtr<UINV_InventoryItem> InventoryItem;
	
	bool bAvailable{true};
	
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GridSlot;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Unoccupied;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Occupied;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Selected;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_GrayedOut;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	EINV_GridSlotState GridSlotState;
	
	
};
