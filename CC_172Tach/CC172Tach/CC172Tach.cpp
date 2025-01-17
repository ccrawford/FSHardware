
#include "CC172Tach.h"

#include "allocateMem.h"
#include "commandmessenger.h"
#include "Art\fullBackground.h"
#include "Art\tachNeedle.h"
#include "Art\tachMask.h"
#include "Art\windPointer.h"
#include "ArialBold16.h"
#include "NotoSansBold15.h"
#include "Art\windArrow.h"
#include "Sans48b.h"

#include "Art\aoaBackground.h"
#include "Art\redbardown.h"
#include "Art\donutbottom.h"
#include "Art\donuttop.h"
#include "Art\donut.h"


#define MF_CUSTOMDEVICE_SUPPORT 1
#define TFT_ORANGEYELLOW 0xFD20

TFT_eSPI tft = TFT_eSPI();

TFT_eSprite tachNeedleSpr = TFT_eSprite(&tft); //Needle for tachometer
TFT_eSprite tachMaskSpr = TFT_eSprite(&tft); //Mask for tachometer

TFT_eSprite windPointer = TFT_eSprite(&tft); //Wind compass needle
TFT_eSprite windArrow = TFT_eSprite(&tft); //Wind arrow

TFT_eSprite windBackground = TFT_eSprite(&tft); //Wind compass background

TFT_eSprite aoaBackground = TFT_eSprite(&tft); //AoA background
TFT_eSprite redBarDown = TFT_eSprite(&tft); //AoA red bar down
TFT_eSprite donutBottom = TFT_eSprite(&tft); //AoA donut bottom
TFT_eSprite donutTop = TFT_eSprite(&tft); //AoA donut top
TFT_eSprite donut = TFT_eSprite(&tft); //AoA donut

#define GAUGE_BACKGROUND_COLOR TFT_DARKGREY

CC172Tach::CC172Tach()
{
}

