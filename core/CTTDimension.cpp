
#include "IDimension.hpp"

class CTTGameDimension: public iGameDimension
{
	public:
 	CTTGameDimension(){}

		f32 getTableLong()
		{
			return 2.74f;
		}
        f32 getTableWidth()
		{
			return 1.525f;
		}

        f32 getNetWidth()
		{
			return 0.1525f + 1.525f;
		}

        f32 getNetHight()
		{
			return 0.1525f;
		}

        f32 getBallSize()
		{
			return 0.02f;
		}
        f32 getTableHight()
		{
			return 0.76f;
		}
};

iGameDimension * getGameDimension()
{
   static CTTGameDimension dimention;
   return &dimention;
}