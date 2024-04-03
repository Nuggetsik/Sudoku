#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

#define FPS 60                          //ФПС 
#define SIZE_FONT 60                    //Размер шрифта

int W = 720;                           //Ширина
int H = 720;                           //Высота


const int resolution[3][2] = { {720, 720}, {900, 900}, {1000, 1000} };
//Структура кнопки
struct BUTTON
{
    char name_file[250];                                //Путь к изображению
    float width_b;                                      //Ширина кнопки
    float height_b;                                     //Высота кнопки
    int x;                                              //Координаты
    int y;
    void (*show)(char*, float, float, int, int);        //Поинтер на функцию (показать кнопку)
};

struct ICON
{
    char name_file[250];                                //Путь к изображению
    float width_b;                                      //Ширина кнопки
    float height_b;                                     //Высота кнопки
    int x;                                              //Координаты
    int y;
    void (*show)(char*, float, float, int, int);        //Поинтер на функцию
};


//Функция для показа кнопки
void show(char* name_file, float width_b, float height_b, int x, int y)
{
    ALLEGRO_BITMAP* button = al_load_bitmap(name_file);
    if (!button) exit(-3);

    al_draw_scaled_bitmap(button, 0, 0, al_get_bitmap_width(button), al_get_bitmap_height(button), x, y, width_b, height_b, 0);
    al_destroy_bitmap(button);
}

void change_resolution(const ALLEGRO_DISPLAY* display, int width, int height) {
    al_resize_display(display, width, height);
    // Дополнительные действия, если необходимо
}


bool setting(const ALLEGRO_DISPLAY* display, const  ALLEGRO_EVENT_QUEUE* event_queue, const  ALLEGRO_TIMER* timer, bool* finish)
{
    ALLEGRO_BITMAP* setting_menu = al_load_bitmap("image/setting.png");
    if (!setting_menu) exit(-3);

    int width = W / 1.5;
    int height = H / 2.5;

    ALLEGRO_FONT* font = al_load_ttf_font("Tenada.ttf", 25, 0);      //Шрифт (при ошибке добавления шрифта возвращает -2)
    if (!font) return -2;

    char text_setting[] = "Setting";                                                                 //Текст который выведится на экран (НАЗВАНИЕ ИГРЫ)
    int text_setting_width = al_get_text_width(font, text_setting);
    char text_resolution[] = "Resolution window ";
    char text_resolution_num[10];
    int variant_size = 0;
    int i = variant_size;
   

    struct BUTTON b_accept = { "image/accept.png", width / 6, height / 8, 0, 0, show };
    b_accept.x = W - ( (W - width) / 2 ) - b_accept.width_b - 20;
    b_accept.y = H - ((H - height) / 2) - b_accept.height_b - 20;

    
    struct BUTTON b_right = { "image/right_arrow.png", width / 20, height / 18, 0, 0, show }; 
    b_right.x = W - (W-width) / 2 - b_right.width_b - 15;
    b_right.y = (H - height) / 2 + 60;

    struct BUTTON b_left = { "image/left_arrow.png", width / 20, height / 18, 0, 0, show };
    b_left.x = width / 2 + b_left.width_b + 130;
    b_left.y = (H - height) / 2 + 60;

    bool open_setting = true;
    while (open_setting)
    {
        ALLEGRO_EVENT event;            //Переменная хранящее наше событие, то что мы делаем

        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)                  //проверка на закрытие окна
        {
            open_setting = false;
            *finish = true;
            
          
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)              //Проверка на отпускания кнопки мыши
        {
            if (event.mouse.button & 1)                                                     //нажатие ЛКМ (1 - ЛКМ, 2 - ПКМ, 3 - колесико)
            {
                int x = event.mouse.x;                              //Получения координаты мыши
                int y = event.mouse.y;

                if (x >= b_accept.x && x < (b_accept.x + b_accept.width_b) && y >= b_accept.y && y < (b_accept.y + b_accept.height_b))    //Проверка совпадает ли координаты мыши с кнопкой (кароче нажатие кнопки)
                {
                    open_setting = false;
                    W = resolution[i][0];
                    H = resolution[i][1];
                    change_resolution(display, resolution[i][0], resolution[i][1]);
                    al_destroy_font(font);
                    al_destroy_bitmap(setting_menu);
                    return true;
                }       

                if (x >= b_right.x && x < (b_right.x + b_right.width_b) && y >= b_right.y && y < (b_right.y + b_right.height_b))    //Проверка совпадает ли координаты мыши с кнопкой (кароче нажатие кнопки)
                {
                    if (variant_size >= 0 && variant_size < 2)
                    {
                        variant_size += 1;
                        
                    }
                }

                if (x >= b_left.x && x < (b_left.x + b_left.width_b) && y >= b_left.y && y < (b_left.y + b_left.height_b))    //Проверка совпадает ли координаты мыши с кнопкой (кароче нажатие кнопки)
                {
                    if (variant_size > 0 && variant_size <= 2)
                    {
                        variant_size -= 1;
                 

                    }
                }
            }
        }


        if (event.type == ALLEGRO_EVENT_TIMER)          //Таймер
        {
            i = variant_size;
            

            sprintf(text_resolution_num, "%d:%d", resolution[i][0], resolution[i][1]);
            int text_res_num_width = al_get_text_width(font, text_resolution);

            al_draw_scaled_bitmap(setting_menu, 0, 0, al_get_bitmap_width(setting_menu), al_get_bitmap_height(setting_menu), W/2 - width/2, H/2 - height/2,width, height, 0);
            al_draw_text(font, al_map_rgb(0, 0, 0), (W - text_setting_width) / 2, (H - height) / 2 + 25, 0, text_setting);
            
            al_draw_text(font, al_map_rgb(0, 0, 0), (W - text_setting_width) / 2, (H - height) / 2 + 25, 0, text_setting);
            al_draw_text(font, al_map_rgb(0, 0, 0), (W - width) / 2 + 25, (H - height) / 2 + 60, 0, text_resolution);
            al_draw_text(font, al_map_rgb(0, 0, 0), W - ((W - width) / 2) - text_res_num_width + 80, (H - height) / 2 + 60, 0, text_resolution_num);
            
            b_accept.show(b_accept.name_file, b_accept.width_b, b_accept.height_b, b_accept.x, b_accept.y);
            b_right.show(b_right.name_file, b_right.width_b, b_right.height_b, b_right.x, b_right.y);
            b_left.show(b_left.name_file, b_left.width_b, b_left.height_b, b_left.x, b_left.y);

            
            al_flip_display();
        }
    }
    
    al_destroy_font(font);
    al_destroy_bitmap(setting_menu);
    return false;
}



