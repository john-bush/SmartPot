#include "interface.h"

void Interface::Init() {
    Display.InitDisplay();
    Display.ClearDisplay();
    Display.DisplayOn();
    Display.MoveTo(0, 0);
    plantNames[0] = " Succulent  ";
    plantNames[1] = "Leafy Plant ";
    plantNames[2] = "  Cactus    ";
    plantNames[3] = "  Flowers   ";

}

void Interface::LoadingScreen() {
    state = 0;
    Display.FillScreen(Display.Green);
    Display.SetTextColor(Display.Black);
    Display.SetBackColor(Display.Green);
    Display.DrawImage(plant_medium, 36, 58, Display.center, Display.ysize*0.77, false);
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
    Display.SetForeColor(Display.Black);
    Display.FillCenteredRect(tank_width, tank_height, Display.xsize * 0.33, Display.ysize * 0.4);
    Display.SetForeColor(Display.Green);
    Display.FillCenteredRect(tank_width - 6, tank_height, Display.xsize * 0.33, Display.ysize * 0.4 + 3);
    Display.PlotTextCentered(PSTR("[ empty ]"), Display.xsize*0.33, 35, 1);

    // fertilizer tank
    Display.SetForeColor(Display.Black);
    Display.FillCenteredRect(tank_width, tank_height, Display.xsize * 0.67, Display.ysize * 0.4);
    Display.SetForeColor(Display.Green);
    Display.FillCenteredRect(tank_width - 6, tank_height, Display.xsize * 0.67, Display.ysize * 0.4 + 3);
    Display.PlotTextCentered(PSTR("[ empty ]"), Display.xsize*0.67, 35, 1);

    Display.PlotTextCentered(PSTR("[Fill Tanks To Continue]"), Display.xsize>>1, Display.ysize*0.1, 1);
}

void Interface::UpdateTank(int tankNum, int full) {
    state = 1;
    int x = 0;
    if (tankNum == 0) {
        x = Display.xsize * 0.33;
    } else {
        x = Display.xsize * 0.67;
    }
    if (full) {
        Display.SetForeColor(50, 153, 168);
        Display.FillCenteredRect(tank_width - 6, tank_height - 15, x, Display.ysize * 0.4 - 5);
        Display.PlotTextCentered(PSTR("[ full  ]"), x, 35, 1);
    } else {
        Display.SetForeColor(Display.Green);
        Display.FillCenteredRect(tank_width - 6, tank_height, x, Display.ysize * 0.4 + 3);
        Display.PlotTextCentered(PSTR("[ empty ]"), x, 35, 1);
    }
}

void Interface::DrawPlantSelectionScreen() {
    state = 2;
    Display.FillScreen(Display.Green);
    Display.PlotTextCentered(PSTR("Select Plant Type"), Display.xsize>>1, Display.ysize*0.8, 3, true);
    scrollOffset = -1; // set to -1 to force update
    UpdatePlantSelectionScreen();

    Display.DrawImage(push, 20, 20, Display.xsize * 0.2, Display.ysize * 0.1, false);
    Display.PlotTextCentered(PSTR("Select"), Display.xsize * 0.3, Display.ysize * 0.1, 1);
    Display.DrawImage(scroll, 15, 15, Display.xsize * 0.7, Display.ysize * 0.1, false);
    Display.PlotTextCentered(PSTR("Scroll"), Display.xsize * 0.8, Display.ysize * 0.1, 1);
}

