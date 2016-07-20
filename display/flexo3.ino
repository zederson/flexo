#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);

void drawOverview() {
  u8g.setFont(u8g_font_8x13B);

  //Linha superior - temperatura
  u8g.drawStr( 5, 15, "23");
  u8g.drawCircle(28,8,3);
  u8g.drawStr( 34, 15, "C");
  //Hora
  u8g.setFont(u8g_font_fub30);
  u8g.drawStr( 10, 57, "09:35");

  //Texto - AM
  u8g.setFont(u8g_font_5x7);
  u8g.drawStr( 115, 33, "AM");

  //moldura relogio
  u8g.drawRFrame(0,18, 128, 46, 4);

  //Desenho bateria
  u8g.drawRFrame(105, 3, 20,12 , 0);
  u8g.drawBox(125, 6, 2,6);
  u8g.drawBox(107, 5, 4,8);
  u8g.drawBox(114, 5, 4,8);

  //Desenho linhas sinal
  u8g.drawVLine(99,0, 15);
  u8g.drawVLine(98,0, 15);
  u8g.drawVLine(96,4, 11);
  u8g.drawVLine(95,4, 11);
  u8g.drawVLine(93,8, 7);
  u8g.drawVLine(92,8, 7);
  u8g.drawVLine(90,12, 3);
  u8g.drawVLine(89,12, 3);

}

void drawCaneca() {
  u8g.setFont(u8g_font_8x13B);
  u8g.drawRFrame(0, 18, 128, 46, 4);
  u8g.drawStr( 30, 15, "BAFOMETRO");
  u8g.drawStr( 10, 37, "Aguarde");

    u8g.drawBox(80, 25, 20, 30);
    u8g.drawHLine(77, 24, 26);
    u8g.drawRFrame(78, 25 , 24, 32, 0);
    u8g.drawRFrame(77, 25 , 26, 32, 0);
    u8g.drawHLine(76, 57, 28);
    u8g.drawHLine(76, 58, 28);
    u8g.drawRFrame(102, 30 , 7, 20, 2);
    u8g.drawRFrame(102, 28 , 9, 24, 2);
    u8g.setColorIndex(0);
    u8g.drawBox(79, 25, 22, 1);
    u8g.setColorIndex(1);
}



void setup(void) {
  Serial.begin(9600);
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  } else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  } else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  } else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
}

void loop(void) {
  u8g.firstPage();
  do {
    drawOverview();
  } while( u8g.nextPage() );

  delay(10000);
  /* u8g.firstPage(); */
  /* do { */
  /*   drawCaneca(); */
  /* } while( u8g.nextPage() ); */

  /* delay(10000); */
}
