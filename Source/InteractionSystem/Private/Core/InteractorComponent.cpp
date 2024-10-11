// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "Core/InteractorComponent.h"
#include "Core/InteractableComponent.h"

#include "EnhancedInputComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InteractorComponent)

UInteractorComponent::UInteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractorComponent::AddOverlappedInteractable(UInteractableComponent* Interactable)
{
	OverlappedInteractables.AddUnique(Interactable);
	BindInteractionInputAction(Interactable->GetInteractableData().InteractionAction);
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
	for (UInteractableComponent* Interactable : InteractablesToActivate)
	{
		Interactable->Interact(this);

		if (Interactable->GetInteractableData().InteractionInputType == EInteractionInputType::Hold)
		{
			HoldInteractables.Add(Interactable);
		}
	}

	InteractablesToActivate.Reset();
}

void UInteractorComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	for (UInteractableComponent* Interactable : HoldInteractables)
	{
		Interactable->UpdateCurrentInteractionTime(DeltaTime);
	}
}

UEnhancedInputComponent* UInteractorComponent::GetInputComponent() const
{
	return CastChecked<UEnhancedInputComponent>(GetOwner()->InputComponent);
}

void UInteractorComponent::BindInteractionInputAction(UInputAction* InputAction)
{
	if (!InputAction)
	{
		return;
	}

	for (const FEnhancedInputActionEventBinding* Binding : InteractionActionEventBindings)
	{
		if (Binding->GetAction() == InputAction)
		{
			return;
		}
	}
	
	InteractionActionEventBindings.Add(&GetInputComponent()->BindAction(
		InputAction, ETriggerEvent::Triggered, this, &ThisClass::HandleInteractionInputActionActivation, InputAction));
}

void UInteractorComponent::HandleInteractionInputActionActivation(UInputAction* InputAction)
{
	for (UInteractableComponent* Interactable : OverlappedInteractables)
	{
		if (Interactable->GetInteractableData().InteractionAction == InputAction)
		{
			InteractablesToActivate.Add(Interactable);
		}
	}
}
