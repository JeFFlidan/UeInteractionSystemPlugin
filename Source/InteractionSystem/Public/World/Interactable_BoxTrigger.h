// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Interactable_Base.h"
#include "Interactable_BoxTrigger.generated.h"

class UInteractableComponent;
class UBoxComponent;

UCLASS(ClassGroup=(InteractionSystem))
class INTERACTIONSYSTEM_API AInteractable_BoxTrigger : public AInteractable_Base
{
	GENERATED_BODY()
	
public:
	AInteractable_BoxTrigger();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interactable")
	TObjectPtr<UBoxComponent> BoxComponent;
};
