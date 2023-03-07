
  
  

# Bionic Arm

  

## Sensor:

  

***EMG Sensor:***

Oymotion Gravity EMG sensor is used for collecting the sEMG.

[Product Wiki](https://wiki.dfrobot.com/Analog_EMG_Sensor_by_OYMotion_SKU_SEN0240)

  

The EMG_Filters library is designed to work with a sample rate of either 1000Hz or 500Hz meaning the runtime of the arduino code should be 1000us or 2000us respectively.

  

**Arduino Code:**

Instantiate EMG_Sensor class in the EMG.ino file for each sensor. All the required helper classes are defined inside the 'libraries' folder. The Arduino receives sensor readings from each sensor, filters noise and sends the data through serial channel.

  

**Python Code**

A Conda environment is defined for the python code. The required package information is present in 'environment.yml' file. After cloning the repo, create a conda environment using this .yml file.

## Setup

 1. Clone the repo
 2. Install Anaconda. [Link](https://www.anaconda.com/products/distribution)
 3. Navigate to the cloned repo in the Anaconda Prompt.
 4. Create an environment using the environment.yml file of the repo
  `conda env create -f environment.yml`
5. Use VS Code for coding & the Anaconda Prompt to run the python code. Always make sure to switch to the environment before running the code.