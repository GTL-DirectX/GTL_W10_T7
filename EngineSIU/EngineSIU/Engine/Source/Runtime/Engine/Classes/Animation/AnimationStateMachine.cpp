#include "AnimationStateMachine.h"

#include "AnimationRuntime.h"
#include "AnimSequence.h"
#include "Actors/Character/Pawn.h"
#include "Components/SkeletalMesh/SkeletalMesh.h"

void UAnimationStateMachine::Initialize(APawn* InOwner)
{
    Owner = InOwner;
}

void UAnimationStateMachine::ProcessState()
{
    if (Owner->CurrentMovementMode == EIdle)
    {
        CurrentState = AS_Idle;
    }
    else if (Owner->CurrentMovementMode == EDancing)
    {
        CurrentState = AS_Dance;
    }
    else if (Owner->CurrentMovementMode == EDie)
    {
        CurrentState = AS_Die;
    }
    else if (Owner->CurrentMovementMode == EFire)
    {
        CurrentState = AS_Fire;
    }
}

void UAnimationStateMachine::StartAnimSequence(UAnimSequence* InSequence, float InBlendingTime)
{
    if (!CurrentSequence)
    {
        CurrentSequence = InSequence;
        CurrentSequence->BeginSequence();
        return;
    }

    BlendSequence = InSequence;
    BlendTime = InBlendingTime;
    BlendSequence->BeginSequence();
}

void UAnimationStateMachine::UpdateSequence(float DeltaTime, USkeletalMesh* InSkeletalMesh)
{
    if (AnimSequenceMap.IsEmpty())
    {
        return;
    }
    
    if (CurrentSequence)
    {
        CurrentSequence->TickSequence(DeltaTime);
    }

    if (BlendSequence)
    {
        BlendSequence->TickSequence(DeltaTime);
    }

    if (DifferenceSequence)
    {
        DifferenceSequence->TickSequence(DeltaTime);
    }
    
    //바뀌면 애니메이션 체인지 -> 추가할지 바로바꿀지 결정
    if (CurrentState != PreState)
    {
        StartAnimSequence(AnimSequenceMap[CurrentState], 1.0f);

        PreState = CurrentState;
    }

    // Delegate pose calculation to the sequence
    CurrentSequence->GetAnimationPose(InSkeletalMesh, CurrentPose);

    //블렌딩 중이면
    if (BlendSequence)
    {
        TArray<FBonePose> NewBlendPoses;
        
        BlendSequence->GetAnimationPose(InSkeletalMesh, NewBlendPoses);

        //블렌드 진행도 0~1
        float BlendProcess = BlendSequence->LocalTime / BlendTime;

        CurrentPose = FAnimationRuntime::BlendTwoPoseTogether(CurrentPose, NewBlendPoses, BlendProcess);
        
        // 현재 애니메이션이 끝나거나 블렌드시간이 지나면 애니메이션 교체
        if (CurrentSequence->LocalTime > CurrentSequence->GetUnScaledPlayLength() || BlendSequence->LocalTime > BlendTime)
        {
            CurrentSequence = BlendSequence;
            BlendSequence = nullptr;
            BlendTime = 0.f;
        }
    }

    //추가블렌드모드면
    if (DifferenceSequence)
    {
        TArray<FBonePose> DifferenceBlendPoses;

        DifferenceSequence->GetAnimationPose(InSkeletalMesh, DifferenceBlendPoses);

        CurrentPose = FAnimationRuntime::AccumulateAdditivePose(CurrentPose, DifferenceBlendPoses);
    }
}

void UAnimationStateMachine::SetAdditiveMode(UAnimSequence* AdditiveSequence, USkeletalMesh* InSkeletalMesh)
{
    const TArray<FBonePose>& BasePose = InSkeletalMesh->GetSkeletonPose()->BaseLocalTransform;

    //TODO: 복사해야함 지금은 같은 주소 가르쳐서 공유돼서 반복할때마다 계속 이상해짐
    DifferenceSequence = AdditiveSequence;
    DifferenceSequence->BeginSequence();
    
    UAnimDataModel* DifferenceDataModel = DifferenceSequence->GetDataModel();
    TArray<FBoneAnimationTrack> DifferenceAnimationTracks = DifferenceDataModel->GetBoneAnimationTracks();
    
    const FSkeleton* Skeleton = InSkeletalMesh->GetSkeleton();
    int32 BoneCount = Skeleton->BoneCount;

    TMap<FName, int32> BoneIndexMap;
    for (int32 i = 0; i < BoneCount; ++i)
    {
        BoneIndexMap.Add(Skeleton->Bones[i].Name, i);
    }
    
    for (FBoneAnimationTrack& Track : DifferenceAnimationTracks)
    {
        FRawAnimSequenceTrack& Raw = Track.InternalTrackData;
        
        const int32* IndexPtr = BoneIndexMap.Find(Track.Name);
        if (!IndexPtr) continue;
        int32 BoneIndex = *IndexPtr;
        
        for (int i=0;i<Raw.PosKeys.Num(); i++)
        {
            Raw.PosKeys[i] = Raw.PosKeys[i] - BasePose[BoneIndex].Location;
            Raw.RotKeys[i] = FQuat::Inverse(BasePose[BoneIndex].Rotation) * Raw.RotKeys[i];
            Raw.ScaleKeys[i] = FVector::ZeroVector; //스케일이 안변한다고 가정
        }
    }

    DifferenceSequence->GetDataModel()->SetBoneAnimationTracks(DifferenceAnimationTracks);
}
