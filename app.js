var mapImg;
var mapCoOrdLonga = 1;
var mapCoOrdLong;
var coOrdinate = [];
var timeStamp = [];
var date = [];
var clicked = 0;




function preload() {
    mapImg = loadImage("../frenchaymap.png");   //loads map image
}
var pubnub = new PubNub({
    subscribeKey: 'sub-c-71645706-99ca-11e9-9ac8-0ed882abeb26' //PubNub subscribe key to retrieve sensor data from prototype
});
var channel = 'iotchannel'
pubnub.history({
        channel: 'iotchannel',
        count: 36
    },
    function(status, response) {
        console.log(status, response);


        for (var i = 0; i < 4; i++) { // 4 is used for counter for purposes of easier data handling.
        // response.messages.length would be used in future.


            coOrdinate[i] = response.messages[i].entry.eon.latitude; //co-ordinates = response from sensors latitude value on prototype.
            console.log(coOrdinate); // the co-ordinate data that has been passed in from pubnub.
            var block; // variable for UWE blocks
            var j = i + "block";


            switch (coOrdinate[i]) { // switch statement to step through each of the co-ordinate values and assign a block
            // To improve real co-ordinate readings from UWE will be made and the cases will be
            // ranges rather than hard co-ordinates as button could be pressed in different location of the same block.
                case 51.34:
                    block = "X";
                    break;
                case 51.2:
                    block = "Q";
                    break;
                case 51.51:
                    block = "D";
                    break;
            }
            document.getElementById(j).innerHTML = block; // Applies the block value into the table
        }

        for (var i = 0; i < 4; i++) {
            coOrdinate[i] = response.messages[i].entry.eon.latitude;
            var j = i;
            document.getElementById(j).innerHTML = coOrdinate[i]; //Applies the co-ordinate value and adds it tot the correct row of the table.
        }

        for (var i = 0; i < 4; i++) {
            timeStamp[i] = response.messages[i].timetoken / 10000;
            date[i] = new Date(timeStamp[i]);

            date.toString(); // converts time from UTC to correct format.
            console.log(date);
            var j = i + "time";
            document.getElementById(j).innerHTML = date[i];  //Applies the date and add it to the correct row of the table.
        }




    })

function setup() {
    var cnv = createCanvas(1400, 700);
    var x = (windowWidth - width) / 2;
    var y = (windowHeight - height) / 2;
    cnv.position(x, y);
}

function draw() {
    translate((width / 5), (height / 12));
    image(mapImg, -200, -200);
    let c = color(255, 204, 0); // Define colour 'c'
    var block;

    mapCoOrdLong = coOrdinate;

    for (var i = 0; i < 4; i++) {

        switch (mapCoOrdLong[i]) {
            case 51.34:
                block = "X";
                break;
            case 51.2:
                block = "Q";
                break;
            case 51.51:
                block = "D";
                break;
            case 51.63: // value not given from sensor for testing, should not appear on map (line 137)
                block = "N";
                break;
        }

        //console.log(block);

        if (block == "X") { // if there is a X block co-ordinate draw.
            ellipse(560, 200, 35, 35);
            fill(c);
            strokeWeight(4);
        }

        if (block == "Q") { // if there is a Q block co-ordinate draw.
            ellipse(555, 345, 35, 35);
            strokeWeight(4);
            fill(c);
        }

        if (block == "D") { // if there is a D block co-ordinate draw.
            ellipse(380, 300, 35, 35);
            fill(c);
            strokeWeight(4);
        }
        if (block == "N") { // if there were a N block value passed from the sensor this would be drawn.
            ellipse(490, 470, 35, 35);
            fill(c);
            strokeWeight(4);
        }

    }

}

function blockClick() {

    switch (clicked) {
        case 0:
            clicked = 1;
            break;
        case 1:
            clicked = 0;
            break;
    }



}
