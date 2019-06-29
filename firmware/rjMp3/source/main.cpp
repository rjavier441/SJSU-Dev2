#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include <cstdint>
#include <cstdio>

#include "config.hpp"
#include "L0_LowLevel/LPC40xx.h"
#include "L2_HAL/switches/button.hpp"
#include "L2_HAL/memory/sd.hpp"
#include "utility/log.hpp"
#include "utility/macros.hpp"
#include "utility/rtos.hpp"
#include "utility/time.hpp"

// @description   Enumerators to show MP3 Player State
enum class DeviceState
{
  kPaused,
  kPlaying,
  kIdle,
  kUninitialized
};

// State Variables
DeviceState currentState = DeviceState::kUninitialized;
DeviceState previousState = DeviceState::kUninitialized;
bool inputReaderReady = false;
bool codecDataFeedReady = false;

// Mutexes that regulate state variable manipulation
SemaphoreHandle_t xMutexCurrentState = NULL;
SemaphoreHandle_t xMutexInputReaderReady = NULL;
SemaphoreHandle_t xMutexCodecDataFeedReady = NULL;

// Anonymous namespace so these tasks are only visible to this file
namespace mp3Task
{

// @description   This function comprises a RTOS task to handle displaying data
//                on the LCD screen
// @parameter     (void*) pvParameters  A collection of task parameters
// @returns       (void)
void LcdControl(void* pvParameters)
{
  LOG_INFO("Initializing LcdControl Task...");

  // Initialize NHD control pins
  Gpio lcdSlaveSelect(2, 5);
  lcdSlaveSelect.SetAsOutput();

  // NOTE: The current drivers (i.e. i2c, spi) are NOT thread-safe. You will
  // have to put critical sections when you use them

  while (true)
  {

    // Delay this task for a short while during each iteration
    vTaskDelay(50);
  }
}

// @description   This function comprises a RTOS task to handle playing music
//                on the VS1053b Audio Codec
// @parameter     (void*) pvParameters  A collection of task parameters
// @returns       (void)
void CodecDataFeed(void* pvParameters)
{
  LOG_INFO("Initializing CodecDataFeed Task...");

  // Initialize GPIO inputs
  Gpio dreq(1, 24);
  dreq.SetAsInput();

  // Initiate task loop
  while (true)
  {
    // Only issue commands while DREQ is HIGH (See p.37 of datasheet)
    LOG_INFO("I'm sending data to the codec");
    
    // Delay this task for a short while during each iteration
    vTaskDelay(50); // 50ms
  }
}

// @description   This function comprises a RTOS task to handle the button
//                input. It takes input that can change the state of the MP3
//                player.
// @parameter     (void*) pvParameters  A collection of task parameters
// @returns       (void)
void InputReader(void* pvParameters)
{
  LOG_INFO("Initializing InputReader Task...");

  // Initialize a button at GPIO Port 2 Pin 2
  Button btn1(2, 2);
  Button btn2(2, 1);
  Button btn3(2, 3);
  Button btn4(2, 4);
  btn1.Initialize();
  btn2.Initialize();
  btn3.Initialize();
  btn4.Initialize();
  LOG_INFO("Test button initialized...");

  // Initiate task loop
  while (true)
  {
    if (btn1.Pressed())
    {
      LOG_INFO("Button 1 Pressed");
    }
    else if (btn2.Pressed())
    {
      LOG_INFO("Button 2 Pressed");
    }
    else if (btn3.Pressed())
    {
      LOG_INFO("Button 3 Pressed");
    }
    else if (btn4.Pressed())
    {
      LOG_INFO("Button 4 Pressed");
    }

    // Delay this task for a short while during each iteration
    vTaskDelay(50); // 50ms
  }
}

// @description   This function comprises the RTOS task that controls the entire
//                device.
// @parameter     (void*) pvParameters  A collection of task parameters
// @returns       (void)
void ControlUnit(void* pvParameters)
{
  // Initialize cu task
  LOG_INFO("Initializing ControlUnit Task...");
  TaskHandle_t iReadTask = NULL;

  // Create input reader task
  xTaskCreate(mp3Task::InputReader,     // Make InputReader a RTOS task
              "InputReader",            // Task Name
              rtos::StackSize(1024),    // Default task to 1024 bytes of stack
              rtos::kNoParameter,       // No arguments to pass to task
              rtos::Priority::kMedium,  // Give this task medium priority
              &iReadTask);

  // Initiate task loop
  LOG_INFO("Running CU...");
  while (true)
  {
    // 

    // Delay this task for a short while during each iteration
    vTaskDelay(50);
  }
}

} // namespace

// @description   This is the firmware's main routine
// @parameters    (void)
// @returns       (int) code    This function should never return
int main(void)
{
  // Initialize main
  LOG_INFO("BEGIN rjMp3...");
  TaskHandle_t cuTask = NULL;    // A reference to the created input read task

  // Create mutexes/semaphores
  xMutexCurrentState = xSemaphoreCreateMutex();
  xMutexCodecDataFeedReady = xSemaphoreCreateMutex();
  xMutexInputReaderReady = xSemaphoreCreateMutex();

  // Create tasks
  LOG_INFO("Creating Main Task...");
  Delay(1000);
  xTaskCreate(mp3Task::ControlUnit,     // Make ControlUnit a RTOS task
              "ControlUnit",            // Task Name
              rtos::StackSize(1024),    // Default task to 1024 bytes of stack
              rtos::kNoParameter,       // No arguments to pass to task
              rtos::Priority::kHigh,    // Give this task medium priority
              &cuTask);                // Optional reference to the task

  // Launch task scheduler
  LOG_INFO("Starting Task Scheduler...");
  vTaskStartScheduler();
  return 0;
}
