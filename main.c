/*
** Jo Sega Saturn Engine
** Copyright (c) 2012-2017, Johannes Fetz (johannesfetz@gmail.com)
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the Johannes Fetz nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL Johannes Fetz BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <jo/jo.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

jo_camera	cam;
jo_palette	image_pal;

static	float	tx, ty, tz;
static	float	px, py, pz;
static	short	yvel = 0;
static	short	levelID = 3;
static	bool	jump = true;
static	bool	winner = false;
static	char*	error;
static	bool	use_light = true;

static	char*	nums[40][3] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17" "18", "19", "20", "21", "22", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40"};

typedef struct
{
    /* Must be the first field in our custom structure in order to let the storyboard retrieve where the location of our object is */
	jo_3d_object_attributes	attributes;
	int sprite_id;
    // Add other attributes here or not ;)
}	player_t;

static	player_t	player;

void	initCamera(jo_camera * curCam)
{
	(*curCam).viewpoint[X] = toFIXED(0.0);
	(*curCam).viewpoint[Y] = toFIXED(0.0);
	(*curCam).viewpoint[Z] = toFIXED(-150.0);
	(*curCam).z_angle = DEGtoANG(0.0);
	(*curCam).target[X] = toFIXED(0.0);
	(*curCam).target[Y] = toFIXED(0.0);
	(*curCam).target[Z] = toFIXED(0.0);
	slWindow(0, 0, JO_TV_WIDTH-1, JO_TV_HEIGHT-1, 2500, JO_TV_WIDTH_2, JO_TV_HEIGHT_2);
	slPerspective(DEGtoANG(90)); //FOV
}

void	initPlayer(player_t * curPlayer)
{
	(*curPlayer).attributes.x = 0;
	(*curPlayer).attributes.y = -40;
	(*curPlayer).attributes.z = 0;
	(*curPlayer).sprite_id = 1;
}

jo_palette	*my_tga_palette_handling(void)
{
	// We create a new palette for each image. It's not optimal but OK for a demo.
	jo_create_palette(&image_pal);
	return (&image_pal);
}

void	loadTile(void)
{
	jo_img_8bits	tile;
	
	jo_core_tv_off();
	
	jo_enable_background_3d_plane(JO_COLOR_Black);

	//tile.height = 64;
	//tile.width = 64;
	tile.data = JO_NULL;

	jo_tga_8bits_loader(&tile, "TEX", "T.TGA", 0);
	jo_background_3d_plane_a_img(&tile, image_pal.id, true, true);
	jo_free_img(&tile);
	
	jo_core_tv_on();
}

void	my_draw(void)
{
	static short    rx, ry, rz;

	jo_printf(1, 1, jo_get_last_error());
	jo_printf(15, 1, error);
	slPrintFX(toFIXED(jo_3d_get_polygon_count()), slLocate(0,3));
	slPrintFX(toFIXED(jo_3d_get_vertices_count()), slLocate(0,4));
	slPrintFX(toFIXED(jo_3d_get_displayed_polygon_count()), slLocate(0,5));
	slPrintFX(toFIXED(jo_3d_get_displayed_polygon_count()), slLocate(0,5));
	slPrintFX(toFIXED(jo_sprite_count()), slLocate(0,6));
	jo_3d_camera_look_at(&cam);
	jo_3d_push_matrix();
	{
		jo_3d_translate_matrix(JO_CHANGE_SIGN(px), JO_CHANGE_SIGN(py) + 3, JO_CHANGE_SIGN(pz));
		//jo_map_3d_draw(0, 0, 165, 125);
		jo_3d_rotate_matrix(-90, 0, 0);
		jo_map_3d_draw(0, 0, 0, 30);
		jo_map_3d_draw(1, 500, 500, 30);
		jo_map_3d_draw(2, 500, -500, 30);
		jo_3d_draw_sprite_at(1, 0, 100, 0);
	}
	jo_3d_pop_matrix();
	jo_3d_push_matrix();
	{
		jo_3d_rotate_matrix_x(-30);
		jo_3d_draw_sprite_at(player.sprite_id, player.attributes.x, player.attributes.y, player.attributes.x);
		jo_3d_draw_sprite_at(player.sprite_id, player.attributes.x + 50, player.attributes.y, player.attributes.x);
		rx += 1;
		jo_3d_rotate_matrix_x(rx);
		jo_3d_draw_sprite_at(player.sprite_id, player.attributes.x - 50, player.attributes.y, player.attributes.x);
	}
	jo_3d_pop_matrix();

	jo_3d_push_matrix();
	{
		jo_3d_translate_matrix(JO_DIV_BY_8(JO_CHANGE_SIGN((int)px)), JO_DIV_BY_8(JO_CHANGE_SIGN((int)py)), 0);
		jo_background_3d_plane_a_draw(true);
	}

	jo_3d_pop_matrix();
	//jo_clear_background(JO_COLOR_Black);
	//draw_nbg1();
}

void	draw_nbg1(void)
{
	//jo_background_3d_plane_a_img(&bg, &palette1.id, 1, 0);
	//jo_background_3d_plane_a_draw(0);
	//jo_draw_background_line(JO_TV_WIDTH / 2, JO_TV_HEIGHT / 2, (tx / 10) + (JO_TV_WIDTH / 2), (tz / 10) + (JO_TV_HEIGHT / 2), JO_COLOR_Blue);
	jo_vdp2_draw_bitmap_nbg1_square(5, JO_TV_HEIGHT_4, JO_TV_WIDTH - 10, JO_TV_HEIGHT - JO_TV_HEIGHT_4, JO_COLOR_Green);
}

void	apply_effect(void)
{
	jo_3d_set_light(jo_3d_get_sprite_quad(0), use_light);
	jo_3d_set_light(jo_3d_get_sprite_quad(1), use_light);
	//jo_map_apply_light_effect(0, true);
}

