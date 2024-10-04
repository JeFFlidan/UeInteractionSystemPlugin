// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "InteractionSystemSettings.generated.h"

class UInteractableDataSet;

/**
 * 
 */
UCLASS(config = InteractionSystem, DefaultConfig)
class INTERACTIONSYSTEM_API UInteractionSystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UInteractionSystemSettings(const FObjectInitializer& Initializer);

	// It is generally recommended to use one Interactable Data Asset
	UPROPERTY(Config, EditAnywhere, Category = "General")
	TArray<TSoftObjectPtr<UInteractableDataSet>> InteractableDataAssets;
};
