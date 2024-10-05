// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "InteractorComponent.generated.h"

class UInputAction;
class UInteractableComponent;

UCLASS(Blueprintable, ClassGroup=(InteractionSystem), meta=(BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UInteractorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractorComponent();

	UFUNCTION(BlueprintPure, Category = "Interactor")
	static UInteractorComponent* FindInteractorComponent(const AActor* Actor)
	{
		return Actor ? Actor->FindComponentByClass<UInteractorComponent>() : nullptr;
	}

	void AddInteractable(UInteractableComponent* InteractableComponent);
	void RemoveInteractable(UInteractableComponent* InteractableComponent);

	UFUNCTION(BlueprintCallable, Category = "Interactor")
	void Interact();

protected:
	UPROPERTY()
	TArray<TObjectPtr<UInteractableComponent>> ActiveInteractables;

	UPROPERTY()
	TArray<TObjectPtr<const UInputAction>> InteractionInputActions;

	UPROPERTY()
	TArray<TObjectPtr<const UInputAction>> ActiveInteractionInputActions;
	
	virtual void BeginPlay() override;

	void FindInteractionInputActions();
	void BindInteractionInputActions();

	UFUNCTION()
	void OnInputActionPressed(const UInputAction* InputAction);

	UFUNCTION()
	void OnInputActionReleased(const UInputAction* InputAction);
};
