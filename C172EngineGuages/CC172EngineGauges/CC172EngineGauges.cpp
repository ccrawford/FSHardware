#include "CC172EngineGauges.h"
#include "allocateMem.h"
#include "commandmessenger.h"

#include <algorithm>
#include <math.h>

#include "Sprites/Bezel.h"

#include "Sprites/FuelBase.h"
#include "Sprites/FuelLeftMask.h"
#include "Sprites/FuelRightMask.h"

#include "Sprites/EGTFFBase.h"
#include "Sprites/EGTLeftMask.h"
#include "Sprites/EGTRightMask.h"
#include "Sprites/EGTPointer.h"
#include "Sprites/EGTBug.h"

#include "Sprites/OilBase.h"
#include "Sprites/OilLeftMask.h"
#include "Sprites/OilRightMask.h"

#include "Sprites/VacAmpBase.h"
#include "Sprites/VacAmpLeftMask.h"
#include "Sprites/VacAmpRightMask.h"

#define GAUGE_BACKGROUND_COLOR TFT_BLACK
/* **********************************************************************************
    This is just the basic code to set up your custom device.
    Change/add your code as needed.
********************************************************************************** */

void drawFuel();
void drawEGTFF();
void drawOil();
void drawVacAmp();

double calculateAngle(
    double input,           // The input value
    double inputRangeStart, // Start of linear mapping range (e.g., 3)
    double inputRangeEnd,   // End of linear mapping range (e.g., 28)
    double outputRangeStart,// Start of output range for linear mapping (e.g., 63)
    double outputRangeEnd   // End of output range for linear mapping (e.g., -136)
);

double sigmoid(float x, float L, float k, float x0, float b);

CC172EngineGauges::CC172EngineGauges(uint8_t Pin1, uint8_t Pin2)
{
}

LGFX   lcd = LGFX();

LGFX_Sprite canvas(&lcd);

static LGFX_Sprite bezelSprite(&canvas);

static LGFX_Sprite fuelBase(&canvas);  
static LGFX_Sprite fuelLeftMask(&canvas);  
static LGFX_Sprite fuelRightMask(&canvas);  


// static LGFX_Sprite pointerSprite(&canvas);


static LGFX_Sprite egtFfBaseSprite(&canvas);   
static LGFX_Sprite egtLeftMask(&canvas);
static LGFX_Sprite egtRightMask(&canvas);
static LGFX_Sprite egtPointer(&canvas);
static LGFX_Sprite egtBug(&canvas);


static LGFX_Sprite oilBaseSprite(&canvas);   
static LGFX_Sprite oilLeftMask(&canvas);
static LGFX_Sprite oilRightMask(&canvas);

static LGFX_Sprite vacAmpBase(&canvas);
static LGFX_Sprite vacAmpLeftMask(&canvas);
static LGFX_Sprite vacAmpRightMask(&canvas);


bool isMobiRunning = false;
double _leftFuel = 0.0;
double _rightFuel = 0.0;
double _egt = 0.0;
double _egtBug = 0.0;
double _ff = 0.0;
double _oilTemp = 0.0;
double _oilPress = 0.0;
double _vacuum = 0.0;
double _amps = 0.0;


