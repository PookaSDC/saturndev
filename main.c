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

#define WORLD_MAP_ID		(0)

jo_camera	cam;
jo_img_8bits	bg;
//static jo_palette      palette1;

static	float	tx, ty, tz;
static	float	px, py, pz;
static	short	pSpriteIndex = 1;
static	short	yvel = 0;
static	short	levelID = 3;
static	char	filename[10];
static	char	file[64];

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

// Each time a 8 bits TGA is loaded, this function is called. (jo_set_tga_palette_handling)
//jo_palette      *my_tga_palette_handling(void)
//{
//    // Here you can handle palettes as you want.
//    // You can create a new one or pass an existing.
//    // jo_create_palette() also give you an ID for your palette (palette1.id)
//    jo_create_palette(&palette1);
//    // The palette returned will be filled using color map inside TGA file
//    // If you return JO_NULL, the palette inside the TGA is ignored
//    // ▲ Note : Only 8 palettes are available. So try to mutualize them as much as you can
//    return (&palette1);
//}

//we want to load the assets for the level here

void			    my_draw(void)
{
	static short    rx, ry, rz;

	jo_printf(1, 1, jo_get_last_error());
	jo_printf(1, 2, "AaBbCcDdEeFfGgHhIiJjKkLl");
	jo_printf(1, 3, "MmNnOoPpQqRrSsTtUuVvWwXxYyZz");
	jo_printf(1, 4, filename);
	jo_printf(1, 5, file);
	jo_3d_camera_look_at(&cam);
	jo_3d_push_matrix();
	{
		jo_3d_translate_matrix(JO_CHANGE_SIGN(px), JO_CHANGE_SIGN(py), JO_CHANGE_SIGN(pz));
		jo_3d_rotate_matrix(rx - 90, ry, 0);
		//      rx += 3;
		//      ry += 2;
		rz += 1;
		jo_map_3d_draw(WORLD_MAP_ID, 0, 0, 30);
		jo_3d_rotate_matrix_x(90);
		jo_3d_draw_sprite_at(1, 0, 0, 0);
		jo_3d_draw_sprite_at(1, 300, 0, -500);
		jo_3d_draw_sprite_at(2, 320, 0, -350);
		jo_3d_draw_sprite_at(3, 320, 0, -50);
		jo_3d_draw_sprite_at(4, 350, 0, -50);
		jo_3d_draw_sprite_at(5, 390, 0, -50);
//		jo_3d_draw_sprite_at(2, 320, 0, -50);
//		jo_3d_draw_sprite_at(2, 490, 0, -350);
//		jo_3d_draw_sprite_at(2, 420, 0, -550);
		jo_3d_rotate_matrix_x(rz);
		jo_3d_draw_sprite_at(1, 30, 0, 0);
		jo_3d_rotate_matrix_x(rz);
		jo_3d_rotate_matrix_y(rz);
		jo_3d_rotate_matrix_z(rz);
		jo_3d_draw_sprite_at(1, -30, 0, 0);
	}
	jo_3d_pop_matrix();
	jo_3d_push_matrix();
	{
		//jo_3d_translate_matrix(px, py, pz);
		//jo_3d_rotate_matrix_y(rz * 6);
		jo_3d_draw_sprite_at(pSpriteIndex, 0, 0, 0);
	}
	jo_3d_pop_matrix();

	jo_clear_background(JO_COLOR_DarkGray);
	//draw_background();
}

