#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_ArduinoBLE.h>
#include <NewPing.h>

#define TrigPin1 10
#define EchoPin1 9
#define TrigPin2 8
#define EchoPin2 7
#define MaxDist 200

NewPing sonar1(TrigPin1, EchoPin1, MaxDist);
NewPing sonar2(TrigPin2, EchoPin2, MaxDist);

int sensor1Dist = 0;
int sensor2Dist = 0;
int lastNote = -1; // tracks currently playing note

BLEMIDI_CREATE_DEFAULT_INSTANCE();
bool isConnected = false;

void setup() {
  Serial.begin(115200);
  // NO while(!Serial) here

  MIDI.begin(MIDI_CHANNEL_OMNI);

  BLEMIDI.setHandleConnected([]() {
    isConnected = true;
    Serial.println("Connected!");
  });

  BLEMIDI.setHandleDisconnected([]() {
    isConnected = false;
    Serial.println("Disconnected.");
  });
}

void loop() {
  MIDI.read();

  Serial.print("Connected: ");
  Serial.println(isConnected);

  if (isConnected) {
    sensor1Dist = sonar1.ping_cm();
    Serial.print("Dist: ");
    Serial.println(sensor1Dist);

    int newNote = -1;

    Serial.print("NewNote check: ");
    Serial.println(sensor1Dist > 0 && sensor1Dist <= 5 ? "in range" : "out of range");
    if (sensor1Dist > 0 && sensor1Dist <= 5) newNote = 60; // C
    else if (sensor1Dist <= 10) newNote = 62; // D
    else if (sensor1Dist <= 15) newNote = 64; // E
    else if (sensor1Dist <= 20) newNote = 65; // F
    else if (sensor1Dist <= 25) newNote = 67; // G
    else if (sensor1Dist <= 30) newNote = 69; // A
    else if (sensor1Dist <= 35) newNote = 71; // B

    // Only change note if it actually changed
    if (newNote != lastNote) {
      if (lastNote != -1) {
        MIDI.sendNoteOff(lastNote, 0, 1); // stop old note
      }
      //if (newNote != -1) {
        MIDI.sendNoteOn(newNote, 100, 1); // play new note
        Serial.println(newNote);
      //}
      lastNote = newNote;
    }

    delay(50);
  }
}
