// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "Core/InteractorComponent.h"
#include "Core/InteractableComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InteractorComponent)

UInteractorComponent::UInteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractorComponent::AddOverlappedInteractable(UInteractableComponent* Interactable)
{
	OverlappedInteractables.AddUnique(Interactable);
}

void UInteractorComponent::RemoveOverlappedInteractable(UInteractableComponent* Interactable)
{
	OverlappedInteractables.Remove(Interactable);
}

void UInteractorComponent::RemoveHoldInteractable(UInteractableComponent* Interactable)
{
	HoldInteractables.Remove(Interactable);
}

void UInteractorComponent::Interact()
{
	for (UInteractableComponent* Interactable : OverlappedInteractables)
	{
		if (!IsValid(Interactable))
		{
			HoldInteractables.Remove(Interactable);
			continue;
		}
		
		Interactable->Interact(this);

		if (Interactable->GetInteractableData().InteractionInputType == EInteractionInputType::Hold)
		{
			HoldInteractables.Add(Interactable);
		}
	}
}

void UInteractorComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	for (UInteractableComponent* Interactable : HoldInteractables)
	{
		Interactable->UpdateCurrentInteractionTime(DeltaTime);
	}
}
