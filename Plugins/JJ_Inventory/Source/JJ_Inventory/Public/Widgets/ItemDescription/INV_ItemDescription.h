// Copyright © Jodiah Jensen

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "INV_ItemDescription.generated.h"

class USizeBox;
/**
 * 
 */
UCLASS()
class JJ_INVENTORY_API UINV_ItemDescription : public UUserWidget
{
	GENERATED_BODY()

public:
	
private:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox;

	
};
