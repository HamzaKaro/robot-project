
#pragma once

#include "global.hpp"   // A global header with constants and enums
#include "pin.hpp"      // A pin on a port, or a row of pins

// MEMORY & CONSOLE
//#include "memoryAccess.hpp"
#include "debug.hpp"    // Contains functions to print to console
#include "uart.hpp"     // Allows UART communication with console

// DISPLAY
#include "display.hpp"  // Display screen of 0-F
//#include "led.hpp"      // A simple bidirectional LED
//#include "matrix.hpp"   // A LED matrix displaying LED rectangles

// USER INTERACTION & CAPTORS
#include "button.hpp"   // An interruption button with debounce
#include "sensor.hpp"      // An analog to digital converter

// MOBILITY
#include "motor.hpp"    // A simple two-wheeled motor


using namespace UART;


namespace Interface
{
    class Robot
    {
    public:
        struct Time
        {
            uint8_t m;
            uint8_t s;
            uint8_t cs;
        };

        enum State { STARTUP, DETECTION, EXECUTION };

        enum FrequencyMode { _1ps = 1, _2ps = 2, _4ps = 4 };     // 1, 2, 4
        enum DetectionMode
        {
            FREQUENCY,
            VALUE_CHANGE,
            STATUS_CHANGE
        };
        enum SensorMode { IN, OUT };
        enum DisplayMode { DECIMAL, HEXADECIMAL };

        FrequencyMode frequencyMode_    = _1ps;
        DetectionMode detectionMode_    = FREQUENCY;
        SensorMode sensorMode_          = IN;
        DisplayMode displayMode_        = DECIMAL;

    private:
        State state_;
        uint8_t maneuver_;
        uint16_t baudRate_;

        Time time_ = { 0, 0, 0 };
        Time oldTime_ = { 0, 0, 0 };
        float realTime_;

        Sensor sensor_ = Sensor(
            Sensor::I_CAN, Pin(A, 0),
            Pins(A, 2, 2), Pins(B, 2, 3)
        );
        Button button_ = Button(Pin(D, 2));
        Motor motor_ = Motor(Pin(D, 3), Pin(D, 6));
        Display dLeft_[2]
        {
            Display(
                A, 4,
                Pin(B, 5),
                Pin(B, 7)
            ),
            Display(
                A, 4,
                Pin(B, 6),
                Pin(B, 7)
            )
        };
        Display dRight_[2]
        {
            Display(
                A, 4,
                Pin(C, 0),
                Pin(D, 7)
            ),
            Display(
                A, 4,
                Pin(C, 1),
                Pin(D, 7)
            )
        };

        void setDisplay(uint16_t hexa)
        {
            setDisplay((hexa >> 8) & 0xFF, hexa & 0xFF);
        }


        uint8_t hexaToDecimal(uint8_t hexa)
        {
            uint8_t convertedHexa = 0;
            uint8_t decalage = 0;

            uint8_t subValue;
            while (hexa > 0)
            {
                subValue = hexa % 10;
                hexa /= 10;

                convertedHexa |= (subValue << decalage);
                decalage += 4;
            }

            return convertedHexa;
        }

        void setDisplay(uint8_t hexaL, uint8_t hexaR)
        {
            switch (displayMode_)
            {
                case DisplayMode::DECIMAL:
                    hexaL = hexaToDecimal(hexaL);
                    hexaR = hexaToDecimal(hexaR);
                    break;
                
                case DisplayMode::HEXADECIMAL:
                break;
            }
            
            dLeft_[0].set(0xF & (hexaL >> 4));
            dLeft_[1].set(0xF & (hexaL >> 0));
            dRight_[0].set(0xF & (hexaR >> 4));
            dRight_[1].set(0xF & (hexaR >> 0));

            
        }

