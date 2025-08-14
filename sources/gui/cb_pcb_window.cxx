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

///////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//
//-----------H----H--X----X-----CCCCC----22222----0000-----0000------11----------//
//----------H----H----X-X-----C--------------2---0----0---0----0--1--1-----------//
//---------HHHHHH-----X------C----------22222---0----0---0----0-----1------------//
//--------H----H----X--X----C----------2-------0----0---0----0-----1-------------//
//-------H----H---X-----X---CCCCC-----222222----0000-----0000----1111------------//
//-------------------------------------------------------------------------------//
//----------------------------------------------------- http://hxc2001.free.fr --//
///////////////////////////////////////////////////////////////////////////////////
// File : cb_pcb_window.cxx
// Contains: pcb window callbacks.
//
// Written by: Jean-François DEL NERO
//
// Change History (most recent first):
///////////////////////////////////////////////////////////////////////////////////s

#include "pcb_window.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include <stdint.h>

#include "fl_includes.h"

#include "cb_pcb_window.h"

#include "main.h"

#include "fl_mouse_box.h"

#include "main_gui.h"

#include "annot_ctx.h"

#include "script_exec.h"
#include "env.h"

extern "C"
{
#include "bmp_file.h"
}

extern bitmap_data bdata[10];

extern s_gui_context * guicontext;

#define PI    ((float)  3.141592654f)

#define MAX_TRACK_MODES_INDEX 2

#define PIX_SIZE 4

extern annot_ctx actx;

#pragma pack(1)
typedef struct s_col_
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
}s_col;
#pragma pack()

#define COLOR_RGBA(R,G,B,A) ( (R & 0xFF) | ( ( G & 0xFF ) << 8 ) | ( ( B & 0xFF ) << 16 ) | ( ( A & 0xFF ) << 24 ) )
#define COLOR_RGB2RGBA(RGB,A) ( RGB | ( ( A & 0xFF ) << 24 ) )

static uint32_t alpha(uint32_t dst, uint32_t src, uint8_t alpha_value)
{
	return  ( ( ( (uint32_t)(alpha_value) * ((src&0x0000FF)>>0) + (uint32_t)(255-alpha_value) * ((dst&0x0000FF)>>0) ) >> 8 ) & 0xFF ) | \
			( ( (uint32_t)(alpha_value) * ((src&0x00FF00)>>8) + (uint32_t)(255-alpha_value) * ((dst&0x00FF00)>>8) ) & 0xFF00 ) | \
			( ( ( (uint32_t)(alpha_value) * ((src&0xFF0000)>>16) + (uint32_t)(255-alpha_value) * ((dst&0xFF0000)>>16) ) << 8 ) & 0xFF0000);
}

static void putpix(uint32_t * dst, int x, int y, int xsize, int ysize, uint32_t col, uint8_t alphaparam)
{
	if( x>=0 && y>=0 && x < xsize && y < ysize )
	{
		dst[(y*xsize) + x] = alpha(dst[(y*xsize) + x], col, alphaparam);
	}
}

static int draw_pins(uint32_t * dst, int xsize, int ysize)
{
	int i,j,x,y,xe,k;
	int xc,yc;
	part * p;
	pin * pi;
	uint32_t col;
	uint8_t alphaparam;

	alphaparam = guicontext->alpha[8];
	for(i=0;i<actx.parts_cnt+1;i++)
	{
		p =  (part *)&actx.parts[i];
		for(j=0;j<p->pins_cnt;j++)
		{
			pi = &p->pins_list[j];

			xc = pi->x - guicontext->pic_offset_x;
			yc = pi->y - guicontext->pic_offset_y;

			x = xc - 6;
			xe = xc + 6;
			for(;x<xe;x++)
			{
			   if(x&1)
					col = 0x000000;
				else
					col = 0xFFFFFF;

				if(pi->s)
					col = 0x0000FF;

				for(k=0;k<3;k++)
					putpix(dst, x, yc+(6+k), xsize, ysize, col, alphaparam);
			}

			x = xc - 6;
			xe = xc + 6;
			for(;x<xe;x++)
			{
				if(x&1)
					col = 0x000000;
				else
					col = 0xFFFFFF;

				if(pi->s)
					col = 0x0000FF;

				for(k=0;k<3;k++)
					putpix(dst, x, yc-(6+k), xsize, ysize, col, alphaparam);
			}


			y = yc - 6;
			xe = yc + 6;

			for(;y<xe;y++)
			{
				if(y&1)
					col = 0x000000;
				else
					col = 0xFFFFFF;

				if(pi->s)
					col = 0x0000FF;

				for(k=0;k<3;k++)
					putpix(dst, xc-(6+k), y, xsize, ysize, col, alphaparam);
			}

			y = yc - 6;
			xe = yc + 6;
			for(;y<xe;y++)
			{
				if(y&1)
					col = 0x000000;
				else
					col = 0xFFFFFF;

				if(pi->s)
					col = 0x0000FF;

				for(k=0;k<3;k++)
					putpix(dst, xc+(6+k), y, xsize, ysize, col, alphaparam);
			}

		}
	}

	return 0;
}


