#include <ultra64.h>

void dma_copy(u8*, u8*, u8*);

extern u8 _customSegmentRomStart[];
extern u8 _customSegmentRomEnd[];
extern u8 _customSegmentStart[];

void custom_init(void);

void dma_code()
{
    dma_copy(_customSegmentRomStart, _customSegmentRomEnd, _customSegmentStart);
    custom_init();
}
