// Copyright © Jodiah Jensen

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Manifest/INV_ItemManifest.h"

#include "INV_ItemComponent.generated.h"




UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class JJ_INVENTORY_API UINV_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UINV_ItemComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FINV_ItemManifest GetItemManifest() const { return ItemManifest; }
	
	FString GetPickupMessage() const { return PickupMessage; }
		
protected:

private:

	UPROPERTY(EditAnywhere, Replicated, Category = "Inventory")
	FINV_ItemManifest ItemManifest;
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FString PickupMessage;
};
