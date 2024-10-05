﻿// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "InteractableDataSet.generated.h"

class UInputAction;

UENUM(BlueprintType)
enum class EInteractionInputType : uint8
{
	Press UMETA(DisplayName = "Press"),
	Hold UMETA(DisplayName = "Hold")
};

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	EInteractionInputType InteractionInputType{EInteractionInputType::Press};
};

UCLASS()
class INTERACTIONSYSTEM_API UInteractableDataSet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TMap<FName, FInteractableData> InteractableData;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif
};
