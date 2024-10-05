// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "Core/InteractorComponent.h"
#include "Core/InteractableDataSet.h"
#include "Core/InteractionSystemSettings.h"

#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "Core/InteractableComponent.h"

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

void UInteractorComponent::Interact()
{
	for (UInteractableComponent* Interactable : ActiveInteractables)
	{
		if (ActiveInteractionInputActions.Contains(Interactable->GetInteractableData()->InteractionAction))
		{
			Interactable->Interact(this);
		}
	}
}

void UInteractorComponent::BeginPlay()
{
	Super::BeginPlay();
	FindInteractionInputActions();
	BindInteractionInputActions();
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
	APawn* Owner = CastChecked<APawn>(GetOwner());
	UEnhancedInputComponent* InputComponent = CastChecked<UEnhancedInputComponent>(Owner->InputComponent);
	
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
