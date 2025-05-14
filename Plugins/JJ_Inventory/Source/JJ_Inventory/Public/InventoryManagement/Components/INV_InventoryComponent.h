// Copyright © Jodiah Jensen

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "INV_InventoryComponent.generated.h"

class UINV_ItemComponent;
class UINV_InventoryItem;
class UINV_InventoryBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChange, UINV_InventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoRoomInInventory);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class JJ_INVENTORY_API UINV_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UINV_InventoryComponent();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	void TryAddItem(UINV_ItemComponent* ItemComponent);
	
	void ToggleInventoryMenu();

	FInventoryItemChange OnItemAdded;
	FInventoryItemChange OnItemRemoved;
	FNoRoomInInventory NoRoomInInventory;
protected:
	virtual void BeginPlay() override;

private:

	TWeakObjectPtr<APlayerController> OwningController;
	
	void ConstructInventory();

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UINV_InventoryBase>  InventoryMenuClass;

	UPROPERTY()
	TObjectPtr<UINV_InventoryBase> InventoryMenu;

	bool bInventoryMenuOpen;
	void OpenInventoryMenu();
	void CloseInventoryMenu();
	
};
