


#include "Items/Manifest/INV_ItemManifest.h"

#include "Items/INV_InventoryItem.h"
#include "Items/Components/INV_ItemComponent.h"

UINV_InventoryItem* FINV_ItemManifest::Manifest(UObject* NewOuter)
{
	UINV_InventoryItem* Item = NewObject<UINV_InventoryItem>(NewOuter, UINV_InventoryItem::StaticClass());

	Item->SetItemManifest(*this);

	return Item;
}

void FINV_ItemManifest::SpawnPickupActor(const UObject* WorldContextObject, const FVector& SpawnLocation,
	const FRotator& SpawnRotation)
{
	if (!IsValid(PickupActorClass) || !IsValid(WorldContextObject)) return;

	AActor* SpawnedActor = WorldContextObject->GetWorld()->SpawnActor<AActor>(PickupActorClass, SpawnLocation, SpawnRotation);
	if (!IsValid(SpawnedActor)) return;
	
	// Set the item manifest, item category, item type, etc.

	UINV_ItemComponent* ItemComp = SpawnedActor->FindComponentByClass<UINV_ItemComponent>();
	check(ItemComp);
	
	ItemComp->InitItemManifest(*this);
	
}
