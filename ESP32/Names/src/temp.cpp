// main.cpp
// ```
// #include <Puzzle.h>

// Puzzle puzzle;

// void setup()
// {
//     puzzle.LoadPuzzle();
// }

// void loop()
// {    
//     puzzle.Loop();
// }
// ```
// Puzzle.h
// ```
// #include <FastLED.h>
// class Puzzle 
// {
//     public:
//         void LoadPuzzle();       
//         void Loop();
//     private:
//         CRGB leds[58];
//         const int ledAmount = 58;        
// };
// ```
// Puzzle.cpp
// ```
// #include <Puzzle.h>

// void Puzzle::Loop()
// {
//     for (int i = 0; i < ledAmount; i++)
//     {
//         leds[i] = CRGB::White;
//         FastLED.show();
//         delay(100);
//         leds[i] = CRGB::Black;
//         FastLED.show();
//     }
// }

// void Puzzle::LoadPuzzle()
// {
//     FastLED.addLeds<WS2811, 4, RGB>(leds, ledAmount);
//     FastLED.setBrightness(128);
// }
// ```

