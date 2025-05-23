﻿// Copyright © Jodiah Jensen


#include "Widgets/Utils/INV_WidgetUtils.h"

int32 UINV_WidgetUtils::GetIndexFromPosition(const FIntPoint& Position, const int32 Columns)
{
	return Position.X + Position.Y * Columns;
}

FIntPoint UINV_WidgetUtils::GetPositionFromIndex(int32 Index, const int32 Columns)
{
	return FIntPoint(Index % Columns, Index / Columns);
}