void CC172Tach::begin()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    tft.begin();
    tft.initDMA();
    tft.setRotation(3);
    tft.setSwapBytes(true);
    tft.fillScreen(GAUGE_BACKGROUND_COLOR);

    tft.pushImage(0,0, FULLBACKGROUND_WIDTH, FULLBACKGROUND_HEIGHT, FULLBACKGROUND_IMG_DATA, TFT_RED);
    
    //mainSpr.pushSprite(0, 0);
    // tft.setTextColor(TFT_LIGHTGREY);
    tachMaskSpr.loadFont(ArialBold16);
    tft.loadFont(NotoSansBold15);
    
    // tft.showFont(1000);

    tachMaskSpr.createSprite(TACHMASK_WIDTH, TACHMASK_HEIGHT);
    tachMaskSpr.setSwapBytes(true);
    tachMaskSpr.pushImage(0, 0, TACHMASK_WIDTH, TACHMASK_HEIGHT, TACHMASK_IMG_DATA);

    tachNeedleSpr.createSprite(TachNeedleWidth, TachNeedleHeight);
    tachNeedleSpr.setSwapBytes(true);
    tachNeedleSpr.setPivot(TachNeedlePivot_x, TachNeedlePivot_y);
    tachNeedleSpr.pushImage(0, 0, TachNeedleWidth, TachNeedleHeight, TachNeedle);

    int bounding_box = (int) std::sqrt(WINDARROW_WIDTH*WINDARROW_WIDTH + WINDARROW_HEIGHT*WINDARROW_HEIGHT);

    windBackground.createSprite(bounding_box, bounding_box);  // Big enough to hold the wind compass pointer
    windBackground.fillSprite(GAUGE_BACKGROUND_COLOR);

    windPointer.createSprite(WINDPOINTER_WIDTH, WINDPOINTER_HEIGHT);
    windPointer.setSwapBytes(true);
    windPointer.setPivot(WINDPOINTER_WIDTH/2, WINDPOINTER_HEIGHT/2);
    windPointer.pushImage(0, 0, WINDPOINTER_WIDTH, WINDPOINTER_HEIGHT, WINDPOINTER_IMG_DATA);


    windArrow.createSprite(WINDARROW_WIDTH, WINDARROW_HEIGHT);
    windArrow.setSwapBytes(true);
    windArrow.setPivot(WINDARROW_WIDTH/2, WINDARROW_HEIGHT/2);
    windArrow.pushImage(0, 0, WINDARROW_WIDTH, WINDARROW_HEIGHT, WINDARROW_IMG_DATA);
    
    aoaBackground.createSprite(AOABACKGROUND_WIDTH, AOABACKGROUND_HEIGHT);
    aoaBackground.setSwapBytes(true);
    aoaBackground.pushImage(0, 0, AOABACKGROUND_WIDTH, AOABACKGROUND_HEIGHT, AOABACKGROUND_IMG_DATA);

    redBarDown.createSprite(REDBARDOWN_WIDTH, REDBARDOWN_HEIGHT);
    redBarDown.setSwapBytes(true);
    redBarDown.pushImage(0, 0, REDBARDOWN_WIDTH, REDBARDOWN_HEIGHT, REDBARDOWN_IMG_DATA);

    donutBottom.createSprite(DONUTBOTTOM_WIDTH, DONUTBOTTOM_HEIGHT);
    donutBottom.setSwapBytes(true);
    donutBottom.pushImage(0, 0, DONUTBOTTOM_WIDTH, DONUTBOTTOM_HEIGHT, DONUTBOTTOM_IMG_DATA);

    donutTop.createSprite(DONUTTOP_WIDTH, DONUTTOP_HEIGHT);
    donutTop.setSwapBytes(true);
    donutTop.pushImage(0, 0, DONUTTOP_WIDTH, DONUTTOP_HEIGHT, DONUTTOP_IMG_DATA);

    donut.createSprite(DONUT_WIDTH, DONUT_HEIGHT);
    donut.setSwapBytes(false);
    donut.pushImage(0, 0, DONUT_WIDTH, DONUT_HEIGHT, DONUT_IMG_DATA);


    // aoaBackground.loadFont(arial48pt7b);
}

void CC172Tach::attach()
{
}

void CC172Tach::detach()
{
    if (!_initialised)
        return;
    _initialised = false;
}

void CC172Tach::set(int16_t messageID, char *setPoint)
{
    /* **********************************************************************************
        Each messageID has it's own value
        check for the messageID and define what to do.
        Important Remark!
        MessageID == -2 will be send from the board when PowerSavingMode is set
            Message will be "0" for leaving and "1" for entering PowerSavingMode
        MessageID == -1 will be send from the connector when Connector stops running
        Put in your code to enter this mode (e.g. clear a display)

    ********************************************************************************** */
    if(setPoint == NULL) return;

    // do something according your messageID
    switch (messageID) {
    case -1:
        // tbd., get's called when Mobiflight shuts down
    case -2:
        // tbd., get's called when PowerSavingMode is entered
    case 0:
         _curRpm = atoi(setPoint);
        break;
    case 1:
         _curHours = atof(setPoint); 
        break;
    case 2:
        _windDir = atof(setPoint);
        break;
    case 3:
        _windSpeed = atof(setPoint);
        break;
    case 4:
        _angleOfAttack = atof(setPoint);
        break;
    default:
        break;
    }
}

unsigned long lastMillis = 0;

void CC172Tach::update()
{
    
    updateRPM();
    updateEngineHours(); 

    // The mask masks both hours and rpm scale.
    tachMaskSpr.pushSprite(TACHMASK_X_OFFSET, TACHMASK_Y_OFFSET, TFT_RED);

    updateWind();

    updateAoA();

}