void CC172EngineGauges::begin()
{
    // cmdMessenger.sendCmd(kDebug, F("Starting CC172EngineGauges"));

    lcd.init();
    lcd.setRotation(2);
    lcd.fillScreen(GAUGE_BACKGROUND_COLOR);
//     lcd.setBrightness(0);  // This doesn't work :-( I'm not sure if we can turn off the backlight or control brightness.

    lcd.setColorDepth(16);
    lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    lcd.setTextSize(10);

    canvas.createSprite(228, 228);

    bezelSprite.setBuffer(const_cast<std::uint16_t*>(BEZEL_IMG_DATA), BEZEL_IMG_WIDTH, BEZEL_IMG_HEIGHT, 16);

    fuelBase.setBuffer(const_cast<std::uint16_t*>(FUELBASE_IMG_DATA), FUELBASE_IMG_WIDTH, FUELBASE_IMG_HEIGHT, 16);
    fuelLeftMask.setBuffer(const_cast<std::uint16_t*>(FUELLEFTMASK_IMG_DATA), FUELLEFTMASK_IMG_WIDTH, FUELLEFTMASK_IMG_HEIGHT, 16);
    fuelRightMask.setBuffer(const_cast<std::uint16_t*>(FUELRIGHTMASK_IMG_DATA), FUELRIGHTMASK_IMG_WIDTH, FUELRIGHTMASK_IMG_HEIGHT, 16);

    egtFfBaseSprite.setBuffer (const_cast<std::uint16_t*>(EGTFFBASE_IMG_DATA), EGTFFBASE_IMG_WIDTH, EGTFFBASE_IMG_HEIGHT, 16);
    egtLeftMask.setBuffer    (const_cast<std::uint16_t*>(EGTLEFTMASK_IMG_DATA), EGTLEFTMASK_IMG_WIDTH, EGTLEFTMASK_IMG_HEIGHT, 16);
    egtRightMask.setBuffer   (const_cast<std::uint16_t*>(EGTRIGHTMASK_IMG_DATA), EGTRIGHTMASK_IMG_WIDTH, EGTRIGHTMASK_IMG_HEIGHT, 16);
    egtPointer.setBuffer     (const_cast<std::uint16_t*>(EGTPOINTER_IMG_DATA), EGTPOINTER_IMG_WIDTH, EGTPOINTER_IMG_HEIGHT, 16);
    egtPointer.setPivot(6, EGTPOINTER_IMG_HEIGHT/2);
    egtBug.setBuffer         (const_cast<std::uint16_t*>(EGTBUG_IMG_DATA), EGTBUG_IMG_WIDTH, EGTBUG_IMG_HEIGHT, 16);
    egtBug.setPivot(0, EGTBUG_IMG_HEIGHT/2);

    oilBaseSprite.setBuffer  (const_cast<std::uint16_t*>(OILBASE_IMG_DATA), OILBASE_IMG_WIDTH, OILBASE_IMG_HEIGHT, 16);
    oilLeftMask.setBuffer    (const_cast<std::uint16_t*>(OILLEFTMASK_IMG_DATA), OILLEFTMASK_IMG_WIDTH, OILLEFTMASK_IMG_HEIGHT, 16);
    oilRightMask.setBuffer   (const_cast<std::uint16_t*>(OILRIGHTMASK_IMG_DATA), OILRIGHTMASK_IMG_WIDTH, OILRIGHTMASK_IMG_HEIGHT, 16);
    
    vacAmpBase.setBuffer     (const_cast<std::uint16_t*>(VACAMPBASE_IMG_DATA), VACAMPBASE_IMG_WIDTH, VACAMPBASE_IMG_HEIGHT, 16);
    vacAmpLeftMask.setBuffer (const_cast<std::uint16_t*>(VACAMPLEFTMASK_IMG_DATA), VACAMPLEFTMASK_IMG_WIDTH, VACAMPLEFTMASK_IMG_HEIGHT, 16);
    vacAmpRightMask.setBuffer(const_cast<std::uint16_t*>(VACAMPRIGHTMASK_IMG_DATA), VACAMPRIGHTMASK_IMG_WIDTH, VACAMPRIGHTMASK_IMG_HEIGHT, 16);

}

bool CC172EngineGauges::isValidNumber(const char *str)
{    // Check if the string is a valid number
    if (*str == '-' || *str == '+') str++; // skip sign
    bool hasDecimal = false;
    while (*str) {
        if (*str == '.') {
            if (hasDecimal) return false; // more than one decimal point
            hasDecimal = true;
        } else if (!isdigit(*str)) {
            return false; // non-digit character
        }
        str++;
    }
    return true;
}

void CC172EngineGauges::attach(uint16_t Pin3, char *init)
{
}

void CC172EngineGauges::detach()
{
    if (!_initialised)
        return;
    _initialised = false;
}

