// Copyright © Jodiah Jensen

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/INV_GridTypes.h"

#include "INV_InventoryBase.generated.h"

class UINV_ItemComponent;
struct FINV_SlotAvailabilityResult;
/**
 * 
 */
UCLASS()
class JJ_INVENTORY_API UINV_InventoryBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual FINV_SlotAvailabilityResult HasRoomForItem(UINV_ItemComponent* ItemComponent) const { return FINV_SlotAvailabilityResult();	}

	
};
