// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "Core/InteractionSystemSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InteractionSystemSettings)

UInteractionSystemSettings::UInteractionSystemSettings(const FObjectInitializer& Initializer)
	: Super(Initializer)
{
	CategoryName = "Plugins";
	SectionName = "Interaction System";
}
