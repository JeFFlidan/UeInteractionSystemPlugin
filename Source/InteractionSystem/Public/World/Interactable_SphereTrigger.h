// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Interactable_Base.h"
#include "Interactable_SphereTrigger.generated.h"

class USphereComponent;

UCLASS(ClassGroup=(InteractionSystem))
class INTERACTIONSYSTEM_API AInteractable_SphereTrigger : public AInteractable_Base
{
	GENERATED_BODY()
	
public:	
	AInteractable_SphereTrigger();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interactable")
	TObjectPtr<USphereComponent> SphereComponent;
};
