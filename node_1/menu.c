#include "menu.h"


extern uint8_t menu_position[];

extern uint8_t menu_type;



void navigate_menu_with_joystick(void)
{
  volatile char joystick_dir = direction_of_joystick_DULRN();
  volatile int joystick_button_pressed = is_joystick_button_pressed();

  if (joystick_button_pressed){
    joystick_dir = 'P';
  }

  switch (joystick_dir)
  {
  case 'D':
    if (menu_position[0] == 1){
      menu_position[0] = 2;
    } else{
      menu_position[0] = 1;
    }

    break;

  case 'U':
    if (menu_position[0] == 2){
      menu_position[0] = 1;
    } else{
      menu_position[0] = 2;
    }
    break;

  case 'P':
    if (menu_position[1] == 1){
      menu_position[1] = 2;
    } else{
      menu_position[1] = 1;
    }
    break;

  default:
    break;
  }
  
}

uint8_t get_menu_type(){
  uint8_t menu = (menu_position[0]*10) + menu_position[1];
  return menu;
}

void draw_menu(uint8_t menu_type){
  switch(menu_type){
    default:
      break;

    case 11:
      oled_reset();
      oled_goto_line(0);
      oled_goto_column(0);
      oled_print("Play Game X");
      oled_goto_line(1);
      oled_goto_column(0);
      oled_print("Settings");
      break;

    case 12:
      oled_reset();
      oled_goto_line(0);
      oled_goto_column(0);
      oled_print("Very cool game");
      oled_goto_line(1);
      oled_goto_column(0);
      oled_print("is now");
      oled_goto_line(2);
      oled_goto_column(0);
      oled_print("playing");
      break;

    case 13:
    oled_reset();
    oled_goto_line(0);
    oled_goto_column(0);
    oled_print(" GAME OVER");
    oled_goto_line(1);
    oled_goto_column(0);
    oled_print("Return with");
    oled_goto_line(2);
    oled_goto_column(0);
    oled_print("LEFT BUTTON");
    break;
    

    case 21:
      oled_reset();
      oled_goto_line(0);
      oled_goto_column(0);
      oled_print("Play Game");
      oled_goto_line(1);
      oled_goto_column(0);
      oled_print("Settings X");
      
      break;
    case 22:
      oled_reset();
      oled_goto_line(0);
      oled_goto_column(0);
      oled_print("no settings");
      oled_goto_line(1);
      oled_goto_column(0);
      oled_print("nerd");
      oled_goto_line(2);
      oled_goto_column(0);
      oled_print("click 2 go back");
      break;}
}

void menu_func(void)
{
  volatile char joystick_dir = direction_of_joystick_DULRN();
  volatile int joystick_button_pressed = is_joystick_button_pressed();
  
  
  joystick_dir = direction_of_joystick_DULRN();
  joystick_button_pressed = is_joystick_button_pressed(); 
  uint8_t current_menu_type = get_menu_type();
  
  if((joystick_dir == 'D') || (joystick_dir == 'U') || (joystick_button_pressed)){
      oled_reset();
      navigate_menu_with_joystick();
      _delay_ms(1000);
      draw_menu(current_menu_type);
    }
  
}