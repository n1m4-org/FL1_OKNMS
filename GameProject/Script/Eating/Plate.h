#pragma once
#include "Script.h"

/// <summary>
/// 
/// </summary>
TCLASS()
class Plate : public Tako::Script {
    GENERATED_BODY()

    TPROPERTY(Edit, ClampMin=0, Speed=0.1)
    float amount_ = 1.0f;

public:
    /** @brief 皿から取る
     * @param amount 取りたい量(希望)
     * @return 実際に取れた量(現実)
     */
    float Take(float amount);
};