void gfx_line(uint32_t * dst, int xsize, int ysize,  int first_x ,int first_y, int last_x, int last_y, uint32_t col, uint8_t alphaparam )
{
	int dx;
	int dy;
	int inc1,inc2;
	int sub,remain,error;

	dx = last_x - first_x;
	dy = last_y - first_y;

	if ( ( !dx ) && ( !dy ) )
	{
		return; // rien a tracer
	}

	if ( dx < 0 )
	{
		dx = -dx;
		inc2 = -1;
	}
	else
	{
		inc2 = 1;
	}

	if (dy<0)
	{
		dy = -dy;
		inc1 = -1;
	}
	else
	{
		inc1 = 1;
	}

	if ( dx > dy )
	{
		sub = dx - dy;
		error = dy - ( dx >> 1);

		remain = (dx + 1);

		do
		{
			putpix(dst, first_x, first_y, xsize, ysize, col, alphaparam);

			if (error>=0)
			{
				putpix(dst, first_x + inc2, first_y, xsize, ysize, col, alphaparam);

				first_y += inc1;
				error -= sub;

				putpix(dst, first_x, first_y, xsize, ysize, col, alphaparam);
			}
			else
				error += dy;

			first_x += inc2;

		} while ( --remain > 0 );

		if ( !(dx&1) )
			putpix(dst, first_x, first_y, xsize, ysize, col, alphaparam);

		return;
	}
	else
	{
		sub = dy - dx;
		error = dx - ( dy >> 1 );

		remain = ( dy + 1 );

		do
		{
			putpix(dst, first_x, first_y, xsize, ysize, col, alphaparam);

			if ( error >= 0 )
			{
				putpix(dst, first_x, first_y + inc1, xsize, ysize, col, alphaparam);

				first_x += inc2;
				error -= sub;

				putpix(dst, first_x, first_y, xsize, ysize, col, alphaparam);

			}
			else
				error += dx;

			first_y += inc1;

		} while ( --remain > 0 );

		if ( !(dy&1) )
			putpix(dst, first_x, first_y, xsize, ysize, col, alphaparam);

		return;
	}
}

static int draw_nets(uint32_t * dst, int xsize, int ysize)
{
	int i,j;
	int xc,yc;
	int xd,yd;
	net * n;
	node * ni;
	node * np;
	uint8_t alpha;

	alpha = guicontext->alpha[9];

	for(i=0;i<actx.nets_cnt+1;i++)
	{
		n =  (net *)&actx.nets[i];

		for(j=1;j<n->nodes_cnt;j++)
		{
			ni = &n->nodes_list[j];
			np = &n->nodes_list[j-1];

			xd = np->x - guicontext->pic_offset_x;
			yd = np->y - guicontext->pic_offset_y;

			xc = ni->x - guicontext->pic_offset_x;
			yc = ni->y - guicontext->pic_offset_y;

			if(ni->s || np->s)
				gfx_line(dst, xsize, ysize,  xd ,yd, xc, yc, 0x0000FF, alpha );
			else
				gfx_line(dst, xsize, ysize,  xd ,yd, xc, yc, 0xFFFFFF, alpha );
		}
	}

	return 0;
}