        void setDisplayAndIntensity(int8_t hexaL, int8_t hexaR)
        {
            motor_.setIntensity(hexaL, hexaR);

            if (hexaL < 0)
                hexaL *= -1;
            if (hexaR < 0)
                hexaR *= -1;
            setDisplay(hexaL, hexaR);
        }

        void resetDisplay()
        {
            dLeft_[0].reset();
            dLeft_[1].reset();
            dRight_[0].reset();
            dRight_[1].reset();
        }

        void detect()
        {
            state_ = DETECTION;

            for (;;)
            {
                button_.update();

                bool distanceChanged = sensor_.updateDistance();
                bool statusChanged = sensor_.updateStatus();
                
                // Go to execute
                if (button_.getLowEdge())
                    break;

                // Depending on modes, print or not
                bool mustPrint = false;
                if (detectionMode_ == FREQUENCY)
                {
                    // If AT LEAST 1 second has passed, print
                    if (time_.m > oldTime_.m ||
                        time_.s > oldTime_.s + 1)
                        mustPrint = true;

                    // If 1ps and at least 1 second has passed (dcs >= 0), print
                    if (frequencyMode_ == _1ps)
                        if (time_.s == oldTime_.s + 1 &&
                            time_.cs >= oldTime_.cs)
                            mustPrint = true;

                    // If 2ps and at least 0.5 second has passed (either :)
                    //      A. 1 second has passed and dcs >= -0.5
                    //      B. 0 second has passed and dcs >= 0.5
                    if (frequencyMode_ == _2ps)
                        if ((time_.s == oldTime_.s + 1 && time_.cs - oldTime_.cs >= -50) ||
                            (time_.s == oldTime_.s && time_.cs - oldTime_.cs >= 50))
                            mustPrint = true;

                    // If 4ps and at least 0.25 second has passed (either :)
                    //      A. 1 second has passed and dcs >= -0.75
                    //      B. 0 second has passed and dcs >= 0.25
                    if (frequencyMode_ == _4ps)
                        if ((time_.s == oldTime_.s + 1 && time_.cs - oldTime_.cs >= -75) ||
                            (time_.s == oldTime_.s && time_.cs - oldTime_.cs >= 25))
                            mustPrint = true;

                    if (mustPrint)
                    {
                        // Update time
                        if (frequencyMode_ == _1ps)
                            oldTime_.cs += 100;
                        else if (frequencyMode_ == _2ps)
                            oldTime_.cs += 50;
                        else if (frequencyMode_ == _4ps)
                            oldTime_.cs += 25;

                        while (oldTime_.cs >= 100)
                        {
                            oldTime_.cs -= 100;
                            oldTime_.s += 1;
                        }
                        while (oldTime_.s >= 60)
                        {
                            oldTime_.s -= 60;
                            oldTime_.m += 1;
                        }
                    }
                }



                // If the print is according to status or value change
                if ((detectionMode_ == STATUS_CHANGE && statusChanged) ||
                    (detectionMode_ == VALUE_CHANGE && distanceChanged))
                {
                    mustPrint = true;

                    oldTime_.m = time_.m;
                    oldTime_.s = time_.s;
                    oldTime_.cs = time_.cs;
                }



                // Print data
                if (mustPrint)
                {
                    // Update oldTime
                    oldTime_.m = time_.m;
                    oldTime_.s = time_.s;
                    oldTime_.cs = time_.cs;

                    // Print time
                    if (time_.m < 10)
                        dPrint("0");
                    dPrint(time_.m); dPrint(":");
                    if (time_.s < 10)
                        dPrint("0");
                    dPrint(time_.s); dPrint(":");
                    if (time_.cs < 10)
                        dPrint("0");
                    dPrint(time_.cs);

                    dPrint(" - ");

                    // Print distances
                    dPrint("G:"); dPrint(sensor_.getDistLeft() / 10);
                    dPrint("."); dPrint(sensor_.getDistLeft() % 10); dPrint(" ");
                    dPrint("C:"); dPrint(sensor_.getDistCenter() / 10);
                    dPrint("."); dPrint(sensor_.getDistCenter() % 10); dPrint(" ");
                    dPrint("D:"); dPrint(sensor_.getDistRight() / 10);
                    dPrint("."); dPrint(sensor_.getDistRight() % 10); dPrint(" ");



                    Sensor::Status statusLeft = sensor_.getStatusLeft();
                    Sensor::Status statusCenter = sensor_.getStatusCenter();
                    Sensor::Status statusRight = sensor_.getStatusRight();

                    // Print status
                    switch (statusLeft)
                    {
                        case Sensor::NONE:
                            dPrint("NONE");
                            break;

                        case Sensor::OK:
                            dPrint("OK");
                            break;

                        case Sensor::WARNING:
                            dPrint("ATTENTION");
                            break;

                        case Sensor::DANGER:
                            dPrint("DANGER");
                            break;
                    }
                    dPrint(" | ");
                    
                    switch (statusCenter)
                    {
                        case Sensor::NONE:
                            dPrint("NONE");
                            break;

                        case Sensor::OK:
                            dPrint("OK");
                            break;

                        case Sensor::WARNING:
                            dPrint("ATTENTION");
                            break;

                        case Sensor::DANGER:
                            dPrint("DANGER");
                            break;
                    }
                    dPrint(" | ");

                    switch (statusRight)
                    {
                        case Sensor::NONE:
                            dPrint("NONE");
                            break;

                        case Sensor::OK:
                            dPrint("OK");
                            break;

                        case Sensor::WARNING:
                            dPrint("ATTENTION");
                            break;

                        case Sensor::DANGER:
                            dPrint("DANGER");
                            break;
                    }

                    dPrint(" ");



                    // Calculate maneuver
                    if (statusLeft == Sensor::OK &&
                        statusCenter == Sensor::WARNING &&
                        statusRight == Sensor::WARNING)
                        maneuver_ = 1;
                    else if (statusLeft == Sensor::WARNING &&
                        statusCenter == Sensor::WARNING &&
                        statusRight == Sensor::OK)
                        maneuver_ = 2;
                    else if (statusLeft == Sensor::DANGER &&
                        statusCenter == Sensor::DANGER &&
                        statusRight == Sensor::DANGER)
                        maneuver_ = 3;
                    else if (statusLeft == Sensor::OK &&
                        statusCenter == Sensor::OK &&
                        statusRight == Sensor::DANGER)
                        maneuver_ = 4;
                    else if (statusLeft == Sensor::DANGER &&
                        statusCenter == Sensor::OK &&
                        statusRight == Sensor::OK)
                        maneuver_ = 5;
                    else
                        maneuver_ = 0;

                    

                    // Print maneuver
                    dPrint("(");
                    if (maneuver_ == 0)
                        dPrint("-");
                    else
                        dPrint(maneuver_);
                    dPrint(")");



                    lineBreak();
                }
            }

            execute();
        };
        void execute()
        {
            state_ = EXECUTION;

            switch (maneuver_)
            {
                case 1:
                    dPrint("Manoeuvre 1");
                    setDisplayAndIntensity(-35,35);
                    _delay_ms(1500) ;
                    setDisplayAndIntensity(35,35);
                    _delay_ms(2000) ;
                    setDisplayAndIntensity(35,-35);
                    _delay_ms(1500) ;
                    setDisplayAndIntensity(35,35);
                    _delay_ms(125) ;
                    for(int i = 40; i < 96; i+=5)
                    {
                        setDisplayAndIntensity(i,i);
                        _delay_ms(125) ;
                    }
                    _delay_ms(2000) ;
                    break;

                case 2:
                    dPrint("Manoeuvre 2");
                    setDisplayAndIntensity(35,35);
                    _delay_ms(1500) ;
                    setDisplayAndIntensity(35,35);
                    _delay_ms(2000) ;
                    setDisplayAndIntensity(-35,35);
                    _delay_ms(1500) ;
                    setDisplayAndIntensity(35,35);
                    _delay_ms(125) ;
                    for(int i = 40; i < 96; i+=5)
                    {
                        setDisplayAndIntensity(i,i);
                        _delay_ms(125) ;
                    }
                    _delay_ms(2000) ;
                    break;

                case 3:
                    dPrint("Manoeuvre 3");
                    setDisplayAndIntensity(-50,50);
                    _delay_ms(1000) ;
                    setDisplayAndIntensity(-70,70);
                    _delay_ms(1500) ;
                    setDisplayAndIntensity(0,0);
                    _delay_ms(50) ;
                    for(int i = 3; i < 100; i+=3)
                    {
                        setDisplayAndIntensity(i,i);
                        _delay_ms(50) ;
                    }
                    for(int i = 94; i > 73; i-=5)
                    {
                        setDisplayAndIntensity(i,i);
                        _delay_ms(500) ;
                    }
                    _delay_ms(2000) ;
                    break;

                case 4:
                    dPrint("Manoeuvre 4");
                    setDisplayAndIntensity(78,78);
                    for(int i = 78; i > 47; i-=2)
                    {
                        setDisplayAndIntensity(i,78);
                        _delay_ms(250) ;
                    }
                    _delay_ms(1500) ;
                    for(int i = 50; i < 79; i+=2)
                    {
                        setDisplayAndIntensity(i,78);
                        _delay_ms(250) ;
                    }
                    _delay_ms(2000) ;
                    break;

                case 5:
                    dPrint("Manoeuvre 5");
                    setDisplayAndIntensity(78,78);
                    for(int i = 78; i > 47; i-=2)
                    {
                        setDisplayAndIntensity(78,i);
                        _delay_ms(250) ;
                    }
                    _delay_ms(1500) ;
                    for(int i = 50; i < 79; i+=2)
                    {
                        setDisplayAndIntensity(78,i);
                        _delay_ms(250) ;
                    }
                    _delay_ms(2000) ;
                    break;

                default:
                    dPrint("Combinaison non evaluee");
                    _delay_ms(2000); // Wait for 2 seconds
                    break;
            }

            // Turn the motor off
            setDisplayAndIntensity(0, 0);
            resetDisplay();

            detect();
        };


