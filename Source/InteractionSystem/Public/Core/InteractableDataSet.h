// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "InteractableDataSet.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FInteractableData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User Interface")
	FText DisplayText{FText::GetEmpty()};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User Interface")
	FText DescriptionText{FText::GetEmpty()};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float DetectionDistance{250.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractionDuration{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<const UInputAction> InteractionAction;
};

UCLASS()
class INTERACTIONSYSTEM_API UInteractableDataSet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TMap<FName, FInteractableData> InteractableData;
};
