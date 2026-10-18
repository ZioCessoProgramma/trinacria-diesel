#include "Trinacria/Sprite.h"
#include "Trinacria/log.h"

TRCN_CORE_NAMESPACE::QuadTexCoords TRCN_CORE_NAMESPACE::SpriteSheet::GetCell(uint32_t xIndex, uint32_t yIndex)
{
	QuadTexCoords out(0.f);

	out.Coord0 = { xIndex * CellWidth, yIndex * CellHeight };
	out.Coord1 = { (xIndex + 1) * CellWidth, yIndex * CellHeight };
	out.Coord2 = { (xIndex + 1) * CellWidth, (yIndex + 1)* CellHeight };
	out.Coord3 = { xIndex * CellWidth, (yIndex + 1)* CellHeight };

	return out;
}

TRCN_CORE_NAMESPACE::QuadTexCoords TRCN_CORE_NAMESPACE::Sprite::GetTexCoords()
{
	QuadTexCoords out = _parent->GetCell(_xIndex, _yIndex);

	out.Coord1.x += (_xExtent - 1) * _parent->CellWidth;

	out.Coord2.y += (_yExtent - 1) * _parent->CellHeight;
	out.Coord3.y += (_yExtent - 1) * _parent->CellHeight;

	return out;
}

Trinacria::DSL::TriangleTexCoords Trinacria::DSL::Sprite::GetTriangleTexCoords()
{
	QuadTexCoords cell = _parent->GetCell(_xIndex, _yIndex);

	TriangleTexCoords out(0.f);

	out.Coord0 = { cell.Coord0.x, cell.Coord0.y};
	out.Coord1 = { cell.Coord1.x, cell.Coord1.y};
	out.Coord2 = { cell.Coord2.x, cell.Coord2.y};

	return out;
}

TRCN_CORE_NAMESPACE::SpriteSheet* TRCN_CORE_NAMESPACE::Sprite::GetParent()
{
	return _parent;
}

void TRCN_CORE_NAMESPACE::Sprite::Init(SpriteSheet* parent, uint32_t xIndex, uint32_t yIndex, uint32_t xExtent, uint32_t yExtent)
{
	if (!_initialized)
	{
		_parent = parent;
		_xIndex = xIndex;
		_yIndex = yIndex;
		_xExtent = xExtent;
		_yExtent = yExtent;

		_initialized = true;
	}
	else
	{
		TRCN_LOG("Cannot initiliaze an asset more than one time");
	}
}
