#include <Arduino.h>

#include "tft_display_lib.h"

class Interface : public TFTDisplay
{
    public:
        TFTDisplay Display = TFTDisplay();

        void Init();

        // STATE 0 : starting state upon power up
        void LoadingScreen();

        // STATE 1 : draws the tank filling screen base
        void DrawTankScreen();
        void UpdateTank(int tankNum, int full);

        // STATE 2 : draws the plant selection screen base
        void DrawPlantSelectionScreen();
        void UpdatePlantSelectionScreen();

        // STATE 3 : draws the plant confirmation screen base
        void DrawPlantConfirmationScreen();
        void UpdatePlantConfirmationScreen();

        // STATE 4: draws the plant dashboard screen
        void DrawPlantDashboard();
        void UpdatePlantDashboard();

        void ScrollForward();
        void ScrollBackward();
        void ButtonPress();

        void PollSensors();

        void SetTank(int tank1, int tank2);
    
    private:
        int state = 0; // general state variable
        int numPlants = 4; // number of plant presets loaded in the system
        
        int confirmationButton = 0; // 0 highlights "back", 1 highlights "confirm"

        const int tank_width = 40;
        const int tank_height = 40;

        const int plant_icon_dim = 60;

        int plantSelection = 0; // 0 is first plant, 1 is second plant, etc.
        int currPlantHover = 0; // indicating the plant that the user is hovering on currently
        int scrollOffset = 0; // the scrolling offset of the plant selection screen. 0 is no offset, 1 is one plant offset, etc.
        int prevPlantHover = 0;

        bool configured = false; // true if the user has configured the system

        char* plantNames[4];

        // Sensor variables
        float temperature_ = 0.0f;
        float light_ = 0.0f;
        float humidity_ = 0.0f;

        int tankLevel1 = 0; // 0 is empty, 1 is full
        int tankLevel2 = 0; // 0 is empty, 1 is full
        

        
};
