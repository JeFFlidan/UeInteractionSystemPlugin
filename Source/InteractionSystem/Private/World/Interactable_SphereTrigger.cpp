// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "World/Interactable_SphereTrigger.h"

#include "Components/SphereComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(Interactable_SphereTrigger)

AInteractable_SphereTrigger::AInteractable_SphereTrigger()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionArea_Sphere"));
	SphereComponent->AttachToComponent(GetRootComponent(), GetAttachmentRules());
}
