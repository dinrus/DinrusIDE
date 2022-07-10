#include "PdfDraw.h"

namespace РНЦП {

void   TTFReader::дайГолову::сериализуй(TTFStream& s)
{
	s % version % fontRevision % checkсуммаAdjustment % magicNumber % flags % unitsPerEm;
	s.дайРяд(created, 8);
	s.дайРяд(modified, 8);
	s % xMin % yMin % xMax % yMax % macStyle % lowestRecPPEM % fontDirectionHint
	  % indexToLocFormat % glyphDataFormat;
}

void   TTFReader::Hhea::сериализуй(TTFStream& s)
{
	reserved1 = reserved2 = reserved3 = reserved4 = 0;
	s % version % ascent % descent % lineGap % advanceWidthMax % minLeftSideBearing
	  % minRightSideBearing % xMaxExtent % caretSlopeRise % caretSlopeRun % caretOffset
	  % reserved1 % reserved2 % reserved3 % reserved4 % metricDataFormat % numOfLongHorMetrics;
}

void   TTFReader::Maxp::сериализуй(TTFStream& s)
{
	s % version % numGlyphs % maxPoints % maxContours % maxComponentPoints
	  % maxComponentContours % maxZones % maxTwilightPoints % maxStorage
	  % maxFunctionDefs % maxInstructionDefs % maxStackElements
	  % maxSizeOfInstructions % maxComponentElements % maxComponentDepth;
}

void   TTFReader::пост::сериализуй(TTFStream& s)
{
	reserved = 0;
	s % формат % italicAngle % underlinePosition % underlineThickness % isFixedPitch
	  % reserved % minMemType42 % maxMemType42 % minMemType1 % maxMemType1;
}

}
