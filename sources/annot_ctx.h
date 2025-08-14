/*
//
// Copyright (C) 2025 Jean-François DEL NERO
//
// This file is part of PCB Annot.
//
// PCB Annot may be used and distributed without restriction provided
// that this copyright statement is not removed from the file and that any
// derivative work contains the original copyright notice and the associated
// disclaimer.
//
// PCB Annot is free software; you can redistribute it
// and/or modify  it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// PCB Annot is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//   See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with PCB Annot; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
*/

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_STR_LEN 32

typedef struct pin_
{
	int x;
	int y;
	int r;
	int s;
	unsigned char name[MAX_STR_LEN];
}pin;

#define MAX_PIN_NUM 128

typedef struct part_
{
	unsigned char name[MAX_STR_LEN];
	int pins_cnt;
	pin pins_list[MAX_PIN_NUM];
}part;

#define MAX_PARTS_NUM 512
#define MAX_NETS_NUM 4096

#define MAX_NODES_NUM 1024

typedef struct node_
{
	int x;
	int y;
	int r;
	int s;
	unsigned char name[MAX_STR_LEN];
}node;

typedef struct net_
{
	unsigned char name[MAX_STR_LEN];
	int nodes_cnt;
	node nodes_list[MAX_NODES_NUM];
}net;

typedef struct annot_ctx_
{
	char * logfile;

	int xsize,ysize;

	unsigned char * flayoutframebuffer;
	int pic_x;
	int pic_y;

	int pic_offset_x;
	int pic_offset_y;

	int pic_strtdrag_x;
	int pic_strtdrag_y;

	int point_offset_x;
	int point_offset_y;

	int update_graph;

	int edit_mode;
	int zoom;

	int parts_cnt;
	part parts[MAX_PARTS_NUM];

	int nets_cnt;
	net nets[MAX_NETS_NUM];

	//envvar_entry * env;
	void * envvar;
	void * scriptctx;

	char lastsel[512];
	char lastnetname[512];
	char connectionslist[32*1024];

}annot_ctx;

#ifdef __cplusplus
}
#endif

