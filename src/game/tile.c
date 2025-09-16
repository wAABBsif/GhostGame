#include "tile.h"

#include <string.h>

#include "gfx/tile_render.h"

bool tile_is_flip_x(const tile t)
{
	return t.transform & TILE_FLIP_X_MASK;
}

bool tile_is_flip_y(const tile t)
{
	return t.transform & TILE_FLIP_Y_MASK;
}

bool tile_is_rotate_ccw(const tile t)
{
	return t.transform & TILE_ROTATE_CCW_MASK;
}

bool tile_is_rotate_cw(const tile t)
{
	return t.transform & TILE_ROTATE_CW_MASK;
}

uint8_t tile_get_z(const tile t)
{
	return t.transform & TILE_Z_MASK;
}

void set_tilemap_atlas(const texture t)
{
	tile_rendering_set_texture(t);
}

void tile_draw(const uint16_t x, const uint16_t y, const tile t)
{
	const uint8_t z = tile_get_z(t);

	tile_quad quad = {};
	quad.vertices[0] = tile_vertex_set_all(0, x + 0, y + 1, z, 1);
	quad.vertices[1] = tile_vertex_set_all(0, x + 0, y + 0, 0, 0);
	quad.vertices[2] = tile_vertex_set_all(0, x + 1, y + 0, z, 16);
	quad.vertices[3] = tile_vertex_set_all(0, x + 1, y + 1, z, 17);

	if (tile_is_flip_x(t))
	{
		tile_quad nquad;
		nquad.vertices[0] = tile_vertex_set_x(quad.vertices[0], tile_vertex_get_x(quad.vertices[3]));
		nquad.vertices[1] = tile_vertex_set_x(quad.vertices[1], tile_vertex_get_x(quad.vertices[2]));
		nquad.vertices[2] = tile_vertex_set_x(quad.vertices[2], tile_vertex_get_x(quad.vertices[1]));
		nquad.vertices[3] = tile_vertex_set_x(quad.vertices[3], tile_vertex_get_x(quad.vertices[0]));

		quad = nquad;
	}

	if (tile_is_flip_y(t))
	{
		tile_quad nquad;
		nquad.vertices[0] = tile_vertex_set_y(quad.vertices[0], tile_vertex_get_y(quad.vertices[1]));
		nquad.vertices[1] = tile_vertex_set_y(quad.vertices[1], tile_vertex_get_y(quad.vertices[0]));
		nquad.vertices[2] = tile_vertex_set_y(quad.vertices[2], tile_vertex_get_y(quad.vertices[3]));
		nquad.vertices[3] = tile_vertex_set_y(quad.vertices[3], tile_vertex_get_y(quad.vertices[2]));

		quad = nquad;
	}

	if (tile_is_rotate_ccw(t))
	{
		tile_quad nquad;
		nquad.vertices[0] = tile_vertex_set_xy(quad.vertices[0], tile_vertex_get_x(quad.vertices[3]), tile_vertex_get_y(quad.vertices[3]));
		nquad.vertices[1] = tile_vertex_set_xy(quad.vertices[1], tile_vertex_get_x(quad.vertices[0]), tile_vertex_get_y(quad.vertices[0]));
		nquad.vertices[2] = tile_vertex_set_xy(quad.vertices[2], tile_vertex_get_x(quad.vertices[1]), tile_vertex_get_y(quad.vertices[1]));
		nquad.vertices[3] = tile_vertex_set_xy(quad.vertices[3], tile_vertex_get_x(quad.vertices[2]), tile_vertex_get_y(quad.vertices[2]));

		quad = nquad;
	}

	if (tile_is_rotate_cw(t))
	{
		tile_quad nquad;
		nquad.vertices[0] = tile_vertex_set_xy(quad.vertices[0], tile_vertex_get_x(quad.vertices[1]), tile_vertex_get_y(quad.vertices[1]));
		nquad.vertices[1] = tile_vertex_set_xy(quad.vertices[1], tile_vertex_get_x(quad.vertices[2]), tile_vertex_get_y(quad.vertices[2]));
		nquad.vertices[2] = tile_vertex_set_xy(quad.vertices[2], tile_vertex_get_x(quad.vertices[3]), tile_vertex_get_y(quad.vertices[3]));
		nquad.vertices[3] = tile_vertex_set_xy(quad.vertices[3], tile_vertex_get_x(quad.vertices[0]), tile_vertex_get_y(quad.vertices[0]));

		quad = nquad;
	}

	tile_rendering_add_tile(quad);
}