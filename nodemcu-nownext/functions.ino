// Definisjon fra yr på værtekst

String getWeatherText(String weatherNumber){
  if(weatherNumber=="1"){return  "Sol";}
  if(weatherNumber=="2"){return  "Lettskyet";}
  if(weatherNumber=="3"){return  "Delvis    skyet";}
  if(weatherNumber=="4"){return  "Skyet";}
  if(weatherNumber=="40"){return "Lette     regnbyger";}
  if(weatherNumber=="5"){return  "Regnbyger";}
  if(weatherNumber=="41"){return "Kraftige  regnbyger";}
  if(weatherNumber=="24"){return "Lette regnbyger og torden";}
  if(weatherNumber=="6"){return  "Regnbyger og torden";}
  if(weatherNumber=="25"){return "Kraftige  regnbyger og torden";}
  if(weatherNumber=="42"){return "Lette     sluddbyger";}
  if(weatherNumber=="7"){return  "Sluddbyger";}
  if(weatherNumber=="43"){return "Kraftige  sluddbyger";}
  if(weatherNumber=="26"){return "Lette     sluddbyger og torden";}
  if(weatherNumber=="20"){return "Sluddbyger og torden";}
  if(weatherNumber=="27"){return "Kraftige  sluddbyger og torden";}
  if(weatherNumber=="44"){return "Lette     snobyger";}
  if(weatherNumber=="8"){return  "Snobyger";}
  if(weatherNumber=="45"){return "Kraftige  snobyger";}
  if(weatherNumber=="28"){return "Lette     snobyger og torden";}
  if(weatherNumber=="21"){return "Snøbyger  og torden";}
  if(weatherNumber=="29"){return "Kraftige  snobyger og torden";}
  if(weatherNumber=="46"){return "Lett      regn";}
  if(weatherNumber=="9"){return  "Regn";}
  if(weatherNumber=="10"){return "Kraftig   regn";}
  if(weatherNumber=="30"){return "Lett  regn og torden";}
  if(weatherNumber=="22"){return "Regn og   torden";}
  if(weatherNumber=="11"){return "Kraftig   regn og torden";}
  if(weatherNumber=="47"){return "Lett      sludd";}
  if(weatherNumber=="12"){return "Sludd";}
  if(weatherNumber=="48"){return "Kraftig   sludd";}
  if(weatherNumber=="31"){return "Lett      sludd og torden";}
  if(weatherNumber=="23"){return "Sludd og  torden";}
  if(weatherNumber=="32"){return "Kraftig   sludd og torden";}
  if(weatherNumber=="49"){return "Lett sno";}
  if(weatherNumber=="13"){return "Sno";}
  if(weatherNumber=="50"){return "Kraftig   sno";}
  if(weatherNumber=="33"){return "Lett sno  og torden";}
  if(weatherNumber=="14"){return "Sno og    torden";}
  if(weatherNumber=="34"){return "Kraftig   sno og torden";}
  if(weatherNumber=="15"){return "Taake";}
  return "Ukjent";
  }

 
// Rutine for å skrive ut en melding til skjerm 

void displayInfo(String info, int seconds, int loading){
    display.clearDisplay();
    display.setTextSize(2);
    if(info.length()>16){
        display.setTextSize(1);
      }
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println(info);
    if(loading=1){
          display.setTextSize(2);
          display.setTextColor(WHITE);
          display.setCursor(0,0);
          display.println("LOADING");
      }
    
    display.display();
    Serial.println(info); // skriv ut linjen til konsoll
    delay((seconds*10));
}

// Later, add icons like:
// https://github.com/andydoro/Huzzah-Weather-Display/blob/master/Huzzah-Weather-Display/icons.h
// void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
// where bitmap is progmem
// https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives

