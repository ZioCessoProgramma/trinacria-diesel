#pragma once

#include "Texture.h"
#include "Renderer.h"

namespace TRCN_CORE_NAMESPACE
{
	/**
	 * @brief polymorphic child class of Texture, it represents a spritesheet
	 * @note use this when dealing with Tiles, and obviously with spritesheets
	 */

	class SpriteSheet : public Texture
	{
	public:
		/**
		 * @brief constructor of the spritesheet
		 * @param cellWidth width of the cell in range 0 to width
		 * @param cellHeight height of the cell in range 0 to height
		 */

		SpriteSheet(float cellWidth, float cellHeight) : CellWidth(cellWidth), CellHeight(cellHeight) { }

		/**
		 *
		 * @param xIndex the index in x coordinate in the grid
		 * @param yIndex the index in y coordinate in the grid
		 * @return returns the tex coords of the cell
		 */
		QuadTexCoords GetCell(uint32_t xIndex, uint32_t yIndex);

		/**
		 * @brief the width of the cell in range 0 to width
		 */

		float CellWidth;

		/**
		 * @brief the height of the cell in range 0 to height
		 */

		float CellHeight;
	};

	/**
	 * @brief class representing a sprite
	 */

	class Sprite
	{
	public:
		/**
		 * @brief constructor of the sprite
		 * @param parent the spritesheet the sprite is from
		 * @param xIndex the x index of the cell of the sprite
		 * @param yIndex the y index of the cell of the sprite
		 * @param xExtent how many cell is the sprite long
		 * @param yExtent how many cell is the sprite tall
		 */

		Sprite(SpriteSheet* parent, uint32_t xIndex, uint32_t yIndex, uint32_t xExtent = 1, uint32_t yExtent = 1) :
			_parent(parent), _xIndex(xIndex), _yIndex(yIndex), _xExtent(xExtent), _yExtent(yExtent)
		{
			_initialized = true;
		};

		Sprite() = default;

		/**
		 * @brief init function of the sprite
		 * @param parent the spritesheet the sprite is from
		 * @param xIndex the x index of the cell of the sprite
		 * @param yIndex the y index of the cell of the sprite
		 * @param xExtent how many cell is the sprite long
		 * @param yExtent how many cell is the sprite tall
		 */

		void Init(SpriteSheet* parent, uint32_t xIndex, uint32_t yIndex, uint32_t xExtent = 1, uint32_t yExtent = 1);

		/**
		 * @brief get the texture coordinates of the sprite
		 * @return the texture coordinates of the sprite
		 */

		QuadTexCoords GetTexCoords();

		/**
		 * @brief get the triangle texture coordinates of the sprite
		 * @return returns the triangle texture coordinates of the sprite
		 */

		TriangleTexCoords GetTriangleTexCoords();

		/**
		 * @brief get the spritesheet parent of the sprite
		 * @return returns spritesheet of the sprite
		 */

		SpriteSheet* GetParent();

	private:
		SpriteSheet* _parent;

		uint32_t _xIndex, _yIndex;
		uint32_t _xExtent, _yExtent;

		bool _initialized = false;
	};
}