void Interface::UpdatePlantSelectionScreen() {
    state = 2;
    int offset = (currPlantHover > 2) ? 1 : 0;

    for (int i = 0; i < 3; i++) {
        int x = 40 + (2 * i + 1) * (Display.xsize-80)/6;
        int image_y = Display.ysize*0.5;
        int index = i + offset % 4;
        
        // draw highlight boxes
        if (currPlantHover == index) {
            Display.SetForeColor(252, 227, 3);
            Display.DrawCenteredRect(62, 62, x, image_y, 4);
        } else {
            Display.SetForeColor(Display.Black);
            Display.DrawCenteredRect(62, 62, x, image_y, 4);
        }
        
        // update plant icons if scroll changes
        if (scrollOffset != offset) {
            Display.DrawImage(plantPtr[index], 30, 30, x, image_y, false, 2);
            Display.PlotTextCentered(plantNames[index], x, Display.ysize*0.25, 1, 12);
        }
        
    }

    if (offset == 0) { // if the scroll offset is zero, there are items to the right
        Display.SetForeColor(Display.Green);
        Display.FillCenteredRect(25, 25, Display.xsize * 0.07, Display.ysize>>1);
        Display.DrawImage(left_arrow, 25, 25, Display.xsize * 0.92, Display.ysize>>1, true);
    } else {
        Display.SetForeColor(Display.Green);
        Display.FillCenteredRect(25, 25, Display.xsize * 0.92, Display.ysize>>1);
        Display.DrawImage(left_arrow, 25, 25, Display.xsize * 0.07, Display.ysize>>1, false);
    }

    scrollOffset = offset;

}

void Interface::DrawPlantConfirmationScreen() {
    state = 3;
    Display.FillScreen(Display.Green);
    Display.PlotTextCentered(PSTR("Confirm Plant Type"), Display.xsize>>1, Display.ysize*0.85, 2, true);
    Display.DrawImage(plantPtr[currPlantHover], 30, 30, Display.xsize>>1, Display.ysize*0.6, false, 2);
    Display.PlotTextCentered(plantNames[currPlantHover], Display.xsize>>1, Display.ysize*0.35, 2, 12);
    
    Display.DrawImage(left_arrow, 25, 25, Display.xsize * 0.1, Display.ysize*0.2, false);
    Display.PlotTextCentered(PSTR("return"), Display.xsize*0.3, Display.ysize*0.2, 1);

    Display.DrawImage(left_arrow, 25, 25, Display.xsize * 0.9, Display.ysize*0.2, true);
    Display.PlotTextCentered(PSTR("confirm"), Display.xsize * 0.7, Display.ysize*0.2, 1);

    UpdatePlantConfirmationScreen();
}

void Interface::UpdatePlantConfirmationScreen() {
    if (confirmationButton == 0) {
        Display.SetForeColor(Display.Black);
        Display.DrawTo(Display.xsize * 0.25, Display.ysize*0.15, Display.xsize * 0.35, Display.ysize*0.15);
        Display.DrawTo(Display.xsize * 0.25+1, Display.ysize*0.15, Display.xsize * 0.35, Display.ysize*0.15);
        Display.SetForeColor(Display.Green);
        Display.DrawTo(Display.xsize * 0.65, Display.ysize*0.15, Display.xsize * 0.75, Display.ysize*0.15);
        Display.DrawTo(Display.xsize * 0.65+1, Display.ysize*0.15, Display.xsize * 0.75, Display.ysize*0.15);
    } else {
        Display.SetForeColor(Display.Black);
        Display.DrawTo(Display.xsize * 0.65, Display.ysize*0.15, Display.xsize * 0.75, Display.ysize*0.15);
        Display.DrawTo(Display.xsize * 0.65+1, Display.ysize*0.15, Display.xsize * 0.75, Display.ysize*0.15);
        Display.SetForeColor(Display.Green);
        Display.DrawTo(Display.xsize * 0.25, Display.ysize*0.15, Display.xsize * 0.35, Display.ysize*0.15);
        Display.DrawTo(Display.xsize * 0.25 + 1, Display.ysize*0.15, Display.xsize * 0.35, Display.ysize*0.15);

    }
}

