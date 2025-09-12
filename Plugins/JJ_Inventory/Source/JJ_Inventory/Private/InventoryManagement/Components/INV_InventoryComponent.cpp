// Copyright © Jodiah Jensen


#include "InventoryManagement/Components/INV_InventoryComponent.h"

#include "Items/Components/INV_ItemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/Inventory/InventoryBase/INV_InventoryBase.h"
#include "Items/INV_InventoryItem.h"
#include "Items/Fragments/INV_ItemFragment.h"

// Sets default values for this component's properties
UINV_InventoryComponent::UINV_InventoryComponent() : InventoryList(this)
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
	bInventoryMenuOpen = false;
	
}

void UINV_InventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

void UINV_InventoryComponent::TryAddItem(UINV_ItemComponent* ItemComponent)
{
	FINV_SlotAvailabilityResult Result = InventoryMenu->HasRoomForItem(ItemComponent);

	UINV_InventoryItem* FoundItem = InventoryList.FindFirstItemByType(ItemComponent->GetItemManifest().GetItemType());

	Result.Item = FoundItem;
	
	if (Result.TotalRoomToFill == 0)
	{
		NoRoomInInventory.Broadcast();
		return;
	}


	if (Result.Item.IsValid() && Result.bStackable)
	{
		// add stacks to an item that already exists in the inventory. We only want to update the stack count
		// not create a new item.
		OnStackChange.Broadcast(Result);
		Server_AddStacksToItem(ItemComponent, Result.TotalRoomToFill, Result.Remainder);
	}
	else if (Result.TotalRoomToFill > 0)
	{
		// This item doesn't exist in the inventory. Create a new one and update all pertinent slots.
		Server_AddNewItem(ItemComponent, Result.bStackable ? Result.TotalRoomToFill : 0);
	}
	
}

void UINV_InventoryComponent::Server_AddNewItem_Implementation(UINV_ItemComponent* ItemComponent, int32 StackCount)
{
	UINV_InventoryItem* NewItem = InventoryList.AddEntry(ItemComponent);
	NewItem->SetTotalStackCount(StackCount);

	if (GetOwner()->GetNetMode() == NM_ListenServer || GetOwner()->GetNetMode() == NM_Standalone )
	{
		OnItemAdded.Broadcast(NewItem);
	}

	//tell the item component to destroy owning actor
	ItemComponent->PickedUp();
	
}

void UINV_InventoryComponent::Server_AddStacksToItem_Implementation(UINV_ItemComponent* ItemComponent, int32 StackCount,
	int32 Remainder)
{
	const FGameplayTag& ItemType =IsValid(ItemComponent) ? ItemComponent->GetItemManifest().GetItemType() : FGameplayTag::EmptyTag;
	UINV_InventoryItem* Item = InventoryList.FindFirstItemByType(ItemType);
	if (!IsValid(Item)) return;

	Item->SetTotalStackCount(Item->GetTotalStackCount() + StackCount);


	if (Remainder == 0)
	{
		ItemComponent->PickedUp();
	}
	else if (FINV_StackableFragment* StackableFragment = ItemComponent->GetItemManifest().GetFragmentOfTypeMutable<FINV_StackableFragment>())
	{
		StackableFragment->SetStackCount(Remainder);
	}
	
}

void UINV_InventoryComponent::Server_DropItem_Implementation(UINV_InventoryItem* Item, int32 StackCount)
{
	const int32 NewStackCount = Item->GetTotalStackCount() - StackCount;
	if (NewStackCount <=0)
	{
		InventoryList.RemoveEntry(Item);
	}
	else
	{
		Item->SetTotalStackCount(NewStackCount);
	}

	SpawnDroppedItem(Item, StackCount);
	
}

void UINV_InventoryComponent::SpawnDroppedItem(UINV_InventoryItem* Item, int32 StackCount)
{
	const APawn* OwningPawn = OwningController->GetPawn();
	FVector RotatedForward = OwningPawn->GetActorForwardVector();
	RotatedForward = RotatedForward.RotateAngleAxis(FMath::FRandRange(DropSpawnAngleMin, DropSpawnAngleMax), FVector::UpVector);
	FVector SpawnLocation = OwningPawn->GetActorLocation() + RotatedForward * FMath::FRandRange(DropSpawnDistanceMin, DropSpawnDistanceMax);
	SpawnLocation.Z += RelativeSpawnElevation;
	const FRotator SpawnRotation = FRotator::ZeroRotator;

	FINV_ItemManifest& ItemManifest = Item->GetItemManifestMutable();
	if (FINV_StackableFragment* StackableFragment = ItemManifest.GetFragmentOfTypeMutable<FINV_StackableFragment>() )
	{
		StackableFragment->SetStackCount(StackCount);
	}
	ItemManifest.SpawnPickupActor(this, SpawnLocation, SpawnRotation);
	
	
	
}


void UINV_InventoryComponent::Server_ConsumeItem_Implementation(UINV_InventoryItem* Item)
{
	const int32 NewStackCount = Item->GetTotalStackCount() - 1;
	if (NewStackCount <= 0)
	{
		InventoryList.RemoveEntry(Item);
	}
	else
	{
		Item->SetTotalStackCount(NewStackCount);
	}

	// TODO: Get the Consumable Fragment and call OnConsume()
	// (actually create the Consumable Fragment)

}

void UINV_InventoryComponent::ToggleInventoryMenu()
{
	if (bInventoryMenuOpen)
	{
		CloseInventoryMenu();
	}
	else
	{
		OpenInventoryMenu();
	}
}

void UINV_InventoryComponent::AddRepSubObj(UObject* SubObj)
{
	if(IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(SubObj))
	{
		AddReplicatedSubObject(SubObj);
	}

	
}


void UINV_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ConstructInventory();
	
}

void UINV_InventoryComponent::ConstructInventory()
{
	OwningController = Cast<APlayerController>(GetOwner());
	checkf(OwningController.IsValid(), TEXT("Inventory component should have a player controller as owner"));
	if (!OwningController->IsLocalController()) return;

	InventoryMenu = CreateWidget<UINV_InventoryBase>(OwningController.Get(), InventoryMenuClass);
	InventoryMenu->AddToViewport();
	CloseInventoryMenu();
}

void UINV_InventoryComponent::OpenInventoryMenu()
{
	if (!IsValid(InventoryMenu)) return;

	InventoryMenu->SetVisibility(ESlateVisibility::Visible);
	bInventoryMenuOpen = true;

	if (!OwningController.IsValid()) return;

	FInputModeGameAndUI InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(true);
	
}

void UINV_InventoryComponent::CloseInventoryMenu()
{
	if (!IsValid(InventoryMenu)) return;

	InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);

	bInventoryMenuOpen = false;

	FInputModeGameOnly InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(false);

	
}