//Игровой процес (сама Судоку)        параметры(экран, события, таймер (для ФПС нужен))

int game(const ALLEGRO_DISPLAY* display, const ALLEGRO_EVENT_QUEUE* event_queue)
{
    ALLEGRO_FONT* font = al_load_ttf_font("Tenada.ttf", 25, 0);      //Шрифт (при ошибке добавления шрифта возвращает -2)
    if (!font) return -2;

    ALLEGRO_BITMAP* background = al_load_bitmap("image/background.png");    //Задний фонн (при ошибке возвращает -3)
    if (!background) exit(-3);

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    
    struct BUTTON clue = { "image/clue.png", W / 8, H / 8, 0,0, show };   //Кнопка подсказки
    clue.x = W - clue.width_b - 10;
    clue.y = H - clue.height_b - 25;

    struct BUTTON b_setting = { "image/icon_setting.png", W / 20, H / 20, 0, 0, show};
    b_setting.x = W - b_setting.width_b - 5;
    b_setting.y = 5;

    const int total_clue = 3;                                               //Всего подсказок (статическое значение)
    int count_clue = 3;                                                     //Кол-во подсказок
    char count_clue_str[10];                                                //Для конвертации с int на char(строку кароче)


    int count_heart = 3;                                                    //Всего сердец
    int heart_width = W / 9;                            
    int heart_height = H / 9;
   
    bool change_set = false;

    struct ICON hearts[3];                                                //Иконки сердец
    for (int i = 0; i < 3; i++)
    {
        hearts[i] = (struct ICON){"image/heart.png", heart_width, heart_height, W - heart_width - 20, 30 + (i*heart_height+15), show};  //Сердца в массив закидываем
    }
    

 
    bool finish = false;
    al_start_timer(timer);          //Запуск таймера (для ФПС)
    while (!finish)
    {


        ALLEGRO_EVENT event;            //Переменная хранящее наше событие, то что мы делаем

        

        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)                  //проверка на закрытие окна
        {
            finish = true;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)              //Проверка на отпускания кнопки мыши
        {

           
                if (event.mouse.button & 1)                                                     //нажатие ЛКМ (1 - ЛКМ, 2 - ПКМ, 3 - колесико)
                {
                    int x = event.mouse.x;                              //Получения координаты мыши
                    int y = event.mouse.y;

                    if (count_clue > 0 && count_clue <= 3)
                    {
                        if (x >= clue.x && x < (clue.x + clue.width_b) && y >= clue.y && y < (clue.y + clue.height_b))    //Проверка совпадает ли координаты мыши с кнопкой (кароче нажатие кнопки)
                        {
                            count_clue -= 1;        //Уменьшение кол-во подсказок на 1
                        }
                    }
                    if (x >= b_setting.x && x < (b_setting.x + b_setting.width_b) && y >= b_setting.y && y < (b_setting.y + b_setting.height_b))
                    {
                        change_set = setting(display, event_queue, timer, &finish);

                    }
                }
                
            
        }

        if (event.type == ALLEGRO_EVENT_TIMER)          //Таймер
        {
            

            sprintf(count_clue_str, "%d/%d", count_clue, total_clue);                                                                       //Конвертация с int на char (string)
            al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background), 0, 0, W, H, 0);      //Отрисовка заднего фона
            al_draw_text(font, al_map_rgb(0, 0, 0), clue.x+20, clue.y - 50, 0, count_clue_str);                                             //Отрисовка текста (кол-ва подсказок)
            clue.show(clue.name_file, clue.width_b, clue.height_b, clue.x, clue.y);                                                         //Показ кнопки подсказки
            b_setting.show(b_setting.name_file, b_setting.width_b, b_setting.height_b, b_setting.x, b_setting.y);
            
          
          
         
            al_flip_display();
        }

        if (change_set == true)
        {
            clue.width_b = W / 8;
            clue.height_b = H / 8;
            clue.x = W - clue.width_b - 10;
            clue.y = H - clue.height_b - 25;

            b_setting.width_b = W / 20; 
            b_setting.height_b= H / 20;
            b_setting.x = W - b_setting.width_b - 5;
            b_setting.y = 5;

            change_set = false;
        }

    }
    al_stop_timer(timer);           //Остановка таймера
    al_destroy_timer(timer);
    al_destroy_font(font);
    al_destroy_bitmap(background);
    return 0;
}


