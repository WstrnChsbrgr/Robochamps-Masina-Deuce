#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "control.h"
#include "pines.h"
#include "motores.h"
#include "servo_seed.h"
#include "color.h"
#include "settings.h"

WebServer servidor(80);

const char* NOMBRE_RED = "MasinaDeuce";        
const char* CLAVE_RED  = "Autode12";

const char PAGINA[] = R"rawliteral(
<!DOCTYPE html><html><head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
  body{font-family:sans-serif;text-align:center;user-select:none;-webkit-user-select:none;touch-action:pan-y}
  button{width:40vw;height:16vh;font-size:2em;margin:6px;touch-action:none}
</style></head><body>
<h1>Robot</h1>
<button id="f">Forward</button><br>
<button id="l">Left</button><button id="r">Right</button><br>
<button id="b">Back</button><br>
<button id="s">STOP</button>
<button id="g">Grab</button><button id="o">Release</button>
<br>
<button id="c0">Floor</button><button id="c1">Red</button><button id="c2">Green</button>
<button id="c3">Blue</button><button id="c4">Yellow</button>
<hr>
<div id="panel"></div>
<button id="save">Save settings</button>
<button id="rd">Read color</button>
<pre id="color"></pre>
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
document.getElementById('g').addEventListener('pointerdown', () => send('/grab'));
document.getElementById('o').addEventListener('pointerdown', () => send('/release'));
function cal(i){
  fetch('/cal?id='+i).then(r=>{
    document.getElementById('color').textContent = r.ok ? 'calibrated '+i : 'read FAILED, retry';
  });
}
for (let i=0;i<5;i++) document.getElementById('c'+i).addEventListener('pointerdown', ()=>cal(i));

const sliders=[['drive','Speed',0,255],['turn','Turn speed',0,255],['trimL','Trim left',-30,30],
  ['trimR','Trim right',-30,30],['open','Gripper open',0,180],['close','Gripper close',0,180]];
fetch('/status').then(r=>r.text()).then(t=>{
  const v=t.split(',');
  sliders.forEach(([k,label,min,max],i)=>{
    const d=document.createElement('div');
    d.innerHTML=label+': <span id="v_'+k+'">'+v[i]+'</span><br><input type="range" min="'+min+'" max="'+max+'" value="'+v[i]+'" style="width:80vw">';
    const inp=d.querySelector('input');
    inp.addEventListener('input',e=>{document.getElementById('v_'+k).textContent=e.target.value;});
    inp.addEventListener('change',e=>send('/set?k='+k+'&v='+e.target.value));
    document.getElementById('panel').appendChild(d);
  });
});
document.getElementById('save').addEventListener('click',()=>send('/save'));
document.getElementById('rd').addEventListener('click',()=>
  fetch('/color').then(r=>r.text()).then(t=>{document.getElementById('color').textContent='R,G,B,color = '+t;}));
hold('f','/forward'); hold('l','/left'); hold('r','/right'); hold('b','/back');
document.getElementById('s').addEventListener('pointerdown', () => send('/stop'));
</script></body></html>
)rawliteral";

const unsigned long WATCHDOG_MS = 300;

unsigned long lastCommand = 0;
bool moving = false;
bool is_moving() { return moving; }

void sendOk() {
  servidor.send(200, "text/plain", "ok");
}

void goForward() {
  forward(settings.speedDrive);
  lastCommand = millis(); moving = true;
  sendOk();
}

void goBack() {
  forward(-settings.speedDrive);
  lastCommand = millis(); moving = true;
  sendOk();
}

void goLeft() {
  turn(-settings.speedTurn);
  lastCommand = millis(); moving = true;
  sendOk();
}

void goRight() {
  turn(settings.speedTurn);
  lastCommand = millis(); moving = true;
  sendOk();
}

void doStop() {
  stop();
  moving = false;
  sendOk();
}

void doGrab() {
  grab_seed();
  sendOk();
}

void doRelease() {
  release_seed();
  sendOk();
}

void doCalibrate() {
  int id = servidor.arg("id").toInt();
  if (id < 0 || id > 4) { servidor.send(400, "text/plain", "bad id"); return; }
  if (!calibrate_color(id)) { servidor.send(500, "text/plain", "read failed"); return; }
  sendOk();
}

void doSet() {
  String k = servidor.arg("k");
  int v = servidor.arg("v").toInt();
  if      (k == "drive") settings.speedDrive = constrain(v, 0, 255);
  else if (k == "turn")  settings.speedTurn  = constrain(v, 0, 255);
  else if (k == "trimL") settings.trimLeft   = constrain(v, -30, 30);
  else if (k == "trimR") settings.trimRight  = constrain(v, -30, 30);
  else if (k == "open")  { settings.angleOpen  = constrain(v, 0, 180); release_seed(); }
  else if (k == "close") { settings.angleClose = constrain(v, 0, 180); grab_seed(); }
  else { servidor.send(400, "text/plain", "bad key"); return; }
  sendOk();
}

void doSave() {
  settings_save();
  sendOk();
}

void doStatus() {
  String s = String(settings.speedDrive) + "," + settings.speedTurn + "," +
             settings.trimLeft + "," + settings.trimRight + "," +
             settings.angleOpen + "," + settings.angleClose;
  servidor.send(200, "text/plain", s);
}

void doColor() {
  int r, g, b;
  read_rgb(&r, &g, &b);
  servidor.send(200, "text/plain", String(r) + "," + g + "," + b + "," + detect_color());
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
    servidor.on("/grab", doGrab);
    servidor.on("/release", doRelease);
    servidor.on("/cal", doCalibrate);
    servidor.on("/set", doSet);
    servidor.on("/save", doSave);
    servidor.on("/status", doStatus);
    servidor.on("/color", doColor);
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
