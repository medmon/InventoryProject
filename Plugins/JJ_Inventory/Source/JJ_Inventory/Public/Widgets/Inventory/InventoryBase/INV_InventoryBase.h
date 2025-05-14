// Copyright © Jodiah Jensen

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/INV_GridTypes.h"
#include "INV_InventoryBase.generated.h"

struct FINV_SlotAvailability;
/**
 * 
 */
UCLASS()
class JJ_INVENTORY_API UINV_InventoryBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual FINV_SlotAvailability HasRoomForItem(UINV_ItemComponent* ItemComponent) const { return FINV_SlotAvailability();	}

	
};
