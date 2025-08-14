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
// File : main.cxx
// Contains: Application entry point
//
// Written by: Jean-François DEL NERO
//
// Change History (most recent first):
///////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <stdlib.h>

#include "fl_includes.h"

#include "pcb_window.h"
#include "cb_pcb_window.h"

#include "main.h"

#include "main_gui.h"

s_gui_context * guicontext;

int main(int argc, char **argv)
{

#ifdef WIN32
	OSVERSIONINFO osvi;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&osvi);

	if( osvi.dwMajorVersion < 5 ) // At least Windows 2000 ?
	{
		// No ... Windows 95 & Windows 98 have no unicode support... FLTK can't be used with these OS...
		MessageBox(NULL,"This version of Windows is unsupported !\nWindows 2000 or higher is required !\nFor Windows 95/98 please use an older version of this software (v1.X.X.X)",
						"Unsupported Windows version",MB_OK|MB_ICONSTOP);
		exit(-1);
	}
#endif

	guicontext=(s_gui_context *)malloc(sizeof(s_gui_context));
	if(guicontext)
	{
		memset(guicontext,0,sizeof(s_gui_context));

		new Main_Window(argc, argv);

		return 0;
	}
	return -1;
}
