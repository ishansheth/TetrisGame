#pragma once
#include "DisplayContainer.h"
#include "IShape.h"

class StageManager {
    unsigned int currentStageNumber;
    DisplayContainer disPlayContainer;

    public:
    StageManager ();

    void generateStageScreen ();

    std::vector<IShape*> getAllowedShapedOfStage ();
};