void CC172EngineGauges::set(int16_t messageID, char *setPoint)
{
    /* **********************************************************************************
        Each messageID has its own value
        check for the messageID and define what to do.
        Important Remark!
        MessageID == -2 will be send from the board when PowerSavingMode is set
            Message will be "0" for leaving and "1" for entering PowerSavingMode
        MessageID == -1 will be send from the connector when Connector stops running
        Put in your code to enter this mode (e.g. clear a display)

    ********************************************************************************** */
    // cmdMessenger.sendCmd(kStatus, F("Got a message in CC172EngineGauges"));
    isMobiRunning = true;
    // lcd.powerSaveOff();
    // do something according your messageID
    switch (messageID) {
    case -1:
        isMobiRunning = false;
        // tbd., get's called when Mobiflight shuts down
        // lcd.setBrightness(50);
        // lcd.drawString("MobiFlight stopped", 0, 0, 1);           
        lcd.powerSaveOn();
        break;
    case -2:
        isMobiRunning = false;
        // lcd.setBrightness(50);
        // lcd.drawString("PowerSavingMode", 0, 100, 1);
        // tbd., get's called when PowerSavingMode is entered
        lcd.powerSaveOn();
        break;
    case 0:
        // Check to see if setPoint is '' or NULL
        if (setPoint != nullptr && strlen(setPoint)> 0 && isValidNumber(setPoint)) {
            _leftFuel = atof(setPoint);
        } 
        break;
    case 1:
        if (setPoint != nullptr && strlen(setPoint)> 0 && isValidNumber(setPoint)) {
            _rightFuel = atof(setPoint);
        } 
        break;
    case 2:
        if (setPoint != nullptr && strlen(setPoint)> 0 && isValidNumber(setPoint)) {
            _egt = atof(setPoint);
        } 
        break;
    case 3:
        if (setPoint != nullptr && strlen(setPoint)> 0 && isValidNumber(setPoint)) {
            _egtBug = atof(setPoint);
        } 
        break;
    case 4:
        if (setPoint != nullptr && strlen(setPoint)> 0 && isValidNumber(setPoint)) {
            _ff = atof(setPoint);
        } 
        break;
    case 5:
        if (setPoint != nullptr && strlen(setPoint)> 0 && isValidNumber(setPoint)) {
            _oilTemp = atof(setPoint);
        } 
        break;
    case 6:
        if (setPoint != nullptr && strlen(setPoint)> 0 && isValidNumber(setPoint)) {
            _oilPress = atof(setPoint);
        } 
        break;
    case 7:
        if (setPoint != nullptr && strlen(setPoint)> 0 && isValidNumber(setPoint)) {
            _vacuum = atof(setPoint);
        } 
        break;
    case 8:
        if (setPoint != nullptr && strlen(setPoint)> 0 && isValidNumber(setPoint)) {
            _amps = atof(setPoint);
        } 
        break;
    default:
        break;
    }
}

void CC172EngineGauges::update()
{
// I don't think the Back light on this thing can be controlled. 

//    if(!isMobiRunning) {
//         lcd.powerSaveOn();
//         return;
//     }

    drawFuel();
    drawEGTFF();
    drawOil();
    drawVacAmp();
}



void drawFuel()
{
   // Display in top left corner
    int top_x = 0 + ((lcd.width()/2 - BEZEL_IMG_WIDTH)/2);
    int top_y = 0 + ((lcd.height()/2 - BEZEL_IMG_HEIGHT)/2);
    double upperBound = -136.0;
    double lowerBound = 63.0;
    float unusable_fuel = 1.5;
    // 1/26 = 0.0385 which is the range of values. less than 2 is 0. It really goes from 0 to 28.
//    float leftFuelAngle = std::min(std::max(lowerBound + ((_leftFuel-2) * 0.0385 * upperBound), 0.0), upperBound);
 //   float leftFuelAngle = std::min(std::max(lowerBound + ((_leftFuel-2) * 0.0385 * upperBound), lowerBound), upperBound);
 //   float rightFuelAngle = std::max(std::min((180-lowerBound) - ((_rightFuel-2) * 0.0385* upperBound), 180.0 - lowerBound), 180.0 - upperBound);

    // Fuel has 2 unusable gallons.

    double leftFuelAngle =  calculateAngle(_leftFuel  - unusable_fuel, 0.0, 26.0, 73.0,  -73.0);
    double rightFuelAngle = calculateAngle(_rightFuel - unusable_fuel, 0.0, 26.0, 107.0, 253.0);
    
    // Helps with testing ranges.
    //leftFuelAngle = _leftFuel;
    //rightFuelAngle = _rightFuel;
    // Clear Canvas
    canvas.fillSprite(GAUGE_BACKGROUND_COLOR);

    // Draw bezel centered on canvas coordinates
    bezelSprite.pushSprite(&canvas, 0, 0, TFT_BLACK);
    // Draw Fuel background centered
    fuelBase.pushSprite(&canvas, (BEZEL_IMG_WIDTH - FUELBASE_IMG_WIDTH)/2, (BEZEL_IMG_HEIGHT - FUELBASE_IMG_HEIGHT)/2, TFT_BLACK);
    // Draw Left Fuel pointer
    canvas.setPivot(48, canvas.height()/2);
    egtPointer.pushRotated(&canvas, leftFuelAngle, TFT_BLACK);
    
    // Draw Right pointer
    canvas.setPivot(180, canvas.height()/2);
    egtPointer.pushRotated(&canvas, rightFuelAngle, TFT_BLACK);


    // Draw left and right covers
    fuelLeftMask.pushSprite((BEZEL_IMG_WIDTH - FUELBASE_IMG_WIDTH)/2 - FUELLEFTMASK_IMG_WIDTH, (BEZEL_IMG_HEIGHT - FUELLEFTMASK_IMG_HEIGHT)/2, TFT_BLACK);
    fuelRightMask.pushSprite(&canvas, (BEZEL_IMG_WIDTH - FUELBASE_IMG_WIDTH)/2 + FUELBASE_IMG_WIDTH, (BEZEL_IMG_HEIGHT - FUELRIGHTMASK_IMG_HEIGHT)/2, TFT_BLACK);
    // Push to LCD
    canvas.pushSprite(&lcd, top_x, top_y);
}