static int save_project()
{
	int i,j,x,y;
	net * n;
	node * ni;
	part * p;
	pin * pi;

	FILE * f;

	f = fopen("project.script","wb");
	if( f )
	{
		for(i=0;i<actx.parts_cnt;i++)
		{
			fprintf(f,"# part %d\n",i);

			p =  (part *)&actx.parts[i];

			fprintf(f,"set_partname \"%s\"\n",p->name);

			for(j=0;j<p->pins_cnt;j++)
			{
				pi = &p->pins_list[j];

				x = pi->x;
				y = pi->y;

				fprintf(f,"add_pin %d %d\n",x,y);
			}

			fprintf(f,"\nadd_part\n");
		}

		for(i=0;i<actx.nets_cnt;i++)
		{
			n =  (net *)&actx.nets[i];

			fprintf(f,"# net %d\n",i);
			fprintf(f,"set_netname \"%s\"\n",n->name);
			for(j=0;j<n->nodes_cnt;j++)
			{
				ni = &n->nodes_list[j];

				fprintf(f,"add_node %d %d\n",ni->x,ni->y);
			}
			fprintf(f,"add_net\n");
		}

		fclose( f );
	}

	return 0;
}


void update_graph()
{
	int i,j,picx,picy, layer;
	uint32_t * dst;
	uint32_t * src;

	layer = 0;

	memset((uint32_t*)guicontext->flayoutframebuffer, 0x20, guicontext->pic_x * guicontext->pic_y * PIX_SIZE);

	while( bdata[layer].ysize )
	{
		dst = (uint32_t*)&guicontext->flayoutframebuffer[(0*guicontext->pic_x)*PIX_SIZE];

		for(j=0;(j+guicontext->pic_offset_y)<0;j++)
		{
			for(i=0;i<(guicontext->pic_x);i++)
			{
				dst++;
			}
		}

		if(guicontext->pic_offset_y>=0)
		{
			picy = guicontext->pic_offset_y;
		}
		else
		{
			picy = 0;
		}

		for(; (j<guicontext->pic_y) && (picy<bdata[layer].ysize) ; j++,picy++)
		{
			dst = (uint32_t*)&guicontext->flayoutframebuffer[(j*guicontext->pic_x)*PIX_SIZE];

			for(i=0;(i+guicontext->pic_offset_x)<0;i++)
			{
				*dst++ = 0x20202020;
			}

			if(guicontext->pic_offset_x>=0)
			{
				picx = guicontext->pic_offset_x;
			}
			else
			{
				picx = 0;
			}

			src = (uint32_t*)(bdata[layer].data) + (((bdata[layer].xsize*picy)+picx));

			if( (j+guicontext->pic_offset_y) >= 0 )
			{
				for(;i<guicontext->pic_x && picx<bdata[layer].xsize;i++,picx++)
				{
					*dst = alpha( *dst, *src, guicontext->alpha[layer]);
					dst++;
					src++;
				}
			}
			else
			{
			   for(;i<guicontext->pic_x && picx<bdata[layer].xsize;i++,picx++)
			   {
					dst++;
					src++;
			   }
			}

			for(;i<guicontext->pic_x;i++)
			{
				dst++;
			}
		}

		layer++;
	}

	dst = (uint32_t*)&guicontext->flayoutframebuffer[0];

	update_sel(actx.point_offset_x, actx.point_offset_y);
	draw_pins(dst, guicontext->pic_x, guicontext->pic_y);
	draw_nets(dst, guicontext->pic_x, guicontext->pic_y);
}

