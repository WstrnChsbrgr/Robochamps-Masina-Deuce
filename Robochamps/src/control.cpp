#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "control.h"
#include "pines.h"
#include "motores.h"

WebServer servidor(80);

const char* NOMBRE_RED = "MasinaDeuce";        
const char* CLAVE_RED  = "Autode12";

const char PAGINA[] = R"rawliteral(
<!DOCTYPE html><html><head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
  body{font-family:sans-serif;text-align:center;user-select:none;-webkit-user-select:none;touch-action:none}
  button{width:40vw;height:16vh;font-size:2em;margin:6px;touch-action:none}
</style></head><body>
<h1>Robot</h1>
<button id="f">Forward</button><br>
<button id="l">Left</button><button id="r">Right</button><br>
<button id="b">Back</button><br>
<button id="s">STOP</button>
<script>
let timer = null;
function send(path){ fetch(path).catch(()=>{}); }
function hold(id, path){
  const el = document.getElementById(id);
  el.addEventListener('pointerdown', e => {
    e.preventDefault();
    send(path);
    clearInterval(timer);
    timer = setInterval(() => send(path), 100);
  });
  const up = () => { clearInterval(timer); timer = null; send('/stop'); };
  el.addEventListener('pointerup', up);
  el.addEventListener('pointercancel', up);
  el.addEventListener('pointerleave', up);
}
hold('f','/forward'); hold('l','/left'); hold('r','/right'); hold('b','/back');
document.getElementById('s').addEventListener('pointerdown', () => send('/stop'));
</script></body></html>
)rawliteral";

const int SPEED_DRIVE = 100;
const int SPEED_TURN  = 100;
const unsigned long WATCHDOG_MS = 300;

unsigned long lastCommand = 0;   // time of the last command received
bool moving = false;

void sendOk() {
  servidor.send(200, "text/plain", "ok");
}

void goForward() {
  forward(SPEED_DRIVE);
  lastCommand = millis(); moving = true;
  sendOk();
}

void goBack() {
  forward(-SPEED_DRIVE);
  lastCommand = millis(); moving = true;
  sendOk();
}

void goLeft() {
  turn(-SPEED_TURN);
  lastCommand = millis(); moving = true;
  sendOk();
}

void goRight() {
  turn(SPEED_TURN);
  lastCommand = millis(); moving = true;
  sendOk();
}

void doStop() {
  stop();
  moving = false;
  sendOk();
}

void showpage() {
  servidor.send(200, "text/html", PAGINA);
}

void LedOn() {
  digitalWrite(LED_PIN, HIGH);
  showpage();
}

void LedOff() {
  digitalWrite(LED_PIN, LOW);
  showpage();
}


void init_controller() {
    pinMode(LED_PIN, OUTPUT);
    WiFi.softAP(NOMBRE_RED, CLAVE_RED);
    servidor.on("/", showpage);
    Serial.println(WiFi.softAPIP());
    servidor.on("/forward", goForward);
    servidor.on("/back", goBack);
    servidor.on("/left", goLeft);
    servidor.on("/right", goRight);
    servidor.on("/stop", doStop);
    servidor.begin();
}

void attend_controller() {
  servidor.handleClient();
  if (moving && millis() - lastCommand > WATCHDOG_MS) {
    stop();
    moving = false;
    Serial.println("WATCHDOG: stop");
  }
}
