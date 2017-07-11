// simple communication with esp8266
window.onload = (e) => {

// box- for event handle
// span- print basic info about x y inputs
// div- red circle indicating touch/ keyboard inputs
// xyCoeffForTouchScreen- x & y coeffisients used when phone is rotated (landscape to normal)
// deg- typical range of servos from 90 - deg/2 to 90 + deg/2 the max range is 90 - deg and 90 + deg
// stepTouchScreen- (value of x or y)%step === 0 will fire ajax 
// memoLR - remember left and right servo old setting
// xy - [(curent x), (curent y), (old x), (old y)]

let box = document.body,
	span = document.querySelector('span'),
	div = document.querySelector('div'),
	xyCoeffForTouchScreen = [0,0],
	deg = 50,
	stepTouchScreen = 5,
	memoLR = [],
	xy = [0,0,0,0];

// add event listeners to box element for 5 events (you can add more or less easily)
['keyup', 'keydown', 'touchmove', 'touchstart', 'touchend'].forEach(
	e => box.addEventListener( e,e.indexOf('touch')? keyboard : touch, false)
);

// turning your phone vertical/horizontal will fire this event and will create xy coefficients 
window.onresize = (e) => setSizeCoeff();
function setSizeCoeff() {
	xyCoeffForTouchScreen[0] = 100 / box.offsetWidth;
	xyCoeffForTouchScreen[1] = 100 /  box.offsetHeight;
}
// initialize xy coefficients values the hard core way
setSizeCoeff();

// touch function handler
function touch(e) {
	let el = e.targetTouches[0],
		xyPct = [];

	// set default position when touch ends
	if(e.type === 'touchend') { 
		calculatePrintSend(e.target, [50,50]);
		return; 
	}

	// get procentage value of x and y of your touch 
	xyPct[0] = Math.floor(el.pageX * xyCoeffForTouchScreen[0]);
	xyPct[1] = Math.floor(el.pageY * xyCoeffForTouchScreen[1]);

	// fire ajax and print values on the screen
	calculatePrintSend(e.target, xyPct);
}

function calculatePrintSend(element, xyPct) {
	let xy = [],
		calcLR = [];
	
	// print info on the screen
	infoToSee(element,xyPct);

	// calculating correct possition of each servo 
	xy[0] = xyPct[0]*deg / 100 - deg* 0.5;
	xy[1] = xyPct[1]*deg / 100 - deg*0.5;

	calcLR[0] = Math.floor(xy[1] + xy[0]) + 90;
	calcLR[1] = Math.floor(xy[1] - xy[0]) + 90;

	if(( memoLR[0] === calcLR[0] || memoLR[1] === calcLR[1]) 
	|| (calcLR[0]%stepTouchScreen === 0 && calcLR[1]%(stepTouchScreen) === 1)) return;

	memoLR[0] = calcLR[0];
	memoLR[1] = calcLR[1];

	// send data to esp8266
	ajax('a' + calcLR[0] + 'b' + calcLR[1] + 'c');
}

function keyboard(e) {
	// some stuff to detect which key is up/down
	let typ = e.type,
	ch = String.fromCharCode(e.which),
	x = 'AWDS'.indexOf(ch);

	if(x === -1) return;

	// calculating x and y position in degrees 
	xy[x%2] = ( x > 1 ? deg/2 : -deg/2 ) * (typ === 'keyup' ? 0 : 1);

	if(xy[0] === xy[2] && xy[1] === xy[3]) return;

	xy[2] = xy[0];
	xy[3] = xy[1];

	// print data on the screen. Map is used to convert from procentage back to x & y coords of the screen
	infoToSee(box,xy.map(e => {return Math.ceil( (e + deg/2) * 100/deg); }));

	// send data to esp8266
	ajax('a' + (xy[1] - xy[0] + 90 ) + 'b' + (xy[1] + xy[0] + 90 ) + 'c');
}

// basic ajax call. Use promise to smoothe out the reaction of device
function ajax(query) {
	let x = new Promise((a,b) => {
		let xhr = new XMLHttpRequest();
		xhr.open('GET','http://192.168.4.1/d?' + query, true);

		// uncomment it and you will get simple servo angles response from esp8266 on your console
		/*
		xhr.onreadystatechange = () => {
			if(xhr.readyState === 4) {
				if(xhr.status === 200) console.log(xhr.responseText);
			}
		}; 
		*/
		xhr.send(null);
		// you can hadle errors and such by calling b(send some data here);
		a();
	});
	x.then(e => {
		// do stuff if you want like print on the screen some info ? 
	});
}

// prints data on the screen and moves circle around
function infoToSee(element, [x,y]) {
	// print out x and y coords on the screen
	span.innerHTML = 'X ' + String('00' + x).slice(-3) + ' Y ' + String('00' + y).slice(-3);

	// you can use gradient to show your touch or key press inputs. Looks better but takes more energy to calculate
	// element.style.background = 'radial-gradient(circle at ' + x + '% '+ y +'%, red 5%, black 20%)';

	// move circle around to indicate user inputs
	div.style.top = Math.floor(y/xyCoeffForTouchScreen[1]  -  div.offsetHeight / 2)+ 'px';
	div.style.left = Math.floor(x/xyCoeffForTouchScreen[0]  - div.offsetWidth / 2)+ 'px';

}

}; //window.onload closing bracket




