#pragma once

#include "Define.h"
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "Container/Map.h"
#include "Core/Math/Quat.h"
>>>>>>> main
=======
#include "Container/Map.h"
#include "Core/Math/Quat.h"
>>>>>>> main
=======
#include "Container/Map.h"
#include "Core/Math/Quat.h"
>>>>>>> main

struct FRawAnimSequenceTrack
{
    TArray<FVector> PosKeys;
    TArray<FQuat> RotKeys;
    TArray<FVector> ScaleKeys;
};

struct FBoneAnimationTrack
{
    FRawAnimSequenceTrack InternalTrackData;
    int32 BoneTreeIndex = INDEX_NONE;
    FName Name;
};

struct FAnimNotifyEvent 
{
    float TriggerTime;
    float Duration;
    FName NotifyName;
};
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> main
=======
>>>>>>> main

struct FFrameRate
{
    /**
     * Default construction to a frame rate of 60000 frames per second (0.0166 ms)
     */
    FFrameRate()
        : Numerator(60000), Denominator(1)
    {
    }

    FFrameRate(uint32 InNumerator, uint32 InDenominator)
        : Numerator(InNumerator), Denominator(InDenominator)
    {
    }

    /** IMPORTANT: If you change the struct data, ensure that you also update the version in NoExportTypes.h  */

    /**
     * The numerator of the framerate represented as a number of frames per second (e.g. 60 for 60 fps)
     */
    int32 Numerator;

    /**
     * The denominator of the framerate represented as a number of frames per second (e.g. 1 for 60 fps)
     */
    int32 Denominator;
};

struct FAnimationCurveData
{
    TMap<FName, TArray<float>> FloatCurves;  // 이름별 플로트 커브 데이터
    TMap<FName, TArray<bool>> BoolCurves;    // 이름별 불린 커브 데이터
};
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> main
=======
>>>>>>> main
=======
>>>>>>> main
