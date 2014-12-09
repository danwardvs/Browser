#include <string>
#include <allegro.h>
#include <alpng.h>
#include <wininet.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

BITMAP* buffer;
BITMAP* reload;
BITMAP* cursor;

char* weburl="http://adsgames.net/server/browser/version.html";
char DataReceived[4096];

int local_version_number;
int server_version_number;

bool new_version=false;
bool failed_to_connect=false;
bool url_loaded=false;

std::string  edittext;
std::string::iterator iter;
std::string url_from_adress_bar;

FONT* font_12;
FONT* f1;
FONT* f2;
FONT* f3;
FONT* f4;
FONT* f5;

//Area clicked
bool location_clicked(int min_x,int max_x,int min_y,int max_y){
    if(mouse_x>min_x && mouse_x<max_x && mouse_y>min_y && mouse_y<max_y && mouse_b & 1 || mouse_x>min_x && mouse_x<max_x && mouse_y>min_y && mouse_y<max_y && joy[0].button[1].b)
        return true;
    else return false;
}

//A function that handles error messages
void abort_on_error(const char *message){
	 if (screen != NULL){
	    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	 }
	 allegro_message("%s.\n %s\n", message, allegro_error);
	 exit(-1);
}


void get_webpage(char* newWeburl){
    HINTERNET connect = InternetOpen(newWeburl,INTERNET_OPEN_TYPE_PRECONFIG,NULL, NULL, 0);

    if(!connect){
      alert(NULL, NULL, "Connection Failed or Syntax error.","&Continue", NULL, 'c', 0);
    }

    HINTERNET OpenAddress = InternetOpenUrl(connect,weburl, NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE|INTERNET_FLAG_KEEP_CONNECTION, 0);

    if ( !OpenAddress ){
      DWORD ErrorNum = GetLastError();

      alert(NULL, NULL, "Failed to open URL.","&Continue", NULL, 'c', 0);
      failed_to_connect=true;
      InternetCloseHandle(connect);

   }


   DWORD NumberOfBytesRead = 0;
   while(InternetReadFile(OpenAddress, DataReceived, 4096, &NumberOfBytesRead) && NumberOfBytesRead ){
   }

   InternetCloseHandle(OpenAddress);
   InternetCloseHandle(connect);
   url_loaded=true;
}

void check_updates(){
    textprintf_ex(buffer,font,10,720,makecol(255,0,0),-1,"Checking for updates");
    draw_sprite(screen,buffer,0,0);
    get_webpage("http://adsgames.net/server/browser/version.html");
    ifstream read("version.dat");
	  read >> local_version_number;
  	read.close();
  	server_version_number=atoi(DataReceived);
  	if(server_version_number>local_version_number){
        new_version=true;
        alert(NULL, NULL, "A new version of browser is available!","&Continue", NULL, 'c', 0);

  	}else{
        new_version=false;
        alert(NULL, NULL, "You are running the newest version.","&Continue", NULL, 'c', 0);
    }
}



void update(){

    rectfill(buffer,0,0,1024,768,makecol(255,255,255));
    textprintf_ex(buffer,font,30,3,makecol(0,0,0),-1,"%s",weburl);
    hline(buffer,0,27,1024,makecol(0,0,0));
    draw_sprite(buffer,reload,0,0);
    textprintf_ex(buffer,font,3,30,makecol(0,0,0),-1,"%s",DataReceived);
    if(!url_loaded)
        textprintf_ex(buffer,font,10,740,makecol(255,0,0),-1,"Loading...");
    if(failed_to_connect)
        textprintf_ex(buffer,font,10,740,makecol(255,0,0),-1,"URL Not Available.");
    if(location_clicked(0,27,0,27)){
        textprintf_ex(buffer,font,10,740,makecol(255,0,0),-1,"Loading...");
        draw_sprite(screen,buffer,0,0);
        url_loaded=false;
        get_webpage(weburl);
    }
    if(key[KEY_C])check_updates();

    if(new_version)textprintf_ex(buffer,font,815,740,makecol(155,155,155),-1,"Version %i, %i is available.",local_version_number,server_version_number);
    else textprintf_ex(buffer,font,940,740,makecol(155,155,155),-1,"Version %i", local_version_number);


    draw_sprite(buffer,cursor,mouse_x,mouse_y);
    draw_sprite(screen,buffer,0,0);
}






void setup(){
    buffer=create_bitmap(1024,768);

      if(!(reload = load_bitmap("reload.png",NULL))){
        abort_on_error( "Cannot find reload.png.\n Please check your files and try again.");
    }
    if(!(cursor= load_bitmap("cursor.png",NULL))){
        abort_on_error( "Cannot find cursor.png.\n Please check your files and try again.");
    }

    if(!(f1 = load_font("font_12.pcx", NULL, NULL))){
    abort_on_error( "Cannot find fonts/font_12.pcx \n Please check your files and try again");
    }
    f2 = extract_font_range(f1, ' ', 'A'-1);
    f3 = extract_font_range(f1, 'A', 'Z');
    f4 = extract_font_range(f1, 'Z'+1, 'z');

    //Merge temporary fonts to create "pixelart"
    font_12 = merge_fonts(f4, f5 = merge_fonts(f2, f3));
    font = merge_fonts(f4, f5 = merge_fonts(f2, f3));




  destroy_font(f1);
  destroy_font(f2);
  destroy_font(f3);
  destroy_font(f4);
  destroy_font(f5);

}


int main(){



  allegro_init();
  alpng_init();
  install_timer();
  install_keyboard();
  install_mouse();
  set_color_depth(32);

  set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1024,768, 0, 0);
  install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,".");



  set_window_title("Browser");
  setup();
  update();
  check_updates();
  get_webpage(weburl);


    while(!key[KEY_ESC]){
        update();
  	}

	return 0;
}
END_OF_MAIN()

