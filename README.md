# NXP Application Code Hub

[<img src="https://mcuxpresso.nxp.com/static/icon/nxp-logo-color.svg" width="100"/>](https://www.nxp.com)

## Label CIFAR10 image on FRDM-MCXN947

This example is based on the machine learning algorithm, powered by MCXN947,
which can label the images from the camera and show the type of the object at the bottom of the LCD.

The model is trained on dataset CIFAR10, it supports 10 classes of images: 

"airplane", "automobile", "bird", "cat", "deer", "dog", "frog", "horse", "ship", "truck".

![](image/block_diagram.png)

![](image/data_diagram.png)

#### Boards: FRDM-MCXN947

#### Categories: HMI, AI/ML

#### Peripherals: DISPLAY

#### Toolchains: MCUXpresso IDE, VS Code

## Table of Contents

1. [Software](#step1)
2. [Hardware](#step2)
3. [Setup](#step3)
4. [Results](#step4)
5. [FAQs](#step5)
6. [Support](#step6)
7. [Release Notes](#step7)

## 1. Software <a name="step1"></a>

- Download [SDK_2_14_0_FRDM-MCXN947](https://mcuxpresso.nxp.com/en/welcome)
- Download and install [MCUXpresso IDE V11.9.0 or later](https://www.nxp.com/design/design-center/software/development-software/mcuxpresso-software-and-tools-/mcuxpresso-integrated-development-environment-ide:MCUXpresso-IDE).
- MCUXpresso for Visual Studio Code: This example supports MCUXpresso for Visual Studio Code, for more information about how to use Visual Studio Code please refer [here](https://www.nxp.com/design/training/getting-started-with-mcuxpresso-for-visual-studio-code:TIP-GETTING-STARTED-WITH-MCUXPRESSO-FOR-VS-CODE).

## 2. Hardware <a name="step2"></a>

* 3.5" TFT LCD module by NXP (P/N PAR-LCD-S035)
* Camera module: OV7670
* FRDM-MCXN947(SCH-90818_REV B) board
* Type-C USB cable

## 3. Setup<a name="step3"></a>

### 3.1 Step 1

Rework for camera pins on FRDM-MCXN947 because the camera is not the first function.
Please change SJ16, SJ26, and SJ27 from the A side to the B side.

Board before rework.

![](image/before%20reworking.jpeg)

Board after rework.

![](image/after%20reworking.jpeg)

Here is the detail.

![](image/frdm_board_rework.png)

Attach the LCD shield (J1: Pins 5-28 skip first 4 pins) to FRDM (J8). 
Attach the Camera shield to the FRDM (J9: Pins 5-23; skip first 4 pins), as shown below:

![](image/frdm_board_connections.png)

Connect the debug port on board with the laptop.

![](image/debug_connection.png)

### 3.2 Step 2
#### Develop in MCUXpresso IDE

Import project into MCUXpresso, click 'Import project from Application Code Hub', search 'label cifar10 images on mcxn947' example, and clone to the local workspace.

![](image/mcux%20_import_project.png)

Build the project, after compile complete, use the GUI Flash Tool (2 in the following figure) to write the program to the board.

![](image/build_download.png)

#### Develop in VS code

In VS code, select the 'MCUXpresso For VScode' plugin, and click 'Application Code Hub' in the QUICKSTART PANEL. 
Search the 'label cifar10 images on mcxn947' example, and clone it to the local workspace.
After a while, the project is shown in the 'Projects'.

![](image/vs_import_project.png)

Build the project, and after compile complete, flash the board.

![](image/vs_build_project.png)

## 4. Results <a name="step4"></a>

Reset the board, the preview of camera is showing in the top of LCD (if the preview is blank it's because of the voltage mismatch between Camera module and FRDM board, please reset the board).

Show a bird picture (print the image on a paper or use you mobilephone display the test image) to the camera, the type of object is showing at the bottom of the LCD.

![](image/running_result.jpg)

## 5. FAQs <a name="step5"></a>

The model training section is not included in this project, if you want to train your custom dataset, please refer to the eIQ toolkit for the model training.

eIQ use guide, please refer to https://community.nxp.com/t5/eIQ-Machine-Learning-Software/tkb-p/eiq%40tkb

## 6. Support <a name="step6"></a>

*Please contact NXP for additional support.*

#### Project Metadata

<!----- Boards ----->

[![Board badge](https://img.shields.io/badge/Board-FRDM–MCXN947-blue)](https://github.com/search?q=org%3Anxp-appcodehub+FRDM-MCXN947+in%3Areadme&type=Repositories)

<!----- Categories ----->

[![Category badge](https://img.shields.io/badge/Category-HMI-yellowgreen)](https://github.com/search?q=org%3Anxp-appcodehub+hmi+in%3Areadme&type=Repositories) [![Category badge](https://img.shields.io/badge/Category-AI/ML-yellowgreen)](https://github.com/search?q=org%3Anxp-appcodehub+aiml+in%3Areadme&type=Repositories)

<!----- Peripherals ----->

[![Peripheral badge](https://img.shields.io/badge/Peripheral-DISPLAY-yellow)](https://github.com/search?q=org%3Anxp-appcodehub+display+in%3Areadme&type=Repositories)

<!----- Toolchains ----->

[![Toolchain badge](https://img.shields.io/badge/Toolchain-MCUXPRESSO%20IDE-orange)](https://github.com/search?q=org%3Anxp-appcodehub+mcux+in%3Areadme&type=Repositories) [![Toolchain badge](https://img.shields.io/badge/Toolchain-VS%20CODE-orange)](https://github.com/search?q=org%3Anxp-appcodehub+vscode+in%3Areadme&type=Repositories)

Questions regarding the content/correctness of this example can be entered as Issues within this GitHub repository.

> **Warning**: For more general technical questions regarding NXP Microcontrollers and the difference in expected funcionality, enter your questions on the [NXP Community Forum](https://community.nxp.com/)

[![Follow us on Youtube](https://img.shields.io/badge/Youtube-Follow%20us%20on%20Youtube-red.svg)](https://www.youtube.com/@NXP_Semiconductors)
[![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us%20on%20LinkedIn-blue.svg)](https://www.linkedin.com/company/nxp-semiconductors)
[![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us%20on%20Facebook-blue.svg)](https://www.facebook.com/nxpsemi/)
[![Follow us on Twitter](https://img.shields.io/badge/Twitter-Follow%20us%20on%20Twitter-white.svg)](https://twitter.com/NXP)

## 7. Release Notes<a name="step7"></a>

| Version | Description / Update                    |                                  Date |
| :-----: | --------------------------------------- | ------------------------------------: |
|   1.0   | Initial release on Application Code Hub | January 30<sup>th</sup> 2024 |


