      //
      // Jolene Michel
      // start date 5/8/26
      // end date 
      //  Personal project: 3
      //
      // the goal of the project is to create a functional mp3 player using an esp32 microcontroller. In order to do this the main part of the code is the state machine. 
      //  There are currently 3 states: the main menu, the selection, and song
      // the the main menu state is the first state that the program goes it. it is a very simple state that just welcomes you in and waits for a button input
      // the selection state is where you pick a song from the playlist. In the future I want to see if I will be able to keep the same code but use different songs
      // lastly the song screen is the screen that shows what is currently playing. this screen also allows you to skip, replay, loop, and shuffle the songs.
      // in the future I want to rework the state machine a little so I can include bluetooth capabilities to it. 
      // the device itself uses a oled screen in order to display everything along with a DFplayer in order to transfer the audio files into actusl sound.
      // there are also 5 buttons that help select and navigate the device
      //

      #include <Arduino.h>
      #include <U8g2lib.h>
      #include "DFRobotDFPlayerMini.h"
      #include <HardwareSerial.h>
      #include <ezButton.h>

      HardwareSerial mySerial(2); //uart2
      DFRobotDFPlayerMini player;

    // oled set up
      #define OLED_CLK  18
      #define OLED_MOSI 23
      #define OLED_CS   5
      #define OLED_DC   16
      #define OLED_RES  17

      U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI u8g2(
        U8G2_R0,
        OLED_CLK,
        OLED_MOSI,
        OLED_CS,
        OLED_DC,
        OLED_RES
      );

    // button set up
      #define BUTTON_PIN_1 13
      #define BUTTON_PIN_2 14
      #define BUTTON_PIN_3 32
      #define BUTTON_PIN_4 33
      #define BUTTON_PIN_5 25


      ezButton red(BUTTON_PIN_1);
      ezButton green(BUTTON_PIN_2);
      ezButton yellow(BUTTON_PIN_3);
      ezButton blue(BUTTON_PIN_4);
      ezButton option(BUTTON_PIN_5);


    // variable intialization
        int num = 0;
      int selected = 0;
    bool play = true;
    bool loops = false;
    bool shuffle = false;
    int volumenum = 15;
    int lastVolume = -1;

      // menu + adding songs
      struct MenuOption {
      const char* name;
      int x;
      int y;
    };

      int menu = 0;
      const int songCount = 10;  // amount of songs in the mico sd file

      const char* songnames[] = {"Die With A Smile", "Billie Jean", "Purple Hearts", "Calm Down", "Picture of you", "Living Mice", "Sienna", "Lacy", "Head in a Jar", "20 Something" }; // songs in the list would appear
      const char* artists[] = {"Bruno Mars, Lady Gaga", "Micheal Jackson", "Kendrick Lamar", "Selena Gomez, Rema", "The Cure", "C418", "The Marias", "Olivia Rodrigo", "Khamari", "Sza" }; // songs in the list would appear

    MenuOption songmenu[] = {{"loop", 10, 50},{ "exit", 50, 60}, {"shuffle",80,50}}; // for the menu state
    const int menucount = 4;

      enum states { main_menu, selection, song}; // state machine set up
      states state = main_menu;

    //bluetooth set up

    // class MyServerCallbacks: public BLEServerCallbacks {
    //   void onConnect(BLEServer* pServer) {
    //     deviceConnected = true;
    //     Serial.println("Phone connected");
    //   }

    //   void onDisconnect(BLEServer* pServer) {
    //     deviceConnected = false;
    //     Serial.println("Phone disconnected");

    //     delay(200);
    //     BLEDevice::startAdvertising();
    //     Serial.println("Advertising restarted");
    //   }
    // };

    // class MyCallbacks: public BLECharacteristicCallbacks {
    //   void onWrite(BLECharacteristic *pCharacteristic) {
    //     String value = pCharacteristic->getValue();

    //     if (value.length() > 0) {
    //       Serial.print("Received: ");
    //       Serial.println(value);

    //       char ch = value.charAt(0);

    //       if (ch >= '0' && ch <= '9') {
    //         int num = ch - '0';
    //         displayDigit(num);

    //         Serial.print("Showing digit: ");
    //         Serial.println(num);

    //         pCharacteristic->setValue(value);
    //       }
    //     }
    //   }
    // };


      void setup() {

        Serial.begin(115200);
      // set up the buttons to be pull up resistors
      pinMode(BUTTON_PIN_1, INPUT_PULLUP);
      pinMode(BUTTON_PIN_2, INPUT_PULLUP);
      pinMode(BUTTON_PIN_3, INPUT_PULLUP);
      pinMode(BUTTON_PIN_4, INPUT_PULLUP);
      pinMode(BUTTON_PIN_5, INPUT_PULLUP);

    // set up how long it takes for it to read per press
        red.setDebounceTime(170);
        green.setDebounceTime(170);
        yellow.setDebounceTime(170);
        blue.setDebounceTime(170);
        option.setDebounceTime(170);

    // make sure the oled and dfplayer are working
        u8g2.begin();
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.clearBuffer();
        u8g2.drawStr(0, 20, "OLED works");
        u8g2.drawStr(0, 40, "Starting DFPlayer");
        u8g2.sendBuffer();


    // begin dfplayer, rx 26, tx 27
        mySerial.begin(9600, SERIAL_8N1, 26, 27);

        if (!player.begin(mySerial)) {
          Serial.println("DFPlayer not detected");
        
          u8g2.clearBuffer();
          u8g2.drawStr(0, 20, "OLED works");
          u8g2.drawStr(0, 40, "DFPlayer failed");
          u8g2.sendBuffer();
          delay(10);
          return;
        }
          // make sure the player is working in console
        Serial.println("DFPlayer connected");


      // player.volume(volumenum);

        Serial.println("DFPlayer connected");

      }



      void songnum(int num) {
          u8g2.drawStr(0, 20, songnames[num]);
          u8g2.drawStr(0, 40, artists[num]);
      }

      void loop() {

        red.loop();
        green.loop();
        yellow.loop();
        blue.loop();
        option.loop();

        if(state == song && (player.available())) {
       if (player.readType() == DFPlayerPlayFinished){

          if (shuffle) {
            num = random(0, songCount);
        } else {

        num++;
          if (num >= songCount) {
         num = 0;
          }
        }
        player.play(num + 1);

        }
       }

        int analogValue = analogRead(36); // set up pot to gpio36
       volumenum = map(analogValue, 0, 4095, 0, 30); // set up the analog value with the volume max (30)
        // player value stays with volume num
        if (volumenum != lastVolume) {
         player.volume(volumenum);
        lastVolume = volumenum;
        }

        switch(state) {
          
          case main_menu: // this case controls the start up screen. a simple welcome! and a blinking "press any button" until you move on to the next state
          player.stop();

                  u8g2.clearBuffer();
                  u8g2.drawStr(0, 10, "welcome!");

                      if ((millis() / 800) % 2 == 0) {
                          u8g2.drawStr(0, 50, "press any button");
                        }

                u8g2.sendBuffer();


                if (red.isPressed() || blue.isPressed() ||
            green.isPressed() || yellow.isPressed()) {
                num = 0;
          state = selection;
        }

        u8g2.drawStr(0, 10, "Hello!");


      
        break;

          case selection: // this state is where you select the intital song you want to listen to. at a time 6 songs can appear on the screen, so the design uses a "scrollable" led screen 

          {

          
        player.stop(); // when in this state music shouldn't be playing

       if (blue.isPressed()) { // going up on the selection
        menu--;

       if (menu < 0) {
         menu = songCount - 1;
        }
       }

        if (red.isPressed()) { // going down on the selection
       menu++;

        if (menu >= songCount) { // wraps the selection back around when you reach a number that the array doesn't have yet. 
        menu = 0;
          }
        }
            Serial.println(num);
            Serial.println(songnames[num]);

            u8g2.clearBuffer();

// scrollable menu set up
int start = menu - 2; 

if (start < 0) {
    start = 0;
}

if (start > songCount - 6) {
    start = songCount - 6;
}

for (int i = 0; i < 6; i++) { // for loop to make it where whatever song is selected at the time has ">>"" next to it

    int songIndex = start + i;

    if (songIndex >= songCount) {
        break;
    }

    if (songIndex == menu) {
        u8g2.drawStr(0, 10 + i * 10, ">>");
    }

    u8g2.drawStr(18, 10 + i * 10, songnames[songIndex]);
}

        u8g2.sendBuffer();


       if (option.isPressed()) {  // changes to the state with the selected song tied to the num variable
        num = menu;
          player.play(num + 1);
          selected = 0;
        state = song;
        }


        break; // end of selection case
          }
        // song case
       case song:

         u8g2.clearBuffer();

        if (red.isPressed()) { // changes the song to a previous one
         num++;

         if (num >= songCount) {
          num = 0;
         }

      player.play(num + 1);
    }

    if (blue.isPressed()) { // changes the song to the next one
      num--;

      if (num < 0) {
        num = songCount - 1;
      }

      player.play(num + 1);
    }

    if  (green.isPressed()) {   // scrolls between the loop, exit, shuffle, and pause/unpause selection
          Serial.println("Button 1 pressed");
              selected++;
          if (selected >= menucount) {
            selected = 0;
          } 
          Serial.println(selected);

         }   
        
            if (yellow.isPressed()) {  // scrolls between the loop, exit, shuffle, and pause/unpause selection
          Serial.println("Button 2 pressed");
              selected--;
          if (selected < 0) {
            selected = menucount - 1;
          }
          Serial.println(num);

          }
  // at the top of the led displays the song name and the artist
          u8g2.drawStr(0, 10, "Now Playing:");
          u8g2.drawStr(0, 20, songnames[num]);
          u8g2.drawStr(0, 30, artists[num]);



  for (int i = 0; i < menucount; i++) { 
    if (i == 3) {
      continue; // pause/play option
    }
    const char* text = songmenu[i].name;

    if (i == 0 && loops) { // when loops is selected
      text = "[loop]";
    }

    if (i == 2 && shuffle) {
      text = "[shuffle]";
    }

    if (i == selected) {
      u8g2.drawStr(songmenu[i].x - 5, songmenu[i].y, ">");
      u8g2.drawStr(songmenu[i].x + 30, songmenu[i].y, "<");
    }

    u8g2.drawStr(songmenu[i].x, songmenu[i].y, text);
  }

      if(option.isPressed()) {

        if (selected == 0) {
        loops = !loops;
        shuffle = false;

      }

      if (selected == 1) {
        player.stop();
        state = main_menu;
        }

      if (selected == 2) {
        shuffle = !shuffle;
         loops = false;

      }

        if (selected == 3) {
      play = !play;
        }

            if (play) {
        player.start();
      } 
      else {
        player.pause();
      }

          if (loops == true){
            player.enableLoop();
            
          }
            else {
            player.disableLoop();

        }

    }
        
    u8g2.sendBuffer();
      
    break;

      }

    }
    