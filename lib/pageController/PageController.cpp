#include <Arduino.h>
#include <DataProfiles.h>
#include <DisplayHelper.h>
#include <GyroMeasure.h>
#include <PageContentHelper.h>
#include <PageController.h>
#include <PageHelper.h>
#include <SingleButton.h>

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

void PageController::enterProfilePage() {
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
        pageHelper->setPageIndex(3);
        pageHelper->setPageChanged(true);
        displayHelper->setDisplayChanged(true);
        displayHelper->blink();
        delay(500);
        singleButton->longPressTriggerDone();
    }
}

void PageController::setupGforcePage() {
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
        pageHelper->setPageIndex(5);
        pageHelper->setPageChanged(true);
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

void PageController::setupShotDelayPage() {
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
        pageHelper->setPageIndex(6);
        pageHelper->setPageChanged(true);
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

void PageController::resetProfilePage() {
    if (displayHelper->getDisplayChanged()) {
        displayHelper->clear();
        pageContentHelper->resetProfilePage();
        displayHelper->render();
        displayHelper->setDisplayChanged(false);
    }
    // Shortpress handler
    if (singleButton->shortPressTrigger()) {
        pageHelper->setPageIndex(0);
        pageHelper->setPageChanged(true);
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

void PageController::calibrationPage() {
    if (displayHelper->getDisplayChanged() && calibrationDisplayUpdateDelay < millis()) {
        // Wake sensor
        gyroMeasure->sensorWake();
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
        pageHelper->setPageIndex(4);
        pageHelper->setPageChanged(true);
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