double sigmoid(float x, float L, float k, float x0, float b)
{
    return L / (1 + exp(-k*(x-x0))) + b;
}

// The oil temp gauge is non-linear. This seems to work.
double oilTempToAngle(double oilTemp)
{
    return sigmoid(oilTemp, 133.14, -0.03752, 174.275, -69.37);
}

void drawOil()
{
   // Display in bottom left corner
    int top_x = 0 + ((lcd.width()/2 - BEZEL_IMG_WIDTH)/2);
    int top_y = 240 + ((lcd.height()/2 - BEZEL_IMG_HEIGHT)/2);
    // oil temp goes from 75 to 245, or 0 to 170
    //double oilTempAngle = std::min(std::max(63.0 - ((_oilTemp - 75.0) * 0.00385 * 136.0), 63.0), 199.0);
    // min 115 max 243
    //double oilPressAngle = 115.0 + ((_oilPress) * 0.0087 * 243);
    
    float oilTempAngle = oilTempToAngle(_oilTemp);
    //  double oilTempAngle = calculateAngle(_oilTemp, 75.0, 60.0, 75.0, 245.0, 60, -60);  // Gauge is not linear.
    float oilPressAngle = calculateAngle(_oilPress, 0.0, 115.0, 118.0, 250.0);

    // Clear Canvas
    canvas.fillSprite(GAUGE_BACKGROUND_COLOR);

    // Draw bezel centered on canvas coordinates
    bezelSprite.pushSprite(&canvas, 0, 0, TFT_BLACK);
    // Draw background centered
    oilBaseSprite.pushSprite(&canvas, (BEZEL_IMG_WIDTH - OILBASE_IMG_WIDTH)/2, (BEZEL_IMG_HEIGHT - OILBASE_IMG_HEIGHT)/2, TFT_BLACK);
    // Draw Left pointer
    canvas.setPivot(48, canvas.height()/2);
    egtPointer.pushRotated(&canvas, oilTempAngle, TFT_BLACK);
    
    // Draw Right pointer
    canvas.setPivot(180, canvas.height()/2);
    egtPointer.pushRotated(&canvas, oilPressAngle, TFT_BLACK);


    // Draw left and right covers
    oilLeftMask.pushSprite((BEZEL_IMG_WIDTH - FUELBASE_IMG_WIDTH)/2 - FUELLEFTMASK_IMG_WIDTH, (BEZEL_IMG_HEIGHT - FUELLEFTMASK_IMG_HEIGHT)/2, TFT_BLACK);
    oilRightMask.pushSprite(&canvas, (BEZEL_IMG_WIDTH - FUELBASE_IMG_WIDTH)/2 + FUELBASE_IMG_WIDTH, (BEZEL_IMG_HEIGHT - FUELRIGHTMASK_IMG_HEIGHT)/2, TFT_BLACK);
    // Push to LCD
    canvas.pushSprite(&lcd, top_x, top_y);
}

void drawVacAmp()
{
   // Display in bottom right corner
    int top_x = 240 + ((lcd.width()/2 - BEZEL_IMG_WIDTH)/2);
    int top_y = 240 + ((lcd.height()/2 - BEZEL_IMG_HEIGHT)/2);
    // float vacAngle = 65 - ((_vacuum) * 0.14 * 150);
    // float ampAngle = 115 + ((_amps+60) * 0.0083 * 150);
    
    //double vacAngle = _vacuum;
//    double ampAngle = _amps;

    double vacAngle = calculateAngle(_vacuum, 3.0, 7.0, 60, -60);
    double ampAngle = calculateAngle(_amps, -60.0, 60.0, 127.0, 244.0);

    // Clear Canvas
    canvas.fillSprite(GAUGE_BACKGROUND_COLOR);

    // Draw bezel centered on canvas coordinates
    bezelSprite.pushSprite(&canvas, 0, 0, TFT_BLACK);
    // Draw  background centered
    vacAmpBase.pushSprite(&canvas, (BEZEL_IMG_WIDTH - VACAMPBASE_IMG_WIDTH)/2, (BEZEL_IMG_HEIGHT - VACAMPBASE_IMG_HEIGHT)/2, TFT_BLACK);
    // Draw Left  pointer
    canvas.setPivot(48, canvas.height()/2);
    egtPointer.pushRotated(&canvas, vacAngle, TFT_BLACK);
    
    // Draw Right pointer
    canvas.setPivot(180, canvas.height()/2);
    egtPointer.pushRotated(&canvas, ampAngle, TFT_BLACK);


    // Draw left and right covers
    vacAmpLeftMask.pushSprite((BEZEL_IMG_WIDTH - VACAMPBASE_IMG_WIDTH)/2 - VACAMPLEFTMASK_IMG_WIDTH, (BEZEL_IMG_HEIGHT - VACAMPLEFTMASK_IMG_HEIGHT)/2, TFT_BLACK);
    vacAmpRightMask.pushSprite(&canvas, (BEZEL_IMG_WIDTH - VACAMPBASE_IMG_WIDTH)/2 + VACAMPBASE_IMG_WIDTH, (BEZEL_IMG_HEIGHT - VACAMPRIGHTMASK_IMG_HEIGHT)/2, TFT_BLACK);
    // Push to LCD
    canvas.pushSprite(&lcd, top_x, top_y);
}

