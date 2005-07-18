#include <allegro.h>
#include <cstdio>
#include <stdint.h>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

char *code          = "AHETHBSFDgfsdasfdgSDFAGFafhrsghsARHhgad5eahgue9npae8h9p5en9ashawpgjr8ega9prna489g3yh8gepa84nj9aghn9aqhng3sauj56hES^J&*YJWYJJ^QWHSH45w7qgygheajnw75qyeseesqnjghgty4q823tyg9v28vyt79bcy4h3t78bhw38b7thtgahw5rHYGRSahaweWGPA7GFILW";
uint16_t codelength = 11;

int main() {
    uint32_t cur;
    uint32_t dat;
    uint32_t cc = 0;
    uint32_t x, y;
    uint32_t col;
    BITMAP *xx;
    
    dat = strlen(code);
    
    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT, 1152, 864, 0, 0);
    
    xx = load_bitmap("enc.tga", NULL);
    
    for (y=0;y<xx->h;y++) {
        for (x=0;x<xx->w;x++) {
            col = _getpixel32(xx, x, y);
            
            col = col ^ ((uint32_t)code[cc] | ((uint32_t)code[cc] << 8) | ((uint32_t)code[cc] << 16));
            
            _putpixel32(screen, x, y, col);
            
            cc++;
            if (cc >= dat) cc = 0;
        }    
    }    
    
    destroy_bitmap(xx);
    readkey();
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
    return 0;
}
END_OF_MAIN();



    