bool CC172Tach::updateWind()
{
    // Wind direction
    windBackground.fillSprite(GAUGE_BACKGROUND_COLOR);
    windBackground.setPivot(windBackground.width()/2, windBackground.height()/2);
    // windPointer.pushRotated(&windBackground,_windDir, TFT_WHITE);
    windArrow.pushRotated(&windBackground,_windDir, TFT_WHITE);

//    _windDir = _windDir + 1.0;

    windBackground.pushSprite(320 + 80 - windBackground.width()/2, 80 - windBackground.height()/2);

    // Wind speed
    tft.setTextColor(TFT_WHITE, GAUGE_BACKGROUND_COLOR, true);
    tft.setCursor(400, 135);
    // Print windspeed as an integer.
    tft.loadFont(NotoSansBold15);
    tft.printf("%d kts   ", (int)_windSpeed);  // Spaces will correct the 2-digit to 1-digit transition.
    
    // _windSpeed = _windSpeed + 0.1;
    // if(_windSpeed > 12) _windSpeed = 0;

    return true;
}


bool CC172Tach::updateRPM()
{
    _curRpm = min(3500, _curRpm);
    _curRpm = max(0, _curRpm);
    double angle = ((double)_curRpm - 500.0)/13.889 ;  

    tachMaskSpr.pushImage(0, 0, TACHMASK_WIDTH, TACHMASK_HEIGHT, TACHMASK_IMG_DATA);
    tachMaskSpr.setPivot(160-TACHMASK_X_OFFSET, 160-TACHMASK_Y_OFFSET);

//    tft.setPivot(BackgroundCenter_x-22, BackgroundCenter_y-22);
    tachNeedleSpr.pushRotated(&tachMaskSpr, (int16_t)angle, TFT_BLACK);



//    _curRpm = _curRpm + 10;
     
    // Try pushing straight to the screen
    //tft.setPivot(BackgroundCenter_x, BackgroundCenter_y);  
    //tachNeedleSpr.pushRotated( (int16_t)angle, TFT_BLACK);
    

    return true;
}

void left_pad_float(float num, char* padded_number, char* decimal_part) {
    // Truncate to get integer and first decimal digit
    int integer_part = (int)num;
    int decimal = (int)((num * 10) - (integer_part * 10));

    // Format padded number with spaces
    sprintf(padded_number, "%d  %d  %d  %d  %d ", 
            (integer_part / 10000) % 10, 
            (integer_part / 1000) % 10, 
            (integer_part / 100) % 10, 
            (integer_part / 10) % 10, 
            integer_part % 10);

    // Convert decimal part to string
    sprintf(decimal_part, " %d", decimal);
}

