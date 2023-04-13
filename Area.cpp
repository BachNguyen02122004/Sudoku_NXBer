#include "Area.h"
#include "CellData.h"
#include <stdio.h>
#include <random>

Area::Area ()
{
}

Area::~Area ()
{
}

void Area::init (AreaType type, int idx)
{
	this->type = type;
	this->idx = idx;
}

void Area::DigitAdded (int &digit, CellData (&data) [9][9], int &x, int &y)
{
	counter[digit-1]++;
	if (counter[digit-1] > 1)
	{
		for (int i = 0; i < 9; i++)
		{
			int xi = -1, yi = -1;
			switch (type)
			{
				case AREA_ROW:
				{
					xi = i;
					yi = idx;
					break;
				}
				case AREA_COLUMN:
				{
					xi = idx;
					yi = i;
					break;
				}
				case AREA_BOX:
				{
					xi = 3 * (idx % 3) + (i % 3);
					yi = 3 * (idx / 3) + (i / 3);
					break;
				}
			}
			CellData& cd = data[xi][yi];
			if (cd.digit == digit)
			{
				if (xi == x && yi == y) 
					cd.vldcnt += counter[digit-1] - 1;
				else
					cd.vldcnt++;
				printf ("ValidityCount in area #%d of type %d, at offset %d (Cell(%d,%d)) = %d\n", idx, type, i, xi, yi, cd.vldcnt);
			}			
		}
	}
}

void Area::DigitRemoved (int &digit, CellData (&data) [9][9], int &x, int &y)
{
	counter[digit-1]--;
	if (counter[digit-1] >= 1)
	{
		for (int i = 0; i < 9; i++)
		{
			CellData * cd = nullptr;
			int xi = -1, yi = -1;
			switch (type)
			{
				case AREA_ROW:
				{
					xi = i;
					yi = idx;
					break;
				}
				case AREA_COLUMN:
				{
					xi = idx;
					yi = i;
					break;
				}
				case AREA_BOX:
				{
					xi = 3 * (idx % 3) + (i % 3);
					yi = 3 * (idx / 3) + (i / 3);
					break;
				}
			}
			cd = &data[xi][yi];
			
			if (xi == x && yi == y) // empty position
			{
				cd->vldcnt -= counter[digit-1];
				printf ("ValidityCount in area #%d of type %d, at offset %d (Cell(%d,%d)) = %d\n", idx, type, i, xi, yi, cd->vldcnt);
			}
			else if (cd->digit == digit) 
			{
				cd->vldcnt--;
				printf ("ValidityCount in area #%d of type %d, at offset %d (Cell(%d,%d)) = %d\n", idx, type, i, xi, yi, cd->vldcnt);
			}			
		}
	}
}