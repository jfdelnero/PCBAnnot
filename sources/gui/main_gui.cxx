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
// File : main_gui.cxx
// Contains: Main GUI window
//
// Written by: Jean-François DEL NERO
//
// Change History (most recent first):
///////////////////////////////////////////////////////////////////////////////////

#define WINDOW_XSIZE 100

#include "fl_includes.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <stdint.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "fl_dnd_box.h"

#include "annot_ctx.h"

#ifdef WIN32
#include "win32/resource.h"
 #if !defined(__MINGW32__) && !defined(__MINGW64__)
  #define intptr_t int
 #endif
#endif

#include "pcb_window.h"
#include "cb_pcb_window.h"

#include "main_gui.h"
#include "main.h"

#include "script_exec.h"
#include "env.h"

extern "C"
{
#include "bmp_file.h"
}

bitmap_data bdata[10];

extern s_gui_context * guicontext;
extern annot_ctx actx;

#ifdef GUI_DEBUG
void print_dbg(char * str)
{
	printf("%s\n",str);
	fflush(stdout);
}
#endif

#if 0
static void tick_mw(void *v) {
	Main_Window *window;
	window=(Main_Window *)v;

	Fl::repeat_timeout(0.10, tick_mw, v);
}
#endif

Main_Window::Main_Window(int argc, char **argv)
	#ifdef __EMSCRIPTEN__
	: Fl_Window(16,192,WINDOW_XSIZE,428)
	#else
	: Fl_Window(WINDOW_XSIZE,428)
	#endif
{
	int i;
	char tmpstr[512];

	txtindex=0;
	i=0;
	evt_txt=0;

	#ifdef GUI_DEBUG
	print_dbg((char*)"Main_Window : Entering Main_Window");

	print_dbg((char*)"Main_Window : Screen scaling supported = %d", Fl::screen_scaling_supported());
	#endif

	#if 0
	Fl::screen_scale(Fl::screen_count() - 1,300);
	#endif

	memset(&actx,0,sizeof(actx));

	actx.envvar = (envvar_entry *)initEnv(NULL, NULL);
	actx.scriptctx = (void*)init_script((void*)&actx,0x00000000,(void*)actx.envvar);

	memset(&bdata,0,sizeof(bdata));

	for(i=0;i<10;i++)
	{
		sprintf(tmpstr,"L%.2d.bmp",i);
		if( bmp_load(tmpstr,&bdata[i]) < 0)
		{
			if(i == 0)
				return;

			break;
		}
	}

	execute_file_script( (script_ctx *)actx.scriptctx, (char*)"project.script" );

	// First lock() call to Enable lock/unlock threads support.
	Fl::lock();
	Fl::visual(FL_RGB);

	Fl::scheme("gtk+");

	Fl_Group group(0,0,WINDOW_XSIZE,392);

	//////////////////////////////////////////////
	// pcb view window

	infos_window = new pcb_window();
	if(infos_window)
	{
		infos_window->slide_l00->bounds(255, 0.0);
		infos_window->slide_l01->bounds(255, 0.0);
		infos_window->slide_l02->bounds(255, 0.0);
		infos_window->slide_l03->bounds(255, 0.0);
		infos_window->slide_l04->bounds(255, 0.0);
		infos_window->slide_l05->bounds(255, 0.0);
		infos_window->slide_l06->bounds(255, 0.0);
		infos_window->slide_l07->bounds(255, 0.0);
		infos_window->slide_pins->bounds(255, 0.0);
		infos_window->slide_nets->bounds(255, 0.0);

		infos_window->slide_l00->value(255);
		infos_window->slide_l01->value(0);
		infos_window->slide_l02->value(0);
		infos_window->slide_l03->value(0);
		infos_window->slide_l04->value(0);
		infos_window->slide_l05->value(0);
		infos_window->slide_l06->value(0);
		infos_window->slide_l07->value(0);
		infos_window->slide_pins->value(255);
		infos_window->slide_nets->value(255);

		i = 0;
		while(i<10 && bdata[i].xsize && bdata[i].ysize)
		{
			sprintf(tmpstr,"L%.2d.bmp",i);
			//infos_window->viewlayer->add(tmpstr);
			i++;
		}

		guicontext->zoom = 8;
		guicontext->flayoutframebuffer = (unsigned char*)malloc( infos_window->pcb_disp->w() * infos_window->pcb_disp->h() * 3);
		if(guicontext->flayoutframebuffer)
		{
			memset(guicontext->flayoutframebuffer,0xFF,infos_window->pcb_disp->w()*infos_window->pcb_disp->h() * 3);
		}

		infos_window->buf = new Fl_Text_Buffer;
		if(infos_window->buf)
			infos_window->object_txt->buffer(infos_window->buf);
		
//      infos_window->window->position(16 + 8 + WINDOW_XSIZE,192);
//      infos_window->window->border(1);
//      infos_window->window->box(FL_THIN_DOWN_FRAME);
		infos_window->window->show();

		Fl::add_timeout(0.1, tick_infos, (void*)infos_window);

		#ifdef GUI_DEBUG
		print_dbg((char*)"Main_Window : PCB viewer window done !");
		#endif
	}

	//this->icon((char *)LoadIcon(fl_display, MAKEINTRESOURCE(101)));
	//label("PCB Annot v" STR_FILE_VERSION2);
	//  show();

	user_data((void*)(this));

//  resize (this->x_root(),this->y_root(),WINDOW_XSIZE,BUTTON_YPOS+(BUTTON_YSTEP*j) - 25);

	guicontext->main_window = (Main_Window *)this;

	//Fl::add_timeout( 0.1, tick_mw, (void*)this);

	#ifdef GUI_DEBUG
	print_dbg((char*)"Main_Window : All done !");
	#endif

	Fl::run();
}

Main_Window::~Main_Window()
{
}

#ifdef __EMSCRIPTEN__
extern "C"
{
	void EMSCRIPTEN_KEEPALIVE upload_file(char * filename, char * folder, void * inBuffer, int inBufSize )
	{
		char filepath[512];
		struct stat st = {0};
		FILE * f;

		snprintf(filepath,sizeof(filepath), "%s/%s",folder,filename);

		printf("Upload : %s (%d B) \n",filepath,inBufSize);

		if (stat(folder, &st) == -1)
		{
			mkdir(folder, 0700);
		}

		f = fopen(filepath,"wb");
		if(f)
		{
			fwrite(inBuffer,inBufSize,1,f);
			fclose(f);
		}
	}
}

extern "C"
{
	void EMSCRIPTEN_KEEPALIVE load_file_img(char * filename, char * folder)
	{
		char filepath[512];

		snprintf(filepath,sizeof(filepath), "%s/%s",folder,filename);

		printf("Load : %s\n",filepath);

		load_file(filepath);
	}
}
#endif