void Interface::DrawPlantDashboard() {
    state = 4;
    Display.FillScreen(Display.Green);
    Display.PlotTextCentered(PSTR("Your Plant Dashboard"), Display.xsize>>1, Display.ysize*0.9, 2, true);
    Display.DrawImage(plantPtr[currPlantHover], 30, 30, Display.xsize*0.8, Display.ysize*0.6, false, 2);
    Display.PlotTextCentered(plantNames[currPlantHover], Display.xsize*0.8, Display.ysize*0.35, 2, 12);


    // show temperature
    Display.PlotTextLeftAligned(PSTR("Temp:     "), Display.xsize * 0.05, Display.ysize*0.7, 2, true);

    // show humidity
    Display.PlotTextLeftAligned(PSTR("Humidity: "), Display.xsize * 0.05, Display.ysize*0.6, 2, true);

    // show light level
    Display.PlotTextLeftAligned(PSTR("Light:    "), Display.xsize * 0.05, Display.ysize*0.5, 2, true);

    // Water Tank
    Display.PlotTextCentered(PSTR("Water"), Display.xsize*0.2, Display.ysize*0.15, 2, true);

    // Fertilizer Tank
    Display.PlotTextCentered(PSTR("Fertilizer"), Display.xsize*0.67, Display.ysize*0.15, 2, true);

    UpdatePlantDashboard();
}

void Interface::UpdatePlantDashboard() {
    char buffer[10];
    
    // Update Temperature
    dtostrf(temperature_, 4, 3, buffer);
    Display.PlotTextCentered(buffer, Display.xsize * 0.5, Display.ysize*0.7+6, 2, 5);

    // Update Humidity
    dtostrf(humidity_, 4, 3, buffer);
    Display.PlotTextCentered(buffer, Display.xsize * 0.5, Display.ysize*0.6+6, 2, 5);

    // Update Light Level
    dtostrf(light_, 4, 3, buffer);
    Serial.println(buffer);
    Display.PlotTextCentered(buffer, Display.xsize * 0.5, Display.ysize*0.5+6, 2, 5);
    
    // Update Water Tank Indicator
    if (tankLevel1 == 1) {
        Display.SetForeColor(66, 135, 245);
    } else {
        Display.SetForeColor(209, 4, 4);
    }
    Display.FillCircle(10, Display.xsize*0.4, Display.ysize*0.2);

    // Update Fertilizer Tank Indicator
    if (tankLevel2 == 1) {
        Display.SetForeColor(66, 135, 245);
    } else {
        Display.SetForeColor(209, 4, 4);
    }
    Display.FillCircle(10, Display.xsize*0.95, Display.ysize*0.2);
}

void Interface::ScrollForward() {
    switch (state) {
        case 2: // plant selection screen
            prevPlantHover = currPlantHover;
            currPlantHover = (currPlantHover + 1) % 4;
            UpdatePlantSelectionScreen();
            break;
        case 3: // plant confirmation screen
            confirmationButton = 1;
            UpdatePlantConfirmationScreen();
            break;
    }
}

void Interface::ScrollBackward() {
    switch (state) {
        case 2: // plant selection screen
            prevPlantHover = currPlantHover;
            currPlantHover = (currPlantHover - 1) % 4;
            UpdatePlantSelectionScreen();
            break;
        case 3: // plant confirmation screen
            confirmationButton = 0;
            UpdatePlantConfirmationScreen();
            break;
    }
}

void Interface::ButtonPress() {
    switch (state) {
        case 0: // loading screen
            DrawTankScreen();
            break;
        case 1: // Tank screen
            // there is no user action at this screen.
            break;
        case 2: // plant selection screen
            DrawPlantConfirmationScreen();
            break;
        case 3: // plant confirmation screen
            if (confirmationButton) {
                DrawPlantDashboard();
            } else {
                DrawPlantSelectionScreen();
            }
            break;
    }
}

void Interface::SetTank(int tank1, int tank2) {
    if (tank1 != tankLevel1) {
        tankLevel1 = tank1;
        if (state == 1) {
            UpdateTank(1, tankLevel1);
        }
    }
    if (tank2 != tankLevel2) {
        tankLevel2 = tank2;
        if (state == 1) {
            UpdateTank(2, tankLevel2);
        }
    }
}