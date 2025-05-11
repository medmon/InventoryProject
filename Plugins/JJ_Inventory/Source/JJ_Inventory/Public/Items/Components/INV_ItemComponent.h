// Copyright © Jodiah Jensen

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "INV_ItemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class JJ_INVENTORY_API UINV_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UINV_ItemComponent();

	FString GetPickupMessage() const { return PickupMessage; }
		
protected:

public:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FString PickupMessage;
};
