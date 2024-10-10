// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "World/Interactable_Base.h"
#include "Core/InteractableComponent.h"

#include "Components/SceneComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(Interactable_Base)

AInteractable_Base::AInteractable_Base() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(SceneComponent);
}

FAttachmentTransformRules AInteractable_Base::GetAttachmentRules() const
{
	return FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
}
