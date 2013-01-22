#include "gui.h"
#include <gtkmm/stock.h>
#include <iostream>
#include <cstdio>

HelloWorld::HelloWorld() : m_adjustment_amp(0.0, 0.0, 1000.0, 0.000001, 0.0001, 0.0), // for the spinbutton
	m_spinbutton_amp(m_adjustment_amp),
	m_table(5, 2, true),
	m_button1("Button 1"),
	m_button2("Button 2"),
	m_frame_currentpulse("Current Pulse"),
	m_label_currentpulse("Use these options to configure\n1 mA pulsed current mode")
	{ 

	set_title("ARTEMIS - A Remote Terminal ExperiMent Interface System");
	set_default_size(400,300);

	//Menu Bar code
	add(m_box1);
	add(m_table);

	m_refActionGroup = Gtk::ActionGroup::create();

	//File menu:
	m_refActionGroup->add(Gtk::Action::create("FileMenu", "File"));

	m_refActionGroup->add(Gtk::Action::create("FileNew", Gtk::Stock::NEW),
		sigc::mem_fun(*this, &HelloWorld::on_menu_file_new_generic));
	m_refActionGroup->add(Gtk::Action::create("FileOpen", Gtk::Stock::OPEN),
		sigc::mem_fun(*this, &HelloWorld::on_menu_file_open_clicked));
	m_refActionGroup->add(Gtk::Action::create("FileSave", Gtk::Stock::SAVE),
		sigc::mem_fun(*this, &HelloWorld::on_menu_file_save_clicked));
	m_refActionGroup->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT),
		sigc::mem_fun(*this, &HelloWorld::on_menu_file_quit));

	//Edit menu
	m_refActionGroup->add(Gtk::Action::create("EditMenu", "Edit"));

	m_refActionGroup->add(Gtk::Action::create("EditCopy", Gtk::Stock::COPY),
		sigc::mem_fun(*this, &HelloWorld::on_menu_others));
	m_refActionGroup->add(Gtk::Action::create("EditPaste", Gtk::Stock::PASTE),
		sigc::mem_fun(*this, &HelloWorld::on_menu_others));

	//View menu
	m_refActionGroup->add(Gtk::Action::create("ViewMenu", "View"));

	m_refActionGroup->add(Gtk::Action::create("ViewZoomIn", Gtk::Stock::ZOOM_IN),
		sigc::mem_fun(*this, &HelloWorld::on_menu_others));
	m_refActionGroup->add(Gtk::Action::create("ViewZoomOut", Gtk::Stock::ZOOM_OUT),
		sigc::mem_fun(*this, &HelloWorld::on_menu_others));

	//Help menu
	m_refActionGroup->add(Gtk::Action::create("HelpMenu", "Help"));

	m_refActionGroup->add(Gtk::Action::create("HelpAbout", Gtk::Stock::HELP),
		sigc::mem_fun(*this, &HelloWorld::on_menu_help_about_clicked));

	m_refUIManager =Gtk::UIManager::create();
	m_refUIManager->insert_action_group(m_refActionGroup);

	add_accel_group(m_refUIManager->get_accel_group());

	//Laying out menubar and toolbar
	Glib::ustring ui_info = 
		"<ui>"
		"	<menubar name='MenuBar'>"
		"		<menu action='FileMenu'>"
		"			<menuitem action='FileNew'/>"
		"			<menuitem action='FileOpen'/>"
		"			<menuitem action='FileSave'/>"
		"			<separator/>"
		"			<menuitem action='FileQuit'/>"
		"		</menu>"
		"		<menu action='EditMenu'>"
		"			<menuitem action='EditCopy'/>"
		"			<menuitem action='EditPaste'/>"
		"		</menu>"
		"		<menu action='ViewMenu'>"
		"			<menuitem action='ViewZoomIn'/>"
		"			<menuitem action='ViewZoomOut'/>"
		"		</menu>"
		"		<menu action='HelpMenu'>"
		"			<menuitem action='HelpAbout'/>"
		"		</menu>"
		"	</menubar>"
		"</ui>";

	try	{

		m_refUIManager->add_ui_from_string(ui_info);

	}

	catch(const Glib::Error& ex)	{

		std::cerr <<"building menus failed: " << ex.what();

	}


	Gtk::Widget* pMenubar = m_refUIManager->get_widget("/MenuBar");
	if(pMenubar)
		m_box1.pack_start(*pMenubar, Gtk::PACK_SHRINK);

	Gtk::Widget* pToolbar = m_refUIManager->get_widget("/ToolBar");
	if(pToolbar)
		m_box1.pack_start(*pToolbar, Gtk::PACK_SHRINK);

	//Now when the button is clicked, we call the "on_button_clicked" function
	//with a pointer to "button 1" as its argument
	m_button1.signal_clicked().connect(sigc::bind<Glib::ustring>(
		sigc::mem_fun(*this, &HelloWorld::on_button_clicked), "button 1"));

	//instead of gtk_container_add, we pack this button into the invisible
	//box, which has been packind into the window.
	m_table.attach(m_button1, 0, 1, 1, 2);

	m_button1.show();

	m_button2.signal_clicked().connect(sigc::bind<-1, Glib::ustring>(
		sigc::mem_fun(*this, &HelloWorld::on_button_clicked), "button 2"));

	m_table.attach(m_button2, 1, 2, 1, 2);

	m_button2.show();

	//m_spinbutton_amp.set_wrap();
	//m_table.attach(m_spinbutton_amp, 0, 1, 0, 1);

	m_frame_currentpulse.add(m_label_currentpulse);
	m_table.attach(m_frame_currentpulse, 0, 1, 0, 1, Gtk::EXPAND, Gtk::EXPAND, 0, 15);

	m_box1.show();

	m_box1.pack_end(m_table);

	show_all_children();

	// And finally make a keithley device to interact with.
	// Future iterations of this program may use this point to choose the 
	// settings for which Keithley device to use

	KeithleyDevice kdevice=KeithleyDevice();
}

