#include <Arduino.h>
#include <DataProfiles.h>
#include <DisplayHelper.h>
#include <GyroMeasure.h>
#include <PageContentHelper.h>
#include <PageController.h>
#include <PageHelper.h>
#include <SingleButton.h>

#include "../../include/Grafics.h"

PageController::PageController(DataProfiles *_dataProfiles,
                               DisplayHelper *_displayHelper,
                               GyroMeasure *_gyroMeasure,
                               PageContentHelper *_pageContentHelper,
                               PageHelper *_pageHelper,
                               SingleButton *_singleButton,
                               bool _printDebug) {
    dataProfiles = _dataProfiles;
    displayHelper = _displayHelper;
    gyroMeasure = _gyroMeasure;
    pageContentHelper = _pageContentHelper;
    pageHelper = _pageHelper;
    singleButton = _singleButton;
    printDebug = _printDebug;
}

void PageController::setup() {
    shotCounter = dataProfiles->setup();
}

void PageController::loop() {
    // Button handler
    singleButton->loop();

    // Check for short press
    if (singleButton->shortPressTrigger() && pageHelper->getPageIndex() < pageHelper->getMaxMainPages()) {
        nextMainPage();
    }

    if (pageHelper->getPageIndex() == 0) {
        // Main page 1: Display shot counter data
        counterPage();
    } else if (pageHelper->getPageIndex() == 1) {
        // Main page 2: Wait for shots
        waitingForShotsPage(Aim);
    } else if (pageHelper->getPageIndex() == 2) {
        // Main page 3: Enter setup page
        enterProfilePage(3);
    } else if (pageHelper->getPageIndex() == 3) {
        // Setup sub page 1: Calibration helper page
        calibrationPage(4);
    } else if (pageHelper->getPageIndex() == 4) {
        // Setup sub page 2: Set min g force for shot count
        setupGforcePage(5);
    } else if (pageHelper->getPageIndex() == 5) {
        // Setup sub page 3: Set shot count delay value
        setupShotDelayPage(6);
    } else if (pageHelper->getPageIndex() == 6) {
        // Setup sub page 4: Reset profile page
        resetProfilePage(0);
    }

    // Delay after page change to disable multiple page changes
    if (pageHelper->getPageChanged()) {
        delay(150);
        pageHelper->setPageChanged(false);
    }
}

void PageController::counterPage() {
    // Show profile values
    if (displayHelper->getDisplayChanged()) {
        displayHelper->clear();
        pageContentHelper->counterPage(shotCounter);
        displayHelper->render();
        displayHelper->setDisplayChanged(false);
    }
    // Longpress handler
    if (singleButton->longPressTrigger()) {
        dataProfiles->nextShotCounter();
        shotCounter = dataProfiles->getShotCounter();
        displayHelper->setDisplayChanged(true);
        displayHelper->blink();
        singleButton->longPressTriggerDone();
    }
}

void PageController::waitingForShotsPage(const uint8_t *bitmap) {
    if (displayHelper->getDisplayChanged()) {
        // Wake sensor
        gyroMeasure->sensorWake();
        // Display waiting for shots page
        displayHelper->clear();
        pageContentHelper->waitingForShotsPage();
        displayHelper->bitmapIcon(44, 10, bitmap, 40, 20);
        displayHelper->render();
        displayHelper->setDisplayChanged(false);
    }
    // Longpress handler
    if (singleButton->longPressTrigger()) {
        shotCounter.shotsSeries = 0;
        dataProfiles->putShotCounter(shotCounter);
        displayHelper->blink();
        singleButton->longPressTriggerDone();
    }

    // Get value from sensor
    float gMax = gyroMeasure->getAccelerationMax();

    // Count shots depending on g force setting of profile
    if (gMax >= shotCounter.countGforce) {
        // Store counted g value for later usage
        gyroMeasure->setGCountedLast(gMax);

        // Debug
        if (printDebug) {
            Serial.print(F("SHOT COUNTED "));
            Serial.println(gMax);
        }

        // Delay to prevent multiple counts
        shotCounter.shotsSeries++;
        shotCounter.shotsTotal++;
        dataProfiles->putShotCounter(shotCounter);
        displayHelper->blink(shotCounter.shotDelay);
    }
}

void PageController::enterProfilePage(int subMenuPageIndex) {
    if (displayHelper->getDisplayChanged()) {
        // Set sensor to sleep for lower power consumption
        gyroMeasure->sensorSleep();
        // Display enter setup page
        displayHelper->clear();
        pageContentHelper->enterProfilePage();
        displayHelper->render();
        displayHelper->setDisplayChanged(false);
    }
    // Longpress handler
    if (singleButton->longPressTrigger()) {
        pageHelper->setPageIndex(subMenuPageIndex);
        displayHelper->setDisplayChanged(true);
        displayHelper->blink();
        delay(500);
        singleButton->longPressTriggerDone();
    }
}

