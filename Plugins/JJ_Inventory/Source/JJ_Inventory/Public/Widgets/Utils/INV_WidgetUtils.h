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
	UFUNCTION(BlueprintCallable, Category = "Inventory" )
	static FVector2D GetWidgetPosition(UWidget* Widget);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory" )
	static FVector2D GetWidgetSize(UWidget* Widget);

	UFUNCTION(BlueprintCallable, Category = "Inventory" )
	static bool IsWithinBounds(const FVector2D& BoundaryPos, const FVector2D& WidgetSize, const FVector2D& MousePos);

	
	
	static int32 GetIndexFromPosition(const FIntPoint& Position, const int32 Columns);
	static FIntPoint GetPositionFromIndex(int32 Index, const int32 Columns);
};
