//
//  debug.h
//  mySpreadSheet
//
//  Created by Bankde on 7/6/16.
//  Copyright © 2016 Bankde. All rights reserved.
//

#ifndef debug_h
#define debug_h

//#define MYDEBUG 0

#ifdef MYDEBUG
    #define DEBUG_PRINT printf
#else
    #define DEBUG_PRINT
#endif

#endif /* debug_h */
