// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "Interactable_base.generated.h"

class UInteractableComponent;
class USceneComponent;

UCLASS(ClassGroup=(InteractionSystem))
class INTERACTIONSYSTEM_API AInteractable_Base : public AActor
{
	GENERATED_BODY()

public:
	AInteractable_Base();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interactable")
	TObjectPtr<UInteractableComponent> InteractableComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interactable")
	TObjectPtr<USceneComponent> SceneComponent;

	FAttachmentTransformRules GetAttachmentRules() const;
};
