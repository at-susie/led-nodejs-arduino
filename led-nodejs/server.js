const express = require('express');
const path = require('path');
const http = require('http');
const bodyParser = require('body-parser');

const app = express();
const port = 3000;

app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.static(path.join(__dirname, 'public')));

const arduinoHost = '192.168.0.207'; // Change to the IP address of your Arduino if needed
const arduinoPort = 80; // Change the port if your Arduino is running on a different port

app.listen(port, () => {
  console.log(`Server is running at http://localhost:${port}`);
});

// Turn On
app.post('/toggle-on', (req, res) => {
  sendCommand('/toggle-on', res);
  console.log('Server receives Toggle ON');
});

// Turn Off
app.post('/toggle-off', (req, res) => {
  sendCommand('/toggle-off', res);
  console.log('Server receives Toggle OFF');
});

function sendCommand(command, res) {
  const options = {
    hostname: arduinoHost,
    port: arduinoPort,
    path: command,
    method: 'POST',
  };

  const arduinoReq = http.request(options, (arduinoRes) => {
    let data = '';

    arduinoRes.on('data', (chunk) => {
      data += chunk;
    });

    arduinoRes.on('end', () => {
      console.log('Arduino Response:', arduinoRes.statusCode, arduinoRes.headers);
      console.log('Arduino Data:', data);
      res.send('Toggle success');
    });
  });

  arduinoReq.on('error', (error) => {
    console.error('Error communicating with Arduino:', error.message);
    if (error.code === 'ECONNRESET') {
      // Handle ECONNRESET (socket hang up) gracefully
      console.warn('Socket hang up error, ignoring.');
    } else {
      res.status(500).send('Error communicating with Arduino: ' + error.message);
    }
  });

  arduinoReq.end();
}


