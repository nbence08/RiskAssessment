#include "physicalzonegraphicsview.hpp"

static int nameCounter = 0;

PhysicalZoneGraphicsView::PhysicalZoneGraphicsView():DiagramItem(":/icons/PZone", &"pZone"[nameCounter++])
{

}
