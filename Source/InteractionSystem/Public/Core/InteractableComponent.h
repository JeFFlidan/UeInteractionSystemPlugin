// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

class USphereComponent;
class UInteractorComponent;
struct FInteractableData;

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
	
	FInteractableData* GetInteractableData() const { return InteractableData; }

	UFUNCTION(BlueprintCallable, Category = "Interactable", meta = (DisplayName = "Get Interactable Data"))
	void BP_GetInteractableData(UPARAM(DisplayName = "Interactable Data") FInteractableData& OutInteractableData) const;

protected:
	// Row name from Interactable Data Table
	UPROPERTY(EditDefaultsOnly, Category = "Interactable")
	FName InteractableDataName;
	
	UPROPERTY()
	TObjectPtr<USphereComponent> DetectionSphere;

	FInteractableData* InteractableData{nullptr};
	
	virtual void BeginPlay() override;

	void CreateDetectionSphere();
	void FindInteractableData();

	UFUNCTION()
	void BeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);
};
