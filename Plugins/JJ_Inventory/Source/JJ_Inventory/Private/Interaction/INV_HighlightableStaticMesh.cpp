// Copyright © Jodiah Jensen


#include "Interaction/INV_HighlightableStaticMesh.h"

void UINV_HighlightableStaticMesh::Highlight_Implementation()
{
	SetOverlayMaterial(HighlightMaterial);	
}

void UINV_HighlightableStaticMesh::UnHighlight_Implementation()
{
	SetOverlayMaterial(nullptr);	
	
}