//Функция меню 
int menu(const ALLEGRO_DISPLAY* display, const  ALLEGRO_EVENT_QUEUE* event_queue)
{

    ALLEGRO_FONT* font = al_load_ttf_font("Tenada.ttf", SIZE_FONT, 0);                       //Шрифт
    if (!font) return -2;

    ALLEGRO_BITMAP* background = al_load_bitmap("image/menu_bg.png");                       //Задний фон
    if (!background) exit(-3);

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);

    al_register_event_source(event_queue, al_get_mouse_event_source());                     //Загрузка события мышки
    al_register_event_source(event_queue, al_get_display_event_source(display));            //Загрузка события окна
    al_register_event_source(event_queue, al_get_timer_event_source(timer));                //Загрузка события таймера


    struct BUTTON b_start = { "image/start.png", W / 4, H / 4, 0, 0, show };              //Создание кнопки
    b_start.x = W / 2 - b_start.width_b / 2;
    b_start.y = H / 1.25 - b_start.height_b / 2;

   

    char text[] = "Sudoku";                                                                 //Текст который выведится на экран (НАЗВАНИЕ ИГРЫ)
    int text_width = al_get_text_width(font, text);

    bool finish = false;
    bool start_game = false;

   

    al_start_timer(timer);
    while (!finish)
    {


        ALLEGRO_EVENT event;

        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            finish = true;
        }
        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)                                  //Проверка на нажатие кнопки мыши
        {
            if (event.mouse.button & 1)                                                     //нажатие ЛКМ (1 - ЛКМ, 2 - ПКМ, 3 - колесико)
            {
                int x = event.mouse.x;                              //Получения координат мыши
                int y = event.mouse.y;

                if (x >= b_start.x && x < (b_start.x + b_start.width_b) && y >= b_start.y && y < (b_start.y + b_start.height_b))    //Проверка совпадает ли координаты мыши с кнопкой (кароче нажатие кнопки)
                {
                    start_game = true;
                    finish = true;
                }
            }
        }
        if (event.type == ALLEGRO_EVENT_TIMER)
        {
            al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background), 0, 0, W, H, 0);      //Отрисовка заднего фона

            b_start.show(b_start.name_file, b_start.width_b, b_start.height_b, b_start.x, b_start.y);                                       //Показ кнопки старта
            

            al_draw_text(font, al_map_rgb(0, 0, 0), (W - text_width) / 2, H / 4, 0, text);                                                     //Отображение текста - название игры

            al_flip_display();                                                                                                              //Обновления экрана
        }

        if (start_game)
            game(display, event_queue);                                                                                              //Переход к игре

    }

    
    //Сброс всего, уничтожение 
    al_stop_timer(timer);

    al_destroy_timer(timer);
    al_destroy_font(font);
   
    al_destroy_bitmap(background);
    
    return 0;
}



int main(void)
{
    al_init();
    al_init_font_addon();       //Шрифт
    al_init_ttf_addon();        //Хз, но для шрифта
    al_init_image_addon();      //Изображения
    al_install_mouse();         //Мышка

    const ALLEGRO_DISPLAY* display = al_create_display(W, H);         //Создание окна W*H
    if (!display) return -1;

    al_set_window_title(display, "Sudoku");                     //Название окна


    const ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();     //События

    menu(display, event_queue);


    //Уничтожение окна и событий
    
    al_uninstall_mouse(); // Deinitialize mouse 
   
    al_shutdown_image_addon();    // Deinitialize image addon
    al_shutdown_ttf_addon();      // Deinitialize TrueType font addon
    al_shutdown_font_addon();     // Deinitialize font addon

    al_destroy_event_queue(event_queue);
    al_destroy_display(display);


    return 0;
}