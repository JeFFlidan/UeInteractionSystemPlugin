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

	void AddOverlappedInteractable(UInteractableComponent* Interactable);
	void RemoveOverlappedInteractable(UInteractableComponent* Interactable);
	void RemoveHoldInteractable(UInteractableComponent* Interactable);

	UFUNCTION(BlueprintCallable, Category = "Interactor")
	void Interact();

protected:
	UPROPERTY()
	TArray<TObjectPtr<UInteractableComponent>> OverlappedInteractables;

	UPROPERTY()
	TArray<TObjectPtr<UInteractableComponent>> HoldInteractables;
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