void tick_infos(void *w) {
	int i;
	pcb_window *window;
	//Main_Window *mwindow;

	window=(pcb_window *)w;
	//mwindow = (Main_Window *)guicontext->main_window;

	if(window->window->shown())
	{
		for(i=0;i<10;i++)
		{
			switch(i)
			{
				case 0:
					guicontext->alpha[i] = window->slide_l00->value();
				break;
				case 1:
					guicontext->alpha[i] = window->slide_l01->value();
				break;
				case 2:
					guicontext->alpha[i] = window->slide_l02->value();
				break;
				case 3:
					guicontext->alpha[i] = window->slide_l03->value();
				break;
				case 4:
					guicontext->alpha[i] = window->slide_l04->value();
				break;
				case 5:
					guicontext->alpha[i] = window->slide_l05->value();
				break;
				case 6:
					guicontext->alpha[i] = window->slide_l06->value();
				break;
				case 7:
					guicontext->alpha[i] = window->slide_l07->value();
				break;
				case 8:
					guicontext->alpha[i] = window->slide_pins->value();
				break;
				case 9:
					guicontext->alpha[i] = window->slide_nets->value();
				break;
			}
		}

		window->window->make_current();
		if( (window->pcb_disp->w() != guicontext->pic_x) || (window->pcb_disp->h() != guicontext->pic_y) )
		{
			// Resized... Realloc needed
			free(guicontext->flayoutframebuffer);

			guicontext->pic_x = window->pcb_disp->w();
			guicontext->pic_y = window->pcb_disp->h();
			guicontext->flayoutframebuffer = (unsigned char*)malloc(guicontext->pic_x*guicontext->pic_y*4);

			guicontext->update_graph = 1;
		}
		else
		{
			if(guicontext->update_graph)
			{
				update_graph();
				fl_draw_image((unsigned char *)guicontext->flayoutframebuffer, window->pcb_disp->x(), window->pcb_disp->y(), guicontext->pic_x, guicontext->pic_y , 4, 0);
				guicontext->update_graph = 0;
				window->global_status->value(actx.lastsel);
				window->y_pos->value(actx.lastnetname);
				window->buf->remove(0,window->buf->length());
				window->buf->append((char*)actx.connectionslist);
				window->object_txt->buffer(window->buf);
			}
		}
	}

	Fl::repeat_timeout(0.1, tick_infos, w);
}

void pcb_infos_window_callback(Fl_Widget *o, void *v)
{
	//pcb_window *window;
	//window=((pcb_window*)(o->user_data()));

    //update_graph(window);
}


void pcb_save(Fl_Button*, void* w)
{
	save_project();

	//fdw->window->hide();
}

void bt_next_part_callback(Fl_Button*, void* w)
{
	//pcb_window *fdw;
	//fdw=(pcb_window *)w;

	add_part();
	guicontext->update_graph = 1;
}

void bt_next_net_callback(Fl_Button*, void* w)
{
	//pcb_window *fdw;
	//fdw=(pcb_window *)w;

	add_net();
	guicontext->update_graph = 1;
}

void bt_clear_net_callback(Fl_Button*, void* w)
{
	//pcb_window *fdw;
	//fdw=(pcb_window *)w;

	clear_net(actx.nets_cnt);
	guicontext->update_graph = 1;
}

void bt_clear_pin_callback(Fl_Button*, void* w)
{
	//pcb_window *fdw;
	//fdw=(pcb_window *)w;

	clear_pin(actx.parts_cnt);
	guicontext->update_graph = 1;
}


void bt_zm_callback(Fl_Button*, void* w)
{
	//pcb_window *fdw;
	//fdw=(pcb_window *)w;

	if( guicontext->zoom > 0)
		guicontext->zoom--;
}

void bt_zp_callback(Fl_Button*, void* w)
{
	//pcb_window *fdw;
	//fdw=(pcb_window *)w;

	if( guicontext->zoom < 15)
		guicontext->zoom++;
}

void slide_changed(Fl_Slider*, void* w)
{
	//pcb_window *fdw;
	//fdw=(pcb_window *)w;

	guicontext->update_graph = 1;
}

