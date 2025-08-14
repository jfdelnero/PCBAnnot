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

void tick_infos(void *v);

void bt_next_part_callback(Fl_Button*, void* w);
void bt_next_net_callback(Fl_Button*, void* w);
void bt_clear_net_callback(Fl_Button*, void* w);
void bt_clear_pin_callback(Fl_Button*, void* w);

void bt_zp_callback(Fl_Button*, void* w);
void bt_zm_callback(Fl_Button*, void* w);

void slide_changed(Fl_Slider*, void*);

void pcb_infos_window_callback(Fl_Widget *o, void *v);

