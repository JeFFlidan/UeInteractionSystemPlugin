// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "Core/InteractorComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InteractorComponent)

UInteractorComponent::UInteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractorComponent::AddInteractable(UInteractableComponent* InteractableComponent)
{
	ActiveInteractables.AddUnique(InteractableComponent);
}

void UInteractorComponent::RemoveInteractable(UInteractableComponent* InteractableComponent)
{
	ActiveInteractables.Remove(InteractableComponent);
}

void UInteractorComponent::BeginPlay()
{
	Super::BeginPlay();
}
