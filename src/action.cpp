#include "action.h"

DrawAction::DrawAction(Vector2 startPoint, Vector2 endPoint, uint32_t strokeWidth, Color color)
	:startPoint(startPoint), endPoint(endPoint), strokeWidth(strokeWidth), color(color)
{
}
