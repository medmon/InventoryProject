#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Types/INV_GridTypes.h"
#include "StructUtils/InstancedStruct.h"

#include "INV_ItemManifest.generated.h"

/**
 * The item manifest contains all of the necessary data
 *  for creating a new inventory item
*/


USTRUCT(BlueprintType)
struct JJ_INVENTORY_API FINV_ItemManifest
{
 GENERATED_BODY()

 UINV_InventoryItem* Manifest(UObject* NewOuter);
 EINV_ItemCategory GetItemCategory() const { return ItemCategory; }
 FGameplayTag GetItemType() const { return ItemType; }
 
 
private:

 UPROPERTY(EditAnywhere, Category = "Inventory")
	EINV_ItemCategory ItemCategory{EINV_ItemCategory::None};

 UPROPERTY(EditAnywhere, Category = "Inventory")
 FGameplayTag ItemType;
 
};


