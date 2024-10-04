// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "InteractorComponent.generated.h"

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

protected:
	UPROPERTY()
	TArray<TObjectPtr<UInteractableComponent>> ActiveInteractables;
	
	virtual void BeginPlay() override;
};
