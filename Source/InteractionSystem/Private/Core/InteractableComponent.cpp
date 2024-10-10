// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "Core/InteractableComponent.h"
#include "Core/InteractorComponent.h"
#include "LogChannels.h"

#include "InputAction.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"
#include "Components/PrimitiveComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InteractableComponent)

UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	CurrentInteractionTime = 0.0f;
	HoldReleaseBindingHandle = 0;
}

void UInteractableComponent::Interact(UInteractorComponent* Interactor)
{
	check(Interactor);
	CurrentInteractor = Interactor;
	
	switch (InteractableData.InteractionInputType)
	{
	case EInteractionInputType::Press:
	{
		OnInteract.Broadcast(CurrentInteractor, this);
		break;
	}
	case EInteractionInputType::Hold:
	{
		BindHoldReleaseActionBinding();
		
		GetWorld()->GetTimerManager().SetTimer(InteractionTimerHandle, FTimerDelegate::CreateLambda([this]
		{
			OnInteract.Broadcast(CurrentInteractor, this);
			CurrentInteractor->RemoveHoldInteractable(this);
		}), InteractableData.InteractionDuration, false);

		break;
	}
	}
}

void UInteractableComponent::UpdateCurrentInteractionTime(float DeltaTime)
{
	if (CurrentInteractionTime + DeltaTime < InteractableData.InteractionDuration)
	{
		CurrentInteractionTime += DeltaTime;
	}
	else
	{
		CurrentInteractionTime = InteractableData.InteractionDuration;
	}
}

void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();
	SetupOverlapCallbacks();
}

void UInteractableComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindHoldReleaseActionBinding();
	Super::EndPlay(EndPlayReason);
}

void UInteractableComponent::SetupOverlapCallbacks()
{
	AActor* Owner = GetOwner();
	check(Owner);
	PrimitiveComponent = Owner->FindComponentByClass<UPrimitiveComponent>();
	
	PrimitiveComponent->SetGenerateOverlapEvents(true);
	PrimitiveComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

	PrimitiveComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap_Implementation);
	PrimitiveComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap_Implementation);
}

void UInteractableComponent::ResetInteractionTime()
{
	GetWorld()->GetTimerManager().ClearTimer(InteractionTimerHandle);
	CurrentInteractionTime = 0.0f;
}

void UInteractableComponent::BindHoldReleaseActionBinding()
{
	HoldReleaseBindingHandle = GetInputComponent()->BindAction(InteractableData.InteractionAction, ETriggerEvent::Completed,
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