bool CC172Tach::updateAoA()
{
    // Angle of attack

    aoaBackground.pushImage(0, 0, AOABACKGROUND_WIDTH, AOABACKGROUND_HEIGHT, AOABACKGROUND_IMG_DATA);
  

    tft.setTextColor(TFT_WHITE, GAUGE_BACKGROUND_COLOR, true); // Two green.
    if (_angleOfAttack < 1.5) {
        tft.setTextColor(TFT_DARKGREEN, GAUGE_BACKGROUND_COLOR, true);
        aoaBackground.fillRect(16, 129, 30, 12, TFT_GREEN);
        aoaBackground.fillRect(16, 112, 30, 12, TFT_GREEN); // 5 pixel gap
    }
    else if (_angleOfAttack < 3.3)                             // Green and Yellow
    {   
        tft.setTextColor(TFT_GREEN, GAUGE_BACKGROUND_COLOR, true);
        aoaBackground.fillRect(16, 112, 30, 12, TFT_GREEN); 
        aoaBackground.fillRect(16, 95, 30, 12, TFT_ORANGEYELLOW); 
    }
    else if (_angleOfAttack < 4.2)                             // Two Plain Yellow
    {
        tft.setTextColor(TFT_ORANGEYELLOW, GAUGE_BACKGROUND_COLOR, true);
        aoaBackground.fillRect(16, 95, 30, 12, TFT_ORANGEYELLOW); 
        aoaBackground.fillRect(16, 78, 30, 12, TFT_ORANGEYELLOW); 
    }
    else if (_angleOfAttack < 5)                             // Yellow with Donut Bottom
    {
        tft.setTextColor(TFT_ORANGEYELLOW, GAUGE_BACKGROUND_COLOR, true);
        aoaBackground.fillRect(16, 78, 30, 12, TFT_ORANGEYELLOW); 
        donutBottom.pushToSprite(&aoaBackground, 16, 61);
    }
    else if (_angleOfAttack < 7.5)                            // Full donut
    {
        tft.setTextColor(TFT_BLUE, GAUGE_BACKGROUND_COLOR, true);
        donut.pushToSprite(&aoaBackground, 16, 45, TFT_WHITE);
    }
    else if (_angleOfAttack < 8.3)                            // Red top donut with bottom donut
    { 
        tft.setTextColor(TFT_RED, GAUGE_BACKGROUND_COLOR, true);
        donut.pushToSprite(&aoaBackground, 16, 45, 0x4A69);
        donutTop.pushToSprite(&aoaBackground, 16, 45);
        // donutTop.pushToSprite(&aoaBackground, 16, 44);
//        aoaBackground.fillRect(16, 27, 30, 12, TFT_RED); 
    }                           
    else if (_angleOfAttack < 9.1)                            // Red top donut and red
    {
        tft.setTextColor(TFT_RED, GAUGE_BACKGROUND_COLOR, true);
        donutTop.pushToSprite(&aoaBackground, 16, 45);
        aoaBackground.fillRect(16, 27, 30, 12, TFT_RED);

    }
    else if (_angleOfAttack < 10.0)                            // One red with one pull up
    {
        tft.setTextColor(TFT_RED, GAUGE_BACKGROUND_COLOR, true);
        aoaBackground.pushImage(aoaBackground.width()/2 - redBarDown.width()/2, 10, redBarDown.width(), redBarDown.height(), REDBARDOWN_IMG_DATA); 
        aoaBackground.fillRect(16, 27, 30, 12, TFT_RED);

    }
    else   // Red with pull-up
    {
        tft.setTextColor(TFT_RED, GAUGE_BACKGROUND_COLOR, true);
        aoaBackground.pushImage(aoaBackground.width()/2 - redBarDown.width()/2, 27, redBarDown.width(), redBarDown.height(), REDBARDOWN_IMG_DATA);       
        aoaBackground.pushImage(aoaBackground.width()/2 - redBarDown.width()/2, 10, redBarDown.width(), redBarDown.height(), REDBARDOWN_IMG_DATA);       
    }
    
    tft.loadFont(ArialBold16);
    tft.setCursor(460, 300);
    // round, don't truncate, the aoa and print to tft.
    tft.printf("%d  ", max(0, (int)round(_angleOfAttack)));

    aoaBackground.pushSprite(400 - AOABACKGROUND_WIDTH/2, 240 - AOABACKGROUND_HEIGHT/2);



    // _angleOfAttack = _angleOfAttack + 0.1;
    // if(_angleOfAttack > 15) _angleOfAttack = 0;

    return true;
}


bool CC172Tach::updateEngineHours()
{
    static double lastEngineHours = 0;
   // if (lastEngineHours != _curHours)
    {
        // Testing!
    //    _curHours += 0.1;
        
        char hours[18];
        char tenths[6];
        left_pad_float(_curHours, hours, tenths);
        lastEngineHours = _curHours;

        tachMaskSpr.setCursor(101 - TACHMASK_X_OFFSET, 207 - TACHMASK_Y_OFFSET);
        tachMaskSpr.setTextColor(TFT_LIGHTGREY, TFT_BLACK, true);
        tachMaskSpr.print(hours);
        tachMaskSpr.setTextColor(TFT_BLACK, TFT_TENTHS_BACKGROUND_GREY, true);
        tachMaskSpr.print(tenths);
    }
    return true;
}