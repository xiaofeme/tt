#ifndef __IDIMENSION_H__
#define __IDIMENSION_H__
#include "platform_types.h"
class iGameDimension
{
    //unit : meter
    public:
        virtual f32 getTableLong() = 0;
        virtual f32 getTableWidth() = 0;
        virtual f32 getNetWidth() = 0;
        virtual f32 getNetHight() = 0;
        virtual f32 getBallSize() = 0;
        virtual f32 getTableHight() = 0;
};

extern iGameDimension * getGameDimension();

#endif