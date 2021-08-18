/*
 * This file is part of Cleanflight.
 *
 * Cleanflight is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cleanflight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cleanflight.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "drivers/io_types.h"
// #include "drivers/pwm_output_counts.h"
#include "flight/mixer.h" // MAX_SUPPORTED_MOTORS
#include "flight/servos.h" // MAX_SUPPORTED_SERVOS
#include "drivers/timer.h"

#ifdef USE_DSHOT
#define MOTOR_DSHOT1200_HZ    24000000
#define MOTOR_DSHOT600_HZ     12000000
#define MOTOR_DSHOT300_HZ     6000000
#define MOTOR_DSHOT150_HZ     3000000


#define DSHOT_MOTOR_BIT_0       7
#define DSHOT_MOTOR_BIT_1       14
#define DSHOT_MOTOR_BITLENGTH   20

#define DSHOT_DMA_BUFFER_SIZE   18 /* resolution + frame reset (2us) */
#endif

typedef struct {
    TCH_t * tch;
    bool configured;
    uint16_t value;

    // PWM parameters
    volatile timCCR_t *ccr;         // Shortcut for timer CCR register
    float pulseOffset;
    float pulseScale;

#ifdef USE_DSHOT
    // DSHOT parameters
    timerDMASafeType_t dmaBuffer[DSHOT_DMA_BUFFER_SIZE];
#endif
} pwmOutputPort_t;

/*
typedef struct TCH_s {
    struct timHardwareContext_s *   timCtx;         // Run-time initialized to parent timer
    const timerHardware_t *         timHw;          // Link to timerHardware_t definition (target-specific)
    const timerCallbacks_t *        cb;
    DMA_t                           dma;            // Timer channel DMA handle
    volatile tchDmaState_e          dmaState;
    void *                          dmaBuffer;
} TCH_t;

/*

/*
Betaflght : 
typedef struct {
    timerChannel_t channel;
    // float pulseScale;
    // float pulseOffset;
    bool forceOverflow;
    bool enabled;
    IO_t io;
} pwmOutputPort_t;

*/

typedef struct {
    pwmOutputPort_t *   pwmPort;        // May be NULL if motor doesn't use the PWM port
    uint16_t            value;          // Used to keep track of last motor value
    bool                requestTelemetry;
} pwmOutputMotor_t;

typedef struct motorDevConfig_s {
    uint16_t motorPwmRate;                  // The update rate of motor outputs (50-498Hz)
    uint8_t  motorPwmProtocol;              // Pwm Protocol
    uint8_t  motorPwmInversion;             // Active-High vs Active-Low. Useful for brushed FCs converted for brushless operation
    uint8_t  useUnsyncedPwm;
    uint8_t  useBurstDshot;
    ioTag_t  ioTags[MAX_SUPPORTED_MOTORS];
} motorDevConfig_t;


typedef struct servoDevConfig_s {
    // PWM values, in milliseconds, common range is 1000-2000 (1ms to 2ms)
    uint16_t servoCenterPulse;              // This is the value for servos when they should be in the middle. e.g. 1500.
    uint16_t servoPwmRate;                  // The update rate of servo outputs (50-498Hz)
    ioTag_t  ioTags[MAX_SUPPORTED_SERVOS];
} servoDevConfig_t;



void pwmRequestMotorTelemetry(int motorIndex);

ioTag_t pwmGetMotorPinTag(int motorIndex);

void pwmWriteMotor(uint8_t index, uint16_t value);
void pwmShutdownPulsesForAllMotors(uint8_t motorCount);
void pwmCompleteMotorUpdate(void);
// void pwmCompleteMotorUpdate(uint8_t motorCount);
bool isMotorProtocolDigital(void);

void pwmWriteServo(uint8_t index, uint16_t value);

void pwmDisableMotors(void);
void pwmEnableMotors(void);
struct timerHardware_s;

void pwmMotorPreconfigure(void);
bool pwmMotorConfig(const struct timerHardware_s *timerHardware, uint8_t motorIndex, bool enableOutput);

void pwmServoPreconfigure(void);
bool pwmServoConfig(const struct timerHardware_s *timerHardware, uint8_t servoIndex, uint16_t servoPwmRate, uint16_t servoCenterPulse, bool enableOutput);
void pwmWriteBeeper(bool onoffBeep);
void beeperPwmInit(ioTag_t tag, uint16_t frequency);