float ffToAngle(float ff)
{
    return 0.3532*ff*ff - 0.0144*ff + 116.37;
}

void drawEGTFF()
{
    // Display in top right corner
    int top_x = lcd.width()/2 + ((lcd.width()/2 - BEZEL_IMG_WIDTH)/2);
    int top_y = 0 + ((lcd.height()/2 - BEZEL_IMG_HEIGHT)/2);
    double upperBound = -136.0;
    double lowerBound = 63.0;
    // double egtAngle = 63 - (_egt * 0.000625 * 150);
    double bugAngle = calculateAngle(_egtBug, 0.0, 1.0, 62.0, -62.0);
    double egtAngle = calculateAngle(_egt, 1330.0, 1730.0, 62.0, -58.0);
    double ffAngle = ffToAngle(_ff);

//     double ffAngle = _ff;
//    double egtAngle = _egt;
//    double bugAngle = _egtBug;

    // Clear Canvas
    canvas.fillSprite(GAUGE_BACKGROUND_COLOR);

    // Draw bezel centered on canvas coordinates
    bezelSprite.pushSprite(&canvas, 0, 0, TFT_BLACK);
    // Draw EGT/FF background centered
    egtFfBaseSprite.pushSprite(&canvas, (BEZEL_IMG_WIDTH - EGTFFBASE_IMG_WIDTH)/2, (BEZEL_IMG_HEIGHT - EGTFFBASE_IMG_HEIGHT)/2, TFT_BLACK);
    // Draw EGT pointer
    canvas.setPivot(48, 108);
    egtPointer.pushRotated(&canvas, egtAngle, TFT_BLACK);
    // Draw EGT bug
    egtBug.pushRotated(&canvas, bugAngle, TFT_BLACK);

    // Draw FF pointer
    canvas.setPivot(180, 108);
    egtPointer.pushRotated(&canvas, ffAngle, TFT_BLACK);


    // Draw left and right covers
    egtLeftMask.pushSprite((BEZEL_IMG_WIDTH - EGTFFBASE_IMG_WIDTH)/2 - EGTLEFTMASK_IMG_WIDTH, (BEZEL_IMG_HEIGHT - EGTLEFTMASK_IMG_HEIGHT)/2, TFT_BLACK);
    egtRightMask.pushSprite(&canvas, (BEZEL_IMG_WIDTH - EGTFFBASE_IMG_WIDTH)/2 + EGTFFBASE_IMG_WIDTH, (BEZEL_IMG_HEIGHT - EGTLEFTMASK_IMG_HEIGHT)/2, TFT_BLACK);
    // Push to LCD
    canvas.pushSprite(&lcd, top_x, top_y);
}

double calculateAngle(
    double input,
    double inputRangeStart,
    double inputRangeEnd,
    double outputRangeStart,
    double outputRangeEnd
) {
    // Calculate slope and intercept
    double slope = (outputRangeEnd - outputRangeStart) / (inputRangeEnd - inputRangeStart);
    double intercept = outputRangeStart - slope * inputRangeStart;

    // Calculate linear output
    double output = slope * input + intercept;

    // Clamp the output based on the slope direction
    if (slope > 0) {
        // Positive slope: clamp between [outputRangeStart, outputRangeEnd]
        output = std::max(outputRangeStart, std::min(outputRangeEnd, output));
    } else {
        // Negative slope: clamp between [outputRangeEnd, outputRangeStart]
        output = std::max(outputRangeEnd, std::min(outputRangeStart, output));
    }

    return output;
}

