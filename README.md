# Home Assistant AC Dimmer

```c++
/*
Version:		V1.0
Author:			Makerfabs
Create Date:	2025/8/21
Note:
	

*/
```


## Makerfabs

[Makerfabs home page](https://www.makerfabs.com/)

[Makerfabs Wiki](https://wiki.makerfabs.com/)

## Intruduce

[Product Link](https://www.makerfabs.com/ac-dimmer-for-home-assistant.html)

[Wiki Link](https://wiki.makerfabs.com/How_to_use_AC_Dimmer_for_Home_Assistant.html)



## 1. Product Features

- **Main Controller**: ESP32
- **Dimmer Controller**: STM32G030F6
- **Power Supply**: On-board AC-DC module, no external DC power supply needed.
- **Safety Design**: On-board relay to completely cut off current, preventing the "ghosting" or faint glow of some LEDs when off.
- **Voltage Compatibility**: AC 110V ~ 230V, with automatic frequency adaptation.
- **Maximum Load**: 2KW

## 2. Prerequisites

Before you begin, ensure you have the following ready.

**Hardware**:

- A Raspberry Pi (or other Linux host) with Docker and Docker Compose installed.
- The Makerfabs AC Dimmer module.
- A dimmable AC lamp or bulb to act as the load.
- A USB Type-C data cable.
- A screwdriver for wiring.

**Software**:

- An SSH client (e.g., PuTTY, Termius, or your system's built-in terminal).
- A modern web browser (Chrome or Edge recommended).
- **ESPHome Version**: This tutorial is based on `2025.7.x` or newer.

**Configuration Files**:

- Download the required ESPHome configuration files for the dimmer from the official Makerfabs Wiki or product page, which typically includes:
  - `dimmer.yaml` (The main configuration file)
  - A `common_components/` directory containing files like `arduino_dimmer.h` and `output.py`.

## 3. Environment Setup: Docker Deployment

We will use Docker Compose to manage the Home Assistant and ESPHome containers.

1. Log in to your Raspberry Pi via SSH.
2. Create a directory to store your Docker configurations, e.g., `~/docker`.
3. Inside the `~/docker` directory, create a file named `docker-compose.yml` with the following content.

```
# docker-compose.yml

version: '3'
services:
  homeassistant:
    container_name: homeassistant
    image: "ghcr.io/home-assistant/home-assistant:stable"
    volumes:
      - /home/pi/docker/homeassistant/config:/config # Left side is host path, right is container path
      - /etc/localtime:/etc/localtime:ro
      - /run/dbus:/run/dbus:ro
    restart: unless-stopped
    privileged: true
    network_mode: host

  esphome:
    container_name: esphome
    image: "ghcr.io/esphome/esphome"
    volumes:
      - /home/pi/docker/esphome/config:/config # Left side is host path, right is container path
      - /etc/localtime:/etc/localtime:ro
    restart: always
    privileged: true
    network_mode: host
```

**Note on `volumes`**: The path on the left of the colon (`:`) is the actual directory on your host machine (Raspberry Pi). Ensure these directories exist.

1. In the same directory as your `docker-compose.yml` file, run the following command to start the services:

   ```
   docker-compose up -d
   ```

## 4. ESPHome Device Configuration

1. Open a web browser and navigate to your ESPHome web UI at `http://<YOUR_RASPBERRY_PI_IP>:6052`.
   ![image-20250820110253207](https://easyimage.linwanrong.com/i/2025/08/20/i8fl6t-0.webp)

2. Click the **“NEW DEVICE”** button in the bottom-right corner, then click **“Continue”**.

3. Give your device a name (e.g., `ac_dimmer`) and enter your Wi-Fi credentials.

   ![image-20250821165653536](https://easyimage.linwanrong.com/i/2025/08/21/re9o5v-0.webp)

4. On the device type selection screen, choose **“ESP32”**.

   ![image-20250820110410956](https://easyimage.linwanrong.com/i/2025/08/20/i9da9l-0.webp)

5. Click **“SKIP”** to bypass the initial installation and return to the ESPHome dashboard.

6. **Upload Custom Components**: Use an SCP client (like WinSCP) or the `scp` command-line tool to upload the entire `common_components` folder to your ESPHome config directory on the Raspberry Pi (e.g., `/home/pi/docker/esphome/config/`).

7. **Edit the YAML Configuration File**:

   - Back in the ESPHome UI, click the **“EDIT”** button on the `ac_dimmer` card.

   - At the top of the file, under `esphome:`, add the `external_components` declaration. **Pay attention to indentation**.

     ```
     external_components:
       - source:
           type: local
           path: common_components
     ```

   - Next, copy the **entire** contents of the `dimmer.yaml` file provided by Makerfabs and paste it at the very end of the file.

8. Click **“SAVE”** to save your changes, then click **“CLOSE”** to exit the editor.

## 5. Compiling and Flashing the Firmware

1. On the ESPHome dashboard, click the three dots on the `ac_dimmer` card and select **“Install”**.

2. In the pop-up window, choose **“Manual Download”**, then select **“Modern format”**. ESPHome will begin compiling the firmware online. Please be patient.

3. Once compilation is successful, your browser will automatically download a `.bin` firmware file.

   ![image-20250820140853527](https://easyimage.linwanrong.com/i/2025/08/20/namw84-0.webp)

4. **Flashing the Firmware for the First Time**:

   - Connect the AC Dimmer module to your computer using the USB-C cable.

   - In your browser, navigate to the ESPHome Web Flasher: [**https://web.esphome.io/**](https://web.esphome.io/)

     ![image-20250820112428934](https://easyimage.linwanrong.com/i/2025/08/20/ildsc4-0.webp)

   - Click **“CONNECT”** and select the appropriate COM port for your device from the pop-up list.

     ![image-20250820120932734](https://easyimage.linwanrong.com/i/2025/08/20/k007nd-0.webp)

   - After connecting, click **“INSTALL”**, choose the `.bin` file you just downloaded, and click **“INSTALL”** again to begin flashing.

     ![image-20250820112601438](https://easyimage.linwanrong.com/i/2025/08/20/imeluv-0.webp)

   - **Flashing failed?** Press and hold the **“Flash”** button, then press and release the **“Reset”** button, and finally release the **“Flash”** button. Then try again.

5. After a successful flash, click **“LOGS”** to view the device's output. Wait for it to connect to your Wi-Fi, and you will see its assigned IP address in the logs. Make a note of this IP address.

## 6. Home Assistant Integration

1. Open your Home Assistant UI at `http://<YOUR_RASPBERRY_PI_IP>:8123`.

2. Home Assistant will often auto-discover new devices. If you see a notification, simply click **“CONFIGURE”**.

   ![image-20250820142307530](https://easyimage.linwanrong.com/i/2025/08/20/njaj7x-0.webp)

3. If the device is not discovered automatically, add it manually:

   - Go to **“Settings”** > **“Devices & Services”**.
   - Click **“ADD INTEGRATION”**, search for, and select **“ESPHome”**.

4. In the pop-up, enter the IP address of your device that you noted earlier, then click **“SUBMIT”**.

5. Next, Home Assistant will ask for the device's Encryption Key.

   - Go back to the ESPHome UI and click **“EDIT”** on your `ac_dimmer` card.

   - In the YAML file, find the `api:` section and copy the long string of characters after `key:`.

   - Paste this key into the Home Assistant input box and click **“SUBMIT”**.

     ![image-20250820142416697](https://easyimage.linwanrong.com/i/2025/08/20/njxgrl-0.webp)

6. Once added, assign the device to an area and click **“FINISH”**.

1. ## 7. Home Assistant Dashboard Setup

   1. Navigate to your main "Overview" dashboard in Home Assistant.
   2. Click the three dots in the top-right corner and select **“Edit Dashboard”**.
   3. Click the **“ADD CARD”** button in the bottom-right corner.
   4. From the card selection list, choose a card type. For example, select the **“Gauge”** card.
   5. In the card configuration screen, click the **“Entity”** dropdown and select the brightness entity for your AC Dimmer.
   6. Click **“SAVE”**. You can repeat this process to add a "Light" card for an on/off toggle and brightness slider.
   7. Finally, click **“DONE”** in the top-right corner to exit edit mode.