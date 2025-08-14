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

#define DEFAULT_TEXT_BUFFER_SIZE 4096

typedef struct s_gui_context_
{
	char bufferfilename[DEFAULT_TEXT_BUFFER_SIZE];
	char buffertext[DEFAULT_TEXT_BUFFER_SIZE];

	char * logfile;

	unsigned char * flayoutframebuffer;
	int pic_x;
	int pic_y;

	int pic_offset_x;
	int pic_offset_y;

	int pic_strtdrag_x;
	int pic_strtdrag_y;

	int update_graph;

	int alpha[10];

	int edit_mode;
	int zoom;

	volatile int updating;

	void * main_window;

	int exit;
}s_gui_context;
