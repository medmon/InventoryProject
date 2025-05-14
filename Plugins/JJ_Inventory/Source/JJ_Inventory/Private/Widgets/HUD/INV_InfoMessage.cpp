﻿// Copyright © Jodiah Jensen


#include "INV_InfoMessage.h"

#include "Components/TextBlock.h"
#include "TimerManager.h"

void UINV_InfoMessage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Text_Message->SetText(FText::GetEmpty());
	MessageHide();

	bIsMessageActive = false;
	
}

void UINV_InfoMessage::SetMessage(const FText& Message)
{
	Text_Message->SetText(Message);

	if (!bIsMessageActive)
	{
		MessageShow();
	}
	bIsMessageActive = true;

	GetWorld()->GetTimerManager().SetTimer(MessageTimer, [this]()
	{
		MessageHide();
		bIsMessageActive = false;
		
	}, MessageLifetime, false);
}
