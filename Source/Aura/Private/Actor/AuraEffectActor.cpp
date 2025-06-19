// Xiaopi


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"


AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TArray<TSubclassOf<UGameplayEffect>> GameplayEffectClasses) {
	if (GameplayEffectClasses.Num() == 0) return;

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	for (const TSubclassOf<UGameplayEffect>& EffectClass : GameplayEffectClasses) {
		check(EffectClass);
		FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(EffectClass, 1.f, EffectContextHandle);
		const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

		const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
		if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) {
			ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
		}
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor) {
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClasses); // 修改：传入数组
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClasses); // 修改：传入数组
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClasses); // 修改：传入数组
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor) {
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) {
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClasses); // 修改：传入数组
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) {
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClasses); // 修改：传入数组
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) {
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClasses); // 修改：传入数组
	}
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) {
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles) {
			if (TargetASC == HandlePair.Value) {
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		for (auto& Handle : HandlesToRemove) {
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}



