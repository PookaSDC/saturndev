#ifndef __DATA_H__
#define __DATA_H__

typedef struct
{
	int mapID;
	int maxLayers;
} level;

typedef struct
{
	jo_pos3d 3dpos;
	int behavior;
} entity;

static entity
