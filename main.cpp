#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#define GAME_TERMINATE -1
enum KEYS{ UP, DOWN, LEFT, RIGHT};

ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_FONT* font = NULL;
ALLEGRO_BITMAP* img = NULL;
ALLEGRO_BITMAP* bg = NULL;
ALLEGRO_BITMAP* title = NULL;
ALLEGRO_BITMAP* papa = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;

typedef struct character
{
    int x;
    int y;
    ALLEGRO_BITMAP *image_path;

} Character;

Character character1;

const char *gametitle = "bibebgsat";
const int width = 1280;
const int height = 720;
int window = 1;
bool next_window = false;
    bool done = false;
	int pos_x = width / 2;
	int pos_y = height / 2;
    bool keys[4] = {false, false, false, false};

void show_err_msg(int msg);
void game_init();
void game_begin();
void game_destroy();
int game_run();
int process_event();



int main(int argc, char *argv[])
{
    int msg = 0;

    printf("Loading...\n");
    al_rest(1);

    game_init();
    game_begin();

    while (msg != GAME_TERMINATE)
    {
        msg = game_run();
        if (msg == GAME_TERMINATE)
            printf("Bye\n");
    }

    game_destroy();
    return 0;
}


void show_err_msg(int msg)
{
    fprintf(stderr, "unexpected msg: %d", msg);
    game_destroy();
    exit(9);
}


void game_init()
{
    if (!al_init())
        show_err_msg(-1);

    display = al_create_display(width,height);
    al_set_window_position(display, 0, 0);
    al_set_window_title(display, gametitle);

    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_install_mouse();

    event_queue = al_create_event_queue();
    al_register_event_source(event_queue,al_get_display_event_source(display));
    al_register_event_source(event_queue,al_get_keyboard_event_source());

}


void game_begin()
{
    ALLEGRO_COLOR al_map_rgb(unsigned char r, unsigned char g, unsigned char b);
    font = al_load_ttf_font("Wildy-Sans.ttf", 15, 0);
    //al_clear_to_color(al_map_rgb(0,0,0));
    img = al_load_bitmap("johnny.png");
    al_draw_bitmap(img,0, 0, 0); //for character
    bg = al_load_bitmap("title_bg.png");
    title = al_load_bitmap("title.png");
    papa = al_load_bitmap("papa.png");
    al_draw_bitmap(bg, 0, 0, 0);
    al_draw_bitmap(title, 0, 0, 0);
    al_draw_bitmap(papa, 0, 0, 0);
    al_draw_text(font,al_map_rgb(218,240,238),width,height,ALLEGRO_ALIGN_CENTER,"PRESS ENTER TO START");
    al_flip_display();

}

int process_event()
{

    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);

    if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        return GAME_TERMINATE;

    if(event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(event.keyboard.keycode)
			{
				case ALLEGRO_KEY_UP:
					keys[UP] = true;
					break;
				case ALLEGRO_KEY_DOWN:
					keys[DOWN] = true;
					break;
				case ALLEGRO_KEY_RIGHT:
					keys[RIGHT] = true;
					break;
				case ALLEGRO_KEY_LEFT:
					keys[LEFT] = true;
					break;
                case ALLEGRO_KEY_ESCAPE:
                    return GAME_TERMINATE;
                    break;
			}
		}
		else if(event.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch(event.keyboard.keycode)
			{
				case ALLEGRO_KEY_UP:
					keys[UP] = false;
					break;
				case ALLEGRO_KEY_DOWN:
					keys[DOWN] = false;
					break;
				case ALLEGRO_KEY_RIGHT:
					keys[RIGHT] = false;
					break;
				case ALLEGRO_KEY_LEFT:
					keys[LEFT] = false;
					break;
				case ALLEGRO_KEY_ESCAPE:
					done = true;
					break;
                case ALLEGRO_KEY_ENTER:
                    next_window = true;
                    break;
			}
		}



		character1.y -= keys[UP] * 10;
		character1.y += keys[DOWN] * 10;
		character1.x -= keys[LEFT] * 10;
		character1.x += keys[RIGHT] * 10;


}

int game_run()
{
    int error = 0;
    if(window == 1)
    {
        if (!al_is_event_queue_empty(event_queue))
        {
            error = process_event();
            if(next_window)
            {
                window = 2;
                // Setting Character
                character1.x = width / 2;
                character1.y = height / 2 + 150;
                character1.image_path = img;
                bg = al_load_bitmap("title_bg.png");

                //Initialize Timer
                /* timer  = al_create_timer(1.0/15.0);
                 timer2  = al_create_timer(1.0);
                 timer3  = al_create_timer(1.0/10.0);
                 al_register_event_source(event_queue, al_get_timer_event_source(timer)) ;
                 al_register_event_source(event_queue, al_get_timer_event_source(timer2)) ;
                 al_register_event_source(event_queue, al_get_timer_event_source(timer3)) ;
                 al_start_timer(timer);
                 al_start_timer(timer2);
                 al_start_timer(timer3);*/
            }
        }
    }
    // Second window(Main Game)
    else if(window == 2)
    {
        // Change Image for animation
        al_draw_bitmap(bg, 0,0, 0);
        //if(ture)
        al_draw_bitmap(character1.image_path, character1.x, character1.y, 0);

        //if(dir) al_draw_bitmap(character2.image_path, character2.x, character2.y, 0);
        // else al_draw_bitmap(character3.image_path, character2.x, character2.y, 0);

        al_flip_display();
        al_clear_to_color(al_map_rgb(0,0,0));

        // Listening for new event
        if (!al_is_event_queue_empty(event_queue))
        {
            error = process_event();
        }
        //if()
        //    window = 3
    }
    return error;
}

void game_destroy()
{
    al_destroy_display(display);
    al_destroy_font(font);
    al_destroy_bitmap(img);
    al_destroy_bitmap(papa);
    al_destroy_bitmap(title);
    al_destroy_bitmap(bg);
}
