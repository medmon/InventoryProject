// Copyright © Jodiah Jensen

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "INV_InventoryStatics.generated.h"

class UINV_InventoryComponent;
/**
 * 
 */
UCLASS()
class JJ_INVENTORY_API UINV_InventoryStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static UINV_InventoryComponent* GetInventoryComponent(const APlayerController* PlayerController);

	
};
