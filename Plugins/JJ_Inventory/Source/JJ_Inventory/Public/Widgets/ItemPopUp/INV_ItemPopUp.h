// Copyright © Jodiah Jensen

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "INV_ItemPopUp.generated.h"

/**
 *  Item popup widget shows up when right clicking on an item
 *  in the inventory grid
 */

class UButton;
class USlider;
class UTextBlock;
class USizeBox;


UCLASS()

class JJ_INVENTORY_API UINV_ItemPopUp : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	
private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Split;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Drop;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Consume;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> Slider_Split;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_SplitAmount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UFUNCTION()
	void SplitButtonClicked();

	UFUNCTION()
	void DropButtonClicked();

	UFUNCTION()
	void ConsumeButtonClicked();

	UFUNCTION()
	void SliderValueChanged(float Value);
	
};

