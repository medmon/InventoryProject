// Copyright © Jodiah Jensen

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "INV_GridSlot.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class JJ_INVENTORY_API UINV_GridSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetTileIndex(int32 Index) { TileIndex = Index; }
	int32 GetTileIndex() const { return TileIndex; }
private:

	int32 TileIndex;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GridSlot;

	
	
};
