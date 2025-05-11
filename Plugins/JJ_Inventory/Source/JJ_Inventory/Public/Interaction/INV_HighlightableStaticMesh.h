// Copyright © Jodiah Jensen

#pragma once

#include "CoreMinimal.h"
#include "INV_Highlightable.h"
#include "Components/StaticMeshComponent.h"
#include "INV_HighlightableStaticMesh.generated.h"

/**
 * 
 */
UCLASS()
class JJ_INVENTORY_API UINV_HighlightableStaticMesh : public UStaticMeshComponent, public IINV_Highlightable
{
	GENERATED_BODY()
public:
	virtual void Highlight_Implementation() override;
	virtual void UnHighlight_Implementation() override;

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TObjectPtr<UMaterialInterface> HighlightMaterial;
	
};
