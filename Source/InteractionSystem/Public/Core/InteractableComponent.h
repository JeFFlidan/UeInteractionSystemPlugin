// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

class USphereComponent;
class UInteractorComponent;
class UEnhancedInputComponent;
struct FInteractableData;
struct FEnhancedInputActionEventBinding;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractableStateChanged, UInteractorComponent*, Interactor, UInteractableComponent*, Interactable);

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
	
	FInteractableData* GetInteractableData() const { return InteractableData; }
	float GetCurrentInteractionTime() const { return CurrentInteractionTime; }

	UFUNCTION(BlueprintCallable, Category = "Interactable", meta = (DisplayName = "Get Interactable Data"))
	void BP_GetInteractableData(UPARAM(DisplayName = "Interactable Data") FInteractableData& OutInteractableData) const;

protected:
	// Row name from Interactable Data Table
	UPROPERTY(EditDefaultsOnly, Category = "Interactable")
	FName InteractableDataName;
	
	UPROPERTY()
	TObjectPtr<USphereComponent> DetectionSphere;

	// Cached interactor. Set up in Interact function.
	UPROPERTY()
	TObjectPtr<UInteractorComponent> CurrentInteractor;

	FInteractableData* InteractableData;
	FTimerHandle InteractionTimerHandle;
	float CurrentInteractionTime;
	uint32 HoldReleaseBindingHandle;
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void CreateDetectionSphere();
	void FindInteractableData();
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
