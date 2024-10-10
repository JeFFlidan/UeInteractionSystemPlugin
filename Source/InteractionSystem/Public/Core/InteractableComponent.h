// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

class UPrimitiveComponent;
class UInteractorComponent;
class UEnhancedInputComponent;
class UInputAction;
struct FInteractableData;
struct FEnhancedInputActionEventBinding;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractableStateChanged, UInteractorComponent*, Interactor, UInteractableComponent*, Interactable);

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
	float InteractionDuration{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<UInputAction> InteractionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	EInteractionInputType InteractionInputType{EInteractionInputType::Press};
};

UCLASS(Blueprintable, ClassGroup=(InteractionSystem), meta=(BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintAssignable, Category = "Interactable")
	FOnInteractableStateChanged OnBeginOverlap;

	UPROPERTY(BlueprintAssignable, Category = "Interactable")
	FOnInteractableStateChanged OnEndOverlap;

	UPROPERTY(BlueprintAssignable, Category = "Interactable")
	FOnInteractableStateChanged OnInteract;
	
	UInteractableComponent();

	UFUNCTION()
	void Interact(UInteractorComponent* Interactor);

	void UpdateCurrentInteractionTime(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Interactable", meta = (DisplayName = "Get Interactable Data"))
	const FInteractableData& GetInteractableData() const { return InteractableData; }
	float GetCurrentInteractionTime() const { return CurrentInteractionTime; }
	
protected:
	// Row name from Interactable Data Table
	UPROPERTY(EditDefaultsOnly, Category = "Interactable")
	FInteractableData InteractableData;
	
	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> PrimitiveComponent;

	// Cached interactor. Set up in Interact function.
	UPROPERTY()
	TObjectPtr<UInteractorComponent> CurrentInteractor;

	FTimerHandle InteractionTimerHandle;
	float CurrentInteractionTime;
	uint32 HoldReleaseBindingHandle;
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void SetupOverlapCallbacks();
	void ResetInteractionTime();
	void BindHoldReleaseActionBinding();
	void UnbindHoldReleaseActionBinding();
	UEnhancedInputComponent* GetInputComponent() const;

	UFUNCTION()
	void OnBeginOverlap_Implementation(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap_Implementation(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);
};