    public:
        Robot(uint16_t baudRate) :
            baudRate_(baudRate)
        {
            DDRA = 0xFD; // 11111101
            DDRB = 0xFD; // ___11101
            DDRC = 0xE0; // __000000
            DDRD = 0x30; // _11110xx

            // Initialize PORTC to enable interrupt
            PORTC = 0xE0;
        }

        



        // MODE CHANGE METHODS

        void setFrequencyMode(FrequencyMode mode)
        {
            frequencyMode_ = mode;
            dPrint("Frequence de detection : "); dPrint(mode); dPrint(" Hz\n");
        }
        void setDetectionMode(DetectionMode mode)
        {
            detectionMode_ = mode;
            dPrint("Mode de detection : ");
            switch (mode)
            {
                case FREQUENCY:
                    dPrint("En fonction de la frequence");
                    break;

                case VALUE_CHANGE:
                    dPrint("Sur changement de distance");
                    break;

                case STATUS_CHANGE:
                    dPrint("Sur changement de categorie");
                    break;
            }
            dPrint("\n");
        }
        void setConverterMode(Sensor::TypeCan mode)
        {
            sensor_.setTypeCan(mode);
            dPrint("Mode du convertisseur : ");
            switch (mode)
            {
                case Sensor::I_CAN:
                    dPrint("Interne");
                    break;

                case Sensor::E_CAN:
                    dPrint("Externe");
                    break;
            }
            dPrint("\n");
        }
        void toggleDisplayMode()
        {
            dPrint("Mode d'affichage : ");
            if (displayMode_ == DECIMAL)
            {
                displayMode_ = HEXADECIMAL;
                dPrint("Hexadecimal");
            }
            else if (displayMode_ == HEXADECIMAL)
            {
                displayMode_ = DECIMAL;
                dPrint("Decimal");
            }
            dPrint("\n");
        }





