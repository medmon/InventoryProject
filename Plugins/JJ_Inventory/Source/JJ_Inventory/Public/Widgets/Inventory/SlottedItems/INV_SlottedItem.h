// Copyright © Jodiah Jensen

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Items/INV_InventoryItem.h"


#include "INV_SlottedItem.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class JJ_INVENTORY_API UINV_SlottedItem : public UUserWidget
{
	GENERATED_BODY()
public:

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
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	int32 GridIndex;
	FIntPoint GridDimensions;
	
	TWeakObjectPtr<UINV_InventoryItem> InventoryItem;

	bool bIsStackable{false};
	
};
