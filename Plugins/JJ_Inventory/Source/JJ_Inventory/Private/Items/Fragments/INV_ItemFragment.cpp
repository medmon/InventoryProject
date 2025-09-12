#include  "Items/Fragments/INV_ItemFragment.h"

void FINV_HealthPotionFragment::OnConsume(APlayerController* PC)
{
	FINV_ConsumableFragment::OnConsume(PC);

	// Get a stats component from the PC or PC->GetPawn()
	// or Get the Ability System Component and apply a Game Effect
	// or call an interface function for Healing()
	// etc

	GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, "Health Potion Consumed!");
}

void FINV_ManaPotionFragment::OnConsume(APlayerController* PC)
{
	FINV_ConsumableFragment::OnConsume(PC);
	// Get a stats component from the PC or PC->GetPawn()
	// or Get the Ability System Component and apply a Game Effect
	// or call an interface function for Healing()
	// etc

	GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, "Mana Potion Consumed!");
}
