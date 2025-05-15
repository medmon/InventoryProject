#include "Items/INV_ItemTags.h"


namespace GameItems
{
	namespace Equipment
	{
		namespace Weapons
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Axe, "GameItems.Equipment.Weapons.Axe", "Axe");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Sword, "GameItems.Equipment.Weapons.Sword", "Sword");
		}

		namespace Cloaks
		{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(RedCloak, "GameItems.Equipment.Cloaks.RedCloak", "RedCloak");

		}

		namespace Masks
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(SteelMask, "GameItems.Equipment.Masks.SteelMask", "SteelMask");

		}
	}

	namespace Consumables
	{
		namespace Potions
		{
			namespace Red
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Small, "GameItems.Consumables.Potions.Red.Small", "SmallRedPotion");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Large, "GameItems.Consumables.Potions.Red.Large", "LargeRedPotion");

			}

			namespace Blue
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Small, "GameItems.Consumables.Potions.Blue.Small", "SmallBluePotion");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Large, "GameItems.Consumables.Potions.Blue.Large", "LargeBluePotion");
				
			}
		}
	}

	namespace Crafting
	{
		namespace Plants
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(FireFernFruit, "GameItems.Crafting.Plants.FireFernFruit","FireFernFruit");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(LuminDaisy, "GameItems.Crafting.Plants.LuminDaisy", "LuminDaisy");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(ScorchPetalBlossom, "GameItems.Crafting.Plants.ScorchPetalBlossom", "ScorchPetalBlossom");
		}
		
	}

}
