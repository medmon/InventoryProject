// Copyright © Jodiah Jensen

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "INV_InfoMessage.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class JJ_INVENTORY_API UINV_InfoMessage : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void MessageShow();

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void MessageHide();

	void SetMessage(const FText& Message);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Message;

	float MessageLifetime {3.f};
	bool bIsMessageActive{false};
	FTimerHandle MessageTimer;

	
};
