// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "Core/InteractableDataSet.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(InteractableDataSet)

#define LOCTEXT_NAMESPACE "InteractionSystem"

EDataValidationResult UInteractableDataSet::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);

	int32 EntryIndex = 0;
	for (const TPair<FName, FInteractableData>& Pair : InteractableData)
	{
		if (!Pair.Value.InteractionAction)
		{
			Context.AddError(FText::Format(LOCTEXT("InputActionIsNull", "Empty InteractionAction at index {0}."), EntryIndex));
			Result = EDataValidationResult::Invalid;
		}
		
		++EntryIndex;
	}
	
	return Result;
}

#undef LOCTEXT_NAMESPACE