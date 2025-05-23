﻿// Copyright © Jodiah Jensen

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/INV_GridTypes.h"
#include "Widgets/Utils/INV_WidgetUtils.h"
#include "INV_InventoryStatics.generated.h"

class UINV_WidgetUtils;
class UINV_ItemComponent;
class UINV_InventoryComponent;
/**
 * A static helper class for inventory-related functionalities, designed to work as a blueprint function library.
 * This class provides static utility functions to interact with inventory components and perform operations.
 */
UCLASS()
class JJ_INVENTORY_API UINV_InventoryStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static UINV_InventoryComponent* GetInventoryComponent(const APlayerController* PlayerController);

	static EINV_ItemCategory GetItemCategoryFromItemComp(UINV_ItemComponent* ItemComp);
	
	template <typename T, typename FuncT>
	static void ForEach2D(TArray<T>& Array, int32 Index, const FIntPoint& Range2D, int32 GridColumns, const FuncT& Function);
	
	
};

template <typename T, typename FuncT>
void UINV_InventoryStatics::ForEach2D(TArray<T>& Array, int32 Index, const FIntPoint& Range2D, int32 GridColumns, const FuncT& Function)
{
	for ( int32 j = 0; j < Range2D.Y; ++j)
	{
		for ( int32 i = 0; i < Range2D.X; ++i )
		{
			const FIntPoint Coordinates = UINV_WidgetUtils::GetPositionFromIndex(Index, GridColumns) + FIntPoint(i,j);
			const int32 TileIndex = UINV_WidgetUtils::GetIndexFromPosition(Coordinates, GridColumns);
			if (Array.IsValidIndex(TileIndex))
			{
				Function(Array[TileIndex]);
				continue;
			}

			Function(Array[Index + i + j * GridColumns]);
		}
	}
}