bool			update_player(void)
{
	if(!JO_FLOAT_NEARLY_EQUALS(px, tx)){
		if(px < tx){
			px += 4;
			pSpriteIndex = 4;
		} else {
			px -= 4;
			pSpriteIndex = 3;
		}
		return false;
	}

	if(!JO_FLOAT_NEARLY_EQUALS(py, ty)){
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

	if(!JO_FLOAT_NEARLY_EQUALS(pz, tz)){
		if(pz < tz){
			pz += 4;
			pSpriteIndex = 2;
		} else {
			pz -= 4;
			pSpriteIndex = 1;
		}
		return false;
	} 

	return true;
}

void			update_camera(void)
{
	if((cam.viewpoint[X] != toFIXED(tx + 45)) || (cam.viewpoint[Y] != toFIXED(ty - 100)) || (cam.viewpoint[Z] != toFIXED(tz - 160))){
		if((cam.viewpoint[X] - toFIXED(tx)) < toFIXED(0.0)){
			cam.viewpoint[X] += toFIXED(1.0);
		} else {
			cam.viewpoint[X] -= toFIXED(1.0);
		}

		if((cam.viewpoint[Y] - toFIXED(ty - 100)) < toFIXED(0.0)){
			cam.viewpoint[Y] += toFIXED(1.0);
		} else {
			cam.viewpoint[Y] -= toFIXED(1.0);
		}

		if((cam.viewpoint[Z] - toFIXED(tz - 160)) < toFIXED(0.0)){
			cam.viewpoint[Z] += toFIXED(1.0);
		} else {
			cam.viewpoint[Z] -= toFIXED(1.0);
		}
	}

	if((cam.target[X] != toFIXED(tx)) || (cam.target[Y] != toFIXED(ty)) || (cam.target[Z] != toFIXED(tz))){
		if((cam.target[X] - toFIXED(tx)) < toFIXED(0.0)){
			cam.target[X] += toFIXED(1.0);
		} else {
			cam.target[X] -= toFIXED(1.0);
		}

		if((cam.target[Y] - toFIXED(ty)) < toFIXED(0.0)){
			cam.target[Y] += toFIXED(1.0);
		} else {
			cam.target[Y] -= toFIXED(1.0);
		}

		if((cam.target[Z] - toFIXED(tz)) < toFIXED(0.0)){
			cam.target[Z] += toFIXED(1.0);
		} else {
			cam.target[Z] -= toFIXED(1.0);
		}
	}
}

void			draw_background(void)
{
	//jo_background_3d_plane_a_img(&bg, &palette1.id, 1, 0);
	//jo_background_3d_plane_a_draw(0);
	jo_draw_background_line(JO_TV_WIDTH / 2, JO_TV_HEIGHT / 2, (tx / 10) + (JO_TV_WIDTH / 2), (tz / 10) + (JO_TV_HEIGHT / 2), JO_COLOR_Blue);
	jo_draw_background_square(1, 1, JO_TV_WIDTH - 3, JO_TV_HEIGHT - 3, JO_COLOR_Green);
	jo_draw_background_square(3, 3, JO_TV_WIDTH - 7, JO_TV_HEIGHT - 7, JO_COLOR_Green);
	for(int backTileRow = 0; backTileRow < 2.0; backTileRow += 16){
		for(int backTile = 0; backTile < 2.0; backTile += 16){
			jo_draw_background_square(backTileRow, backTile, 16, 16, JO_COLOR_Blue);
		}
	}

	jo_put_pixel_in_background((tx / 4) + 8, (tz / 4) + 8, JO_COLOR_Black);
	jo_put_pixel_in_background((tx / 4) + 8, (tz / 4) + 7, JO_COLOR_Black);
	jo_put_pixel_in_background((tx / 4) + 8, (tz / 4) + 9, JO_COLOR_Black);
	jo_put_pixel_in_background((tx / 4) + 7, (tz / 4) + 8, JO_COLOR_Black);
	jo_put_pixel_in_background((tx / 4) + 9, (tz / 4) + 8, JO_COLOR_Black);
	
}

void			load_sprites()
{
	jo_sprite_add_tga("TEX", "BRK.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TEX", "A.TGA", JO_COLOR_Green);
	jo_sprite_add_tga("TEX", "2.TGA", JO_COLOR_Green);
	jo_sprite_add_tga("TEX", "5.TGA", JO_COLOR_Green);
	jo_sprite_add_tga("TEX", "6.TGA", JO_COLOR_Green);
	jo_sprite_add_tga("TEX", "D.TGA", JO_COLOR_Green);
	jo_sprite_add_tga("TEX", "E.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TEX", "B.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TEX", "P.TGA", JO_COLOR_Transparent);
}

void			load_sprite(char* filename)
{
	jo_sprite_add_tga("TEX", filename, JO_COLOR_Transparent);
}

void			load_map()
{
	//jo_map_load_from_file(WORLD_MAP_ID, 450, "MAP", "WALL.MAP");
	//jo_map_load_from_file(WORLD_MAP_ID, 450, "MAP", "MAP.MAP");
	//jo_map_load_from_file(WORLD_MAP_ID, 450, "MAP", "WALL2.MAP");
	//if(!jo_map_load_from_file(WORLD_MAP_ID, 450, "MAP", "MAP3.MAP"))
	//	jo_printf(1, 2, "map error!");
	//if(!jo_map_load_from_file(mapID, depth, dir, filename))
		//jo_printf(1, 2, "map error!");
	if(!jo_map_load_from_file(WORLD_MAP_ID, 450, "MAP", "MAP3.MAP"))
		jo_printf(1, 2, "map error!");
}

//void			setup_bg()
//{
//	jo_enable_background_3d_plane(JO_COLOR_Green);
//	bg.data = NULL;
//	jo_tga_loader(&bg, "TEX", "C.TGA", JO_COLOR_Transparent);
//}
//
void			my_gamepad(void)
{
	if (!jo_is_pad1_available())
		return ;

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
			ty += 64;
		}
	}
}

void			jo_main(void)
{
	jo_core_init(JO_COLOR_Purple);
	//jo_3d_camera_init(&cam);
	//jo_set_tga_palette_handling(my_tga_palette_handling);
	initCamera(&cam);
	jo_3d_camera_set_viewpoint(&cam, 0, -80, -110);
	load_sprites();
	//setup_bg();
	//initLevel(levelID);
	load_map();
	jo_core_add_callback(my_draw);
	jo_core_add_callback(my_gamepad);
	//jo_core_add_callback(update_camera);
	jo_core_run();
}

/*
** END OF FILE
*/
