// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Engine/DataTable.h"
#include "InteractableData.generated.h"

USTRUCT(BlueprintType)
struct FInteractableData : public FTableRowBase
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
};