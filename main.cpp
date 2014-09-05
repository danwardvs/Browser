#include<iostream>
#include<allegro.h>
#include<alpng.h>
#include<string>
#include <sstream>
#include <iterator>
#include <algorithm>
#include<wininet.h>


BITMAP* buffer;
BITMAP* reload;

char* weburl="http://motherfuckingwebsite.com";
char DataReceived[4096];

FONT* font_12;
FONT* f1;
FONT* f2;
FONT* f3;
FONT* f4;
FONT* f5;

//A function that handles error messages
void abort_on_error(const char *message){
	 if (screen != NULL){
	    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	 }
	 allegro_message("%s.\n %s\n", message, allegro_error);
	 exit(-1);
}


void get_webpage(){
 HINTERNET connect = InternetOpen(weburl,INTERNET_OPEN_TYPE_PRECONFIG,NULL, NULL, 0);

    if(!connect){
      //cout<<"Connection Failed or Syntax error";
      alert(NULL, NULL, "Connection Failed or Syntax error.","&Continue", NULL, 'c', 0);
    }

    HINTERNET OpenAddress = InternetOpenUrl(connect,weburl, NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE|INTERNET_FLAG_KEEP_CONNECTION, 0);

    if ( !OpenAddress ){
      DWORD ErrorNum = GetLastError();
      //cout<<"Failed to open URL \nError No: "<<ErrorNum;
      alert(NULL, NULL, "Failed to open URL.","&Continue", NULL, 'c', 0);
      InternetCloseHandle(connect);

   }


   DWORD NumberOfBytesRead = 0;
   while(InternetReadFile(OpenAddress, DataReceived, 4096, &NumberOfBytesRead) && NumberOfBytesRead ){
           //cout << DataReceived;
   }

   InternetCloseHandle(OpenAddress);
   InternetCloseHandle(connect);
}


void update(){

    rectfill(buffer,0,0,1024,768,makecol(255,255,255));
    textprintf_ex(buffer,font,30,3,makecol(0,0,0),-1,"%s",weburl);
    hline(buffer,0,27,1024,makecol(0,0,0));
    draw_sprite(buffer,reload,0,0);
    textprintf_ex(buffer,font,3,30,makecol(0,0,0),-1,"Data:%s",DataReceived);





    draw_sprite(screen,buffer,0,0);
}
void setup(){
    buffer=create_bitmap(1024,768);

      if(!(reload = load_bitmap("reload.png",NULL))){
        abort_on_error( "Cannot find reload.png.\n Please check your files and try again.");
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


   //system("PAUSE");
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
  get_webpage();

    while(!key[KEY_ESC]){
        update();
  	}

	return 0;
}
END_OF_MAIN()

