#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"


#include "INV_FastArray.generated.h"

class UINV_ItemComponent;
class UINV_InventoryComponent;
class UINV_InventoryItem;

/** A single entry in an inventory */
USTRUCT(BlueprintType)
struct FINV_InventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()
public:
	FINV_InventoryEntry() {}

private:
	friend UINV_InventoryComponent;
	friend struct FINV_InventoryFastArray;

	UPROPERTY()
	TObjectPtr<UINV_InventoryItem> Item = nullptr;
	
};

/** List of Inventory Items */
USTRUCT(BlueprintType)
struct FINV_InventoryFastArray : public FFastArraySerializer
{
	GENERATED_BODY()
public:
	FINV_InventoryFastArray() : OwnerComponent(nullptr) {}
	FINV_InventoryFastArray(UActorComponent* InOwnerComponent) : OwnerComponent(InOwnerComponent) {}

	TArray<UINV_InventoryItem*> GetAllItems() const;

	// FFastArraySerializer Contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	// End of FFastArraySerializer Contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FINV_InventoryEntry, FINV_InventoryFastArray> (Entries, DeltaParams, *this);
	}

	UINV_InventoryItem* AddEntry(UINV_ItemComponent* ItemComponent);
	UINV_InventoryItem* AddEntry(UINV_InventoryItem* Item);
	void RemoveEntry(UINV_InventoryItem* Item);
	
private:
	friend UINV_InventoryComponent;
	
	/** Replicated list of items */
	UPROPERTY()
	TArray<FINV_InventoryEntry> Entries;
	
	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;

	
};

template <>
struct TStructOpsTypeTraits<FINV_InventoryFastArray> : public TStructOpsTypeTraitsBase2<FINV_InventoryFastArray>
{
	enum { WithNetDeltaSerializer = true };
};