        // Change/get attributes
        
        void setRealTime(float value) { realTime_ = value; }

        float getRealTime() { return realTime_; }

        void setTime(uint8_t m, uint8_t s, uint8_t cs)
        {
            time_.m = m;
            time_.s = s;
            time_.cs = cs;
        }






        void startup()
        {
            state_ = STARTUP;

            // Set UART and baud rate
            switch (baudRate_)
            {
                case 9600:
                    initialisationUART(0x33, 0x1);
                    dPrint("9600 bps\n");
                    break;

                case 2400:
                default:
                    initialisationUART(0xCF, 0x1);
                    dPrint("2400 bps\n");
                    break;
            }
            


            // Print ABCD
            displayMode_ = HEXADECIMAL;
            setDisplay(0xABCD);
            _delay_ms(2000);
            resetDisplay();
            displayMode_ = DECIMAL;
            


            // Run wheels
            setDisplayAndIntensity(99, 99);
            _delay_ms(1000);
            setDisplayAndIntensity(-99, -99);
            _delay_ms(1000);
            setDisplayAndIntensity(0, 0);

            // Go to detection
            detect();
        }
    };










    Robot robot = Robot(9600);

    void startup()
    {
        robot.startup();
    }

    void setFrequencyMode(Robot::FrequencyMode mode)
    {
        robot.setFrequencyMode(mode);
    }