HelloWorld::~HelloWorld() {}

void HelloWorld::on_menu_file_quit()

{
	hide();
}

void HelloWorld::on_menu_file_new_generic()
{
	std::cout << "A File|New menu item was selected." << std::endl;
}

void HelloWorld::on_menu_others()
{
	std::cout << "A menu item was selected." << std::endl;
}

void HelloWorld::on_button_clicked(Glib::ustring data)
{
	std::cout <<"Hello World -" << data << "was pressed" << std::endl;
}

void HelloWorld::on_menu_file_open_clicked() {

	Gtk::FileChooserDialog dialog("Please choose file",
		Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);

	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

	//Add filters, so that only certain file types can be selected:

	Gtk::FileFilter filter_text;
	filter_text.set_name("Text files");
	filter_text.add_mime_type("text/plain");
	dialog.add_filter(filter_text);

	Gtk::FileFilter filter_cpp;
	filter_cpp.set_name("C/C++ files");
	filter_cpp.add_mime_type("text/x-c");
	filter_cpp.add_mime_type("text/x-c++");
	filter_cpp.add_mime_type("text/x-c-header");
	dialog.add_filter(filter_cpp);

	Gtk::FileFilter filter_any;
	filter_any.set_name("Any files");
	filter_any.add_pattern("*");
	dialog.add_filter(filter_any);

	int result = dialog.run();

	switch(result) {

		case(Gtk::RESPONSE_OK): {

			std::cout << "Open clicked." << std::endl;

			//Notice that this is an std::string not a Glib::ustring
			std::string filename = dialog.get_filename();
			std::cout << "File selected: " << filename << std::endl;
			break;

		}

		case(Gtk::RESPONSE_CANCEL): {

			std::cout << "Cancel clicked." << std::endl;
			break;

		}

		default: {

			std::cout << "Unexpected button clicked." << std::endl;
			break;

		}

	}

}

void HelloWorld::on_menu_file_save_clicked() {

	Gtk::FileChooserDialog dialog("Please choose a file name",
		Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for(*this);

	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);

	int result = dialog.run();

	switch(result) {

		case(Gtk::RESPONSE_OK): {

			std::cout << "Save clicked." << std::endl;

			std::string filename = dialog.get_filename();
			std::cout << "File saved as: " << filename << std::endl;
			break;

		}

		case(Gtk::RESPONSE_CANCEL): {

			std::cout << "Cancel clicked." << std::endl;
			break;

		}

		default: {

			std::cout << "Unexpected button clicked." << std::endl;
			break;

		}

	}

}

void HelloWorld::on_menu_help_about_clicked() {

	Gtk::MessageDialog dialog(*this, "This is MessageDialog, it will help you!",
		false, Gtk::MESSAGE_INFO,
		Gtk::BUTTONS_OK_CANCEL);
	dialog.set_secondary_text(
		"This program was created by somecompany(R).");

	int result = dialog.run();

	switch(result)
	
	{

		case(Gtk::RESPONSE_OK):

		{
			std::cout << "OK clicked." << std::endl;
			break;
		}

		case(Gtk::RESPONSE_CANCEL):

		{
			std::cout << "Cancel clicked." << std::endl;
			break;
		}

		default:

		{
			std::cout << "Unexpected button clicked." << std::endl;
			break;
		}

	}

}