void PageController::calibrationPage(int nextPageIndex) {
    if (displayHelper->getDisplayChanged() && calibrationDisplayUpdateDelay < millis()) {
        // Wake sensor
        if (calibrationDisplayUpdateDelay == 0) {
            gyroMeasure->sensorWake();
        }
        // Show calibration page
        displayHelper->clear();
        pageContentHelper->calibrationPage(gMaxCalibration);
        displayHelper->render();
        displayHelper->setDisplayChanged(false);
    }

    // Get value from sensor and update view if new max value is set
    float gMax = gyroMeasure->getAccelerationMax();
    if (gMax > gMaxCalibration) {
        gMaxCalibration = gMax;
        // Update display after 1 second to keep fast measuring for the next 1 second
        calibrationDisplayUpdateDelay = millis() + 1000;
        displayHelper->setDisplayChanged(true);
    }

    // Shortpress handler
    if (singleButton->shortPressTrigger()) {
        gMaxCalibration = 0;
        calibrationDisplayUpdateDelay = 0;
        pageHelper->setPageIndex(nextPageIndex);
        displayHelper->setDisplayChanged(true);
        singleButton->shortPressTriggerDone();
    }
    // Longpress handler
    if (singleButton->longPressTrigger()) {
        gMaxCalibration = 0;
        calibrationDisplayUpdateDelay = 0;
        displayHelper->setDisplayChanged(true);
        displayHelper->blink();
        singleButton->longPressTriggerDone();
    }
}

void PageController::setupGforcePage(int nextPageIndex) {
    if (displayHelper->getDisplayChanged()) {
        // Show setup g force page
        displayHelper->clear();
        pageContentHelper->setupGforcePage(shotCounter.countGforce, gyroMeasure->getGCountedLast());
        displayHelper->render();
        displayHelper->setDisplayChanged(false);
    }
    // Shortpress handler
    if (singleButton->shortPressTrigger()) {
        // If settings changed, save profile
        ShotCounter shotCounterStored = dataProfiles->getShotCounter();
        if (shotCounterStored.countGforce != shotCounter.countGforce) {
            dataProfiles->putShotCounter(shotCounter);
        }
        pageHelper->setPageIndex(nextPageIndex);
        displayHelper->setDisplayChanged(true);
        singleButton->shortPressTriggerDone();
    }
    // Longpress handler
    if (singleButton->longPressTrigger()) {
        shotCounter.countGforce = shotCounter.countGforce + 0.5 > 16 ? 0.5 : shotCounter.countGforce + 0.5;
        displayHelper->setDisplayChanged(true);
        displayHelper->blink();
        singleButton->longPressTriggerDone();
    }
}

void PageController::setupShotDelayPage(int nextPageIndex) {
    if (displayHelper->getDisplayChanged()) {
        displayHelper->clear();
        pageContentHelper->setupShotDelayPage(shotCounter.shotDelay);
        displayHelper->render();
        displayHelper->setDisplayChanged(false);
    }
    // Shortpress handler
    if (singleButton->shortPressTrigger()) {
        // If settings changed, save profile
        ShotCounter shotCounterStored = dataProfiles->getShotCounter();
        if (shotCounterStored.shotDelay != shotCounter.shotDelay) {
            dataProfiles->putShotCounter(shotCounter);
        }
        pageHelper->setPageIndex(nextPageIndex);
        displayHelper->setDisplayChanged(true);
        singleButton->shortPressTriggerDone();
    }
    // Longpress handler
    if (singleButton->longPressTrigger()) {
        shotCounter.shotDelay = shotCounter.shotDelay + 50 > 2000 ? 50 : shotCounter.shotDelay + 50;
        displayHelper->setDisplayChanged(true);
        displayHelper->blink();
        singleButton->longPressTriggerDone();
    }
}

void PageController::resetProfilePage(int nextPageIndex) {
    if (displayHelper->getDisplayChanged()) {
        displayHelper->clear();
        pageContentHelper->resetProfilePage();
        displayHelper->render();
        displayHelper->setDisplayChanged(false);
    }
    // Shortpress handler
    if (singleButton->shortPressTrigger()) {
        pageHelper->setPageIndex(nextPageIndex);
        displayHelper->setDisplayChanged(true);
        singleButton->shortPressTriggerDone();
    }
    // Longpress handler
    if (singleButton->longPressTrigger()) {
        shotCounter = dataProfiles->resetShotCounter();
        displayHelper->blink();
        singleButton->longPressTriggerDone();
    }
}
