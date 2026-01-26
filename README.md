# STM32 BLE Pulse Sensor

A real-time heart rate monitoring system using STM32F411RE microcontroller with BlueNRG-2 BLE module. The project implements a sophisticated pulse detection algorithm and transmits BPM (Beats Per Minute) data over Bluetooth Low Energy.

## 📋 Table of Contents

- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Hardware Connections](#hardware-connections)
- [Software Architecture](#software-architecture)
- [Building and Flashing](#building-and-flashing)
- [BLE Services](#ble-services)
- [Pulse Detection Algorithm](#pulse-detection-algorithm)
- [Usage](#usage)
- [Future Enhancements](#future-enhancements)
- [Troubleshooting](#troubleshooting)

## ✨ Features

- **Real-time Pulse Detection**: Adaptive threshold algorithm with hysteresis filtering
- **BPM Calculation**: Accurate heart rate measurement (40-180 BPM range)
- **Signal Quality Assessment**: Real-time signal quality scoring (0-100)
- **BLE Connectivity**: Wireless data transmission via BlueNRG-2 module
- **UART Debugging**: Serial output for monitoring and diagnostics (115200 baud)
- **Physiological Validation**: Built-in BPM range validation
- **Low Power Design**: Efficient ARM Cortex-M4 based implementation

## 🔧 Hardware Requirements

### Main Components

- **STM32F411RE Nucleo Board** - Main microcontroller board
- **BlueNRG-2 BLE Module** - Bluetooth Low Energy connectivity
- **Pulse Sensor** - Optical heart rate sensor (e.g., PulseSensor.com sensor)
- **USB Cable** - For programming and debugging

### Technical Specifications

- **MCU**: STM32F411RET6 (ARM Cortex-M4, 100MHz, 512KB Flash, 128KB RAM)
- **BLE Module**: BlueNRG-2 (Bluetooth 5.0 compliant)
- **ADC Resolution**: 12-bit (0-4095)
- **Sampling Rate**: Continuous ADC conversion

## 🔌 Hardware Connections

### Pulse Sensor Connection

| Pulse Sensor Pin | STM32 Pin | Description |
|------------------|-----------|-------------|
| Signal (S)       | PA4       | Analog signal input (ADC1_IN4) |
| VCC              | 3.3V      | Power supply |
| GND              | GND       | Ground |

### BlueNRG-2 Module Connection (SPI)

| BlueNRG-2 Pin | STM32 Pin | Function |
|---------------|-----------|----------|
| MOSI          | PA7       | SPI1 MOSI |
| MISO          | PA6       | SPI1 MISO |
| SCK           | PA5       | SPI1 Clock |
| CS            | PA1       | Chip Select |
| IRQ           | PA0       | Interrupt Request |
| RESET         | PA8       | Reset Pin |

### Debug UART

| Function | STM32 Pin | Description |
|----------|-----------|-------------|
| TX       | PA2       | USART2 TX (115200 baud) |
| RX       | PA3       | USART2 RX |

## 🏗️ Software Architecture

### Project Structure

```
stm32_ble/
├── Core/
│   ├── Inc/
│   │   ├── main.h                    # Main application header
│   │   ├── pulse_sensor.h            # Pulse detection module
│   │   ├── services.h                # BLE GATT services
│   │   ├── app_bluenrg.h             # BLE stack interface
│   │   └── adc.h                     # ADC configuration
│   └── Src/
│       ├── main.c                    # Main application logic
│       ├── pulse_sensor.c            # Pulse detection algorithm
│       ├── services.c                # BLE service implementation
│       ├── app_bluenrg.c             # BLE initialization & processing
│       └── adc.c                     # ADC driver with interrupt
├── Middlewares/ST/BlueNRG-2/         # BlueNRG-2 BLE stack
├── Drivers/                          # STM32 HAL drivers
└── README.md                         # This file
```

### Key Modules

#### 1. Pulse Sensor Module (`pulse_sensor.c/h`)

- **Adaptive Threshold**: Automatically adjusts detection threshold based on signal amplitude
- **Hysteresis Filtering**: Prevents false triggers from noise (50 ADC units)
- **Beat Detection**: Rising edge detection with debounce protection (100ms)
- **BPM Calculation**: Rolling average over 6 beat intervals
- **Signal Quality**: Real-time quality assessment based on amplitude

#### 2. ADC Module (`adc.c/h`)

- **Configuration**: PA4 as analog input, ADC1 Channel 4
- **Sampling**: Continuous conversion mode with 144-cycle sample time
- **Interrupt-Driven**: EOC (End of Conversion) interrupt for real-time processing
- **Direct Processing**: ADC values fed directly to pulse sensor algorithm

#### 3. BLE Services Module (`services.c/h`)

- **Health Service** (UUID: 0x180F)
  - BPM Characteristic (UUID: 0x190F) - Read & Notify
- **Weather Service** (UUID: 0x1A20) - *Future Use*
  - Temperature Characteristic (UUID: 0x2A20) - Read Only

#### 4. BLE Application (`app_bluenrg.c/h`)

- **Device Name**: "STM32BLE"
- **MAC Address**: 02:80:E1:00:34:12
- **Role**: GAP Peripheral
- **Advertising**: Connectable, discoverable mode
- **Connection Management**: Automatic reconnection handling

## 🚀 Building and Flashing

### Prerequisites

- **STM32CubeIDE** (recommended) or **Keil MDK** or **IAR EWARM**
- **STM32CubeMX** (for configuration changes)
- **ST-Link Drivers** (for programming)

### Build Steps

#### Using STM32CubeIDE

1. **Import Project**
   ```
   File → Open Projects from File System
   Select the project directory
   ```

2. **Build Project**
   ```
   Project → Build All (Ctrl+B)
   ```

3. **Flash to Board**
   ```
   Run → Debug (F11) or Run (Ctrl+F11)
   ```

#### Using Command Line (ARM GCC)

```bash
# Navigate to project directory
cd stm32_ble

# Build
make clean
make all

# Flash (using OpenOCD or ST-Link CLI)
st-flash write build/stm32_ble.bin 0x8000000
```

### Debug Configuration

**UART Output**: Connect to USART2 (PA2) at **115200 baud, 8N1**

```bash
# Linux/macOS
screen /dev/ttyACM0 115200

# Windows (PuTTY, Tera Term, or similar)
# COM Port: Check Device Manager
# Baud: 115200
```

## 📡 BLE Services

### Health Service (Primary BPM Service)

**Service UUID**: `0000180F-0000-1000-8000-00805F9B34FB`

| Characteristic | UUID | Properties | Data Type | Description |
|----------------|------|------------|-----------|-------------|
| BPM            | 0x190F | Read, Notify | uint16_t | Heart rate in beats per minute |

### Weather Service (Future Implementation)

**Service UUID**: `00001A20-0000-1000-8000-00805F9B34FB`

| Characteristic | UUID | Properties | Data Type | Description |
|----------------|------|------------|-----------|-------------|
| Temperature    | 0x2A20 | Read | float (4 bytes) | Temperature in Celsius |

**Note**: Temperature service is implemented but not currently populated with data. This is reserved for future enhancement with a temperature sensor.

### Connecting via BLE

#### Using nRF Connect Mobile App

1. Install **nRF Connect** OR **LightBlue**(iOS/Android)
2. Scan for devices
3. Connect to **"STM32BLE"**
4. Navigate to **Health Service (0x180F)**
5. Enable **notifications** on BPM characteristic
6. Monitor real-time heart rate data

#### Using Python (with `bleak` library)

```python
import asyncio
from bleak import BleakClient, BleakScanner

BPM_CHAR_UUID = "0000190F-0000-1000-8000-00805F9B34FB"

async def notification_handler(sender, data):
    bpm = int.from_bytes(data, byteorder='little')
    print(f"BPM: {bpm}")

async def main():
    device = await BleakScanner.find_device_by_name("STM32BLE")
    
    async with BleakClient(device) as client:
        await client.start_notify(BPM_CHAR_UUID, notification_handler)
        await asyncio.sleep(60)  # Monitor for 60 seconds

asyncio.run(main())
```

## 🔬 Pulse Detection Algorithm

### Algorithm Overview

The pulse detection system uses a sophisticated adaptive threshold approach:

1. **Signal Acquisition**: 12-bit ADC reads pulse sensor at continuous rate
2. **Dynamic Threshold**: Automatically adjusts to signal amplitude
   ```
   Threshold = Signal_Min + (Signal_Max - Signal_Min) × 0.7
   ```
3. **Hysteresis**: 50 ADC unit buffer prevents noise-triggered false positives
4. **Beat Detection**: Rising edge triggers beat event
5. **Interval Validation**: Checks if interval is within 40-180 BPM range
6. **BPM Calculation**: Rolling average of last 6 beat intervals

### Key Parameters

| Parameter | Value | Description |
|-----------|-------|-------------|
| `PULSE_THRESHOLD_RATIO` | 0.7 | Threshold as fraction of amplitude |
| `PULSE_HYSTERESIS` | 50 | Noise rejection buffer (ADC units) |
| `BEAT_HISTORY_SIZE` | 6 | Number of intervals for averaging |
| `MIN_BEAT_INTERVAL_MS` | 333 ms | Max 180 BPM |
| `MAX_BEAT_INTERVAL_MS` | 1500 ms | Min 40 BPM |
| `DEBOUNCE_TIME_MS` | 100 ms | Minimum time between beats |
| `SIGNAL_UPDATE_INTERVAL_MS` | 2000 ms | Reset min/max tracking |

### Signal Quality Scoring

The system provides real-time signal quality assessment:

| Amplitude Range | Quality Score | Interpretation |
|----------------|---------------|----------------|
| < 20           | 10            | Very poor signal |
| 20-49          | 30            | Poor signal |
| 50-99          | 60            | Moderate signal |
| ≥ 100          | 85            | Good signal |

### BPM Validation Criteria

A BPM reading is considered **valid** when:
- ✅ BPM is between 40-180
- ✅ At least 3 beats detected in history
- ✅ Signal quality ≥ 50

## 📱 Usage

### Basic Operation

1. **Power on the board**
2. **Place finger on pulse sensor** (firm but comfortable pressure)
3. **Wait 5-10 seconds** for algorithm to stabilize
4. **Connect via BLE** to view real-time BPM

### Serial Output

The UART outputs diagnostic information:

```
STM32 BLE Pulse Sensor Application Start
Beat! Interval: 857 ms, BPM: 70, Quality: 85
Sending BPM: 70
Beat! Interval: 845 ms, BPM: 71, Quality: 85
Sending BPM: 71
```

### LED Indicators

- **LD2 (Green LED)**: Available as user-configurable indicator

### Best Practices

📌 **Sensor Placement**
- Place sensor on fingertip (index or middle finger)
- Ensure good contact but avoid excessive pressure
- Keep finger still during measurement

📌 **Environmental Considerations**
- Avoid bright direct light on optical sensor
- Room temperature environment (15-25°C)
- Minimize motion artifacts

📌 **Signal Quality**
- Quality score > 60 recommended for reliable readings
- If quality is low, reposition finger
- Clean sensor surface if needed

## 🔮 Future Enhancements

The project architecture supports several planned improvements:

### Temperature Monitoring
- **Status**: Service implemented, sensor integration pending
- **Hardware**: DS18B20 or DHT22 temperature sensor
- **Connection**: Any available GPIO with 1-Wire or I2C
- **Use Case**: Environmental monitoring or body temperature

### Additional Features (Suggested)
- [ ] SpO2 (Blood Oxygen) monitoring with MAX30102
- [ ] Accelerometer integration for motion detection
- [ ] SD card logging for historical data
- [ ] Battery monitoring and management
- [ ] Power-saving sleep modes
- [ ] Mobile app development
- [ ] Cloud data synchronization

## 🔧 Troubleshooting

### Common Issues

#### No BPM Readings

**Symptoms**: BPM stays at 0 or no beat detection
- ✅ Check pulse sensor connection to PA4
- ✅ Verify 3.3V power supply
- ✅ Ensure proper finger placement
- ✅ Check UART output for signal quality
- ✅ Monitor ADC values (should be 100-4000 range)

#### BLE Connection Failed

**Symptoms**: Device not visible or won't connect
- ✅ Verify BlueNRG-2 SPI connections
- ✅ Check CS, MOSI, MISO, SCK pins
- ✅ Ensure IRQ and RESET pins connected
- ✅ Check UART output for BLE initialization errors
- ✅ Power cycle the board

#### Erratic BPM Values

**Symptoms**: BPM jumps wildly or is unstable
- ✅ Improve finger placement (avoid motion)
- ✅ Check signal quality score (should be > 50)
- ✅ Verify no loose connections
- ✅ Shield optical sensor from ambient light
- ✅ Clean sensor surface

#### UART Not Working

**Symptoms**: No serial output
- ✅ Check baud rate is 115200
- ✅ Verify PA2 (TX) connection
- ✅ Ensure correct COM port selected
- ✅ Check USB cable is data-capable

### Debug Commands

You can add the following to the main loop for diagnostics:

```c
// In main.c, add to while loop:
printf("ADC: %d, Min: %d, Max: %d, Threshold: %d, Quality: %d\r\n",
       adc_value,
       get_signal_min(),
       get_signal_max(),
       get_signal_threshold(),
       get_signal_quality());
HAL_Delay(500);
```

### Getting Help

- **Check Serial Output**: Most issues are visible in UART logs
- **Signal Quality**: Use quality score to diagnose sensor issues
- **GitHub Issues**: Report bugs or request features
- **ST Community**: STM32 forums for hardware/HAL questions

## 📄 License

This project uses STMicroelectronics software components which are licensed under their respective terms. Please refer to LICENSE files in subdirectories.

## 🙏 Acknowledgments

- STMicroelectronics for STM32 HAL Library and BlueNRG-2 middleware
- PulseSensor.com community for pulse detection insights
- ARM CMSIS standard for efficient embedded development

---

**Version**: 1.0  
**Last Updated**: January 2026  
**Author**: Kunal  
**Platform**: STM32F411RE + BlueNRG-2