void	animate_player(void)
{
	jo_storyboard               *storyboard;
	jo_animation                *animation;

	// We create a new storyboard for all saw (that means that you can use the same storyboard with multiple object)
	storyboard = jo_storyboard_create(true, false);
	// Now we add a new custom animation (smooth translation + rotation) (that means that you can add multiple animation)
	animation = jo_storyboard_create_animation(storyboard, 0, 0);
	animation->sin_radius = 10;
	animation->translation_speed_y = 2;
	animation->cos_radius = 4;
	animation->translation_speed_x = 1;
	//animation->rotation_speed = 10;
	// Now we can animate any object
	jo_storyboard_add_object(storyboard, &player);
	// Next step in my_draw()
}

bool	update_player(void)
{
	if(!JO_FLOAT_NEARLY_EQUALS(px, tx)){
		if(px < tx){
			px += 4;
			player.sprite_id = 4;
		} else {
			px -= 4;
			player.sprite_id = 3;
		}
		return false;
	}

	if(!JO_FLOAT_NEARLY_EQUALS(py, ty) && jump){
		if(py == 0){
			yvel = 16;
			py -= yvel;
			ty = 0;
		} else {
			yvel -= 1;
			py -= yvel;
		}
		return false;
	}

	if(!JO_FLOAT_NEARLY_EQUALS(py, ty) && !jump){
		if(py < ty){
			py += 4;
		} else {
			py -= 4;
		}
		return false;
	}

	if(!JO_FLOAT_NEARLY_EQUALS(pz, tz)){
		if(pz < tz){
			pz += 4;
			player.sprite_id = 2;
		} else {
			pz -= 4;
			player.sprite_id = 1;
		}
		return false;
	} 

	return true;
}


void	load_sprites()
{
	jo_sprite_add_tga("TEX", "BRK.TGA", JO_COLOR_Green);
	jo_sprite_add_tga("TEX", "A.TGA", JO_COLOR_Green);
	jo_sprite_add_tga("TEX", "2.TGA", JO_COLOR_Green);
	jo_sprite_add_tga("TEX", "5.TGA", JO_COLOR_Green);
	jo_sprite_add_tga("TEX", "6.TGA", JO_COLOR_Green);
	jo_sprite_add_tga("TEX", "TX2.TGA", JO_COLOR_Transparent);
}

void	load_sprite(char* filename)
{
	jo_sprite_add_tga("TEX", filename, JO_COLOR_Transparent);
}

void	load_map()
{
	//jo_map_load_from_file(WORLD_MAP_ID, 450, "MAP", "WALL.MAP");
	//jo_map_load_from_file(WORLD_MAP_ID, 450, "MAP", "MAP.MAP");
	//jo_map_load_from_file(WORLD_MAP_ID, 450, "MAP", "WALL2.MAP");
	//if(!jo_map_load_from_file(WORLD_MAP_ID, 450, "MAP", "MAP3.MAP"))
	//	jo_printf(1, 2, "map error!");
	//if(!jo_map_load_from_file(mapID, depth, dir, filename))
		//jo_printf(1, 2, "map error!");
	if(!jo_map_load_from_file(0, 250, "MAP", "M0L0.MAP"))
		jo_printf(1, 2, "map error!");
	if(!jo_map_load_from_file(1, 250, "MAP", "M0L1.MAP"))
		jo_printf(1, 2, "map error!");
	if(!jo_map_load_from_file(2, 250, "MAP", "M0L2.MAP"))
		jo_printf(1, 2, "map error!");
}

//void			setup_bg()
//{
//	jo_enable_background_3d_plane(JO_COLOR_Green);
//	bg.data = NULL;
//	jo_tga_loader(&bg, "TEX", "C.TGA", JO_COLOR_Transparent);
//}
//
void	my_gamepad(void)
{
	if (!jo_is_pad1_available())
		return;

	if(update_player()){
		if (jo_is_pad1_key_pressed(JO_KEY_LEFT)){
			tx -= 64;
		}
		if (jo_is_pad1_key_pressed(JO_KEY_RIGHT)){
			tx += 64;
		}
		if (jo_is_pad1_key_pressed(JO_KEY_UP)){
			tz += 64;
		}
		if (jo_is_pad1_key_pressed(JO_KEY_DOWN)){
			tz -= 64;
		}
		if (jo_is_pad1_key_pressed(JO_KEY_A)){
			ty -= 64;
		}
		if (jo_is_pad1_key_pressed(JO_KEY_B)){
			ty += 64;
		}
		if (jo_is_pad1_key_pressed(JO_KEY_L)){
			jump ^= true;
		}
	}
}

void	jo_main(void)
{
	jo_core_init(JO_COLOR_DarkGray);
	//jo_3d_camera_init(&cam);
	//jo_set_tga_palette_handling(my_tga_palette_handling);
	initCamera(&cam);
	initPlayer(&player);
	jo_3d_camera_set_viewpoint(&cam, 0, -130, -130);
	jo_set_tga_palette_handling(my_tga_palette_handling);
	loadTile();
	load_sprite("BRK.TGA");
	load_sprite("A.TGA");
	load_sprite("2.TGA");
	load_sprite("5.TGA");
	load_sprite("6.TGA");
	load_sprite("TX2.TGA");
	//load_sprites();
	//setup_bg();
	//initLevel(levelID);
	//parseMapFile();
	animate_player();
	load_map();
	jo_3d_light(-10, -10, -10);
	apply_effect();
	jo_core_add_callback(my_draw);
	jo_core_add_callback(my_gamepad);
	//jo_core_add_callback(update_camera);
	jo_core_run();
}

/*
** END OF FILE
*/
