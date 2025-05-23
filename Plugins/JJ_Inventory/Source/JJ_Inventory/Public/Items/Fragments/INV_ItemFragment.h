#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include  "INV_ItemFragment.generated.h"

USTRUCT(BlueprintType)
struct FINV_ItemFragment
{
	GENERATED_BODY()

	FINV_ItemFragment() {}
	FINV_ItemFragment(const FINV_ItemFragment&) = default;
	FINV_ItemFragment& operator=(const FINV_ItemFragment&) = default;
	FINV_ItemFragment(FINV_ItemFragment&&) = default;
	FINV_ItemFragment& operator=(FINV_ItemFragment&&) = default;
	virtual ~FINV_ItemFragment() {}

	FGameplayTag GetFragmentTag() const { return FragmentTag; }
	void SetFragmentTag(FGameplayTag Tag) { FragmentTag = Tag; }
	
	
private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;
};

USTRUCT(BlueprintType)
struct FINV_GridFragment : public FINV_ItemFragment
{
	GENERATED_BODY()

	FIntPoint GetGridSize() const { return GridSize; }
	void SetGridSize(const FIntPoint& Size) { GridSize = Size; }
	float GetGridPadding() const { return GridPadding; }
	void SetGridPadding(float Padding) { GridPadding = Padding; }

	
private:

	UPROPERTY(EditAnywhere)
	FIntPoint GridSize{1,1};

	UPROPERTY(EditAnywhere)
	float GridPadding{0.f};


	
};

USTRUCT(BlueprintType)
struct FINV_ImageFragment : public FINV_ItemFragment
{
	GENERATED_BODY()

public:

	UTexture2D* GetIcon() const { return Icon; }
	
private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TObjectPtr<UTexture2D> Icon{nullptr};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FVector2D IconDimensions {40.f, 40.f};
	
};

USTRUCT(BlueprintType)
struct FINV_StackableFragment : public FINV_ItemFragment
{
	GENERATED_BODY()
public:

	int32 GetMaxStackSize() const { return MaxStackSize; }
	void SetMaxStackSize(int32 Size) { MaxStackSize = Size; }
	int32 GetStackCount() const { return StackCount; }
	void SetStackCount(int32 Count) { StackCount = Count; }
	void AddToStackCount(int32 Count) { StackCount += Count; }
	void RemoveFromStackCount(int32 Count) { StackCount -= Count; }
	bool IsFull() const { return StackCount >= MaxStackSize; }
	bool IsEmpty() const { return StackCount <= 0; }
	bool IsStackable() const { return MaxStackSize > 1; }
	bool IsSingleItem() const { return MaxStackSize == 1; }
	bool IsStackableAndFull() const { return IsStackable() && IsFull(); }
	
private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MaxStackSize{1};
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 StackCount{1};	
	
	
};
