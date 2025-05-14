#pragma once
#include "BoneDefine.h"

class UAnimSequence;

class FAnimationRuntime
{
public:
    static TArray<FBonePose> BlendTwoPoseTogether(const TArray<FBonePose>& CurrentPose, const TArray<FBonePose>& BlendPose, float BlendProcess);

    static TArray<FBonePose> AccumulateAdditivePose(const TArray<FBonePose>& BasePose, const TArray<FBonePose>& AdditivePose);
};
