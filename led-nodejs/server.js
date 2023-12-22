const express = require('express');
const path = require('path');
const http = require('http');
const bodyParser = require('body-parser');

const app = express();
const port = 3000;

app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.static(path.join(__dirname, 'public')));
//app.use('/test-tree', express.static(path.join(__dirname, 'public')));

// Use http module to send HTTP requests to Arduino
const arduinoHost = '192.168.0.207'; // Change to the IP address of your Arduino if needed
const arduinoPort = 80; // Change the port if your Arduino is running on a different port

app.listen(port, () => {
  console.log(`Server is running at http://localhost:${port}`);
});

app.post('/toggle', (req, res) => {
  // Send HTTP POST request to Arduino to toggle the LED
  const options = {
    hostname: arduinoHost,
    port: arduinoPort,
    path: '/toggle', // Change the path based on your Arduino code
    method: 'POST',
  };

  const arduinoReq = http.request(options, (arduinoRes) => {
    let data = '';

    arduinoRes.on('data', (chunk) => {
      data += chunk;
    });

    arduinoRes.on('end', () => {
      console.log('Arduino Response:', data);
      res.send('Toggle success');
    });
  });

  arduinoReq.on('error', (error) => {
    console.error('Error communicating with Arduino:', error.message);
    res.status(500).send('Error communicating with Arduino');
  });

  arduinoReq.end();
});
