// Copyright © Jodiah Jensen

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/INV_GridTypes.h"

#include "INV_InventoryGrid.generated.h"


/**
 * 
 */
UCLASS()
class JJ_INVENTORY_API UINV_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()
public:

	EINV_ItemCategory GetItemCategory() const { return ItemCategory; }

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category = "Inventory")
	EINV_ItemCategory ItemCategory;
	
};
