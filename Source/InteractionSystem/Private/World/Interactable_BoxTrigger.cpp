// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "World/Interactable_BoxTrigger.h"

#include "Components/BoxComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(Interactable_BoxTrigger)

AInteractable_BoxTrigger::AInteractable_BoxTrigger() : Super()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectionArea_Box"));
	BoxComponent->AttachToComponent(GetRootComponent(), GetAttachmentRules());
}
