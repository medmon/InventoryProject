//	Copyright © Jodiah Jensen
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "INV_PlayerController.generated.h"

class UINV_InventoryComponent;
class UINV_HUDWidget;
class UInputAction;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class JJ_INVENTORY_API AINV_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AINV_PlayerController();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ToggleInventory();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	
private:

	void PrimaryInteract();
	void CreateHUDWidget();
	void TraceForItem();

	TWeakObjectPtr<UINV_InventoryComponent> InventoryComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory | Input")
	TObjectPtr<UInputMappingContext> DefaultIMC;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory | Input")
	TObjectPtr<UInputAction> PrimaryInteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory | Input")
	TObjectPtr<UInputAction> ToggleInventoryAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UINV_HUDWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UINV_HUDWidget> HUDWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	double TraceLength;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TEnumAsByte<ECollisionChannel> ItemTraceChannel;

	TWeakObjectPtr<AActor> ThisActor;
	TWeakObjectPtr<AActor> LastActor;
	
};
