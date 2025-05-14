#include "AnimationRuntime.h"

#include "AnimSequence.h"

TArray<FBonePose> FAnimationRuntime::BlendTwoPoseTogether(const TArray<FBonePose>& CurrentPose, const TArray<FBonePose>& BlendPose, float BlendProcess)
{
    //블렌드 초반엔 원래 포즈의 가중치가 높아야함
    float CurrentWeight = 1 - BlendProcess;
    
    TArray<FBonePose> NewBonePoses;
    NewBonePoses.SetNum(CurrentPose.Num());
    
    //본 갯수가 똑같아야함
    for (int b = 0; b < CurrentPose.Num(); b++)
    {
        const FBonePose& BonePose = CurrentPose[b];
        const FBonePose& BlendBonePose = BlendPose[b];

        FBonePose& NewBonePose = NewBonePoses[b];
        
        NewBonePose.Location = (BonePose.Location * CurrentWeight) + (BlendBonePose.Location * BlendProcess);
        
        NewBonePose.Rotation = FQuat::Slerp(BonePose.Rotation, BlendBonePose.Rotation, BlendProcess).GetNormalized();

        NewBonePose.Scale = (BonePose.Scale * CurrentWeight) + (BlendBonePose.Scale * BlendProcess);
    }

    return NewBonePoses;
}

TArray<FBonePose> FAnimationRuntime::AccumulateAdditivePose(const TArray<FBonePose>& BasePose, const TArray<FBonePose>& AdditivePose)
{
    TArray<FBonePose> NewBonePoses;
    NewBonePoses.SetNum(BasePose.Num());
    
    //본 갯수가 똑같아야함
    for (int b = 0; b < BasePose.Num(); b++)
    {
        const FBonePose& BonePose = BasePose[b];
        const FBonePose& BlendBonePose = AdditivePose[b];

        FBonePose& NewBonePose = NewBonePoses[b];
        
        NewBonePose.Location = BonePose.Location + BlendBonePose.Location;
        
        NewBonePose.Rotation = (BlendBonePose.Rotation * BonePose.Rotation).GetNormalized();

        NewBonePose.Scale = BonePose.Scale + BlendBonePose.Scale;
    }

    return NewBonePoses;
}
