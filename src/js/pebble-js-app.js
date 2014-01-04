
var keymap = [
	"Null",
	"Home",
	"Rev",
	"Fwd",
	"Play",
	"Select",
	"Left",
	"Right",
	"Down",
	"Up",
	"Back",
	"InstantReplay",
	"Info",
	"Backspace",
	"Search",
	"Enter"
];

function sendRokuCommand(cmd) {
	var key = keymap[cmd];
	var req = new XMLHttpRequest();
	req.open('POST', "http://roku.gutwin.org:8060/keypress/" + key);
	req.send(null);
	console.log("Sent key " + key);
}

// Set callback for the app ready event
Pebble.addEventListener(
	"ready",
	 function(e) {
		 console.log("connect!" + e.ready);
		 console.log(e.type);
	 });

// Set callback for appmessage events
Pebble.addEventListener(
	"appmessage",
	 function(e) {
		 console.log("message");
		 if (e.payload.command) {
			 sendRokuCommand(e.payload.command);
		 }
	 });
