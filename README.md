# Theremin
my instrument, no physical touch needed.


This project retrieves the distance from an ultrasonic sensor, after that, based on the distance returned, it sends a signal over bluetooth which a device of your choice can pick up and turn into/play a midi note.

Steps to using:
1. build [cuircut](https://velxio.dev/project/a78f1fa0-0d42-4a9f-b6e3-25577e85eb1b)(either click this link to see the circuit or download the circuit .zip and upload it into [velxio](https://velxio.dev/)), I am using the Nano RP2040 arduino microcontroller so code changes will need to be made if using a different microcontroller. (the library I am using is only compatible with: [these](https://docs.arduino.cc/libraries/ble-midi/)).
2. ensure all neccessary libraries are downloaded: NewPing by TIm Eckel and BLE-MIDI by lathoub(as well as all other libraries prompted to download with these two).
3. upload Theremin-code.ino to your microcontroller.
4. set up your phone by downloading fluidsynth and Bluetooth MIDI Connect on the playstore(not sure on IOS, I don't have access to an IOS device to test on) and connecting to your microcontroller via the Bluetooth MIDI Connect application (you cannot connect via normal bluetooth because it is over BLE). It should be named "BLE-MIDI".
5. once completed, open fluidsynth and make "BLE-MIDI" your MIDI Output Port.
6. ### play with your new instrument!!!
<br/>

#### Look at latest release for examples of it in action!
