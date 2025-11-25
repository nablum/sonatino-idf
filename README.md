# Sonatino IDF

A simple [ESP-IDF](https://github.com/espressif/esp-idf) template for the [Sonatino](https://sonatino.com/) board.  
Use it as an alternative to Arduino or PlatformIO.

## ✨ Features

- Ready-to-use ESP-IDF project structure
- CMake configuration for seamless building and flashing  
- Audio loopback example initialized with Sonatino peripherals
- Works with latest ESP-IDF versions

## 📁 Repository Structure

```bash
sonatino-idf/
├── main/
│   ├── audioconfig.c       # Implements audio initialization functions (ADC/DAC setup)
│   ├── audioconfig.h       # Declares pin mappings, audio parameters, and function prototypes
│   ├── CMakeLists.txt      # Application-specific build configuration for ESP-IDF
│   └── main.c              # Main program; runs the audio loopback using ADC → DAC
└── CMakeLists.txt          # Top-level project build configuration for ESP-IDF
```

## 🚀 Getting Started

### 1. Install ESP-IDF

Follow the official installation [instructions](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/) and make sure `idf.py` is available in your terminal after installation.

### 2. Clone the Repository

```bash
git clone https://github.com/nablum/sonatino-idf.git
cd sonatino-idf
```

### 3. Set the SoC on Sonatino board

```bash
idf.py set-target esp32s3
```

### 4. Configure the Project (Optional)

```bash
idf.py menuconfig
```

### 5. Build & Flash

Connect the Sonatino board via USB and run:

```bash
idf.py build
idf.py flash
idf.py monitor
```

Press `Ctrl+]` to exit the monitor.

## 🧩 Board Notes

Depending on your model you may want to:

- Adjust the flash size
- Configure I2S / I2C / SPI depending on peripherals
- Change default UART baud rate
- Enable PSRAM if your model supports it

## 📝 Example Code

The included `main.c` demonstrates a simple **audio loopback**:

- Reads audio from the ADC input  
- Sends the same signal directly to the DAC output  
- Creates a real-time audio passthrough  

This minimal example shows how to initialize the Sonatino board peripherals and provides a starting point for custom audio applications.

## 💡 Why ESP-IDF?

Using ESP-IDF gives you:

- Full low-level control
- Better performance
- Real-time scheduling options
- Access to all Espressif APIs
- Professional workflow for production-grade firmware

## 📄 License

This project is released under the [MIT License](LICENSE).

## 🙋‍♂️ Contact

Created by [@nablum](https://github.com/nablum)

Feel free to open [issues](https://github.com/nablum/sonatino-idf/issues) or submit pull requests if you find bugs or have feature suggestions!
