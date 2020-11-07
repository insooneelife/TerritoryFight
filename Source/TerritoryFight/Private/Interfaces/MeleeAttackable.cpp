#include "MeleeAttackable.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Hittable.h"


void IMeleeAttackable::TryMeleeAttack(AActor* InOwner)
{
    FHitResult OutHit;

    if (InOwner->HasAuthority())
    {
        EObjectTypeQuery ObjType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
        TArray<AActor*> ActorsToIgnore = TArray<AActor*>{ InOwner };
        TArray<AActor*> OutActors;
        FVector SpherePos = InOwner->GetActorLocation() + InOwner->GetActorForwardVector() * GetMeleeAttackRange();
        float SphereRadius = GetMeleeAttackAreaRadius();

        if (UKismetSystemLibrary::SphereOverlapActors(
            InOwner->GetWorld(),
            SpherePos,
            SphereRadius,
            TArray<TEnumAsByte<EObjectTypeQuery>> { ObjType },
            nullptr,
            ActorsToIgnore,
            OutActors))
        {
            //DrawDebugSphere(GetWorld(), SpherePos, 100.0f, 26, FColor(181, 0, 0), true, -1, 0, 2);

            for (int i = 0; i < OutActors.Num(); ++i)
            {
                AActor* FoundActor = OutActors[i];

                IHittable* HittableActor = Cast<IHittable>(FoundActor);

                if (HittableActor != nullptr)
                {
                    HittableActor->OnHit(GetMeleeAttackDamage(), 0);
                }

                UE_LOG(LogTemp, Warning, TEXT("Sphere overlap actor : %s"), *FoundActor->GetFName().ToString());

            }
        }

    }
}
