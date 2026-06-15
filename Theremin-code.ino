#include <MIDI.h>
#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_ArduinoBLE.h>
#include <NewPing.h>

#define TrigPin1 10
#define EchoPin1 9
#define TrigPin2 8
#define EchoPin2 7
#define MaxDist 200

NewPing sonar1(TrigPin1, EchoPin1, MaxDist);
NewPing sonar2(TrigPin2, EchoPin2, MaxDist);\

//change these values
int vibAmt = 80; //amount it vibrates, higher is more vibrato

int sensor1Dist = 0;
int sensor2Dist = 0;
int oldDist = 170; 
int olderDist = 170; 
int oldestDist = 170;
int oldVib = -1; 
int olderVib = -1; 
int oldestVib = -1;
int viableChange = 3; 
int movingAmt = 5;
int Vol = 0;
bool Switch = false;
bool vibSwitch = false;
int lastNote = -1; // tracks currently playing note
int lastPlayedNote = -1;
int newNote = -1; 
float amtChange = 0.75; // play with to get perfect
float smoothed = 0;
bool playingStart = false;

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

  // Serial.print("Connected: ");
  // Serial.println(isConnected);

  if (isConnected) {
    oldestDist = olderDist;
    olderDist = oldDist;
    oldDist = sensor1Dist;
    sensor1Dist = sonar1.ping_cm(); // set up smoothing vars

    oldestVib = olderVib;
    olderVib = oldVib;
    oldVib = newNote; // set up vibrato vars

    Serial.print("Dist: ");  
    Serial.println(sensor1Dist); 

    if (sensor1Dist < 65 && oldDist > 65) {
      playingStart = true;
    } 
    
    Serial.println(playingStart); 

    newNote = -1; 
    if (Switch == false) {
      Vol = Vol + 1;
      if (Vol > 126) {
        Switch = true;
      }
    } else {
      Vol = Vol - 1;
      if ( Vol < 1) { 
        Switch = false;
      }
    }
 
    if (abs(sensor1Dist - oldDist)  < viableChange) {
      sensor1Dist = oldDist;   
    }  

    if (oldDist > 65) { 
      smoothed = sensor1Dist; 
    } else {
      smoothed = amtChange * sensor1Dist + (1 - amtChange) * smoothed; // gradually changes notes
    }

    // Serial.print("NewNote check: ");
    // Serial.println(sensor1Dist > 0 && sensor1Dist <= 5 ? "in range" : "out of range");
    if (abs(sensor1Dist - oldDist) < movingAmt && abs(oldDist - olderDist) < movingAmt && abs(olderDist - oldestDist) < movingAmt) 
      if (smoothed > 0 && smoothed <= 5) {newNote = 60; Serial.println("C");} // C
      else if (smoothed <= 10) {newNote = 61; Serial.println("C#");} // C Sharp
      else if (smoothed <= 15) {newNote = 62; Serial.println("D");} // D
      else if (smoothed <= 20) {newNote = 63; Serial.println("D#");} // D Sharp
      else if (smoothed <= 25) {newNote = 64; Serial.println("E");} // E
      else if (smoothed <= 30) {newNote = 65; Serial.println("F");} // F
      else if (smoothed <= 35) {newNote = 66; Serial.println("F#");} // F Sharp
      else if (smoothed <= 40) {newNote = 67; Serial.println("G");} // G
      else if (smoothed <= 45) {newNote = 68; Serial.println("G#");} // G Sharp
      else if (smoothed <= 50) {newNote = 69; Serial.println("A");} // A
      else if (smoothed <= 55) {newNote = 70; Serial.println("A#");} // A Sharp
      else if (smoothed <= 65) {newNote = 71; Serial.println("B");} // B
    } else {Serial.println("Not Playing");}// Only change note if it actually changed

    if (playingStart && oldestVib == newNote && vibSwitch) {
      MIDI.sendControlChange(1, vibAmt, 1); //add vibrato
      vibSwitch = false;
      playingStart = false;
      Serial.println("playing");
    } 
    else if (playingStart) {
      MIDI.sendControlChange(1, 0, 1); // turn off vibrato
      vibSwitch = true;
      playingStart = false;
      Serial.println("not playing");
    }

    if (newNote != lastNote) {
      if (lastNote != -1) {
        MIDI.sendNoteOff(lastNote, 0, 1); // stop old note 
      } 
      if (newNote != -1) {  
        MIDI.sendNoteOn(newNote, 100, 1); // play new note
        // Serial.println(newNote); 
      }  
      lastNote = newNote;
    } // else if (newNote == lastPlayedNote && vibSwitch == false) {
    //   MIDI.sendControlChange(1, vibAmt, 1); //add vibrato
    //   vibSwitch = true;
    //   lastPlayedNote = newNote;
    // } else if (vibSwitch == true){
    //   MIDI.sendControlChange(1, 0, 1); //turn off vibrato
    //   vibSwitch = false; 
    //   lastPlayedNote = newNote;
    // }

    if (newNote !=  -1) {
      lastPlayedNote = newNote;
    }
    Serial.println(newNote);
    Serial.println(lastPlayedNote);
    delay(50);
  
}

