﻿// Copyright © Jodiah Jensen


#include "Player/INV_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "JJ_Inventory.h"
#include "Interaction/INV_Highlightable.h"
#include "InventoryManagement/Components/INV_InventoryComponent.h"
#include "Items/Components/INV_ItemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/HUD/INV_HUDWidget.h"

AINV_PlayerController::AINV_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	TraceLength = 800.0;

	ItemTraceChannel =  ECC_GameTraceChannel1;
	
}

void AINV_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceForItem();
	
}

void AINV_PlayerController::ToggleInventory()
{
	if (!InventoryComponent.IsValid()) return;
	InventoryComponent->ToggleInventoryMenu();
}

void AINV_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (IsValid(Subsystem))
	{
		Subsystem->AddMappingContext(DefaultIMC, 0);
	}

	InventoryComponent = FindComponentByClass<UINV_InventoryComponent>();
	
	CreateHUDWidget();
	
}

void AINV_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started, this, &AINV_PlayerController::PrimaryInteract);
	EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &AINV_PlayerController::ToggleInventory);
}

void AINV_PlayerController::PrimaryInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Primary Interact Started"));

	if (!ThisActor.IsValid()) return;

	UINV_ItemComponent* ItemComp = ThisActor->FindComponentByClass<UINV_ItemComponent>();
	if (!IsValid(ItemComp) || !InventoryComponent.IsValid()) return;

	InventoryComponent->TryAddItem(ItemComp);
	
	
	
}

void AINV_PlayerController::CreateHUDWidget()
{
	if (!IsLocalController()) return;

	HUDWidget = CreateWidget<UINV_HUDWidget>(this, HUDWidgetClass);
	if (IsValid(HUDWidget))
	{
		HUDWidget->AddToViewport();
	}
}

void AINV_PlayerController::TraceForItem()
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport) ) return;
		
	FVector2d ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2d ViewportCenter = ViewportSize/2.f;
	FVector TraceStart;
	FVector Forward;
	if (!UGameplayStatics::DeprojectScreenToWorld(this, ViewportCenter, TraceStart, Forward)) return;
	
	const FVector TraceEnd = TraceStart + Forward * TraceLength;
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ItemTraceChannel);

	LastActor = ThisActor;
	ThisActor = HitResult.GetActor();

	if (!ThisActor.IsValid())
	{
		if (IsValid(HUDWidget)) HUDWidget->HidePickupMessage();
	}
	
	if (ThisActor == LastActor) return;

	if (ThisActor.IsValid())
	{
		if(UActorComponent* Highlightable = ThisActor->FindComponentByInterface(UINV_Highlightable::StaticClass()); IsValid(Highlightable))
		{
			IINV_Highlightable::Execute_Highlight(Highlightable);
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Started tracing a new actor"));

		UINV_ItemComponent* ItemComponent = ThisActor->FindComponentByClass<UINV_ItemComponent>();
		if (!IsValid(ItemComponent)) return;

		if (IsValid(HUDWidget)) HUDWidget->ShowPickupMessage(ItemComponent->GetPickupMessage());
		
	}

	if (LastActor.IsValid())
	{
		if(UActorComponent* Highlightable = LastActor->FindComponentByInterface(UINV_Highlightable::StaticClass()); IsValid(Highlightable))
		{
			IINV_Highlightable::Execute_UnHighlight(Highlightable);
		}

		
		UE_LOG(LogTemp, Warning, TEXT("Stopped tracing Last Actor"));	
	}
		
}