    void addTime(float dt)
    {
        float realTime = robot.getRealTime() + dt;

        // Convert real time in mm:ss:cs
        const uint16_t spm = 60;
        const uint16_t csps = 100;
        int int_time = (int)realTime;
        // Minutes
        uint8_t m = int_time / (csps * spm);
        // Seconds
        int_time = int_time - m * csps * spm; // Remove minutes
        uint8_t s = int_time / csps;
        // Centiseconds
        int_time = int_time - s * csps; // Remove seconds
        uint8_t cs = int_time / 1;

        // Set values
        robot.setRealTime(realTime);
        robot.setTime(m, s, cs);
    }

    void changeConfig(uint8_t result) // Result = C[7:2]
    {
        uint8_t col = 0;
        switch (result & 0b000111) // C4 C3 C2
        {
            case 1 << 2: // C4 : col 1
                col = 1;
                break;
            case 1 << 1: // C3 : col 2
                col = 2;
                break;
            case 1 << 0: // C2 : col 3
                col = 3;
                break;
        }

        switch (result & 0b111000) // C7 C6 C5
        {
            case 1 << 3: // C5 : row 1, 124
                if (col == 1)       // 1
                    robot.setFrequencyMode(Robot::_1ps);
                else if (col == 2)  // 2
                    robot.setFrequencyMode(Robot::_2ps);
                else if (col == 3)  // 4
                    robot.setFrequencyMode(Robot::_4ps);
                break;
            case 1 << 4: // C6 : row 2, RVC
                if (col == 1)       // R
                    robot.setDetectionMode(Robot::FREQUENCY);
                else if (col == 2)  // V
                    robot.setDetectionMode(Robot::VALUE_CHANGE);
                else if (col == 3)  // C
                    robot.setDetectionMode(Robot::STATUS_CHANGE);
                break;
            case 1 << 5: // C7 : row 3, IE or #
                if (col == 1)       // I
                    robot.setConverterMode(Sensor::I_CAN);
                else if (col == 2)  // E
                    robot.setConverterMode(Sensor::E_CAN);

                else if (col == 3)  // #
                    robot.toggleDisplayMode();
                    
                break;
        }
    }
}


