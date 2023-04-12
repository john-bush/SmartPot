#include "interface.h"

void Interface::Init() {
    Display.InitDisplay();
    Display.ClearDisplay();
    Display.DisplayOn();
    Display.MoveTo(0, 0);
}

void Interface::LoadingScreen() {
    state = 0;
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
    state = 1;
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
    state = 1;
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
    state = 2;
    Display.FillScreen(Display.Green);
    Display.PlotTextCentered(PSTR("Select Plant Type"), Display.xsize>>1, Display.ysize*0.8, 3, true);
    
    UpdatePlantSelectionScreen();

    Display.DrawImage(push, 20, 20, Display.xsize * 0.1, Display.ysize * 0.1);
    Display.PlotTextCentered(PSTR("Select"), Display.xsize * 0.2, Display.ysize * 0.1, 1);
    Display.DrawImage(scroll, 15, 15, Display.xsize * 0.6, Display.ysize * 0.1);
    Display.PlotTextCentered(PSTR("Scroll"), Display.xsize * 0.7, Display.ysize * 0.1, 1);
}

void Interface::UpdatePlantSelectionScreen() {
    state = 2;
    int offset = (currPlantHover > 2) ? 1 : 0;
    for (int i = 0; i < 3; i++) {
        int x = 20 + (2 * i + 1) * (Display.xsize-40)/6;
        int image_y = Display.ysize*0.6;
        int index = i + offset % 4;
        
        if (currPlantHover == index) {
            Display.SetForeColor(252, 227, 3);
            Display.DrawCenteredRect(62, 62, x, image_y);
            Display.DrawCenteredRect(63, 63, x, image_y);
        } else {
            Display.SetForeColor(Display.Black);
            Display.DrawCenteredRect(62, 62, x, image_y);
        }
        
        Display.DrawImage(plantPtr[index], 30, 30, x, image_y, 2);

        // write plant name under image
        Display.PlotTextCentered(plantNames[index], x, Display.ysize*0.35, 2);
    }

    if (offset == 1) {
        Display.DrawImage(left_arrow, 25, 25, Display.xsize * 0.9, Display.ysize>>1, true);
    } else {
        Display.DrawImage(left_arrow, 25, 25, Display.xsize * 0.1, Display.ysize>>1);
    }
}

void Interface::DrawPlantConfirmationScreen() {
    state = 3;
    Display.FillScreen(Display.Green);
    Display.PlotTextCentered(PSTR("Confirm Plant Type"), Display.xsize>>1, Display.ysize*0.8, 3, true);
    Display.DrawImage(plantPtr[currPlantHover], 30, 30, Display.xsize>>1, Display.ysize*0.6, 2);
    Display.PlotTextCentered(plantNames[currPlantHover], Display.xsize>>1, Display.ysize*0.35, 2);
    
    Display.DrawImage(left_arrow, 25, 25, Display.xsize * 0.2, Display.ysize*0.2);
    Display.PlotTextCentered(PSTR("return"), Display.xsize*0.3, Display.ysize*0.2, 1);

    Display.DrawImage(left_arrow, 25, 25, Display.xsize * 0.8, Display.ysize*0.2, true);
    Display.PlotTextCentered(PSTR("confirm"), Display.xsize * 0.7, Display.ysize*0.2, 1);

    UpdatePlantConfirmationScreen();
}

void Interface::UpdatePlantConfirmationScreen() {
    if (confirmationButton == 0) {
        Display.DrawTo(Display.xsize * 0.2, Display.ysize*0.1, Display.xsize * 0.3, Display.ysize*0.1);
    } else {
        Display.DrawTo(Display.xsize * 0.7, Display.ysize*0.1, Display.xsize * 0.8, Display.ysize*0.1);
    }
}

void Interface::DrawPlantDashboard() {
    state = 4;
    Display.FillScreen(Display.Green);
    Display.PlotTextCentered(PSTR("Your Plant Dashboard"), Display.xsize>>1, Display.ysize*0.8, 3, true);
    Display.DrawImage(plantPtr[currPlantHover], 30, 30, Display.xsize>>1, Display.ysize*0.6);
    Display.PlotTextCentered(plantNames[currPlantHover], Display.xsize>>1, Display.ysize*0.35, 2);
    
    Display.DrawImage(left_arrow, 25, 25, Display.xsize * 0.2, Display.ysize*0.2);
    Display.PlotTextCentered(PSTR("return"), Display.xsize*0.3, Display.ysize*0.2, 1);

    Display.DrawImage(left_arrow, 25, 25, Display.xsize * 0.8, Display.ysize*0.2, true);
    Display.PlotTextCentered(PSTR("confirm"), Display.xsize * 0.7, Display.ysize*0.2, 1);

    UpdatePlantDashboard();
}

void Interface::UpdatePlantDashboard() {

}

void Interface::ScrollForward() {
    switch (state) {
        case 2:
            currPlantHover = (currPlantHover + 1) % 4;
            UpdatePlantSelectionScreen();
            break;
        case 3:
            confirmationButton = (confirmationButton + 1) % 2;
            UpdatePlantConfirmationScreen();
            break;
    }
}

void Interface::ScrollBackward() {

}

void Interface::ButtonPress() {

}