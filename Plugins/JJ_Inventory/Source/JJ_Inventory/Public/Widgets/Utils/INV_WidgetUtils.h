// Copyright © Jodiah Jensen

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "INV_WidgetUtils.generated.h"

/**
 * 
 */
UCLASS()
class JJ_INVENTORY_API UINV_WidgetUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static int32 GetIndexFromPosition(const FIntPoint& Position, const int32 Columns);
};
