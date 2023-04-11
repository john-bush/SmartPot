#include <Arduino.h>

#include "tft_display_lib.h"

class Interface : public TFTDisplay
{
    public:
        TFTDisplay Display = TFTDisplay();

        void Init();
        void LoadingScreen();

        // draws the tank filling screen base
        void DrawTankScreen();
        void UpdateTank(int tankNum, int full);

        void DrawPlantSelectionScreen();
        void UpdatePlantSelectionScreen();

        void DrawPlantConfirmationScreen();
        void UpdatePlantConfirmationScreen();

        void DrawPlantDashboard();

        void ScrollForward();
        void ScrollBackward();
        void ButtonPress();
    
    private:
        int state = 0; // general state variable
        int numPlants = 4; // number of plant presets loaded in the system
        int tankLevel1 = 0; // 0 is empty, 1 is full
        int tankLevel2 = 0; // 0 is empty, 1 is full

        int confirmationButton = 0; // 0 highlights "back", 1 highlights "confirm"

        const int tank_width = 40;
        const int tank_height = 40;

        const int plant_icon_dim = 60;

        int plantSelection = 0; // 0 is first plant, 1 is second plant, etc.
        int currPlantHover = 0; // indicating the plant that the user is hovering on currently


        
};
