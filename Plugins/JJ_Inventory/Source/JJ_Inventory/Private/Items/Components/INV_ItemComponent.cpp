// Copyright © Jodiah Jensen


#include "Items/Components/INV_ItemComponent.h"

#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UINV_ItemComponent::UINV_ItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	PickupMessage = FString("E - Pick Up");
}

void UINV_ItemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemManifest);
	
}

