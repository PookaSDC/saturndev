//void	parseMapFile(void)
//{
//	char* filebuf = jo_fs_read_file("MAP0.CSV", 8);
//	char* line;
//	char* tok;
//	short linenum = 0;
//	short temp = 0;
//	short layer = 0;
//
//	//strncpy(line, filebuf, strcspn(filebuf, ','));
//	//line[8] = '\0';
//	//jo_map_load_from_file(layer, 250, "MAP", line);
//
//	//for(line = strtok(filebuf, "\n"); line && *line; line = strtok(NULL, "\n")){
//	//	for(tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n")){
//	//		tok[8] = '\0';
//	//		jo_map_load_from_file(layer, 250, "MAP", tok);
//	//	}
//	//	layer++;
//	//	linenum++;
//	//	temp = linenum;
//	//	strtok(filebuf, '\n');
//	//	while(temp >= 1){
//	//		strtok(NULL, '\n');
//	//		temp--;
//	//	}
//	//}
//
//	//while(){
//	//	line[8] = '\0';
//	//	for(tok = strtok(line, ";"); tok && *tok; tok = strtok(NULL, ";\n")){
//	//		jo_map_load_from_file(layer, 250, "MAP", line);
//	//	}
//	//	layer++;
//	//}
//	//return;
//}

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
