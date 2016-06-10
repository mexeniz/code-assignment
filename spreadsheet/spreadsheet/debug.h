//
//  debug.h
//  spreadsheet
//
//  Created by pawissakan.ch on 6/9/2559 BE.
//  Copyright © 2559 Throughwave. All rights reserved.
//

#ifndef debug_h
#define debug_h

// #define MYDEBUG 0

#ifdef MYDEBUG
    #define DEBUG_PRINT printf
#else
    #define DEBUG_PRINT
#endif

#endif /* debug_h */