void mouse_di_cb(Fl_Widget *o, void *v)
{
	pcb_window *fiw;
	Fl_Window *dw;
	Fl_Mouse_Box *dnd = (Fl_Mouse_Box*)o;
	int xpos,ypos,buttons_state;

	int event;

	int disp_xsize,disp_ysize,disp_xpos,disp_ypos;

	static int ix, iy;
	static int dragged;
	static int pushed;
	static int sx, sy, sw, sh; // selection box;
	int x2,y2;

	dw=((Fl_Window*)(o->parent()));
	fiw=(pcb_window *)dw->user_data();

	//Main_Window *window;

	//window = (Main_Window *)guicontext->main_window;

	disp_xsize=fiw->pcb_disp->w();
	disp_ysize=fiw->pcb_disp->h();
	disp_xpos=fiw->pcb_disp->x();
	disp_ypos=fiw->pcb_disp->y();

	event = dnd->event();

	if( guicontext->updating )
	{
		dragged = 0;
		pushed = 0;
		return;
	}

	switch(event)
	{
		case FL_DRAG:

			if( guicontext->updating)
				return;

			buttons_state = Fl::event_buttons();
			if( buttons_state & FL_BUTTON3 )
			{
				if(dragged == 1)
				{
					guicontext->pic_offset_x = guicontext->pic_strtdrag_x - Fl::event_x();
					guicontext->pic_offset_y = guicontext->pic_strtdrag_y - Fl::event_y();
				}
				else
				{
					guicontext->pic_strtdrag_x = guicontext->pic_offset_x + Fl::event_x();
					guicontext->pic_strtdrag_y = guicontext->pic_offset_y + Fl::event_y();
					dragged = 1;
				}
				guicontext->update_graph = 1;
				return;
			}

			dragged = 1;
			pushed = 0;

			x2 = Fl::event_x();

			if (x2<fiw->pcb_disp->x())
				x2=fiw->pcb_disp->x();

			if (x2>=fiw->pcb_disp->x()+fiw->pcb_disp->w())
				x2=fiw->pcb_disp->x()+fiw->pcb_disp->w()-1;

			y2 = Fl::event_y();

			if (y2<fiw->pcb_disp->y())
				y2=fiw->pcb_disp->y();

			if (y2>=fiw->pcb_disp->y()+fiw->pcb_disp->h())
				y2=fiw->pcb_disp->y()+fiw->pcb_disp->h()-1;

			// if (button != 1) {ix = x2; iy = y2; return 1;}
			if (ix < x2)
			{
				sx = ix;
				sw = x2 - ix;
			}
			else
			{
				sx = x2;
				sw = ix - x2;
			}

			if (iy < y2)
			{
				sy = iy;
				sh = y2 - iy;
			}
			else
			{
				sy = y2;
				sh = iy-y2;
			}

			fiw->pcb_disp->window()->make_current();
			fl_overlay_rect(sx,sy,sw,sh);

			return;
		break;

		case FL_PUSH:

			if( guicontext->updating)
				return;

			buttons_state = Fl::event_buttons();
			if( 0 )//buttons_state & FL_BUTTON3 )
			{   // Right mouse button - clear zoom
				fl_overlay_clear();
				//dragged = 0;
			}
			else if( buttons_state & FL_BUTTON1 )
			{   // Left mouse button
				pushed = 1;

				ix = Fl::event_x();
				iy = Fl::event_y();
				sx = ix;
				sy = iy;
				sw = 0;
				sh = 0;

				xpos=Fl::event_x() - disp_xpos;
				if(xpos<0) xpos=0;
				if(xpos>disp_xsize) xpos=disp_xsize-1;

				ypos=Fl::event_y() - disp_ypos;

				if(ypos<0) ypos=0;
				if(ypos>disp_ysize) ypos=disp_ysize-1;

				switch(fiw->menu_mode_sel->value())
				{
					case 0: // pin
						add_pin(guicontext->pic_offset_x + ix, guicontext->pic_offset_y + iy, actx.parts_cnt);
						guicontext->update_graph = 1;
					break;

					case 1: // net
						add_node(guicontext->pic_offset_x + ix, guicontext->pic_offset_y + iy, actx.nets_cnt);
						guicontext->update_graph = 1;
					break;
				}

			}
			return;

		break;

		case FL_MOVE:

			if( guicontext->updating )
				return;

			actx.point_offset_x = guicontext->pic_offset_x + Fl::event_x();
			actx.point_offset_y = guicontext->pic_offset_y + Fl::event_y();
			guicontext->update_graph = 1;

			if(dragged)
			{
				fl_overlay_clear();

				dragged = 0;
				return;
			}

			if(pushed)
			{
				pushed = 0;

				return;
			}

			return;
		break;
	}
	return;
}
