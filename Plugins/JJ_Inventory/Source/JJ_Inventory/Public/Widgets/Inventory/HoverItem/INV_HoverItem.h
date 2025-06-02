// Copyright © Jodiah Jensen

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"

#include "INV_HoverItem.generated.h"

class UTextBlock;
class UINV_InventoryItem;
class UImage;
/**
 *	The HoverItem is the item that will appear and follow the mouse
 *	cursor when an inventory item on the grid has been clicked
 */
UCLASS()
class JJ_INVENTORY_API UINV_HoverItem : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetImageBrush( const FSlateBrush& Brush) const;
	void UpdateStackCount(const int32 Count) const;

	int32 GetStackCount() const { return StackCount; }
	bool IsStackable() const { return bIsStackable; }
	int32 GetPreviousGridIndex() const { return PreviousGridIndex; }
	void SetPreviousGridIndex(int32 Index) { PreviousGridIndex = Index; }
	FIntPoint GetGridDimensions() const { return GridDimensions; }
	void SetGridDimensions(const FIntPoint& Dimensions) { GridDimensions = Dimensions; }

	
	void SetIsStackable(bool bStacks);
	FGameplayTag GetItemType() const;
	UINV_InventoryItem* GetInventoryItem() const;
	void SetInventoryItem(UINV_InventoryItem* Item);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_StackCount;

	int32 PreviousGridIndex;
	FIntPoint GridDimensions;
	TWeakObjectPtr<UINV_InventoryItem> InventoryItem;
	bool bIsStackable{false};
	int32 StackCount{0};
};
