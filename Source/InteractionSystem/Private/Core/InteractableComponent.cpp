// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "Core/InteractableComponent.h"
#include "Core/InteractorComponent.h"
#include "Core/InteractableDataSet.h"
#include "Core/InteractionSystemSettings.h"
#include "LogChannels.h"

#include "Components/SphereComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InteractableComponent)

UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractableComponent::Interact(UInteractorComponent* Interactor)
{
	OnInteract.Broadcast(Interactor, this);
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

	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::BeginOverlap);
	DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::EndOverlap);
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

void UInteractableComponent::BeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (UInteractorComponent* Interactor = UInteractorComponent::FindInteractorComponent(OtherActor))
	{
		Interactor->AddInteractable(this);
		OnBeginOverlap.Broadcast(Interactor, this);
	}
}

void UInteractableComponent::EndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (UInteractorComponent* Interactor = UInteractorComponent::FindInteractorComponent(OtherActor))
	{
		Interactor->RemoveInteractable(this);
		OnEndOverlap.Broadcast(Interactor, this);
	}
}
