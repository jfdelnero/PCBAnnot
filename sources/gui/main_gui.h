
typedef struct main_button_list_
{
	Fl_Button * button;
	int label_id;
	int desc_id;
}main_button_list;


// Main class...
class Main_Window : public Fl_Window {

	char *evt_txt;

	unsigned int txtindex;
	Fl_File_Chooser     *fc_load;
	Fl_File_Chooser     *fc_save;

  public:

	Fl_Progress* track_pos;
	Fl_Output* track_pos_str;
	Fl_Output* file_name_txt;

	pcb_window *infos_window;

	Main_Window(int argc, char **argv);
	~Main_Window();
};

void load_file_image_pb(Fl_Widget * widget, void * ptr);
void execute_script_pb(Fl_Widget * widget, void * ptr);
void menu_clicked(Fl_Widget * w, void * fc_ptr);
void save_file_image(Fl_Widget * w, void * fc_ptr);
void format_choice_cb(Fl_Widget *, void *v);

