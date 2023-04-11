#include "interface.h"

void Interface::Init() {
    Display.InitDisplay();
    Display.ClearDisplay();
    Display.DisplayOn();
    Display.MoveTo(0, 0);
}

void Interface::LoadingScreen() {
    Display.FillScreen(Display.Green);
    Display.SetTextColor(Display.Black);
    Display.SetBackColor(Display.Green);
    Display.DrawImage(plant_medium, 36, 58, Display.center, Display.ysize*0.77);
    Display.PlotTextCentered(PSTR("SmartPOT"), Display.xsize>>1, Display.ysize*0.45, 3);
    Display.PlotTextCentered(PSTR("smarter plant care"), Display.xsize>>1, Display.ysize*0.3, 2);
    delay(2000); // 2 second delay
    Display.PlotTextCentered(PSTR("[press button to continue]"), Display.xsize>>1, Display.ysize*0.1, 1);
}

void Interface::DrawTankScreen() {
    Display.FillScreen(Display.Green);
    Display.PlotTextCentered(PSTR("Fill Tanks"), Display.xsize>>1, Display.ysize*0.8, 3, true);
    Display.PlotTextCentered(PSTR("Water"), Display.xsize*0.33, Display.ysize*0.66, 2);
    Display.PlotTextCentered(PSTR("Fertilizer"), Display.xsize*0.67, Display.ysize*0.66, 2);
    
    // water tank
    Display.FillRect(tank_width, tank_height, 85, 50);
    Display.SetForeColor(Display.Green);
    Display.FillRect(tank_width - 6, tank_height, 88, 53);
    Display.PlotTextCentered(PSTR("[ empty ]"), Display.xsize*0.33, 35, 1);

    // fertilizer tank
    Display.FillRect(tank_width, tank_height, 195, 50);
    Display.SetForeColor(Display.Green);
    Display.FillRect(tank_width - 6, tank_height, 198, 53);
    Display.PlotTextCentered(PSTR("[ empty ]"), Display.xsize*0.67, 35, 1);

    Display.PlotTextCentered(PSTR("[Fill Tanks To Continue]"), Display.xsize>>1, Display.ysize*0.1, 1);
}

void Interface::UpdateTank(int tankNum, int full) {
    int x = 0;
    if (tankNum == 0) {
        x = Display.xsize * 0.33 - (tank_width>>1);
    } else {
        x = Display.xsize * 0.67 - (tank_width>>1);
    }
    if (full) {
        Display.SetForeColor(50, 153, 168);
        Display.FillRect(tank_width - 6, tank_height - 15, x, 53);
        Display.PlotTextCentered(PSTR("[ full  ]"), x, 35, 1);
    } else {
        Display.SetForeColor(Display.Green);
        Display.FillRect(tank_width - 6, tank_height, x, 53);
        Display.PlotTextCentered(PSTR("[ empty ]"), x, 35, 1);
    }
}

void Interface::DrawPlantSelectionScreen() {
    
}

void Interface::UpdatePlantSelectionScreen() {

}

void Interface::DrawPlantConfirmationScreen() {

}

void Interface::UpdatePlantConfirmationScreen() {

}

void Interface::DrawPlantDashboard() {

}

void Interface::ScrollForward() {

}

void Interface::ScrollBackward() {

}

void Interface::ButtonPress() {

}