// SystemTimer abstracts the process of changing enabling and setting
// up the SystemTimer.
//
//   Usage:
//      PinConfigure P0_0(0, 0);
//      P0_0.SetAsActiveLow();
//      P0_0.SetPinMode(PinConfigureInterface::PinMode::kPullUp);
#pragma once

#include <cstdio>

#include "L0_LowLevel/LPC40xx.h"
#include "L2_Utilities/macros.hpp"

// LPC4076 does not include P3.26 so supporting methods are not available
class SystemTimerInterface
{
   public:
    virtual void setIsrFunction(void (*isr)(void)) = 0;
    virtual void StartTimer() = 0;
    virtual void SetClockDivider(uint32_t divider)        = 0;
    virtual uint32_t SetTickFrequency(uint32_t frequency)     = 0;
};

class SystemTimer : public SystemTimerInterface
{
   public:
    // Source: "UM10562 LPC408x/407x User manual" table 83 page 132
    enum PinBitMap : uint8_t
    {
        kFunction    = 0,
        kMode        = 3,
        kHysteresis  = 5,
        kInputInvert = 6,
        kSlew        = 9,
        kOpenDrain   = 10
    };
    // Compile time validating SystemTimer factory.
    // Will test the port and pin variables to make sure they are within bounds
    // of the pin_config_register.
    template <unsigned port, unsigned pin>
    static constexpr SystemTimer CreateSystemTimer()
    {
        static_assert(port <= 5, "Port must be between 0 and 5");
        static_assert(pin <= 31, "Pin must be between 0 and 31");
        static_assert(port == 5 && pin <= 4,
            "For port 5, the pin number must be equal to or below 4");
        return SystemTimer();
    }
    void setIsrFunction(void (*isr)(void)) final override
    {

    }
    void StartTimer() final override
    {

    }
    void SetClockDivider(uint32_t divider) final override
    {

    }
    uint32_t SetTickFrequency(uint32_t frequency) final override
    {

    }
    // vRunTimeStatIsrEntry();
    // xPortSysTickHandler();
    // vRunTimeStatIsrExit();
};

extern "C" void SysTick_Handler(void)
{

}