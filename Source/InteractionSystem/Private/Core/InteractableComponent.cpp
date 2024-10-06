// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "Core/InteractableComponent.h"
#include "Core/InteractorComponent.h"
#include "Core/InteractableDataSet.h"
#include "Core/InteractionSystemSettings.h"
#include "LogChannels.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InteractableComponent)

UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentInteractionTime = 0.0f;
	InteractableData = nullptr;
	HoldReleaseBindingHandle = 0;
}

void UInteractableComponent::Interact(UInteractorComponent* Interactor)
{
	check(Interactor);
	CurrentInteractor = Interactor;
	
	switch (InteractableData->InteractionInputType)
	{
	case EInteractionInputType::Press:
	{
		OnInteract.Broadcast(CurrentInteractor, this);
		break;
	}
	case EInteractionInputType::Hold:
	{
		BindHoldReleaseActionBinding();
		
		GetWorld()->GetTimerManager().SetTimer(InteractionTimerHandle, FTimerDelegate::CreateLambda([this, &Interactor]
		{
			OnInteract.Broadcast(CurrentInteractor, this);
			CurrentInteractor->RemoveHoldInteractable(this);
		}), InteractableData->InteractionDuration, false);

		break;
	}
	}
}

void UInteractableComponent::UpdateCurrentInteractionTime(float DeltaTime)
{
	if (CurrentInteractionTime + DeltaTime < InteractableData->InteractionDuration)
	{
		CurrentInteractionTime += DeltaTime;
	}
	else
	{
		CurrentInteractionTime = InteractableData->InteractionDuration;
	}
}

void UInteractableComponent::BP_GetInteractableData(FInteractableData& OutInteractableData) const
{
	if (InteractableData)
	{
		OutInteractableData = *InteractableData;
	}
	else
	{
		OutInteractableData = FInteractableData();
	}
}

void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();
	FindInteractableData();
	CreateDetectionSphere();
}

void UInteractableComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindHoldReleaseActionBinding();
	
	Super::EndPlay(EndPlayReason);
}

void UInteractableComponent::CreateDetectionSphere()
{
	AActor* Owner = GetOwner();
	DetectionSphere = CastChecked<USphereComponent>(Owner->AddComponentByClass(
		USphereComponent::StaticClass(), true, Owner->GetTransform(), false));
	Owner->SetRootComponent(DetectionSphere);
	
	if (InteractableData)
	{
		UE_LOG(LogInteractionSystem, Display, TEXT("Detection Distance: %f"), InteractableData->DetectionDistance)
		DetectionSphere->InitSphereRadius(InteractableData->DetectionDistance);
	}
	else
	{
		constexpr float SphereRadius = 192.0f;
		DetectionSphere->InitSphereRadius(SphereRadius);
	}
	
	DetectionSphere->SetGenerateOverlapEvents(true);
	DetectionSphere->SetCollisionResponseToAllChannels(ECR_Overlap);

	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap_Implementation);
	DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap_Implementation);
}

void UInteractableComponent::FindInteractableData()
{
	const UInteractionSystemSettings* Settings = GetDefault<UInteractionSystemSettings>();

	for (const TSoftObjectPtr<UInteractableDataSet>& DataAsset : Settings->InteractableDataAssets)
	{
		InteractableData = DataAsset.LoadSynchronous()->InteractableData.Find(InteractableDataName);
		if (InteractableData)
		{
			return;
		}
	}

	UE_LOG(LogInteractionSystem, Error, TEXT("Failed to find Interactable Data with name [%s]"),
		*InteractableDataName.ToString());
}

void UInteractableComponent::ResetInteractionTime()
{
	GetWorld()->GetTimerManager().ClearTimer(InteractionTimerHandle);
	CurrentInteractionTime = 0.0f;
}

void UInteractableComponent::BindHoldReleaseActionBinding()
{
	HoldReleaseBindingHandle = GetInputComponent()->BindAction(InteractableData->InteractionAction, ETriggerEvent::Completed,
			this, &ThisClass::ResetInteractionTime).GetHandle();
}

void UInteractableComponent::UnbindHoldReleaseActionBinding()
{
	if (HoldReleaseBindingHandle)
	{
		GetInputComponent()->RemoveBindingByHandle(HoldReleaseBindingHandle);
		HoldReleaseBindingHandle = 0;
	}
}

UEnhancedInputComponent* UInteractableComponent::GetInputComponent() const
{
	return CastChecked<UEnhancedInputComponent>(CurrentInteractor->GetOwner()->InputComponent);
}

void UInteractableComponent::OnBeginOverlap_Implementation(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (UInteractorComponent* Interactor = UInteractorComponent::FindInteractorComponent(OtherActor))
	{
		Interactor->AddOverlappedInteractable(this);
		OnBeginOverlap.Broadcast(Interactor, this);
	}
}

void UInteractableComponent::OnEndOverlap_Implementation(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (UInteractorComponent* Interactor = UInteractorComponent::FindInteractorComponent(OtherActor))
	{
		UnbindHoldReleaseActionBinding();
		Interactor->RemoveOverlappedInteractable(this);
		OnEndOverlap.Broadcast(Interactor, this);
	}
}
