// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "Core/InteractorComponent.h"
#include "Core/InteractableDataSet.h"
#include "Core/InteractableComponent.h"
#include "Core/InteractionSystemSettings.h"

#include "InputAction.h"
#include "EnhancedInputComponent.h"

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
		
		if (ActiveInteractionInputActions.Contains(Interactable->GetInteractableData()->InteractionAction))
		{
			Interactable->Interact(this);

			if (Interactable->GetInteractableData()->InteractionInputType == EInteractionInputType::Hold)
			{
				HoldInteractables.Add(Interactable);
			}
		}
	}
}

void UInteractorComponent::BeginPlay()
{
	Super::BeginPlay();
	FindInteractionInputActions();
	BindInteractionInputActions();
}

void UInteractorComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	for (UInteractableComponent* Interactable : HoldInteractables)
	{
		Interactable->UpdateCurrentInteractionTime(DeltaTime);
	}
}

void UInteractorComponent::FindInteractionInputActions()
{
	const UInteractionSystemSettings* Settings = GetDefault<UInteractionSystemSettings>();

	for (const TSoftObjectPtr<UInteractableDataSet>& InteractableDataSet : Settings->InteractableDataAssets)
	{
		for (const TPair<FName, FInteractableData>& Pair : InteractableDataSet.LoadSynchronous()->InteractableData)
		{
			InteractionInputActions.AddUnique(Pair.Value.InteractionAction);
		}
	}
}

void UInteractorComponent::BindInteractionInputActions()
{
	UEnhancedInputComponent* InputComponent = CastChecked<UEnhancedInputComponent>(GetOwner()->InputComponent);
	
	for (const UInputAction* InputAction : InteractionInputActions)
	{
		InputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::OnInputActionPressed, InputAction);
		InputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &ThisClass::OnInputActionReleased, InputAction);
	}
}

void UInteractorComponent::OnInputActionPressed(const UInputAction* InputAction)
{
	ActiveInteractionInputActions.AddUnique(InputAction);
}

void UInteractorComponent::OnInputActionReleased(const UInputAction* InputAction)
{
	ActiveInteractionInputActions.Remove(InputAction);
}
