**Neurset**

Simulation of an EEG direct Neurofeedback Device using C++ and Qt framework

**Design Patterns:**

Centralized - Device acts as the central class which controls everything that takes place from when the device is turned on until it is turned off. 
Mediator - Device provides and handles all the communication that is between the user and the other classes.
Singleton design pattern - device is the only instance that controls other operations that take place from when the device is turned on until it is turned off. 
Commander & Observer - device observes, receives and executes commands from other classes.

**Use Cases:**

Normal Operation, Successful Case, Emergency Handling, Session History Review & Export, System Maintenance & Diagnostic, Battery Management.

**How to install and use:**

Download the .zip file and extract it, open in Qt creator and run from the mainwindow.ui. (Make sure to be running linux operating system)
