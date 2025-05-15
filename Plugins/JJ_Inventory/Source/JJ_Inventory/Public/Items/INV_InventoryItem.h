// Copyright © Jodiah Jensen

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Items/Manifest/INV_ItemManifest.h"

#include "INV_InventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class JJ_INVENTORY_API UINV_InventoryItem : public UObject
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override { return true; }
	
	void SetItemManifest(const FINV_ItemManifest& Manifest);
	const FINV_ItemManifest& GetItemManifest() const { return ItemManifest.Get<FINV_ItemManifest>(); }
	FINV_ItemManifest& GetItemManifestMutable() { return ItemManifest.GetMutable<FINV_ItemManifest>(); }

	bool IsStackable() const;
	int32 GetTotalStackCount() const { return TotalStackCount; }
	void SetTotalStackCount(int32 Count) { TotalStackCount = Count; }private:

private:	
	UPROPERTY(VisibleAnywhere, meta = (BaseStruct = "/Script/Inventory.INV_ItemManifest"), Replicated, Category = "Inventory")
	FInstancedStruct ItemManifest;

	UPROPERTY(Replicated)
	int32 TotalStackCount{0};

	
};

template<typename FragmentType>
const FragmentType* GetFragment(const UINV_InventoryItem* Item, const FGameplayTag& Tag)
{
	const auto& Manifest = Item->GetItemManifest();

	return Manifest.GetFragmentOfTypeWithTag<FragmentType>(Tag);
}