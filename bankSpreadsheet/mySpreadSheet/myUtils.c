//
//  myUtils.c
//  mySpreadSheet
//
//  Created by Bankde on 9/6/16.
//  Copyright © 2016 Bankde. All rights reserved.
//

#include "myUtils.h"

int validateIntBetween(int validated, int lowerBound, int upperBound)
{
    if (validated >= lowerBound && validated <= upperBound)
        return validated-lowerBound;
    else
        return -1;
}