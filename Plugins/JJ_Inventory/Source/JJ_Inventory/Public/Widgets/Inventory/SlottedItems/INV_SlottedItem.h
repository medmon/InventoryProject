// Copyright © Jodiah Jensen

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Items/INV_InventoryItem.h"


#include "INV_SlottedItem.generated.h"

class UINV_InventoryItem;
class UImage;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSlottedItemClicked, int32, GridIndex, const FPointerEvent&, MouseEvent);

/**
 * 
 */
UCLASS()
class JJ_INVENTORY_API UINV_SlottedItem : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void SetIsStackable(bool bStackable) { bIsStackable = bStackable; }
	UImage* GetImageIcon() const { return Image_Icon; }
	void SetGridIndex(int32 Index) { GridIndex = Index; }
	int32 GetGridIndex() const { return GridIndex; }
	void SetGridDimensions(const FIntPoint& Dimensions) { GridDimensions = Dimensions; }
	FIntPoint GetGridDimensions() const { return GridDimensions; }
	void SetInventoryItem(UINV_InventoryItem* Item) {InventoryItem = Item;}
	UINV_InventoryItem* GetInventoryItem() const;
	bool IsStackable() const { return bIsStackable; }
	int32 GetTotalStackCount() const;
	void SetTotalStackCount(int32 Count);
	void SetItemIcon(UTexture2D* Icon);
	void SetImageBrush(const FSlateBrush& Brush) const { Image_Icon->SetBrush(Brush); }

	void UpdateStackCount(int32 StackCount);

	FSlottedItemClicked OnSlottedItemClicked;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_StackCount;

	
	
	int32 GridIndex;
	FIntPoint GridDimensions;
	TWeakObjectPtr<UINV_InventoryItem> InventoryItem;
	bool bIsStackable{false};
	
};
