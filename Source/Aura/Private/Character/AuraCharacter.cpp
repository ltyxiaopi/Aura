// Xiaopi


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter() {
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	// GetCharacterMovement()->bUseControllerDesiredRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AAuraCharacter::BeginPlay() {
	Super::BeginPlay();

	if (Controller)
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller is valid!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Controller is null!"));
	}
}

void AAuraCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
	
	//Init ability actor info for the Server
	InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_PlayerState() {
	Super::OnRep_PlayerState();

	//Init ability actor info for the Server
	InitAbilityActorInfo();
}

void AAuraCharacter::InitAbilityActorInfo() {
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	
	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if(AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController())) {
		if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD())) {
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}
